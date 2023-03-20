void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
}

uint LDR0;  //Main light
uint LDR1;  //Led1
uint LDR2;  //Led2
uint LDR3;  //Led3

int ledOnIntensity = 920;
int outsideLightIntensity = 700;

void loop() {
  delay(1000);
  checkLights();
}

uint UpdateLDRValue(byte pin) { //Function to get LDR values from pin x
  uint value = 0;
  for (int i = 0; i < 20; i++) { //Gemiddle van 20 metingen
    value = value + analogRead(pin);
  }
  value = value / 20;
  return value;
}
byte error = 0b0000;
bool leds = true;
void checkLights() {
  LDR0 = UpdateLDRValue(A0);
  LDR1 = UpdateLDRValue(A1);
  LDR2 = UpdateLDRValue(A2);
  LDR3 = UpdateLDRValue(A3);
  if (leds) { //Check if leds are on
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
  }

  //sendLoRaMessage(modem.deviceEUI() + ":2:51.23:4.418");
  Serial.println(LDR0);
  Serial.println(LDR1);
  Serial.println(LDR2);
  Serial.println(LDR3);
  Serial.println("-----");
  Serial.println(error, BIN);
    Serial.println("-----");
}
