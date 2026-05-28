// Imports
#include "io/HttpClient/HttpClient.hpp"

#include <iostream>
#include <cassert>
#include <string>


// ============================================================================
// Helpers
// ============================================================================

static int  s_passed = 0;
static int  s_failed = 0;

static void check(bool condition, const std::string& label)
{
    if (condition) {
        std::cout << "  [PASS] " << label << "\n";
        ++s_passed;
    } else {
        std::cout << "  [FAIL] " << label << "\n";
        ++s_failed;
    }
}


// ============================================================================
// Tests
// ============================================================================

// --- Test 1 : requête GET simple -------------------------------------------
static void test_GetReturns200()
{
    std::cout << "\n[TEST] GET example.com\n";

    CarScraper::HttpClient client;

    CarScraper::AntiBlockPolicy policy;
    policy.minDelayBetweenRequests = std::chrono::milliseconds(100);
    policy.maxDelayBetweenRequests = std::chrono::milliseconds(200);
    policy.maxRetries              = 1;
    client.setPolicy(policy);

    CarScraper::HttpResponse response = client.get("https://example.com");

    check(response.success,               "success == true");
    check(response.statusCode == 200,     "statusCode == 200");
    check(!response.body.empty(),         "body non vide");
    check(!response.effectiveUrl.empty(), "effectiveUrl non vide");

    std::cout << "  [INFO] Taille du body : " << response.body.size() << " octets\n";
    std::cout << "  [INFO] URL effective  : " << response.effectiveUrl << "\n";
}


// --- Test 2 : le body contient bien du HTML --------------------------------
static void test_BodyContainsHtml()
{
    std::cout << "\n[TEST] Body contient du HTML\n";

    CarScraper::HttpClient client;
    CarScraper::HttpResponse response = client.get("https://example.com");

    check(response.body.find("<html")  != std::string::npos, "contient <html");
    check(response.body.find("</html>") != std::string::npos, "contient </html>");
}


// --- Test 3 : URL inexistante retourne une erreur --------------------------
static void test_InvalidUrlFails()
{
    std::cout << "\n[TEST] URL invalide\n";

    CarScraper::HttpClient client;

    CarScraper::AntiBlockPolicy policy;
    policy.maxRetries = 0;   // pas de retry pour ne pas ralentir le test
    client.setPolicy(policy);

    CarScraper::HttpResponse response = client.get("https://this-domain-does-not-exist-xyz.com");

    check(!response.success,           "success == false");
    check(!response.errorMsg.empty(),  "errorMsg non vide");

    std::cout << "  [INFO] Erreur : " << response.errorMsg << "\n";
}


// --- Test 4 : compteur de requêtes -----------------------------------------
static void test_RequestCounter()
{
    std::cout << "\n[TEST] Compteur de requêtes\n";

    CarScraper::HttpClient client;

    CarScraper::AntiBlockPolicy policy;
    policy.minDelayBetweenRequests = std::chrono::milliseconds(50);
    policy.maxDelayBetweenRequests = std::chrono::milliseconds(100);
    policy.maxRetries              = 0;
    client.setPolicy(policy);

    client.get("https://example.com");
    client.get("https://example.com");

    check(client.getTotalRequests() == 2, "totalRequests == 2 après 2 GET");

    std::cout << "  [INFO] Total requêtes : " << client.getTotalRequests() << "\n";
}


// --- Test 5 : affichage des 500 premiers caractères du HTML ----------------
static void test_PrintRawHtml()
{
    std::cout << "\n[TEST] Aperçu HTML brut\n";

    CarScraper::HttpClient client;
    CarScraper::HttpResponse response = client.get("https://example.com");

    if (!response.body.empty()) {
        std::string preview = response.body.substr(0, 500);
        std::cout << "  [INFO] HTML (500 premiers caractères) :\n";
        std::cout << "  ----------------------------------------\n";
        std::cout << preview << "\n";
        std::cout << "  ----------------------------------------\n";
        check(true, "affichage OK");
    } else {
        check(false, "body vide — affichage impossible");
    }
}


// ============================================================================
// Entry point
// ============================================================================

int main()
{
    std::cout << "================================================\n";
    std::cout << "  CarScraper — HttpClient tests\n";
    std::cout << "================================================\n";

    test_GetReturns200();
    test_BodyContainsHtml();
    test_InvalidUrlFails();
    test_RequestCounter();
    test_PrintRawHtml();

    std::cout << "\n================================================\n";
    std::cout << "  Résultat : " << s_passed << " passés / "
              << (s_passed + s_failed) << " total";

    if (s_failed > 0)
        std::cout << "  (" << s_failed << " ÉCHEC(S))";

    std::cout << "\n================================================\n";

    return s_failed > 0 ? 1 : 0;
}
