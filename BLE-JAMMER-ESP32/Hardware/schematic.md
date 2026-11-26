 ```markdown
  # ESP32 BLE Jammer — Hardware

  ## 📐 Schematic

                  ┌─────────────┐
            3V3 ──┤             │
                  │   ESP32     │
            GND ──┤  DevKit V1  │
                  │             │
         GPIO 2 ──┤  (LED)      │  ← Status LED
                  │             │
         GPIO 0 ──┤  (BOOT)     │  ← Mode select (optional)
                  └─────────────┘

  **Minimal connections:**
  - **USB** — Power + Serial (flashing/monitor)
  - **GPIO 2** — Onboard blue LED (activity indicator)
  - **EN (Reset)** — Optional external reset button
  - **GPIO 0** — Boot mode (hold low during power-on for download mode)

  ## 📦 Bill of Materials

  | Qty | Part | Spec | Est. Cost |
  |-----|------|------|-----------|
  | 1 | ESP32 DevKit V1 | ESP32-WROOM-32, USB-C | $5-8 |
  | 1 | USB-C Cable | Data + Power | $2 |
  | 1 | Breadboard | 400+ tie points | $3 |
  | 4 | Jumper Wires | Male-Male | $1 |
  | 1 | **Optional: SMA Antenna** | 2.4 GHz, U.FL to SMA | $3 |
  | 1 | **Optional: LiPo Battery** | 3.7V 500mAh + TP4056 | $4 |

  **Total: ~$11-15** (basic) | **~$18-22** (with antenna + battery)

  ## 🔧 Optional Enhancements

  ### External Antenna (Range + Directional)
  ESP32 U.FL ── u.FL to SMA pigtail ── 2.4 GHz 5dBi Omni
                                └── OR ── 2.4 GHz 14dBi Yagi (directional)
  - Requires ESP32-WROOM-32U (U.FL connector) or ESP32-C3 with external antenna
  - Solder 0Ω resistor to switch from PCB to U.FL (check schematic)

  ### Battery Power (Portable)
  LiPo 3.7V ── TP4056 Charger ── ESP32 5V pin (via LDO) or 3.3V pin (bypass LDO)
  - Add power switch on battery lead
  - Monitor voltage via ADC (GPIO 34-39)

  ### Mode Select Switch
  GPIO 0 ── SPST Switch ── GND
  GPIO 4 ── SPST Switch ── GND
  GPIO 5 ── SPST Switch ── GND
  - 3 switches = 8 modes (binary)
  - Read at boot, set `currentMode`

  ## 🛡️ Faraday Cage (Required for Legal Testing)

  Option 1: Commercial
  - Ramsey STE3000 (~$2,000)
  - ETS-Lindgren (~$5,000+)

  Option 2: DIY (<$100)
  - Metal ammo can + conductive gasket
  - Copper mesh + wooden frame
  - Metal trash can with lid (surprisingly effective)

  Option 3: RF Shielded Box
  - Hammond 1590 series + copper tape seams
  - ~$50-80

  Test: Place phone inside → call it → should not ring

  ## 📏 Antenna Placement

  | Setup | Orientation | Use Case |
  |-------|-------------|----------|
  | PCB antenna | Vertical | Omnidirectional, general |
  | External omni | Vertical | Max range, all directions |
  | Yagi | Horizontal, pointed | Targeted, long-range |
  | Near target | Touching/adjacent | Max coupling, minimal power |

  ---

  ## ⚡ Power Consumption

  | Mode | Current (avg) | 500mAh Battery Life |
  |------|---------------|---------------------|
  | Beacon Flood | 120 mA | ~4 hours |
  | Channel Hop | 130 mA | ~3.8 hours |
  | Conn Spam | 150 mA | ~3.3 hours |
  | Deep Sleep | 10 µA | Months |

  **Add `esp_deep_sleep_start()`** for battery-powered deployments.

  ---
