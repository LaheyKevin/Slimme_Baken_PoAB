//Kevin Lahey
//IoT project
//ACS712 testopstelling

#include <MKRWAN.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Serial gestard");
}

void loop() {
  unsigned int x = 0;
  float AcsValue = 0.0, Samples = 0.0, AvgAcs = 0.0, AcsValueF = 0.0;

  for (int x = 0; x < 150; x++) { //Get 150 samples
    AcsValue = analogRead(A1);     //Read current sensor values
    //Serial.println(AcsValue);
    Samples = Samples + AcsValue;  //Add samples together
    delay (20); // let ADC settle before next sample 3ms
  }
  AvgAcs = Samples / 150.0; //Taking Average of Samples

  //((AvgAcs * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts
  //2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes
  //out to be 2.5 which is out offset. If your arduino is working on different voltage than
  //you must change the offset according to the input voltage)
  //0.066v(66mV) is rise in output voltage when 1A current flows at input
  //AcsValueF = (2.5 - (AvgAcs * (5.0 / 1024.0)) ) / 0.185;

  Serial.println(AvgAcs);//Print the read current on Serial monitor
  delay(50);
}
