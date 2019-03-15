#include <Servo.h>

#define NUM_DOF 5

// TODO: Currently 5DOF fixed size robo arm
// TODO: Make config...
class RoboArmController {
public:
  RoboArmController() {
    for (int i = 0; i < NUM_DOF; i++) {
      pins_[i] = 2 + i;
    }
  }

  void init() {
    for (int i = 0; i < NUM_DOF; i++) {
      servos_[i].attach(pins_[i]);
    }
  }

  void reset() {
    for (int i = 0; i < NUM_DOF; i++) {
      servos_[i].write(reset_position_[i]);
    }
  }

  void move(int idx, int d) {
    // TODO: check for sign()
    int s = 1;
    if (d < 0) {
      s = -1;
    }

    for (int i = 0; i < abs(d); i++) {
      int v = servos_[idx].read();
      v += s;
      if (v < threshold_up_[idx] && v > threshold_dwn_[idx]) {
        servos_[idx].write(v);
        delay(100);
      } else {
        break;
      }
    }
  }

private:
  int threshold_dwn_[NUM_DOF] = {40, 80, 30, 10, 0};
  int threshold_up_[NUM_DOF] =  {130, 140, 130, 170, 180};
  int reset_position_[NUM_DOF] = {90, 130, 90, 90, 90};

  int pins_[NUM_DOF];

  Servo servos_[NUM_DOF];
};
