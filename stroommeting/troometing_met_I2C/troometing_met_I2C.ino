#include <ADS1X15.h>

ADS1115 ADS(0x48);

int ADC0 = 0;

void setup() {
  Serial.begin(9600);
  ADS.begin();
}

void loop() {
  ADS.setGain(0);

  ADC0 = ADS.readADC(0);

  float f = ADS.toVoltage(2);

  Serial.print("Analog0: "); Serial.print(ADC0); Serial.print('\t'); Serial.println(ADC0 * f, 3);
  delay(2000);
}
