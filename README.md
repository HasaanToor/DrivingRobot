# DrivingRobot

An embedded project for an autonomous line-following and wall-following robot built in C on a **STM32F103RB** (Cortex-M3) micro-controller using the Keil µVision IDE.

## Overview

The robot consists of a chassis with wheels, motors, two sensors, and the STM32F103RB microcontroller.

The robot operates in two switchable modes:

- **Wall Following Mode** — uses an analog distance sensor to maintain a target distance from the wall on it's left, steering left, right or staying straight based on the sensor reading.
- **Line Avoidance Mode** — uses a digital line sensor to detect black surfaces on the floor and steer away from them.

A hardware button on the microcontroller (blue USER button) toggles between the two modes at runtime.

- [Wall Following Mode Demo](https://www.youtube.com/shorts/QLGb6LNNCp4)
- [Line Avoidance Mode Demo](https://www.youtube.com/shorts/xQRGO2_YNKI)
- [Pin Diagram](./ENEL%20351%20Project%20Pin%20Diagram)
- [Pictures at the bottom of Project Report](./ENEL%20351%20Project%20Report)

## Hardware

| Component | Description |
|---|---|
| MCU | STM32F103RB |
| Analog Distance Sensor | Sharp GP2Y0A51SK0F Analog Distance Sensor - 2cm to 15cm |
| Digital Line Sensor | QRE1113 Miniature Reflective Object Sensor |
| 2 Motor Drivers | Dual Driver TB6612FNG v2 |
| 4 Motors | DC Gearbox Motor "TT Motor", 200RPM, 3 to 6VDC |
| Chassis and 4 Wheels | YIKESHU 4WD 2 Layer Smart Robot Car Chassis Kit with Battery Box |
| Batteries | 1x 9V Battery and 4x 1.5V Batteries |

## How It Works

### Wall Following

The Analog to Digital Converter (ADC) on the microcontroller reads the input from the distance sensor at PA0 with 12-bit resolution and step size of 0.805 mV, it calculates the corresponding voltage from the sensor which is associated with a distance, distance thresholds instruct the robot on how to move in order to follow the wall:

| ADC Value Threshold | Approx. Distance From Wall | Action |
|---|---|---|
| `0x3E5` ≈ 1000 * 0.805mV ≈ 0.80V | > 7 cm | Turn left (too far from wall) |
| `0x3E5` – `0x5DD` 0.80V - 1.20V | 4.5 – 7 cm | Go straight (target zone) |
| `0x5DD` ≈ 1500 * 0.805mV ≈ 1.20V | < 4.5 cm | Turn right (too close to wall) |

### Line Avoidance

The microcontroller reads the digital input from the line sensor at PB7, which simply detects whether a black line is sensed or not. The robot turns right to avoid the black lines, and goes straight otherwise.

| Digital Value | Detection | Action |
|---|---|---|
| `1` | Black Line Detected | Turn right (avoid the line) |
| `0` | White Floor Detected | Go straight (target zone) |

## I/O Details
- **Battery Box + 1.5V Batteries** — provides power/voltage to the 2 motor drivers controlling the motors and wheels. 
- **9V Battery** — provides power/voltage to the STM32F103RB board, as a USB connection is impossible while on the move. 
- **Motor Drivers** — input is connected to the F103RB’s internal pulse width modulation (PWM) via several GPIO ports, utilizing 0-3.3V PWM to control the Duty Cycle of the 6V motors via output. The motors are connected to the wheels, therefore allowing me to control the speed of the motors/wheels and allowing movement of the robot in general. 
- **Line Sensor** — output is connected to a GPIO port in order to provide a digital input to the F103RB board, this digital input is used in the code to avoid the lines it detects. 
- **Distance Sensor** — output is connected to the F103RB’s internal analog to digital converter (ADC) as this sensor provides an analog input. The analog input will be converted via the ADC to maintain a certain distance from a nearby wall to its side.
