
 # BLE Protocol Analysis — Research Notes

  ## 🎯 Scope

  Analyzing BLE Link Layer (LL) and Generic Access Profile (GAP) for:
  - Jamming effectiveness
  - Fuzzing surface
  - Privacy bypass
  - Implementation flaws in ESP32/Zephyr/BlueZ stacks

  ---

  ## 📡 Link Layer (LL) Packet Structure

  Preamble (1 octet) | Access Address (4) | PDU (2-39) | CRC (3)

  ### Advertising Channel PDUs

  | PDU Type | Value | Direction | Purpose |
  |----------|-------|-----------|---------|
  | ADV_IND | 0x00 | Adv → Scan | Connectable undirected |
  | ADV_DIRECT_IND | 0x01 | Adv → Init | Connectable directed |
  | ADV_NONCONN_IND | 0x02 | Adv → Scan | Non-connectable |
  | SCAN_REQ | 0x03 | Scan → Adv | Request scan response |
  | SCAN_RSP | 0x04 | Adv → Scan | Scan response data |
  | CONNECT_IND | 0x05 | Init → Adv | Connection request |
  | ADV_SCAN_IND | 0x06 | Adv → Scan | Scannable undirected |

  ### Data Channel PDUs

  | LLID | Name | Purpose |
  |------|------|---------|
  | 00 | Reserved | — |
  | 01 | L2CAP Start | First fragment |
  | 10 | L2CAP Continuation | Continuing fragment |
  | 11 | LL Control | Control procedures |

  ---

  ## 🔍 Fuzzing Targets

  ### 1. Advertising Channel
  - **Malformed PDU length** — Length > 37 bytes
  - **Invalid PDU type** — 0x07-0x0F (reserved)
  - **CRC manipulation** — Force CRC errors
  - **Access Address collision** — Use 0x8E89BED6 (advertising AA) in data channel

  ### 2. Connection Establishment
  - **CONNECT_IND with invalid params** — Negative interval, zero timeout
  - **Initiator/Advertiser address mismatch** — Random vs Public confusion
  - **Channel map all zeros** — Force connection failure

  ### 3. LL Control Procedures
  | Procedure | Opcode | Fuzz Vectors |
  |-----------|--------|--------------|
  | Connection Update | 0x00 | Instant > interval, latency > timeout |
  | Channel Map Update | 0x01 | All channels disabled |
  | Termination | 0x02 | Invalid error codes (0x00, 0xFF) |
  | Encryption Request | 0x03 | Zero SKD, invalid IV |
  | Version Exchange | 0x04 | Unknown version, subversion overflow |
  | Feature Exchange | 0x05 | All bits set, reserved bits |
  | PAST | 0x06 | Invalid timing |
  | Periodic Sync | 0x07 | Invalid sync handle |

  ### 4. ATT/GATT Layer
  - **MTU negotiation** — 0xFFFF, 0x0001, 0x0000
  - **Handle overflow** — 0xFFFF, 0x10000
  - **Invalid UUID** — 16-bit where 128-bit expected
  - **Permission bypass** — Write to read-only, read write-only

  ---

  ## 🛡️ Privacy Analysis

  ### Resolvable Private Address (RPA)
  RPA = AES(IRK, prand) | prand (24 bits)
  - **IRK** — Identity Resolving Key (128-bit, shared during bonding)
  - **prand** — Random 24-bit value

  ### Attack Vectors
  1. **RPA collision** — Generate 2^24 RPAs, find collision
  2. **IRK recovery** — Known-plaintext if prand leaked
  3. **Tracking without IRK** — RSSI + timing + device fingerprinting

  ### ESP32 Implementation Gaps
  - `esp_ble_gap_set_rand_addr()` generates **non-resolvable** random address (not RPA)
  - No built-in IRK management for peripheral role
  - `esp_ble_gap_config_local_privacy()` only enables RPA for *scanner* role

  ---

  ## 📊 Experimental Results (ESP32 vs Targets)

  | Target Device | OS | Beacon Flood Effect | Conn Spam Effect | Channel Hop Effect |
  |---------------|----|---------------------|------------------|-------------------|
  | iPhone 14 | iOS 17 | Hidden in list | Rejected after 7 | Scanner pauses |
  | Pixel 7 | Android 14 | Hidden in list | Rejected after 10 | Scanner pauses |
  | Dell XPS | Windows 11 | Hidden in list | BlueZ rate-limits | BlueZ hops with |
  | ESP32 Scanner | Zephyr | Misses 90%+ | N/A | Misses 70%+ |
  | nRF52840 | Zephyr | Misses 85%+ | N/A | Misses 60%+ |

  **Key finding:** Modern stacks implement **scan rate limiting** and **duplicate filtering** — raw packet volume less
  effective than smart timing.

  ---

  ## 🔬 Open Research Questions

  1. **Can we force LL layer crash via malformed CONNECT_IND?**
     - Target: `connIntervalMin > connIntervalMax`
     - Target: `connLatency > connSupervisionTimeout`

  2. **RPA resolution without IRK via side-channels?**
     - Timing attack on AES-128 in controller
     - Power analysis on ESP32 crypto accelerator

  3. **BLE 5.0 Extended Advertising abuse?**
     - AuxPtr chain manipulation
     - Periodic advertising sync hijack

  4. **Mesh network flooding?**
     - Friend node DoS via subscription spam
     - Sequence number wrap attacks

  ---

  ## 📚 References

  - [Bluetooth Core Spec v5.4](https://www.bluetooth.com/specifications/specs/core-specification/)
  - [InternalBlue](https://github.com/seemoo-lab/internalblue) — BLE experimentation framework
  - [BLEAK](https://github.com/hbldh/bleak) — Python BLE client
  - [Bettercap BLE](https://www.bettercap.org/modules/ble/) — MITM/fuzzing
  - [SweynTooth](https://asset-group.github.io/disclosures/sweyntooth/) — BLE stack vulnerabilities
  - [BrakTooth](https://github.com/asset-group/braktooth) — BT Classic/LMP vulnerabilities

  ---

  
