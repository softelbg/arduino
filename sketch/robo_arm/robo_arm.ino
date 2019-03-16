/*
  Softel Labs 2019
  s.georgiev@softel.bg
*/

#include <robo_controller.h>

RoboArmController arm;
RoboTarget target(11);

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  arm.init();
  target.init();
}

String readString;

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void parse_cmdln_test() {
  if (readString.length() > 0) {
    Serial.println(readString);

    String cmd = getValue(readString, ':', 0);
    int servo_idx = getValue(readString, ':', 1).toInt();
    int val = getValue(readString, ':', 2).toInt();

    Serial.print("cmd ");
    Serial.print(cmd);
    Serial.print(" servo_idx ");
    Serial.print(servo_idx);
    Serial.print(" val ");
    Serial.println(val);

    if (cmd == "r") {
      Serial.println("reset");
      arm.reset();
    } else if (cmd == "m") {
      arm.move(servo_idx, val);
    }

    Serial.println(arm.position());

    int tval = target.read();
    Serial.print("target ");
    Serial.println(tval);

    readString="";
  }
}

void dbg_blink() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(10);
  }
}

void parse_cmdln() {
  if (readString.length() > 0) {
    String cmd = getValue(readString, ':', 0);
    int servo_idx = getValue(readString, ':', 1).toInt();
    int val = getValue(readString, ':', 2).toInt();

    if (cmd == "r") {
      arm.reset();
    } else if (cmd == "m") {
      arm.move(servo_idx, val);
    } else if (cmd == "z") {
      arm.zero();
    }

    dbg_blink();
    Serial.println(arm.position());

    int tval = target.read();
    Serial.print("target ");
    Serial.println(tval);

    readString="";
  }
}

// TODO: Make separate serial reader component
void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    readString += c;
    delay(2);
  }

  parse_cmdln();
}
