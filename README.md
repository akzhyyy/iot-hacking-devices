
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

  
