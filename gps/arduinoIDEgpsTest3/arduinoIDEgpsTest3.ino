#include <TinyGPSPlus.h>

// The TinyGPSPlus object
TinyGPSPlus gps;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

void loop()
{
  displayInfo();
}

void displayInfo()
{
  Serial.print("Location: ");
  if (gps.location.isValid() == 1 )
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(",");
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print("INVALID");
  }

  Serial.print("  Date/Time: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.year());
    //delay(10000);
    smartDelay(10 * 1000);
  }
  else
  {
    Serial.print("INVALID");
    //delay(1000);
    smartDelay(1000);
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
    //delay(10000);
    smartDelay(10 * 1000);
  }
  else
  {
    Serial.print("INVALID");
    //delay(1000);
    smartDelay(1000);
  }

  Serial.println();
}
