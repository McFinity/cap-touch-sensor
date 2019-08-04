bool charging = true;
int readValue = 0;
int loopCount = 0;
unsigned long dischargeStartMicros = 0;
unsigned long dischargeMicros = 0;
unsigned long baselineDischargeMicros = 0;

void setup() {
  pinMode(1, OUTPUT);
}

void loop() {
  loopCount++;
  if (charging) {

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
 
    charging = false;
    pinMode(2, INPUT);

    dischargeStartMicros = 0;
  } else {
    
    if (dischargeStartMicros == 0) {
      dischargeStartMicros = micros();
    }
 
    readValue = analogRead(1);

    if (readValue < 100) {
      dischargeMicros = micros() - dischargeStartMicros;
      charging = true;

      if (baselineDischargeMicros == 0 && loopCount > 20000) {
        baselineDischargeMicros = dischargeMicros;
      } else if (baselineDischargeMicros != 0 && abs(baselineDischargeMicros - dischargeMicros) > 5) {
        digitalWrite(1, HIGH);
      } else {
        digitalWrite(1, LOW);
      }
    }

  }


}
