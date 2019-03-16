/*
  Softel Labs 2019
  s.georgiev@softel.bg
*/

#include "motor_4wd.h"
#include "softel_iremote_control_black.h"

IRemoteControlBlack ircontrol(2);

Motor4WD car_motor( 4, 37, 35,
                    3, 33, 31,
                    6, 47, 45,
                    7, 43, 41
                   );

void setup() {
  Serial.begin(9600);

  ircontrol.setup();
  car_motor.setup();
}


void test_mode_1(int speed) {
    car_motor.fwd(speed);
    delay(2000);
    car_motor.right(speed);
    delay(500);
    
    car_motor.fwd(speed);
    delay(2000);
    car_motor.right(speed);
    delay(500);

    car_motor.bwd(speed);
    delay(2000);
    car_motor.left(speed);
    delay(500);
    
    car_motor.bwd(speed);
    delay(2000);
    car_motor.left(speed);
    delay(500);
    
    car_motor.fwd(speed);
    delay(2000);
    
    car_motor.left(255);
    delay(3000);
    car_motor.right(255);
    delay(3000);
}

void run_test_mode() {
  for (int i = 64; i += 32; i < 256) {
    test_mode_1(i);
    car_motor.stop();
    delay(1000);
  }
}

int speed = 255;

void loop() {
  String cmd = ircontrol.run();
  if (cmd.length() > 0) {
    if (cmd == "ok") {
      car_motor.stop();
    } else if (cmd == "up") {
      car_motor.fwd(speed);
    } else if (cmd == "down") {
      car_motor.bwd(speed);
    } else if (cmd == "left") {
      car_motor.left(speed);
    } else if (cmd == "right") {
      car_motor.right(speed);
    } else if (cmd == "1") {
      speed = 64;
    } else if (cmd == "2") {
      speed = 128;
    } else if (cmd == "3") {
      speed = 192;
    } else if (cmd == "4") {
      speed = 255;
    } else if (cmd == "*") {
      run_test_mode();
    }

    delay(50);
  }
}
