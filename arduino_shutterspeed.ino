// Shutter speed tester
// Jon Wallace 2022

#define LIGHT_SENSOR (A0)
#define CAL_BUTTON_SHUTTER_CLOSED (19)
#define CAL_BUTTON_SHUTTER_OPEN (10)

#define STATE_CLOSED 1
#define STATE_OPEN 0

volatile int shutter_closed_cal = 1024;
volatile int shutter_open_cal = 0;
volatile int trigger = 512;
volatile int last_state = STATE_CLOSED;
volatile unsigned long open_time = 0;

void setup() {
  Serial.begin(115200);
  pinMode(CAL_BUTTON_SHUTTER_CLOSED, INPUT_PULLUP);
  pinMode(CAL_BUTTON_SHUTTER_OPEN, INPUT_PULLUP);
}

void loop() {
  int sensorValue = analogRead(LIGHT_SENSOR);

    if (digitalRead(CAL_BUTTON_SHUTTER_CLOSED) == 0) {
      shutter_closed_cal = sensorValue;
      set_triggers();
    } else if (digitalRead(CAL_BUTTON_SHUTTER_OPEN) == 0) {
      shutter_open_cal = sensorValue;
      set_triggers();
    }

    if (sensorValue > trigger && last_state == STATE_OPEN) {
      Serial.println(micros() - open_time);
      last_state = STATE_CLOSED;
    } else if (sensorValue <= trigger && last_state == STATE_CLOSED) {
      last_state = STATE_OPEN;
      open_time = micros();
    }

}

void set_triggers()
{
  trigger = (shutter_closed_cal + 5*shutter_open_cal) / 6;
  Serial.print(shutter_closed_cal);
  Serial.print(",");
  Serial.print(trigger);
  Serial.print(",");
  Serial.println(shutter_open_cal);
  return;
}
