//
// Created by Dany on 11/29/2023.
//

#include <WiFi.h>

#define SSID "LoRaChat1"
#define PASSWORD "12345678"

void setupWifi() {
    // Configura el ESP32 en modo AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID, PASSWORD);
    Serial.println("WiFi configured seccessfully");
}