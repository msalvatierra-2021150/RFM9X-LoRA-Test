#include <RadioLib.h>
#include "EspHal.h"
#include "esp_log.h"

static const char *TAG = "main";

// LoRa wiring (ESP32 <-> Adafruit RFM9x):
//  SCK  -> GPIO18
//  MISO -> GPIO19
//  MOSI -> GPIO23
//  CS   -> GPIO5
//  RST  -> GPIO14
//  G0(DIO0) -> GPIO26
//  G1(DIO1) -> GPIO33

// HAL uses SPI pins: SCK, MISO, MOSI
EspHal* hal = new EspHal(
    18,   // SCK
    19,   // MISO
    23    // MOSI
);

// RadioLib Module: (hal, NSS/CS, DIO0, NRST, DIO1)
SX1276 radio = new Module(
    hal,
    5,    // NSS / CS
    26,   // DIO0  (G0 on Adafruit board)
    14,   // NRST / RST
    33    // DIO1  (G1 on Adafruit board)
);

// entry point for ESP-IDF app in C++
extern "C" void app_main(void) {
  ESP_LOGI(TAG, "app_main start");

  ESP_LOGI(TAG, "[SX1276] Initializing ...");
  int state = radio.begin();
  if (state != RADIOLIB_ERR_NONE) {
    ESP_LOGE(TAG, "radio.begin() failed, code %d", state);
    while (true) {
      hal->delay(1000);
    }
  }
  ESP_LOGI(TAG, "radio.begin() success!");

  // loop forever
  for (;;) {
    ESP_LOGI(TAG, "[SX1276] Transmitting packet ...");
    state = radio.transmit("Hello Roberto And Michael!");
    if (state == RADIOLIB_ERR_NONE) {
      ESP_LOGI(TAG, "TX success!");
    } else {
      ESP_LOGE(TAG, "TX failed, code %d", state);
    }

    // wait for a second before transmitting again
    hal->delay(1000);
  }
}
