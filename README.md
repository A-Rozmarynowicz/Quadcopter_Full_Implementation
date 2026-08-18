![Robotics](https://img.shields.io/badge/Robotics-orange)
![Control Engineering](https://img.shields.io/badge/Control_Engineering-gold)
![IT](https://img.shields.io/badge/IT-purple)
![Electronics](https://img.shields.io/badge/Electronics-darkred)
![Mechatronics](https://img.shields.io/badge/Mechatronics-darkgreen)


![Hardware](https://img.shields.io/badge/Hardware-blue)
![Software](https://img.shields.io/badge/Software-lightblue)

# Design and implementation of a quadrotor with a localization algorithm based on Ultra-Wideband beacons.

My Bachelor's project's task is to build a quadcopter from scratch, implement control and trajectory planning algorithms, synthesize a mathematical model, and develop an indoor positioning system. The expected outcome is a functional, maneuverable, and stable drone that is able to follow given trajectories.

Development time: from 01.2026 to 02.2027 (expected).

The project is also referred to as "Day One".

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="Graphics\Logos\DayOne_Logo_White_on_Github.png">
    <source media="(prefers-color-scheme: light)" srcset="Graphics\Logos\DayOne_Logo_Black_on_White.png">
    <img alt="Logo" src="Graphics\Logos\DayOne_Logo_White_on_Github.png" width="450">
  </picture>
</p>

## Table of contents
* [Current progress](#current-progress)
* [General Information](#general-information)
* [UWB Positioning System](#uwb-positioning-system)
* [Technologies Used](#technologies-used)


## Current progress

So far, hardware has been the main focus of development. The main PCB, visible in Figure 2, is designed. It will be soldered and ready by 24th August. The schematic document is available in this file: [.../Quadcopter_Main_PCB_Schematic.pdf](./Hardware/Electronics/Documents/Datasheets/Quadcopter_Main_PCB_Schematic.pdf).

<p align="center">
<img src="./Graphics/Readme_Images/Main_PCB_3D.png" alt="Main PCB" width="550"> <br>
<em>Figure 2: 3D view of the quadcopter main PCB.</em>
 </p>

The current 3D model of the quadcopter is visible in Figure 3.

<p align="center">
<img src="./Graphics/Readme_Images/Quadcopter_Assembly.png" alt="Quadcopter Render" width="550"> <br>
<em>Figure 3: 3D model of the quadcopter.</em>
 </p>

## General information

### Control Engineering
- Implementing a:
    - cascade 4-loop PID controller,
    - polynomial decoupled trajectory planning based on given setpoints,
    - Kalman filter state estimator.
- Utilizing quaternions for attitude representation enabling more stable orientation control.
- Synthesizing a mathematical model of the drone to enable tuning the algorithms in simulation.
- Enabling manual override of the speed or attitude commands.
- Leaving room for a camera to implement visual odometry and obstacle avoidance in the future.


### Electronics
- An STM32F4 is used as the main MCU.
- An ESP32 onboard provides communication with a ground station and a custom positioning system (described later).
- IMU and a magnetometer support the positioning system in state estimation.
- Power source is a 680mAh, 3S, LiHV, lightweight, and small battery.
- Propellers are mounted on BLDC motors for high efficiency and power.
- The onboard power converter steps the voltage down to 3.3V.
- Safety measures are implemented to protect the device from overcurrent, overvoltage, reverse polarity, and ESD events.
- An off-the-shelf ESC drives the motors, commanded by the MCU's DShot signals.


### Mechanics
- Designing a custom:
    - carbon base plate for the robot,
    - housing for the PCBs,
    - shielding frame around the propellers,
    - battery slot.

## UWB Positioning System
Part of the project is a custom positioning system that doesn't rely on a GPS signal. This is achieved by placing at least 4 Ultra-Wideband (UWB) modules near the area of the drone's operation. They measure their distance to the robot and estimate its relative position. This system is almost fully developed, and can be seen in this repository: [https://github.com/A-Rozmarynowicz/UWB_Positioning_System](https://github.com/A-Rozmarynowicz/UWB_Positioning_System).

<p align="center">
<img src="./Graphics/Readme_Images/UWB_PCB_3D_Image.png" alt="./Graphics/Readme_Images/UWB_PCB_3D_Image.png" width="550"> <br>
<em>Figure 1: 3D view of the UWB anchor PCB.</em>
 </p>



## Technologies Used
- Ultra-Wideband radio technology
- Multiple communication protocols: SPI, I2C, UART, ESP-NOW, DShot
- Control engineering:
    - cascade PID
    - polynomial decoupled trajectory calculation
    - Kalman filter

- Programming languages:
    - C
    - C++
    - MATLAB

- Software used:
    - STM32 Cube IDE
    - Altium Designer
    - Autodesk Fusion
    - MATLAB
    - PlatformIO
