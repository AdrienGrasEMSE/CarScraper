/**
 * @file RobotsPolicy.hpp
 * 
 * @brief This file declares the RobotsPolicy class, which represents a robots.txt checker.
 *
 * @author Adrien GRAS
 * @date 2026-07-30
 */


// For only one include
#pragma once


// Imports
#include "core/models/Entity.hpp"
#include "io/httpClient/HttpClient.hpp"
#include <string>
#include <vector>
#include <map>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Struct RobotsRule
    // =========================================================================

    /**
     * @brief Represents a single Allow/Disallow rule from a robots.txt "User-agent: *" group.
     */
    struct RobotsRule {

        bool         _isAllow = false;    ///< true for "Allow", false for "Disallow"   | ex: false
        std::string  _pattern;            ///< Raw robots.txt pattern (may contain * and $) | ex: "/admin/*"

    };


    /**
     * @class RobotsPolicy
     *
     * @brief Fetches, caches and interprets robots.txt files for the generic "User-agent: *" group.
     * @details Only the "*" group is considered: this project always sends realistic browser
     *          User-Agent strings, so it never matches a named bot group (e.g. "GPTBot",
     *          "ClaudeBot"). Rule matching follows the "most specific pattern wins" convention
     *          (RFC 9309): among all matching rules for a path, the one with the longest
     *          pattern applies, regardless of file order.
     *
     * | Attribute     | Type                              | Example                      | Unit |
     * |---------------|------------------------------------|-------------------------------|------|
     * | _rulesByHost  | map<string, vector<RobotsRule>>   | {"www.largus.fr": [...]}      | -    |
     * | _loadedHosts  | map<string, bool>                 | {"www.largus.fr": true}       | -    |
     */
    class RobotsPolicy : public Entity {

        // =========================================================================
        // Private data and methods
        // =========================================================================
        private:

            // -------------------------------------------------------------------------
            // Cache
            // -------------------------------------------------------------------------
            std::map<std::string, std::vector<RobotsRule>>  _rulesByHost;    ///< Parsed "*" group rules, cached per host
            std::map<std::string, bool>                     _loadedHosts;   ///< Whether robots.txt has already been fetched for a host

            // -------------------------------------------------------------------------
            // Collaborators
            // -------------------------------------------------------------------------
            HttpClient  _client;    ///< Dedicated client used only to fetch robots.txt files


            // -------------------------------------------------------------------------
            // Internal logic
            // -------------------------------------------------------------------------

            /**
             * @brief Downloads and parses the robots.txt file for a host, populating the cache.
             * @details Fail-open: if the file cannot be fetched (network error, 404, ...),
             *          the host is cached with an empty rule set, meaning everything is allowed
             *          (same convention as Googlebot).
             * @param host The domain to fetch robots.txt for (e.g. "www.largus.fr").
             */
            void loadRules(const std::string& host);


            /**
             * @brief Parses the raw content of a robots.txt file, keeping only the "*" group.
             * @param content The raw robots.txt content.
             * @return The list of Allow/Disallow rules found in the "User-agent: *" group.
             */
            static std::vector<RobotsRule> parseWildcardGroup(const std::string& content);


            /**
             * @brief Checks whether a path matches a robots.txt pattern.
             * @details Converts the pattern ("*" wildcards, optional trailing "$" anchor)
             *          into a regular expression and matches it against the path.
             * @param path    The path to test (e.g. "/fiche-technique/Peugeot/208.html").
             * @param pattern The robots.txt pattern to test against.
             * @return true if the path matches the pattern.
             */
            static bool matches(const std::string& path, const std::string& pattern);


        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default constructor : define prefix as ROBOTS-POLICY
             */
            RobotsPolicy();


            // -------------------------------------------------------------------------
            // Main logic
            // -------------------------------------------------------------------------

            /**
             * @brief Checks whether a given path may be fetched on a given host.
             * @details Fetches and caches the robots.txt for the host on first call for
             *          that host. Only the "*" group is evaluated, since this project
             *          always identifies itself with realistic browser User-Agent strings.
             *          Among matching rules, the one with the longest (most specific)
             *          pattern wins.
             * @param host The domain (e.g. "www.largus.fr").
             * @param path The path to check (e.g. "/fiche-technique/Peugeot/208.html").
             * @return true if allowed (or if robots.txt is unreachable/absent), false if disallowed.
             */
            bool isAllowed(const std::string& host, const std::string& path);


            /**
             * @brief Clears the cached rules for all hosts, forcing a re-fetch on next check.
             */
            void clearCache();

    };

} // namespace CarScraper
