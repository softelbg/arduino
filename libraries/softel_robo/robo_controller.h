/*
  Softel Labs 2019
  s.georgiev@softel.bg
*/

#ifndef __ROBO_CONTROLLER_H__
#define __ROBO_CONTROLLER_H__

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Servo.h>
#include <stdio.h>

#define NUM_DOF 6

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

    zero();
  }

  void zero() {
    for (int i = 0; i < NUM_DOF; i++) {
      servos_[i].write(zero_position_[i]);
    }
  }

  void reset(int move_delay) {
    for (int i = 0; i < NUM_DOF; i++) {
      move(i, reset_position_[i] - servos_[i].read(), move_delay);
    }
  }

  void move(int idx, int move_distance, int move_delay) {
    // TODO: check for sign()
    int s = 1;
    if (move_distance < 0) {
      s = -1;
    }

    for (int i = 0; i < abs(move_distance); i++) {
      int v = servos_[idx].read();
      v += s;
      if (v < threshold_up_[idx] && v > threshold_dwn_[idx]) {
        servos_[idx].write(v);
        delay(move_delay);
      } else {
        break;
      }
    }
  }

  String position() {
    char buff[64];
    sprintf(buff, "%d %d %d %d %d %d", // TODO: Make it dynamic
            servos_[0].read(), servos_[1].read(), servos_[2].read(),
            servos_[3].read(), servos_[4].read(), servos_[5].read());
    return String(buff);
  }

private:
  int threshold_dwn_[NUM_DOF] = {0, 0, 0, 0, 0, 0};
  int threshold_up_[NUM_DOF] =  {180, 180, 180, 180, 180, 180};
  int reset_position_[NUM_DOF] = {90, 90, 90, 90, 90, 90};
  int zero_position_[NUM_DOF] = {90, 90, 90, 90, 90, 90};

  int pins_[NUM_DOF];

  Servo servos_[NUM_DOF];
};

// Simple target, consider some holding of active value...
class RoboTarget {
public:
  explicit RoboTarget(int pin) : pin_(pin) {}
  
  void init() {
    pinMode(pin_, INPUT);
  }

  int read() {
    return digitalRead(pin_);
  }

private:
  int pin_;
};

#endif
