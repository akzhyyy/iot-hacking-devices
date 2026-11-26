
 # IoT Hacking Devices

  **Custom hardware tools for wireless security research** — ESP32 BLE jammer, Digispark ATtiny85 BadUSB, and protocol
  analysis utilities.

  > ⚖️ **For authorized testing only.** See [LEGAL.md](LEGAL.md).

  ---

  ## 📦 Projects

  | Project | Chip | Category | Status |
  |---------|------|----------|--------|
  | [BLE Jammer](ble-jammer-esp32/) | ESP32 | Wireless DoS / Protocol Fuzzing | ✅ Complete |
  | [Digispark BadUSB](digispark-attiny85/) | ATtiny85 | HID Injection / Physical Access | ✅ Complete |

  ---

  ## 🛠 Hardware Used

  | Device | Purpose |
  |--------|---------|
  | ESP32 DevKit V1 / ESP32-C3 | BLE jamming, beacon spam, raw PDU injection |
  | Digispark ATtiny85 (Rev 3) | BadUSB HID payloads, keystroke injection |
  | USB-UART (CP2102/CH340) | Firmware flashing, debug logs |

  ---

  ## 📚 Research Notes

  - [BLE Protocol Analysis](research/ble-protocol-analysis.md) — GATT/ATT fuzzing, link layer anomalies
  - [ESP32 Security Notes](research/esp32-security-notes.md) — Secure boot, flash encryption, side-channels
  - [ATtiny85 Limitations](research/attiny85-limitations.md) — Timing constraints, EDR evasion techniques

  ---

  ## 🚀 Quick Start

  ### ESP32 BLE Jammer
  ```bash
  # 1. Install ESP32 board support in Arduino IDE
  # Board Manager URL: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  # 2. Open ble-jammer-esp32/src/ble_jammer.ino
  # 3. Select board: ESP32 Dev Module → Upload

  Digispark BadUSB

  # 1. Install Digispark board support
  # Board Manager URL: http://digistump.com/package_digistump_index.json
  # 2. Open digispark-attiny85/badusb/reverse-shell.ino
  # 3. Select board: Digispark (Default - 16.5MHz) → Upload
  # 4. Plug in when prompted (60-second window)

  ---

  📂 Repository Structure

  iot-hacking-devices/
  ├── ble-jammer-esp32/          # ESP32 BLE jammer & fuzzer
  │   ├── src/                   # Arduino/ESP-IDF source
  │   ├── hardware/              # Schematics, BOM, PCB
  │   └── docs/                  # Deep-dive, legal, demo
  ├── digispark-attiny85/        # ATtiny85 BadUSB payloads
  │   ├── badusb/                # HID attack payloads
  │   ├── hid-attacks/           # Ducky converter + payload library
  │   └── hardware/              # Pinout, wiring
  ├── research/                  # Protocol analysis & findings
  └── LEGAL.md                   # Responsible use policy

  ---

  🏷️ Tags

  esp32 attiny85 ble bluetooth badusb hid iot-security wireless penetration-testing hardware-hacking

  ---

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
