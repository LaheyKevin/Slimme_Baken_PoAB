const int analogIn = A0;
int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module
unsigned int RawValue = 0; // can hold upto 64 10-bit A/D readings
float ACSoffset = 2492.6;
double Voltage = 0;
double Amps = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int x = 0; x < 64; x++) // 64 analogue readings for averaging
  {
    RawValue = RawValue + analogRead(analogIn); // add each A/D reading to a total
  }
  Voltage = ((RawValue / 64) / 1023.0) * 5000; // Gets you mV
  Amps = ((Voltage - ACSoffset) / mVperAmp);

  Serial.print("Raw Value = " ); // shows pre-scaled value
  Serial.print(RawValue / 64);
  Serial.print("\t mV = "); // shows the voltage measured
  Serial.print(Voltage, 3); // the '3' after voltage allows you to display 3 digits after decimal point
  Serial.print("\t Amps = "); // shows the voltage measured
  Serial.println(Amps, 3); // the '3' after voltage allows you to display 3 digits after decimal point

  RawValue = 0; // reset value
  delay(2500);
}
