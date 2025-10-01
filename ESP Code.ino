#include <Arduino.h>

// ----------------- UART & RADAR CONFIG -----------------
#define RX_PIN 16
#define TX_PIN 17
#define BAUD_RATE 256000
#define RX_BUF_SIZE 64

// Radar buffer
uint8_t RX_BUF[RX_BUF_SIZE];
volatile int RX_count = 0;

// Target details
int16_t target1_x = 0, target1_y = 0;
int16_t target1_speed = 0;
uint16_t target1_distance_res = 0;
float target1_distance = 0;
float target1_angle = 0;

// Single target detection command
const uint8_t Single_Target_Detection_CMD[12] = {
  0xFD, 0xFC, 0xFB, 0xFA,
  0x02, 0x00, 0x80, 0x00,
  0x04, 0x03, 0x02, 0x01
};

// ----------------- RADAR DATA HANDLER -----------------
void processRadarData(const uint8_t* buf) {
  target1_x = (buf[4] | (buf[5] << 8)) - 0x200;
  target1_y = (buf[6] | (buf[7] << 8)) - 0x8000;
  target1_speed = (buf[8] | (buf[9] << 8)) - 0x10;
  target1_distance_res = (buf[10] | (buf[11] << 8));

  // Convert to polar coordinates
  target1_distance = sqrtf((float)target1_x * target1_x + (float)target1_y * target1_y);
  target1_angle = atan2f((float)target1_y, (float)target1_x) * 57.2958f; // radians → degrees

  // Print as CSV: valid,distance(cm),angle(deg)
  Serial.printf("%d,%.1f,%.2f\n", 1, target1_distance * 0.1f, target1_angle);
}

// ----------------- UART TASK -----------------
void uartTask(void* pvParameters) {
  while (true) {
    while (Serial1.available()) {
      uint8_t b = Serial1.read();
      RX_BUF[RX_count++] = b;
      if (RX_count >= RX_BUF_SIZE) RX_count = 0; // wrap on overflow

      // Detect frame end (0x55 0xCC)
      if (RX_count > 1 && RX_BUF[RX_count - 2] == 0x55 && RX_BUF[RX_count - 1] == 0xCC) {
        processRadarData(RX_BUF);
        RX_count = 0; // reset after parsing
      }
    }
    vTaskDelay(1); // allow other tasks to run
  }
}

// ----------------- SETUP -----------------
void setup() {
  Serial.begin(115200);
  Serial1.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);

  Serial.println("RD-03D Radar Module Initialized");

  // Send single-target detection command
  Serial1.write(Single_Target_Detection_CMD, sizeof(Single_Target_Detection_CMD));
  Serial.println("Single-target detection mode activated.");

  // Create UART reader task pinned to core 1
  xTaskCreatePinnedToCore(
    uartTask,        // task function
    "uartTask",      // task name
    4096,            // stack size
    NULL,            // parameters
    10,              // priority
    NULL,            // task handle
    1                // core 1
  );
}

// ----------------- LOOP -----------------
void loop() {
  // Main loop is free for WiFi, BLE, sensors, etc.
}
