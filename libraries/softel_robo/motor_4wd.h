/*
  Softel Labs 2019
  s.georgiev@softel.bg
*/

#ifndef __SOFTEL_MOTOR_4WD_H__
#define __SOFTEL_MOTOR_4WD_H__

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


class MotorWD {
public:
  MotorWD(int en, int in1, int in2)
    : pin_en(en), pin_in1(in1), pin_in2(in2) { }

  void drive(int speed, int in1, int in2) {
    digitalWrite(pin_in1, in1);
    digitalWrite(pin_in2, in2);
    analogWrite(pin_en, speed);
  }
  
  void setup() {
    pinMode(pin_en, OUTPUT);
    pinMode(pin_in1, OUTPUT);
    pinMode(pin_in2, OUTPUT);
  }

private:
  int pin_en;
  int pin_in1;
  int pin_in2;
};


class Motor4WD {
public:
  Motor4WD( int en_fl, int in_fl_1, int in_fl_2,
            int en_fr, int in_fr_1, int in_fr_2,
            int en_rl, int in_rl_1, int in_rl_2,
            int en_rr, int in_rr_1, int in_rr_2
  )
    : wd_fl(en_fl, in_fl_1, in_fl_2),
      wd_fr(en_fr, in_fr_1, in_fr_2),
      wd_rl(en_rl, in_rl_1, in_rl_2),
      wd_rr(en_rr, in_rr_1, in_rr_2)
  { }

  void setup() {
    wd_fl.setup();
    wd_fr.setup();
    wd_rl.setup();
    wd_rr.setup();
  }
  
  // TODO: no step control, need ir sensors on the rotation
  void stop() {
    wd_fl.drive(0, 0, 0);
    wd_fr.drive(0, 0, 0);
    wd_rl.drive(0, 0, 0);
    wd_rr.drive(0, 0, 0);
  }

  void fwd(int speed) {
    wd_fl.drive(speed, 1, 0);
    wd_fr.drive(speed, 1, 0);
    wd_rl.drive(speed, 1, 0);
    wd_rr.drive(speed, 1, 0);
  }
  
  void bwd(int speed) {
    wd_fl.drive(speed, 0, 1);
    wd_fr.drive(speed, 0, 1);
    wd_rl.drive(speed, 0, 1);
    wd_rr.drive(speed, 0, 1);
  }
  
  void right(int speed) {
    wd_fl.drive(speed, 1, 0);
    wd_fr.drive(speed, 0, 1);
    wd_rl.drive(speed, 1, 0);
    wd_rr.drive(speed, 0, 1);
  }
  
  void left(int speed) {
    wd_fl.drive(speed, 0, 1);
    wd_fr.drive(speed, 1, 0);
    wd_rl.drive(speed, 0, 1);
    wd_rr.drive(speed, 1, 0);
  }
   
private:
  MotorWD wd_fl; // Front Left
  MotorWD wd_fr;
  MotorWD wd_rl;
  MotorWD wd_rr; // Rear Right
};


#endif
  
