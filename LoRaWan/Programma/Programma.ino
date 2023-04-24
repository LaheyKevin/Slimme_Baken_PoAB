//Kevin Lahey
//IoT project
//Slimme baken PoAB

#include <MKRWAN.h>
#include "arduino_secrets.h"

//Variablen
uint LDR0;  //Main light
uint LDR1;  //Led1
uint LDR2;  //Led2
uint LDR3;  //Led3

int ledOnIntensity = 920;
int outsideLightIntensity = 750;

bool updateLeds = false;
bool leds = false;

unsigned long sendOnTime = 0;
unsigned long sendInterval = 5000;

byte error = 0;

LoRaModem modem;

void setup() {
  Serial.begin(115200);
  Serial.println("Serial gestard");

  //LDR's
  pinMode(A0, INPUT); //Main Light
  pinMode(A1, INPUT); //Led1
  pinMode(A2, INPUT); //Led2
  pinMode(A3, INPUT); //Led3
  //Led's
  pinMode(0, OUTPUT); //Led1
  pinMode(1, OUTPUT); //Led2
  pinMode(2, OUTPUT); //Led3

  //LoRaWan
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.println("Module gestart");
  modem.configureClass(CLASS_C);

  int connected = modem.joinOTAA(SECRET_APP_EUI, SECRET_APP_KEY);
  if (!connected) {
    Serial.println("FOUT, geen verbinding, start opnieuw op");
    while (1) {}
  }
  modem.minPollInterval(60);

  delay(5000);
  //Startup message
  int err1;
  modem.setPort(1);
  modem.beginPacket();
  modem.print(modem.deviceEUI() + ":0"); //modem.deviceEUI()
  err1 = modem.endPacket(true);
  if (err1 > 0) {
    Serial.println("Bericht verzonden");
  } else {
    Serial.println("Bericht niet verzonden");
  }
}

void loop() {
  LDR0 = UpdateLDRValue(A0);

  if (LDR0 < outsideLightIntensity && !leds) {
    updateLeds = true;
    leds = true;
    changeLedState(true);
  } else if (LDR0 > outsideLightIntensity && leds) {
    updateLeds = true;
    leds = false;
    changeLedState(false);
  }

  //LoRaWan
  if (modem.available()) {
    char rcv[64] = "";
    int i = 0;
    while (modem.available()) {
      rcv[i % 64] = (char)modem.read();
      ++i;
    }
    String message = rcv;
    Serial.println(message);

    if (message == "L1") {
      changeLedState(true);
    } else if (message == "L0") {
      changeLedState(false);
      sendLoRaMessage(modem.deviceEUI() + ":1:0:0:0:" + 0b0001);
    }
  }

  if (error == 0) {
    error = 1;
  }

  //if (digitalRead(7)) {
  //  sendLoRaMessage("3:51.2304.418");
  //}

  if (sendOnTime + sendInterval >= millis()) {
    sendOnTime = millis();
    Serial.println("5 min gepaseerd");
  }

  delay(1000);
}

uint UpdateLDRValue(byte pin) { //Function to get LDR values from pin x
  uint value = 0;
  for (int i = 0; i < 20; i++) { //Gemiddle van 20 metingen
    value = value + analogRead(pin);
  }
  value = value / 20;
  return value;
}

void changeLedState(bool state) { //Function to turn on leds
  if (state) {
    digitalWrite(0, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    checkLights(true);
  } else {
    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
  }
  checkLights(false);
}

void sendLoRaMessage(String data) { //Function to send data over LoRa
  int err;
  modem.setPort(1);
  modem.beginPacket();
  modem.print(data);
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Bericht verzonden");
  } else {
    Serial.println("Bericht niet verzonden");
  }
}

void checkLights(bool test) {
  LDR1 = UpdateLDRValue(A1);
  LDR2 = UpdateLDRValue(A2);
  LDR3 = UpdateLDRValue(A3);
  if (leds || test) { //Check if leds are on
    if (LDR1 < ledOnIntensity) {
      error |= (1 << 1);
    } else {
      error &= ~(1 << 1);
    }
    if (LDR2 < ledOnIntensity) {
      error |= (1 << 2);
    } else {
      error &= ~(1 << 2);
    }
    if (LDR3 < ledOnIntensity) {
      error |= (1 << 3);
    } else {
      error &= ~(1 << 3);
    }
    sendLoRaMessage(modem.deviceEUI() + ":1:1:1:1:" + error);
  }

  //sendLoRaMessage(modem.deviceEUI() + ":2:51.23:4.418");
}
