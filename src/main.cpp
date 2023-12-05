#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FS.h"
#include <Arduino.h>
#include "wifi_config.h"
#include "lora_config.h"
#include "display_config.h"
#include "server_config.h"

void setup() {
    Serial.begin(115200);

    setupDisplay();
    setupWifi();
    setupServer();
    setupLora();
}

void loop() {
    loopLora();
}