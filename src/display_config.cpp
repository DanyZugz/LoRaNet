//
// Created by Dany on 11/30/2023.
//
#include "HT_SSD1306Wire.h"

SSD1306Wire  display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);

String nodo = "Nodo 1";

void setupDisplay(){
    display.init();
    display.setFont(ArialMT_Plain_24);

    delay(500);
    display.clear();
    display.drawString(0, 0, nodo);
    display.display();
    Serial.println("Display configured successfully");
}

void loopDisplay(int16_t myrss){

    String rss = String(myrss);

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);

    display.drawString(0, 0, nodo);
    display.drawString(0, 29, "RSSI: ");
    display.drawString(80, 29, rss);

    display.display();

}

void clearDisplay(){
    display.clear();
    display.drawString(0, 0, nodo);
    display.drawString(0, 29, " -- -- -- -- ");
    display.display();
}