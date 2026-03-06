#pragma once

/*
 * test_framework.hpp — Minimal test framework
 * AVADON Network Reconnaissance Framework
 *
 * Lightweight alternative to Google Test / Catch2.
 * Zero external dependencies — just include and go.
 */

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <stdexcept>
#include <sstream>

// ── ANSI colors ─────────────────────────────────────────────
#define T_RESET  "\033[0m"
#define T_GREEN  "\033[32m"
#define T_RED    "\033[31m"
#define T_CYAN   "\033[36m"
#define T_YELLOW "\033[33m"
#define T_BOLD   "\033[1m"

// ── Test registry ───────────────────────────────────────────
struct TestCase {
    std::string name;
    std::function<void()> fn;
};

inline std::vector<TestCase>& test_registry() {
    static std::vector<TestCase> reg;
    return reg;
}

struct TestRegistrar {
    TestRegistrar(const std::string& name, std::function<void()> fn) {
        test_registry().push_back({name, fn});
    }
};

// ── Register a test function ────────────────────────────────
#define REGISTER_TEST(fn) \
    static TestRegistrar _reg_##fn(#fn, fn)

// ── Assertion macros ────────────────────────────────────────
#define ASSERT_TRUE(expr) \
    do { \
        if (!(expr)) { \
            std::ostringstream _oss; \
            _oss << "ASSERT_TRUE failed: (" << #expr << ")  " \
                 << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(_oss.str()); \
        } \
    } while(0)

#define ASSERT_FALSE(expr) \
    do { \
        if ((expr)) { \
            std::ostringstream _oss; \
            _oss << "ASSERT_FALSE failed: (" << #expr << ")  " \
                 << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(_oss.str()); \
        } \
    } while(0)

#define ASSERT_EQ(a, b) \
    do { \
        if (!((a) == (b))) { \
            std::ostringstream _oss; \
            _oss << "ASSERT_EQ failed: (" << #a << " == " << #b << ")  " \
                 << "got [" << (a) << "] vs [" << (b) << "]  " \
                 << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(_oss.str()); \
        } \
    } while(0)

#define ASSERT_NEQ(a, b) \
    do { \
        if ((a) == (b)) { \
            std::ostringstream _oss; \
            _oss << "ASSERT_NEQ failed: (" << #a << " != " << #b << ")  " \
                 << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(_oss.str()); \
        } \
    } while(0)

#define ASSERT_GT(a, b) \
    do { \
        if (!((a) > (b))) { \
            std::ostringstream _oss; \
            _oss << "ASSERT_GT failed: " << (a) << " > " << (b) \
                 << "  " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(_oss.str()); \
        } \
    } while(0)

// ── Pass marker (prints inline result) ─────────────────────
#define PASS(msg) \
    std::cout << "  " << T_GREEN << "[PASS]" << T_RESET \
              << " " << (msg) << "\n"

// ── Test runner ─────────────────────────────────────────────
inline int run_all_tests() {
    int passed = 0, failed = 0;

    std::cout << T_BOLD << T_CYAN
              << "\n═══════════════════════════════════════\n"
              << "  AVADON — TEST SUITE\n"
              << "═══════════════════════════════════════\n"
              << T_RESET;

    for (auto& tc : test_registry()) {
        std::cout << T_BOLD << "\n[TEST] " << tc.name << T_RESET << "\n";
        try {
            tc.fn();
            ++passed;
            std::cout << T_GREEN << "  ✓ PASSED\n" << T_RESET;
        } catch (const std::exception& e) {
            ++failed;
            std::cout << T_RED << "  ✗ FAILED: " << e.what() << "\n" << T_RESET;
        }
    }

    std::cout << T_BOLD << T_CYAN
              << "\n═══════════════════════════════════════\n"
              << T_RESET;

    if (failed == 0) {
        std::cout << T_GREEN << T_BOLD
                  << "  ALL TESTS PASSED  "
                  << "(" << passed << "/" << (passed + failed) << ")\n"
                  << T_RESET;
    } else {
        std::cout << T_RED << T_BOLD
                  << "  " << failed << " TEST(S) FAILED  "
                  << "(" << passed << "/" << (passed + failed) << " passed)\n"
                  << T_RESET;
    }

    std::cout << T_BOLD << T_CYAN
              << "═══════════════════════════════════════\n\n"
              << T_RESET;

    return failed == 0 ? 0 : 1;
}
