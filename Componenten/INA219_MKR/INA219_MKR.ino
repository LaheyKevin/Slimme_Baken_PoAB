//Kevin Lahey
//IoT project
//INA219 testopstelling

#include <MKRWAN.h>
#include <Wire.h>

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Wire.begin();
  delay(20);

  uint16_t test = round(0.04096/((3.4 * 3.0517578125e-5)*0.002));  
  Wire.beginTransmission(0x40);
  Wire.write(0x05);
  Wire.write(test >> 8);
  Wire.write(test & 0xFF);
  Wire.endTransmission();
}

void loop()
{
  //SPANNING
  Wire.beginTransmission(0x40);
  Wire.write(0x02);
  Wire.endTransmission();

  delay(10);

  Wire.requestFrom(0x40, byte(2));
  uint16_t value = Wire.read();
  value <<= 8;
  value |= Wire.read();

  float spanning = (value >> 3) * 4e-3;

  //STROOM
  Wire.beginTransmission(0x40);
  Wire.write(0x04);
  Wire.endTransmission();

  delay(10);

  Wire.requestFrom(0x40, byte(2));
  uint16_t value2 = Wire.read();
  value2 <<= 8;
  value2 |= Wire.read();

  float stroom = value2*(3.4 * 3.0517578125e-5);

  Serial.println(spanning, 5);
  Serial.println(stroom, 5);

  delay(2000);
}
