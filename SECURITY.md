# Security Policy

## Supported Versions

| Version | Supported          |
|---------|--------------------|
| 1.0.x   | ✅ Actively maintained |
| < 1.0   | ❌ Not supported   |

---

## Legal Disclaimer

> ⚠️ **AVADON is intended for authorized security testing and educational purposes only.**
>
> Unauthorized use of this tool against systems you do not own or have explicit written
> permission to test is **illegal** and may violate local, national, or international law.
>
> The author (**Oppressor2045**) assumes **no liability** for any misuse, damage, or legal
> consequences arising from the use of this software.
>
> **You are solely responsible for ensuring your use of AVADON is lawful.**

---

## Responsible Use

AVADON is designed for:

- ✅ Authorized penetration testing on systems you own
- ✅ CTF (Capture The Flag) competitions
- ✅ Educational research in controlled environments
- ✅ Network auditing with explicit written permission
- ❌ Unauthorized scanning of third-party systems
- ❌ Any illegal or malicious activity

---

## Reporting a Vulnerability

If you discover a security vulnerability in AVADON, please follow responsible disclosure:

### 1. Do NOT open a public GitHub Issue

Public disclosure before a fix is available puts users at risk.

### 2. Contact privately

Reach out via GitHub private message to **Oppressor2045** with the following details:

```
Subject : [AVADON SECURITY] <brief description>

- Version affected  :
- Vulnerability type: (e.g. buffer overflow, memory leak, RCE)
- Steps to reproduce:
- Potential impact  :
- Suggested fix     : (optional)
```

### 3. Response timeline

| Stage | Timeframe |
|-------|-----------|
| Initial acknowledgement | Within 72 hours |
| Vulnerability assessment | Within 7 days |
| Patch release | Within 30 days (severity dependent) |
| Public disclosure | After patch is released |

---

## Known Security Considerations

| Area | Detail |
|------|--------|
| Raw socket | SYN scan requires root — use in isolated environments only |
| No rate limiting | High thread counts may trigger IDS/IPS on target networks |
| Banner grabbing | Establishes real TCP connections — leaves logs on target |
| UDP scan | Basic probe only — results may be unreliable |

---

## Scope

The following are considered **in scope** for vulnerability reports:

- Memory safety issues (buffer overflow, use-after-free, etc.)
- Privilege escalation via raw socket misuse
- Unintended code execution
- Crash / denial of service in the scanner engine

The following are **out of scope**:

- Issues requiring physical access to the machine
- Social engineering attacks
- Third-party library vulnerabilities unrelated to AVADON

---

*This security policy follows the [GitHub Security Advisory](https://docs.github.com/en/code-security/security-advisories) guidelines.*
