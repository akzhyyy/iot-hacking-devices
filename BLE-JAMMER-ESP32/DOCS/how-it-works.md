 # BLE Jammer — Technical Deep-Dive

  ## 🎯 Attack Surface

  Bluetooth Low Energy (BLE) uses **3 advertising channels** (37, 38, 39) at 2402, 2426, 2480 MHz. Devices broadcast
  advertisements here; scanners listen.

  | Channel | Frequency | Purpose |
  |---------|-----------|---------|
  | 37 | 2402 MHz | Primary advertising |
  | 38 | 2426 MHz | Primary advertising |
  | 39 | 2480 MHz | Primary advertising |
  | 0-36 | 2404-2478 MHz | Data channels (connected) |

  ## 🔴 Mode 1: Beacon Flood

  **Goal:** Saturate scanner with fake advertisements → hide real devices, exhaust scanner resources.

  ### Technique
  1. Generate random MAC with valid OUI (Apple, Samsung, etc.)
  2. Craft minimal ADV_IND packet (non-connectable)
  3. Transmit at minimum interval (20ms = 50 packets/sec per channel)
  4. Rotate MAC every advertisement → appears as hundreds of devices

  ### Packet Structure (ADV_IND)
  Preamble (1) | Access Address (4) | PDU Header (2) | Payload (6-37) | CRC (3)
  - **PDU Type:** `0x00` (ADV_IND) or `0x03` (ADV_NONCONN_IND)
  - **TxAdd:** `1` (Random address)
  - **Payload:** Flags + Shortened Local Name

  ### ESP32 Implementation
  - `esp_ble_gap_config_adv_data_raw()` — raw packet control
  - `esp_ble_gap_set_rand_addr()` — rotate MAC per packet
  - `esp_bt_controller_set_channel()` — force specific channel

  ### Limitations
  - ESP32 single radio → one channel at a time
  - Regulatory: 10ms minimum interval (BLE spec) but firmware allows 20ms
  - Duty cycle limited by thermal/power

  ## 🔴 Mode 2: Connection Spam

  **Goal:** Exhaust target's connection table (max ~10-20 concurrent connections).

  ### Technique
  1. Scan for target devices (active scan)
  2. On discovery: `esp_ble_gap_create_conn()` with minimal interval
  3. Repeat until target rejects or table full
  4. Maintain connections with empty L2CAP traffic

  ### Target Impact
  - iOS: ~7 concurrent peripheral connections
  - Android: ~10-15 (varies)
  - IoT devices: Often 1-3 (single connection design)

  ## 🔴 Mode 3: Channel Hop

  **Goal:** Jam all 3 advertising channels sequentially.

  ### Technique
  - Hop every 100ms across channels 37, 38, 39
  - On each channel: transmit beacon flood for dwell time
  - Creates "barrage" across entire advertising spectrum

  ### Timing
  Channel 37 (100ms) → Channel 38 (100ms) → Channel 39 (100ms) → repeat
  Effective rate: ~3.3 channel hops/sec

  ## 🔴 Mode 4: Custom PDU Injection

  **Goal:** Transmit arbitrary link-layer packets for protocol fuzzing.

  ### Use Cases
  - Malformed PDU headers (length overflow, invalid type)
  - LL control procedure injection (CONNECTION_UPDATE, CHANNEL_MAP)
  - Privacy violation: directed advertisements to target RPA
  - Downgrade attacks: force legacy pairing

  ### Implementation
  ```cpp
  // Raw PDU construction
  uint8_t pdu[37] = {
    // PDU Header
    0x00,                    // Type: ADV_IND
    0x20,                    // Length: 32 bytes
    // Payload
    0x02, 0x01, 0x06,       // Flags
    // ... custom payload
  };
  esp_ble_gap_config_adv_data_raw(pdu, 37);

  🛡️ Detection & Mitigation

  ┌────────────────────────────────────────┬─────────────────────────────────────────────┐
  │               Detection                │                 Mitigation                  │
  ├────────────────────────────────────────┼─────────────────────────────────────────────┤
  │ RSSI anomaly (many devices same power) │ Rate-limit scan results                     │
  ├────────────────────────────────────────┼─────────────────────────────────────────────┤
  │ MAC rotation (OUI valid, NIC random)   │ Track by RSSI + timing fingerprint          │
  ├────────────────────────────────────────┼─────────────────────────────────────────────┤
  │ Channel hopping pattern                │ Monitor channel dwell time                  │
  ├────────────────────────────────────────┼─────────────────────────────────────────────┤
  │ Connection table exhaustion            │ Connection rate limiting, auth before alloc │
  └────────────────────────────────────────┴─────────────────────────────────────────────┘

  📊 Performance Metrics (ESP32 DevKit V1)

  ┌──────────────┬──────────────┬────────────────┬────────┬─────────┐
  │     Mode     │ Packets/sec  │    Channels    │ Power  │  Range  │
  ├──────────────┼──────────────┼────────────────┼────────┼─────────┤
  │ Beacon Flood │ ~1,500       │ 1 at a time    │ ~120mA │ 50-100m │
  ├──────────────┼──────────────┼────────────────┼────────┼─────────┤
  │ Channel Hop  │ ~500/channel │ 3 (sequential) │ ~130mA │ 50-100m │
  ├──────────────┼──────────────┼────────────────┼────────┼─────────┤
  │ Conn Spam    │ ~10 conn/sec │ N/A            │ ~150mA │ 10-30m  │
  └──────────────┴──────────────┴────────────────┴────────┴─────────┘

  🔬 Research Extensions

  1. GATT Fuzzing — Connect then send malformed ATT packets
  2. LL Privacy Bypass — Resolve RPA via side-channel
  3. Mesh Flood — Inject into Bluetooth Mesh networks
  4. AoA/AoD Spoofing — Fake direction finding (ESP32-C3 + antenna array)

  ---

  📚 References

  - Bluetooth Core Spec v5.4 (https://www.bluetooth.com/specifications/specs/core-specification/) — Vol 6, Part B (Link
    Layer)
  - ESP32 BLE Controller Guide
    (https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/controller_vhci.html)
  - BLE Advertising Channel Attacks (https://www.usenix.org/conference/woot18/presentation/li) — LI et al., WOOT'18
  - BlueFrag (https://github.com/ernw/bluefrag) — CVE-2020-0022 reference

  ---
