//Kevin Lahey
//IoT project
//Slimme baken PoAB

//Librarys
#include <MKRWAN.h> //MKR WAN
#include "arduino_secrets.h"  //File for configuration
#include <TinyGPSPlus.h>  //GPS library
#include <Adafruit_INA219.h>  //Voltage/current board
#include <ADS1X15.h>  //I2C ADC for current board

//Variables
unsigned int AmbientLight;  //Main light
unsigned int LDR1;  //Led1
unsigned int LDR2;  //Led2
unsigned int LDR3;  //Led3

unsigned int MeasureLDR1[3];
unsigned int MeasureLDR2[3];
unsigned int MeasureCurrent1[3];
unsigned int MeasureCurrent2[3];
char lights[] = "0:0:0:";
char statusCode[] = "0000";

int ambientLightDark = 400; //When baken needs to turn on

bool lightsStatus = false;
bool automaticMode = true;
bool gotGPS = true;

//Objects
LoRaModem modem;
TinyGPSPlus gps;
Adafruit_INA219 ina219;
ADS1115 ADS(0x48);  //I2C addres 0x48 for current board

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);  //GPS serial 13RX,14TX
  //Serial.println("Serial gestard");

  //Inputs LDR's
  pinMode(A0, INPUT); //Led1
  pinMode(A1, INPUT); //Led2
  pinMode(A2, INPUT); //Led3
  pinMode(A3, INPUT); //Ambient Light

  //Output Led's
  pinMode(0, OUTPUT); //SSR1
  pinMode(1, OUTPUT); //SSR2
  pinMode(2, OUTPUT); //SSR3
  pinMode(6, OUTPUT); //BUILD IN LED

  //INA219 Voltage/Current board
  //if (! ina219.begin()) {
  //  Serial.println("Failed to find INA219 chip");
  //}

  ADS.begin();  //I2C board for current sensor

  //LoRaWan
  if (!modem.begin(EU868)) {
    //Serial.println("Failed to start module");
    digitalWrite(6, HIGH);
    while (1) {}
  };
  //Serial.println("Module gestart");
  modem.configureClass(CLASS_C);

  int connected = modem.joinOTAA(SECRET_APP_EUI, SECRET_APP_KEY);
  if (!connected) {
    //Serial.println("FOUT, geen verbinding, start opnieuw op");
    digitalWrite(6, HIGH);
    while(1){}
  }
  modem.minPollInterval(60);

  //delay(5000);

  sendLoRaMessage("0:0");
}

//Main program
void loop() {
  //Check ambient light
  AmbientLight = GetLDRValue(A3);
  //Serial.println(AmbientLight);

  if (AmbientLight < ambientLightDark && !lightsStatus && automaticMode) { //If darker then value and leds are off
    changeLightState(true);
    //Serial.println("Sensor licht aan");
  } else if (AmbientLight > ambientLightDark && lightsStatus && automaticMode) {// If brighter then value and leds are on
    changeLightState(false);
    //Serial.println("Sensor licht uit");
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
    //Serial.println(message);

    if (message == "L1") {  //LAMP on
      automaticMode = false;
      changeLightState(true);
    } else if (message == "L0") { //LAMP off
      automaticMode = false;
      changeLightState(false);
    } else if (message == "LA") { //Lamp AUTO
      automaticMode = true;
    } else if (message == "GPS" && gotGPS) { //GPS ophalen
      sendLoRaMessage("2:" + String(gps.location.lat()) + ":" + String(gps.location.lng()));
    }
  }

  //GPS
  if (!gotGPS) {
    readGPSData();
  }

  delay(1000);
}

//Functions
void sendLoRaMessage(String data) { //Function to send data over LoRa
  modem.setPort(1);
  modem.beginPacket();
  modem.print(data);
  int err = modem.endPacket(true);
  if (err > 0) {
    //Serial.println("Bericht verzonden");
  } else {
    //Serial.println("Bericht niet verzonden");
  }
}

unsigned int GetLDRValue(byte pin) { //Function to get LDR value from pin x
  unsigned int valueM = 0;
  for (int i = 0; i < 20; i++) { //mean op 20 measures
    valueM = valueM + analogRead(pin);
  }
  valueM = valueM / 20;
  return valueM;
}

unsigned int GetCurrentValue(int pin) { //Function to get current value from pin x
  unsigned int valueM = 0;
  for (int i = 0; i < 20; i++) { //mean op 20 measures
    valueM = valueM + ADS.readADC(pin);
  }
  valueM = valueM / 20;
  return valueM;
}

void changeLightState(bool state) { //Function to turn on leds
  if (state) {  //Set lights on
    lightsStatus = true;
    checkLightsLDR(MeasureLDR1); //Check value before turning on
    checkLightsCurrent(MeasureCurrent1); //Check value before turing on
    digitalWrite(0, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    delay(100);
    checkLightsLDR(MeasureLDR2);  //Check value after turning on
    checkLightsCurrent(MeasureCurrent2); //Check value after turing on
    //Serial.println(MeasureCurrent1[0]);
    //Serial.println(MeasureCurrent2[0]);
    //Serial.println(MeasureLDR1[0]);
    //Serial.println(MeasureLDR2[0]);
    verifyLightStatus(MeasureLDR1 , MeasureLDR2, MeasureCurrent1, MeasureCurrent2); //Verify if correct

    //Set light status
    if (lights != "1:1:1") {
      statusCode[0] = '1';
    }
  } else {  //Set lights off
    lightsStatus = false;
    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    checkLightsLDR(MeasureLDR1);  //Check value after turning off
    checkLightsCurrent(MeasureCurrent1); //Check value after turing off

    verifyLightStatus(MeasureLDR1 , MeasureLDR1, MeasureCurrent1, MeasureCurrent1); //Verify if correct

    //Set light status
    if (lights != "0:0:0") {
      statusCode[0] = '1';
    }
  }
  //Send the status of the lights to LoRaWan
  //Serial.println(lights);
  sendLoRaMessage("1:" + String(lights) + "0.0" + ":" + String(statusCode));
}

void checkLightsLDR(unsigned int arr[]) {  //Get LDR value of lights and place in array
  arr[0] = GetLDRValue(A0);
  arr[1] = GetLDRValue(A1);
  arr[2] = GetLDRValue(A2);
}

void checkLightsCurrent(unsigned int arr[]) {  //Get current of lights and place in array
  arr[0] = GetCurrentValue(0);
  arr[1] = GetCurrentValue(1);
  arr[2] = GetCurrentValue(2);
}

void verifyLightStatus(unsigned int a[], unsigned int b[], unsigned int c[], unsigned int d[]) { //To check if the light value changed
  if ((a[0] < b[0]) && (c[0] < d[0])) { //Check if light is brighter after turn on and there is current flowing
    lights[0] = '1';
  } else {
    lights[0] = '0';
  }
  if ((a[1] < b[1]) && (c[1] < d[1])) {
    lights[2] = '1';
  } else {
    lights[2] = '0';
  }
  if ((a[2] < b[2]) && (c[2] < d[2])) {
    lights[3] = '1';
  } else {
    lights[3] = '0';
  }
}

void readGPSData() {
  //while (Serial1.available() > 0) {
    if (gps.encode(Serial1.read())) {

      if (gps.location.isValid()) {
        //Serial.print(gps.location.lat(), 6);
        //Serial.print(gps.location.lng(), 6);
        gotGPS = true;
        //break;
      }
    //}
  }
}
