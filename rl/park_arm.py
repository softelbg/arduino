from robo_arm_gym import *

rc = RoboArmComm()

p, t = rc.position()

list_action = [81, 81, 90, 130]
for i, a in enumerate(list_action):
  rc.move(i, a - p[i])

