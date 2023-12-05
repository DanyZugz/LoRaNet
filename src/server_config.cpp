//
// Created by Dany on 12/4/2023.
//
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

char my_data[300];

void onEvent(AsyncWebSocket *myserver, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *midata,
             size_t len) {

    switch (type) {
        case WS_EVT_CONNECT:
//            Serial.printf("WebSocket client #%u connected from %s\n", client->id(),
//                          client->remoteIP().toString().c_str());
// fulanito conectado con este Ip, mandar mensaje en broadcast para que el resto lo sepa
            break;
        case WS_EVT_DISCONNECT:
//            Serial.printf("WebSocket client #%u disconnected\n", client->id());

// fulanito con este Ip desconectado, mandar mensaje en broadcast para que el resto lo sepa
            break;
        case WS_EVT_DATA:
            // WebSocket data received
            if (len > 0) {
                // Print the received message to the Serial Monitor
//                char *data = (char *) malloc(sizeof(char) * (len + 1));
//                strncpy(data, (const char *) midata, len);
//                Serial.println(data);
//                free(data);
//                Serial.println(String((const char *) midata));
//                Serial.println(len);
                strncpy(my_data, reinterpret_cast<const char *>(midata), len + 1);
                my_data[len] = '\0';
//                Serial.println(String((const char *) my_data));
                //String((const char *) midata) = "";
                //  midata = nullptr;
                // Additional processing of the message
                // ...
            }
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            // WebSocket event types to handle if needed
            break;
    }
}


void setupServer(){
    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    ws.onEvent(onEvent);
    server.addHandler(&ws);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    server.serveStatic("/", SPIFFS, "/");

    // Start the server
    server.begin();
    Serial.println("Server configured seccessfully");
}

void loopServer(char rxpacket[]){
    ws.cleanupClients();
    ws.textAll(String(rxpacket));
}

String takeData(){

    String x = "";

    if (my_data[0] != '\0'){
        x = my_data;
    }
    my_data[0] = '\0';
    return x;
}