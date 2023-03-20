int LED = 6;
int LDR = A0;

int LED_Toestand = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  //LED_Toestand = random(0, 2);

  Serial.println(LED_Toestand);

  int LDR_Waarde = analogRead(LDR);
  Serial.println(LDR_Waarde);

  if (LED_Toestand == 1) {
    digitalWrite(LED, 255);
  }
  else {
    digitalWrite(LED, LOW);
  }

  if (LDR_Waarde < 100) {
    Serial.println("Led is aan!");
  }
  else {
    Serial.println("Led is uit.");
  }

  Serial.println("");
  delay(1000);
}
