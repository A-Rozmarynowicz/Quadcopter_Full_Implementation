![Robotics](https://img.shields.io/badge/Robotics-orange)
![Control Engineering](https://img.shields.io/badge/Control_Engineering-gold)
![IT](https://img.shields.io/badge/IT-purple)
![Electronics](https://img.shields.io/badge/Electronics-darkred)
![Mechatronics](https://img.shields.io/badge/Mechatronics-darkgreen)


![Hardware](https://img.shields.io/badge/Hardware-blue)
![Software](https://img.shields.io/badge/Software-lightblue)

# Design and implementation of a quadrotor with a localization algorithm based on Ultra-Wideband beacons.

My Bachelor's project's task is to build a quadcopter from scratch, implement control and trajectory planning algorithms, synthesize a mathematical model and develop an indoor positioning system. The expected outcome is a functional, maneuverable, and stable drone that is able to follow given trajectories.

## Table of contents

## General information
### Electronics
- An STM32F4 is used as the main MCU.
- An ESP32 onboard provides communication with a ground station and a custom positioning system (described later).
- IMU and a magnetometer support the positioning system in state estimation.
- Power source is a 680mAh, 3S, LiHV, lightweight, and small battery.
- Propellers are mounted on BLDC motors for high efficiency and power.
- Onboard power converter steps the voltage down to 3.3V.
- Safety measures are implemented to protect the device from overcurrent, overvoltage, reverse polarity and ESD events.
- An off-the-shelf ESC drives the motors, commanded by the MCU.


## Current progress

### Hardware

- The UWB positioning system has been developed in almost a 100%, and the details can be found in this separate repository: [https://github.com/A-Rozmarynowicz/UWB_Positioning_System](https://github.com/A-Rozmarynowicz/UWB_Positioning_System)

<img src="./Images/UWB_PCB_3D_Image.png" alt="./Images/UWB_PCB_3D_Image.png" width="550"/>


---
\
\
\
\
\- The drone's main carbon frame has been designed to a great extend.

---
\
\
\
\
\- The 3D-printed supports and casings are being designed.

### Sofware
- Apart from the UWB system, the software development has not yet began.

### Modeling
- The research on drone modeling techniques is being done.

