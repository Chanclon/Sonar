# 📡 Sonar Project (Servo + Ultrasonic Sensor)

![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)
![IDE](https://img.shields.io/badge/IDE-Code_Composer_Studio-green?style=for-the-badge&logo=eclipse)
![Microcontroller](https://img.shields.io/badge/Microcontroller-MSP430FR6989-red?style=for-the-badge)

This repository contains the C code for a sonar system developed for the MSP430FR6989 microcontroller. The system uses a servomotor to perform sweeps and an ultrasonic sensor to detect the distance of objects in real-time, sending the results via serial communication (UART).

---

## 🛠️ Technologies and Hardware

* **Language:** C
* **Development Environment:** Code Composer Studio (CCS)
* **Microcontroller:** Texas Instruments MSP430FR6989
* **Components:** * Servomotor
  * Ultrasonic Sensor (e.g., HC-SR04)

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
| **Ultrasonic Sensor** | `P1.5` | Echo Signal (ECHO) - Input with Interrupt |
| **Servomotor** | `P1.6` | Position Control Signal - Output |
| **UART (TX/RX)** | `P3.4` / `P3.5` | Data Transmission and Reception |

---

## 🎥 Video Demonstration

You can watch a real-time demonstration of this sonar in action at the following link:

[![YouTube Demonstration](https://img.shields.io/badge/YouTube-Watch_Video-red?style=for-the-badge&logo=youtube)](YOUR_YOUTUBE_VIDEO_LINK_HERE)

---

## 👨‍💻 Contact

Created by **[Your Name/Last Name]**. 

If you'd like to learn more about my work or connect professionally, you can find me here:

[![LinkedIn](https://img.shields.io/badge/LinkedIn-My_Profile-blue?style=for-the-badge&logo=linkedin)](YOUR_LINKEDIN_LINK_HERE)
