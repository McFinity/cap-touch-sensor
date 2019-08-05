#define CHARGE_PIN A0
#define SENSE_PIN 0
#define DISCHARGE_DONE_THRESHOLD 100
#define DISCHARGE_DELTA_THRESHOLD 20
#define SETTLE_LOOPS 100

bool isCharging = true;
int readValue = 0;
int loopCount = 0;
unsigned long dischargeStartMicros = 0;
unsigned long dischargeMicros = 0;
unsigned long baselineDischargeMicros = 0;
unsigned long dischargeDelta = 0;

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  loopCount++;
  if (isCharging) {

    pinMode(CHARGE_PIN, OUTPUT);
    digitalWrite(CHARGE_PIN, HIGH);
 
    isCharging = false;
    pinMode(CHARGE_PIN, INPUT);

    dischargeStartMicros = 0;
  } else {
    
    if (dischargeStartMicros == 0) {
      dischargeStartMicros = micros();
    }
 
    readValue = analogRead(SENSE_PIN);

    if (readValue < DISCHARGE_DONE_THRESHOLD) {
      isCharging = true;
      dischargeMicros = micros() - dischargeStartMicros;
      dischargeDelta = abs(int(dischargeMicros) - int(baselineDischargeMicros));

      #ifdef DEBUG
      Serial.print(baselineDischargeMicros);
      Serial.print("us baseline; ");

      Serial.print(dischargeMicros);
      Serial.print("us discharge; ");

      Serial.print(dischargeDelta);
      Serial.print("us delta\n");
      #endif

      if (baselineDischargeMicros == 0 && loopCount > SETTLE_LOOPS) {
        baselineDischargeMicros = dischargeMicros;
      } else if (baselineDischargeMicros != 0 && dischargeDelta > DISCHARGE_DELTA_THRESHOLD) {
        digitalWrite(LED_BUILTIN, HIGH);
      } else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
  }
}
