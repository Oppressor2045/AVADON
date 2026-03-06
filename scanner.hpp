#pragma once

/*
 * scanner.hpp — Core scanning engine
 * AVADON Network Reconnaissance Framework
 */

#include <csignal>
#include <string>
#include <vector>
#include <atomic>
#include <cstdint>
#include "logger.hpp"

enum class PortState { OPEN, CLOSED, FILTERED, UNKNOWN };
enum class ScanMode  { TCP_CONNECT, TCP_SYN, UDP, PING, FULL };

struct PortResult {
    uint16_t    port;
    PortState   state      = PortState::UNKNOWN;
    std::string protocol   = "tcp";
    std::string service    = "unknown";
    std::string banner;
    uint32_t    latency_us = 0;
};

struct HostResult {
    std::string             ip;
    std::string             hostname;
    std::string             os_guess;
    std::string             mac;
    bool                    is_up = false;
    std::vector<PortResult> ports;
};

struct ScanResult {
    std::vector<HostResult> hosts;
    uint32_t total_hosts = 0;
    uint32_t hosts_up    = 0;
    uint32_t open_ports  = 0;
    double   elapsed_sec = 0.0;
};

struct ScanConfig {
    std::string target;
    std::string port_range = "1-1024";
    std::string mode       = "connect";
    uint32_t    timeout_ms = 1000;
    uint32_t    threads    = 100;
    bool        stealth    = false;
    bool        os_detect  = false;
    bool        svc_detect = false;
};

class Scanner {
public:
    explicit Scanner(const ScanConfig& config, Logger& logger);
    ~Scanner();

    bool       init();
    ScanResult run(volatile sig_atomic_t& g_running);
    void       print_results (const ScanResult& result) const;
    void       export_results(const ScanResult& result,
                               const std::string& filepath,
                               const std::string& format) const;

protected:
    // Protected so ScannerTest subclass can call them directly
    std::vector<std::string> resolve_targets() const;
    std::vector<uint16_t>    parse_ports()     const;
    std::string              map_service(uint16_t port,
                                          const std::string& proto) const;

    ScanConfig m_config;
    Logger&    m_logger;

private:
    int m_raw_sock = -1;

    HostResult scan_host(const std::string& ip,
                          const std::vector<uint16_t>& ports);

    PortResult tcp_connect_scan(const std::string& ip, uint16_t port);
    PortResult tcp_syn_scan    (const std::string& ip, uint16_t port);
    PortResult udp_scan        (const std::string& ip, uint16_t port);

    bool        ping_host      (const std::string& ip);
    std::string grab_banner    (const std::string& ip, uint16_t port);
    std::string reverse_dns    (const std::string& ip) const;
    std::string os_fingerprint (const std::string& ip) const;

    static std::string state_str(PortState s);
    static std::string colorize (PortState s, const std::string& text);
};
