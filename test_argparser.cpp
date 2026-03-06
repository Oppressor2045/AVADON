/*
 * test_argparser.cpp — ArgParser unit tests
 * AVADON Network Reconnaissance Framework
 */

#include "../include/argparser.hpp"
#include "test_framework.hpp"

void test_argparser() {
    // ── Default values ──────────────────────────────────────
    {
        const char* argv[] = { "avadon", "192.168.1.1" };
        ArgParser p(2, const_cast<char**>(argv));
        ASSERT_TRUE(p.parse());
        ASSERT_EQ(p.get_target(),     std::string("192.168.1.1"));
        ASSERT_EQ(p.get_port_range(), std::string("1-1024"));
        ASSERT_EQ(p.get_mode(),       std::string("connect"));
        ASSERT_EQ(p.get_threads(),    100u);
        ASSERT_EQ(p.get_timeout(),    1000u);
        ASSERT_FALSE(p.is_verbose());
        ASSERT_FALSE(p.is_stealth());
        PASS("default values");
    }

    // ── -t flag ─────────────────────────────────────────────
    {
        const char* argv[] = { "avadon", "-t", "10.0.0.1" };
        ArgParser p(3, const_cast<char**>(argv));
        ASSERT_TRUE(p.parse());
        ASSERT_EQ(p.get_target(), std::string("10.0.0.1"));
        PASS("-t flag");
    }

    // ── -p port range ───────────────────────────────────────
    {
        const char* argv[] = { "avadon", "-t", "10.0.0.1", "-p", "22,80,443" };
        ArgParser p(5, const_cast<char**>(argv));
        ASSERT_TRUE(p.parse());
        ASSERT_EQ(p.get_port_range(), std::string("22,80,443"));
        PASS("-p port range");
    }

    // ── -m mode ─────────────────────────────────────────────
    {
        const char* argv[] = { "avadon", "-t", "10.0.0.1", "-m", "syn" };
        ArgParser p(5, const_cast<char**>(argv));
        ASSERT_TRUE(p.parse());
        ASSERT_EQ(p.get_mode(), std::string("syn"));
        PASS("-m mode");
    }

    // ── -T threads ──────────────────────────────────────────
    {
        const char* argv[] = { "avadon", "-t", "10.0.0.1", "-T", "200" };
        ArgParser p(5, const_cast<char**>(argv));
        ASSERT_TRUE(p.parse());
        ASSERT_EQ(p.get_threads(), 200u);
        PASS("-T threads");
    }

    // ── --verbose / --stealth flags ─────────────────────────
    {
        const char* argv[] = { "avadon", "-t", "10.0.0.1", "-v", "-s" };
        ArgParser p(5, const_cast<char**>(argv));
        ASSERT_TRUE(p.parse());
        ASSERT_TRUE(p.is_verbose());
        ASSERT_TRUE(p.is_stealth());
        PASS("--verbose --stealth");
    }

    // ── --help ──────────────────────────────────────────────
    {
        const char* argv[] = { "avadon", "--help" };
        ArgParser p(2, const_cast<char**>(argv));
        ASSERT_TRUE(p.parse());
        ASSERT_TRUE(p.show_help());
        PASS("--help flag");
    }

    // ── No arguments → parse fails ──────────────────────────
    {
        const char* argv[] = { "avadon" };
        ArgParser p(1, const_cast<char**>(argv));
        ASSERT_FALSE(p.parse());
        PASS("no args returns false");
    }

    // ── Output / format flags ───────────────────────────────
    {
        const char* argv[] = { "avadon", "-t", "10.0.0.1",
                                "-o", "report", "-f", "json" };
        ArgParser p(7, const_cast<char**>(argv));
        ASSERT_TRUE(p.parse());
        ASSERT_EQ(p.get_output_file(),   std::string("report"));
        ASSERT_EQ(p.get_output_format(), std::string("json"));
        PASS("-o / -f flags");
    }
}
