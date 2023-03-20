#include <TinyGPSPlus.h>

unsigned long start = millis();

// The TinyGPSPlus object
TinyGPSPlus gps;
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(3000);
}


void loop() {
  //updateSerial();
  while (Serial1.available() > 0)
    if (gps.encode(Serial1.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}


void displayInfo()
{
  if(millis() - start > 5000) {
    Serial.print(F("Location: "));
    if (gps.location.isValid()) {
      Serial.print("Lat: ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(F(","));
      Serial.print("Lng: ");
      Serial.print(gps.location.lng(), 6);
      Serial.println();
    }
    else
    {
      Serial.println(F("INVALID"));
    }

    Serial.print("  Date/Time: ");
    if (gps.date.isValid())
    {
      Serial.print(gps.date.month());
      Serial.print("/");
      Serial.print(gps.date.day());
      Serial.print("/");
      Serial.print(gps.date.year());
    }
    else
    {
      Serial.print("INVALID");
    }

    Serial.print(" ");
    if (gps.time.isValid())
    {
      if (gps.time.hour() < 10) Serial.print("0");
      Serial.print(gps.time.hour());
      Serial.print(":");
      if (gps.time.minute() < 10) Serial.print("0");
      Serial.print(gps.time.minute());
      Serial.print(":");
      if (gps.time.second() < 10) Serial.print("0");
      Serial.print(gps.time.second());
      Serial.print(".");
      if (gps.time.centisecond() < 10) Serial.print("0");
      Serial.print(gps.time.centisecond());
    }
    else
    {
      Serial.print("INVALID");
    }

    Serial.println();
    start = millis();
  }
}


/*void updateSerial()
  {
  delay(500);
  while (Serial.available())
  {
    Serial1.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial1.available())
  {
    Serial.write(Serial1.read());//Forward what Software Serial received to Serial Port
  }
  }*/
