#include <TinyGPSPlus.h>

// The TinyGPSPlus object
TinyGPSPlus gps;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(9600);

  /*Serial.println("BasicExample.ino");
  Serial.println("Basic demonstration of TinyGPSPlus (no device needed)");
  Serial.print("Testing TinyGPSPlus library v. "); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println("by Mikal Hart");
  Serial.println();*/

  /*while (*gpsStream)
    if (gps.encode(*gpsStream++))
      displayInfo();*/

  /*Serial.println();
  Serial.println("Done.");*/
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
    smartDelay(10 * 1000);
  }
  else
  {
    Serial.print("INVALID");
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
    smartDelay(10 * 1000);
  }
  else
  {
    Serial.print("INVALID");
    smartDelay(1000);
  }

  Serial.println();
}
