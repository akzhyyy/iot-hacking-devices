### **2. Legal Disclaimer (Required)**

  **Path:** `LEGAL.md`

  ```markdown
  # Legal & Responsible Use Policy

  ## ⚖️ Authorized Use Only

  All code, hardware designs, and documentation in this repository are provided **strictly for**:

  - Authorized penetration testing engagements (with signed scope/ROE)
  - Academic security research
  - Personal learning on devices you own
  - CTF competitions and authorized labs

  ## 🚫 Prohibited Use

  You may **NOT** use this code to:

  - Attack devices, networks, or systems without explicit written permission
  - Interfere with wireless communications in public/shared spaces (violates FCC Part 15 / ETSI regulations)
  - Deploy BadUSB/HID payloads on computers you do not own or have authorization to test
  - Bypass security controls for unauthorized access
  - Cause denial of service, data exfiltration, or system compromise outside authorized scope

  ## 📡 Radio Regulations

  **BLE jamming transmits on 2.4 GHz ISM band.**

  - FCC (USA): Intentional interference prohibited (47 CFR §15.5)
  - ETSI (EU): Similar restrictions under RED 2014/53/EU
  - Other jurisdictions: Check local spectrum regulations

  **Only operate in:**
  - Faraday cage / RF shielded enclosure
  - Screened room with proper attenuation
  - Fully isolated test environment

  ## 🛡️ Safe Testing Practices

  1. **Document scope** — Written permission with IP ranges, device serials, time windows
  2. **Isolate** — Use shielded environment for wireless attacks
  3. **Log everything** — Timestamp, target, action, result
  4. **Clean up** — Remove payloads, restore configs, revoke access
  5. **Report** — Deliver findings to asset owner with remediation guidance

  ## 📄 License

  Code: **MIT License** — Use, modify, distribute freely with attribution.

  Hardware designs: **CERN-OHL-P v2** — Open hardware license.

  **Neither license grants permission for illegal use.**

  ## 📞 Contact

  Security concerns or responsible disclosure: your.email@example.com

  ---

  *By using this repository, you acknowledge you have read, understood, and agree to comply with this policy.*

  ---
