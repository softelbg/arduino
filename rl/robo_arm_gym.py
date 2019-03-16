import math
import gym
from gym import spaces
from gym.utils import seeding
import numpy as np
from time import sleep
import serial
import cv2

class BaseCamera():
  def __init__(self, idx=0):
    print("Camera {} warming up".format(idx))
    self.idx = idx
    self.cap = cv2.VideoCapture(idx)
    sleep(1)
    print("Camera {} ready".format(idx))

  def frame_process(self):
    pass 

  def clk(self):
    self.ret, self.frame = self.cap.read()
    if self.ret:
      self.frame_process()
    return self.ret
 
  def __del__(self):
    self.cap.release()
    cv2.destroyAllWindows()

class RoboArmCamera(BaseCamera):
  def __init__(self, idx=0):
    super().__init__(idx)

    self.w = 300
    self.h = 300

    self.clk()

  def frame_process(self):
    self.frame_resized = cv2.resize(self.frame, (self.w, self.w), interpolation = cv2.INTER_AREA)
    cv2.imshow('cam_{}'.format(self.idx), self.frame_resized)

class RoboArmComm:
    def __init__(self):
        self.serial_port = '/dev/cu.usbserial-1430'
        print(self.serial_port, "connecting...")
        self.ser = serial.Serial(self.serial_port, 9600)
        sleep(3)
        print(self.serial_port, "connected")
        self.n_iter = 0
        self.reset()

    def read(self):
        rpos = self.ser.readline()
        position_val = list(map(int, rpos.split()))
        rtarget = self.ser.readline()
        target_val = int(rtarget.split()[1])
        print("pos", position_val, "target", target_val)
        return (position_val, target_val)

    def reset(self):
        print("reset")
        self.n_iter = 0
        self.ser.write("r:0:0".encode())
        p, t = self.read()
        return p, t

    def move(self, idx, d):
        print("move", idx, d)
        self.n_iter += 1
        self.ser.write("m:{}:{}".format(idx, d).encode())
        p, t = self.read()
        return p, t

    def position(self):
        self.ser.write("p:0:0".encode())
        p, t = self.read()
        return p, t

    def done(self):
        return self.n_iter > 300

class RoboArmEnv(gym.Env):
    metadata = {
        'render.modes': ['human', 'rgb_array'],
        'video.frames_per_second': 5 
    }

    def __init__(self):
        self.robo_arm = RoboArmComm()
        self.robo_cam = RoboArmCamera(0)

        self.viewer = None

        self.low = np.array([-5, -5, -5, -5])
        self.high = np.array([5, 5, 5, 5])
        self.action_space = spaces.Box(self.low, self.high)
        self.observation_space = spaces.Box(np.array([0, 0, 0, 0]), np.array([180, 180, 180, 180]))

        self.seed()
        self.reset()

    def seed(self, seed=None):
        self.np_random, seed = seeding.np_random(seed)
        return [seed]

    def step(self, action):
        # TODO: Tune reward fn...
        reward = 0.0
        position = []
        for idx, d in enumerate(action):
            p, t = self.robo_arm.move(idx, int(d))
            reward += t
            position = p

        done = self.robo_arm.done() and reward > 0

        self.state = position
        return np.array(self.state), reward, done, {}

    def reset(self):
        p, t = self.robo_arm.reset()
        self.state = np.array(p)
        return self.state

    def render(self, mode='human'):
        self.robo_cam.clk()
        img = self.robo_cam.frame_resized

        if self.viewer is None:
            from gym.envs.classic_control.rendering import SimpleImageViewer
            self.viewer = SimpleImageViewer()
            self.viewer.imshow(img)

        return img

    def close(self):
        if self.viewer:
            self.viewer.close()
            self.viewer = None
