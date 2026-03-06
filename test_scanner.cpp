/*
 * test_scanner.cpp — Scanner unit tests
 * AVADON Network Reconnaissance Framework
 *
 * Tests port parsing, CIDR expansion, service mapping,
 * and banner sanitization — no network calls needed.
 */

#include "../include/scanner.hpp"
#include "../include/logger.hpp"
#include "test_framework.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

// ── Expose private helpers via a test subclass ──────────────
class ScannerTest : public Scanner {
public:
    ScannerTest(const ScanConfig& cfg, Logger& log)
        : Scanner(cfg, log) {}

    // Re-expose private methods for testing
    std::vector<uint16_t> pub_parse_ports()   const { return parse_ports();   }
    std::vector<std::string> pub_resolve()    const { return resolve_targets(); }
    std::string pub_service(uint16_t p, const std::string& proto) const {
        return map_service(p, proto);
    }
};

// ── Helper: build a minimal config ─────────────────────────
static ScanConfig make_config(const std::string& target,
                               const std::string& ports = "1-10") {
    ScanConfig cfg;
    cfg.target     = target;
    cfg.port_range = ports;
    cfg.mode       = "connect";
    cfg.threads    = 10;
    cfg.timeout_ms = 500;
    return cfg;
}

// ── Port parser tests ───────────────────────────────────────
void test_port_parser() {
    Logger log;

    // Single port
    {
        auto cfg = make_config("127.0.0.1", "80");
        ScannerTest s(cfg, log);
        auto ports = s.pub_parse_ports();
        ASSERT_EQ(ports.size(), 1u);
        ASSERT_EQ(ports[0], uint16_t(80));
        PASS("single port: \"80\"");
    }

    // Comma list
    {
        auto cfg = make_config("127.0.0.1", "22,80,443");
        ScannerTest s(cfg, log);
        auto ports = s.pub_parse_ports();
        ASSERT_EQ(ports.size(), 3u);
        ASSERT_EQ(ports[0], uint16_t(22));
        ASSERT_EQ(ports[1], uint16_t(80));
        ASSERT_EQ(ports[2], uint16_t(443));
        PASS("comma list: \"22,80,443\"");
    }

    // Range
    {
        auto cfg = make_config("127.0.0.1", "1-5");
        ScannerTest s(cfg, log);
        auto ports = s.pub_parse_ports();
        ASSERT_EQ(ports.size(), 5u);
        ASSERT_EQ(ports[0], uint16_t(1));
        ASSERT_EQ(ports[4], uint16_t(5));
        PASS("range: \"1-5\"");
    }

    // Mixed range + list
    {
        auto cfg = make_config("127.0.0.1", "22,80-82,443");
        ScannerTest s(cfg, log);
        auto ports = s.pub_parse_ports();
        // 22, 80, 81, 82, 443  = 5 ports
        ASSERT_EQ(ports.size(), 5u);
        ASSERT_EQ(ports[0], uint16_t(22));
        ASSERT_EQ(ports[1], uint16_t(80));
        ASSERT_EQ(ports[2], uint16_t(81));
        ASSERT_EQ(ports[3], uint16_t(82));
        ASSERT_EQ(ports[4], uint16_t(443));
        PASS("mixed: \"22,80-82,443\"");
    }

    // Large range count
    {
        auto cfg = make_config("127.0.0.1", "1-1024");
        ScannerTest s(cfg, log);
        auto ports = s.pub_parse_ports();
        ASSERT_EQ(ports.size(), 1024u);
        PASS("range count: \"1-1024\" → 1024 ports");
    }
}

// ── CIDR expansion tests ────────────────────────────────────
void test_cidr_expand() {
    Logger log;

    // /30 → 2 usable hosts
    {
        auto cfg = make_config("192.168.1.0/30");
        ScannerTest s(cfg, log);
        auto hosts = s.pub_resolve();
        ASSERT_EQ(hosts.size(), 2u);
        ASSERT_EQ(hosts[0], std::string("192.168.1.1"));
        ASSERT_EQ(hosts[1], std::string("192.168.1.2"));
        PASS("CIDR /30 → 2 hosts");
    }

    // /24 → 254 usable hosts
    {
        auto cfg = make_config("10.0.0.0/24");
        ScannerTest s(cfg, log);
        auto hosts = s.pub_resolve();
        ASSERT_EQ(hosts.size(), 254u);
        PASS("CIDR /24 → 254 hosts");
    }

    // Single IP (no CIDR) → 1 host
    {
        auto cfg = make_config("127.0.0.1");
        ScannerTest s(cfg, log);
        auto hosts = s.pub_resolve();
        ASSERT_EQ(hosts.size(), 1u);
        ASSERT_EQ(hosts[0], std::string("127.0.0.1"));
        PASS("single IP → 1 host");
    }
}

// ── Service name mapping tests ──────────────────────────────
void test_service_map() {
    Logger log;
    auto cfg = make_config("127.0.0.1");
    ScannerTest s(cfg, log);

    ASSERT_EQ(s.pub_service(22,    "tcp"), std::string("ssh"));
    PASS("port 22 → ssh");

    ASSERT_EQ(s.pub_service(80,    "tcp"), std::string("http"));
    PASS("port 80 → http");

    ASSERT_EQ(s.pub_service(443,   "tcp"), std::string("https"));
    PASS("port 443 → https");

    ASSERT_EQ(s.pub_service(3306,  "tcp"), std::string("mysql"));
    PASS("port 3306 → mysql");

    ASSERT_EQ(s.pub_service(3389,  "tcp"), std::string("rdp"));
    PASS("port 3389 → rdp");

    ASSERT_EQ(s.pub_service(9999,  "tcp"), std::string("unknown"));
    PASS("port 9999 → unknown");
}
