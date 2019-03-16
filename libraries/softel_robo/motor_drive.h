/*
  Softel Labs 2019
  s.georgiev@softel.bg
*/

class MotorDrive {
public:
  MotorDrive(int ena, int enb, int in1, int in2, int in3, int in4) {
    pins["ENA"] = ena;
    pins["ENB"] = enb;
    pins["IN1"] = in1;
    pins["IN2"] = in2;
    pins["IN3"] = in3;
    pins["IN4"] = in4;
  }
  
  void setup() {
    for (auto it = pins.begin(); it != pins.end(); ++it) {
      pinMode(it->second, OUTPUT);
      Serial.println(it->first + " => " + String(it->second));
    }
  }
  
  void mode(String m) {
    if (m == "1") {
      digitalWrite(pins["IN1"], 1);
      digitalWrite(pins["IN2"], 0);
      digitalWrite(pins["IN3"], 1);
      digitalWrite(pins["IN4"], 0);
      for (int i = 0; i < 256; i++) {
        analogWrite(pins["ENA"], i);
        analogWrite(pins["ENB"], i);
        delay(50);
      }
      for (int i = 255; i >= 0; i--) {
        analogWrite(pins["ENA"], i);
        analogWrite(pins["ENB"], i);
        delay(50);
      }
      
      digitalWrite(pins["IN1"], 0);
      digitalWrite(pins["IN2"], 1);
      digitalWrite(pins["IN3"], 0);
      digitalWrite(pins["IN4"], 1);
      for (int i = 0; i < 256; i++) {
        analogWrite(pins["ENA"], i);
        analogWrite(pins["ENB"], i);
        delay(50);
      }
      for (int i = 255; i >= 0; i--) {
        analogWrite(pins["ENA"], i);
        analogWrite(pins["ENB"], i);
        delay(50);
      }
    }
  }

  void stop() {
    vx = 0;
    vy = 0;
    thrust();
  }

  void fwd(int d) {
    vx += d;
    thrust();
  }
  void bwd(int d) {
    vx -= d;
    thrust();
  }
  void left(int d) {
    vy -= d;
    thrust();
  }
  void right(int d) {
    vy += d;
    thrust();
  }

  void thrust() {
    validate();
    thrust(vx, vy);
  }

  void avoid_front() {
    Serial.println("Avoid Front");
    thrust(-100, 0);
    delay(500);
    thrust(0, -200);
    delay(1000);
    thrust();
  }

private:
  void validate() {
    if (vx < -255) vx = -255;
    if (vx > 255) vx = 255;
    if (vy < -255) vy = -255;
    if (vy > 255) vy = 255;
  }

  void thrust(int vxo, int vyo) {
    int d1 = vxo >= 0 ? 0 : 1;
    int d2 = d1 == 0 ? 1 : 0;
  
    int v1 = abs(vxo);
    int v2 = abs(vxo);
      
    v1 = v1 + vyo;
    v2 = v2 - vyo;
  
    if (v1 < 0) v1 = 0;
    if (v2 < 0) v2 = 0;
    if (v1 > 255) v1 = 255;
    if (v2 > 255) v2 = 255;
      
    digitalWrite(pins["IN1"], d1);
    digitalWrite(pins["IN2"], d2);
    digitalWrite(pins["IN3"], d1);
    digitalWrite(pins["IN4"], d2);
    analogWrite(pins["ENA"], v1);
    analogWrite(pins["ENB"], v2);
  
    Serial.println(String("VXO: ") + String(vxo) + " " + String(vyo));
    Serial.println(String("D: ") + String(d1) + " " + String(d2));
    Serial.println(String("V: ") + String(v1) + " " + String(v2));
  }

  std::map<String, int> pins;

  int vx;
  int vy;
};
  
