#include <Arduino.h>
#include "wifi_config.h"
#include "lora_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FS.h"
#include "display_config.h"

void setup() {
    Serial.begin(115200);
    setupDisplay();
    setupWifi();
    setupLora();
}

void loop() {
    loopLora();
}