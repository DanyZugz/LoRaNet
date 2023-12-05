#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "display_config.h"
#include "server_config.h"

#define RF_FREQUENCY                                923000000 // Hz
#define TX_OUTPUT_POWER                             21        // dBm
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
#define LORA_SPREADING_FACTOR                       12         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false
#define BUFFER_SIZE                                 300 // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;

void OnTxDone(void);

void OnTxTimeout(void);

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

unsigned long tiempoAnterior = 0;
unsigned long intervalo = 5000;

typedef enum {
    LOWPOWER,
    STATE_RX,
    STATE_TX
} States_t;

States_t state;
int16_t Rssi;
String incomingString;
String sendMessage;
char ack[] = "*ok*";
bool ok = false;

void setupLora() {
    Mcu.begin();
    Rssi = 0;

    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxDone = OnRxDone;

    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                      LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                      LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                      true, 0, 0, LORA_IQ_INVERSION_ON, 3000);

    Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                      LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                      LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                      0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
    state = STATE_RX;
    Serial.println("LoRa configured seccessfully");
}


void loopLora() {

    sendMessage = takeData();

    if (millis() - tiempoAnterior >= intervalo) {
        clearDisplay();
        tiempoAnterior = millis();
    }

    if (Serial.available() > 0) {
        incomingString = Serial.readString();
        strcpy(txpacket, incomingString.c_str());
        incomingString = "";
        state = STATE_TX;
    }

    if (sendMessage != ""){
        strcpy(txpacket, sendMessage.c_str());
        state = STATE_TX;
    }

    switch (state) {
        case STATE_TX:
            delay(200);
            Radio.Send((uint8_t *) txpacket, strlen(txpacket));
            state = LOWPOWER;
            break;
        case STATE_RX:
            Radio.Rx(0);
            state = LOWPOWER;
            if (ok){
                Radio.Send((uint8_t *) ack, strlen(ack));
                ok = false;
            }
            break;
        case LOWPOWER:
            Radio.IrqProcess();
            break;
        default:
            break;
    }
}

void OnTxDone() {
    state = STATE_RX;
}

void OnTxTimeout() {
    Radio.Sleep();
    state = STATE_RX;
}

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
    Rssi = rssi;
    memcpy(rxpacket, payload, size);
    rxpacket[size] = '\0';
    Radio.Sleep();
    loopDisplay(Rssi);

    Serial.printf("%s\n", rxpacket);

    if(strcmp(rxpacket, "*ok*") != 0){
        ok = true;
        loopServer(rxpacket);
        rxpacket[0] = '\0';
    }

    state = STATE_RX;
}