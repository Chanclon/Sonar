# 📡 Sonar Project (Servo + Ultrasonic Sensor)

![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)
![IDE](https://img.shields.io/badge/IDE-Code_Composer_Studio-green?style=for-the-badge&logo=eclipse)
![Microcontroller](https://img.shields.io/badge/Microcontroller-MSP430FR6989-red?style=for-the-badge)
![Fusion 360](https://img.shields.io/badge/Autodesk%20Fusion%20360-%23E93C40.svg?style=for-the-badge&logo=autodesk&logoColor=white)

This repository contains the C code for a sonar system developed for the MSP430FR6989 microcontroller. The system uses a servomotor to perform sweeps and an ultrasonic sensor to detect the distance of objects in real-time, sending the results via serial communication (UART).

---

## 🛠️ Technologies and Hardware

* **Language:** C
* **Development Environment:** Code Composer Studio (CCS)
* **3D Design:** Autodesk Fusion 360
* **Microcontroller:** Texas Instruments MSP430FR6989
* **Components:** 
  * Servomotor
  * Ultrasonic Sensor (e.g., HC-SR04)
  * Resistors (for voltage divider)
  * Custom 3D printed base

---

## 🖨️ Custom 3D Printed Base

To hold the system together, the physical structure of the sonar (the base and mounts) was entirely custom-designed by me using **Autodesk Fusion 360** and later **3D printed**. This custom mount ensures a perfect fit for both the servomotor and the ultrasonic sensor, allowing it to perform smooth and stable sweeps.

---

## ⚙️ How the Code Works

The system is designed using interrupts and timers to avoid excessive processor usage (avoiding constant polling) and achieve highly precise operation:

1. **Servomotor Control (Timer_A0):** A PWM-like signal is generated and controlled via interrupts. The servo updates its position every 20 ms, performing continuous sweeps from left to right.
2. **Distance Measurement (Timer_A1 & Port Interrupts):** The `Trig` pin fires a pulse every 100 ms. The `Echo` pin generates an interrupt on a rising edge when it starts receiving the signal and switches to a falling edge detection when it finishes. The time difference is captured by Timer_A1 and converted into centimeters.
3. **UART Communication (UCA1):** The calculated results are sent through the serial interface to the computer in the following format:
   `Angulo: XX | Distancia: YY cm`
   *(Note: If the object is too far away, returns 0, or the sensor is blocked, the system alerts the user by sending "Fuera de rango o bloqueado").*

---

## 🔌 Hardware Connections

To replicate this project, make the following connections between your components and the microcontroller:

| Component | MSP430FR6989 Pin | Description |
| :--- | :--- | :--- |
| **Ultrasonic Sensor** | `P1.4` | Trigger Signal (TRIG) - Output |
| **Ultrasonic Sensor** | `P1.5` | Echo Signal (ECHO) - Input with Interrupt (via Voltage Divider) |
| **Servomotor** | `P1.6` | Position Control Signal - Output |
| **UART (TX/RX)** | `P3.4` / `P3.5` | Data Transmission and Reception |

### ⚠️ Important Hardware Note (Voltage Protection)
The ultrasonic sensor operates at **5V** and outputs a 5V logic signal on its `Echo` pin. However, the MSP430FR6989 microcontroller operates strictly at **3.3V**. 
* **The Issue:** Connecting the `Echo` pin directly to `P1.5` inputs an overvoltage that causes the microcontroller to automatically reset to prevent damage.
* **The Solution:** A **voltage divider** using resistors must be built on the breadboard (protoboard) to safely step down the 5V `Echo` output to a safe 3.3V level before routing it to the microcontroller pin.

---

## 🎥 Video Demonstration

You can watch a real-time demonstration of this sonar in action at the following link:

[![YouTube Demonstration](https://img.shields.io/badge/YouTube-Watch_Video-red?style=for-the-badge&logo=youtube)](https://youtu.be/aFmkXT8TLmY?is=pJZSEPJJsNDwtfes)

---

## 👨‍💻 Contact

Created by **[Gonzalo Ripoll Chanclon]**. 

If you'd like to learn more about my work or connect professionally, you can find me here:

[![LinkedIn](https://img.shields.io/badge/LinkedIn-My_Profile-blue?style=for-the-badge&logo=linkedin)](https://www.linkedin.com/in/chanclon)
