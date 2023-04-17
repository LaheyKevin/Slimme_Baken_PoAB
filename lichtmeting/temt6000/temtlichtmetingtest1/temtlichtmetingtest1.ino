int temt6000Pin = A0;
float light;
int light_value;
void setup() {
  Serial.begin(9600);
  pinMode(temt6000Pin, INPUT); //data pin for ambientlight sensor
}

void loop() {
  int light_value = analogRead(temt6000Pin);
  light = light_value * 0.09775;// percentage calculation 0.0976
  Serial.print(light);
  Serial.println(" %");
  delay(500);
}
