//Slimme bakenverlichting
#include <MKRWAN.h>
#include "arduino_secrets.h"

LoRaModem modem;

//Variable
String appEui = SECRET_APP_EUI;;
String appKey = SECRET_APP_KEY;;

void setup() {
  Serial.begin(115200);

  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.println("Module gestart");

  modem.configureClass(CLASS_C);

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("FOUT, geen verbinding, start opnieuw op");
    while (1) {}
  }

  // Set poll interval to 60 secs.
  modem.minPollInterval(60);

  delay(5000);

  int err1;
  modem.setPort(1);
  modem.beginPacket();
  modem.print("1:" + modem.deviceEUI());
  err1 = modem.endPacket(true);
  if (err1 > 0) {
    Serial.println("Bericht verzonden");
  } else {
    Serial.println("Bericht niet verzonden");
  }

  pinMode(6, OUTPUT); //Led ouput
  pinMode(7, INPUT);  //Button input
}

void loop() {
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
      digitalWrite(6, HIGH);
      sendLoRaMessage("2:100");
    } else if (message == "L0") {
      digitalWrite(6, LOW);
      sendLoRaMessage("2:000");
    }
  }

  if (digitalRead(7)) {
    sendLoRaMessage("3:51.2304.418");
  }

  delay(1000);
}

void sendLoRaMessage(String data) {
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
