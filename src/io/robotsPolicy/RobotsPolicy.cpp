/**
 * @file RobotsPolicy.cpp
 * 
 * @brief This file implements the RobotsPolicy class, which represents a robots.txt checker.
 *
 * @author Adrien GRAS
 * @date 2026-07-30
 */


// Imports
#include "RobotsPolicy.hpp"
#include "core/logger/Logger.hpp"
#include <algorithm>
#include <regex>
#include <sstream>
#include <cctype>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor
    // =========================================================================

    /**
     * @brief Default constructor : define prefix as ROBOTS-POLICY
     */
    RobotsPolicy::RobotsPolicy() : Entity("ROBOTS-POLICY") {}




    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Checks whether a given path may be fetched on a given host.
     * @param host The domain (e.g. "www.largus.fr").
     * @param path The path to check (e.g. "/fiche-technique/Peugeot/208.html").
     * @return true if allowed (or if robots.txt is unreachable/absent), false if disallowed.
     */
    bool RobotsPolicy::isAllowed(const std::string& host, const std::string& path) {

        // Loading the robots.txt rules for this host if not already done
        if (_loadedHosts.find(host) == _loadedHosts.end()) {
            loadRules(host);
        }


        // Finding every rule that matches the path, keeping the most specific one
        // (longest pattern wins, per RFC 9309), regardless of file order
        const std::vector<RobotsRule>& rules = _rulesByHost[host];
        const RobotsRule* bestMatch = nullptr;

        for (const RobotsRule& rule : rules) {
            if (matches(path, rule._pattern)) {
                if (!bestMatch || rule._pattern.size() > bestMatch->_pattern.size()) {
                    bestMatch = &rule;
                }
            }
        }


        // No matching rule -> allowed by default
        if (!bestMatch) {
            return true;
        }


        // Logging and returning the decision
        if (!bestMatch->_isAllow) {
            Logger::warn("[{}].isAllowed({}, {}) → blocked by robots.txt rule \"{}\"",
                getFullId(), host, path, bestMatch->_pattern);
        } else {
            Logger::trace("[{}].isAllowed({}, {}) → allowed by robots.txt rule \"{}\"",
                getFullId(), host, path, bestMatch->_pattern);
        }
        return bestMatch->_isAllow;

    }


    /**
     * @brief Clears the cached rules for all hosts, forcing a re-fetch on next check.
     */
    void RobotsPolicy::clearCache() {
        _rulesByHost.clear();
        _loadedHosts.clear();
    }




    // =========================================================================
    // Internal logic
    // =========================================================================

    /**
     * @brief Downloads and parses the robots.txt file for a host, populating the cache.
     * @param host The domain to fetch robots.txt for (e.g. "www.largus.fr").
     */
    void RobotsPolicy::loadRules(const std::string& host) {

        // Fetching the robots.txt file
        std::string url = "https://" + host + "/robots.txt";
        HttpResponse response = _client.get(url);


        // Fail-open: if the file cannot be fetched, cache an empty rule set (everything allowed)
        if (response.statusCode != 200) {
            Logger::debug("[{}].loadRules({}) → robots.txt unreachable (code {}), allowing all",
                getFullId(), host, response.statusCode);
            _rulesByHost[host] = {};
            _loadedHosts[host] = true;
            return;
        }


        // Parsing the "*" group and caching the result
        _rulesByHost[host] = parseWildcardGroup(response.body);
        _loadedHosts[host] = true;
        Logger::debug("[{}].loadRules({}) → loaded {} rule(s) from robots.txt",
            getFullId(), host, _rulesByHost[host].size());

    }


    /**
     * @brief Parses the raw content of a robots.txt file, keeping only the "*" group.
     * @param content The raw robots.txt content.
     * @return The list of Allow/Disallow rules found in the "User-agent: *" group.
     */
    std::vector<RobotsRule> RobotsPolicy::parseWildcardGroup(const std::string& content) {

        std::vector<RobotsRule> rules;
        std::istringstream stream(content);
        std::string line;
        bool inWildcardGroup = false;


        // Robots.txt is organized in groups, each starting with one or more
        // "User-agent:" lines followed by "Allow:"/"Disallow:" directives.
        // A new "User-agent:" line that does NOT immediately follow another
        // "User-agent:" line starts a brand new group.
        bool previousLineWasUserAgent = false;

        while (std::getline(stream, line)) {

            // Stripping trailing '\r' (CRLF files) and comments
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            auto commentPos = line.find('#');
            if (commentPos != std::string::npos) {
                line = line.substr(0, commentPos);
            }


            // Trimming leading/trailing whitespace
            auto start = line.find_first_not_of(" \t");
            if (start == std::string::npos) {
                previousLineWasUserAgent = false;
                continue;
            }
            auto end = line.find_last_not_of(" \t");
            line = line.substr(start, end - start + 1);


            // Splitting "Directive: value"
            auto colon = line.find(':');
            if (colon == std::string::npos) {
                continue;
            }
            std::string directive = line.substr(0, colon);
            std::string value     = line.substr(colon + 1);


            // Trimming the value
            auto valueStart = value.find_first_not_of(" \t");
            value = (valueStart == std::string::npos) ? "" : value.substr(valueStart);


            // Lower-casing the directive name for case-insensitive comparison
            std::string directiveLower = directive;
            std::transform(directiveLower.begin(), directiveLower.end(), directiveLower.begin(),
                [](unsigned char c) { return std::tolower(c); });


            if (directiveLower == "user-agent") {

                // Starting a new group only if this "User-agent:" does not directly
                // follow another one (i.e. it is not part of the same multi-UA group)
                if (!previousLineWasUserAgent) {
                    inWildcardGroup = false;
                }
                if (value == "*") {
                    inWildcardGroup = true;
                }
                previousLineWasUserAgent = true;

            } else if (directiveLower == "allow" || directiveLower == "disallow") {

                previousLineWasUserAgent = false;
                if (inWildcardGroup && !value.empty()) {
                    rules.push_back(RobotsRule{ directiveLower == "allow", value });
                }

            } else {
                previousLineWasUserAgent = false;
            }

        }

        return rules;

    }


    /**
     * @brief Checks whether a path matches a robots.txt pattern.
     * @param path    The path to test (e.g. "/fiche-technique/Peugeot/208.html").
     * @param pattern The robots.txt pattern to test against.
     * @return true if the path matches the pattern.
     */
    bool RobotsPolicy::matches(const std::string& path, const std::string& pattern) {

        // Building a regex from the robots.txt pattern:
        // - '*' means "any sequence of characters"
        // - a trailing '$' anchors the match to the end of the path
        // - every other regex-special character is escaped
        bool anchoredEnd = !pattern.empty() && pattern.back() == '$';
        std::string body = anchoredEnd ? pattern.substr(0, pattern.size() - 1) : pattern;

        std::string regexStr = "^";
        for (char c : body) {
            if (c == '*') {
                regexStr += ".*";
            } else if (std::string(".^$|()[]{}+?\\").find(c) != std::string::npos) {
                regexStr += '\\';
                regexStr += c;
            } else {
                regexStr += c;
            }
        }
        if (!anchoredEnd) {
            regexStr += ".*";
        }
        regexStr += "$";

        try {
            std::regex re(regexStr);
            return std::regex_match(path, re);
        } catch (const std::regex_error& e) {
            Logger::error("RobotsPolicy::matches() → invalid pattern \"{}\": {}", pattern, e.what());
            return false;
        }

    }

} // namespace CarScraper
