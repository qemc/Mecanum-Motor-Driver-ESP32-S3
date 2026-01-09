# Mecanum Motor Driver — ESP32-S3

Firmware for controlling four DC motors with encoders on a Mecanum wheel robot platform. Communicates with a Raspberry Pi via UART and maintains target speeds using PID control.

---

## Features

- **Individual motor control** — Set speed for each of the 4 wheels independently
- **PID regulation** — Maintains target RPM using encoder feedback
- **UART communication** — Receives commands from Raspberry Pi via serial
- **Real-time encoder processing** — Interrupt-driven pulse counting (462 pulses/revolution)

---

## Architecture

The firmware is structured into three main classes:

| Class | Responsibility |
|-------|----------------|
| **UARTHandler** | Serial communication, command parsing, validation |
| **MotorDriver** | PWM control, encoder interrupts, PID regulation |
| **Service** | Integrates UART and motors, main control loop |

---

## Commands

**Set motor speed:**
```
SET_SPEED_MOTOR_<N>;RPM:<value>;
```

**Set PID parameters:**
```
SET_PID_MOTOR_<N>;Kp:<value>;Ki:<value>;Kd:<value>;
```

Where `<N>` is the motor number (0-3).

---

## Hardware

- **Microcontroller:** ESP32-S3-Pico
- **Motor drivers:** L298N (×2)
- **Motors:** 12V DC with magnetic encoders (240 RPM, 42:1 gearbox)
- **Encoder resolution:** 462 pulses per output shaft revolution

---

## Related

Main robot project: [Mecanum-Wheels-Robot](https://github.com/qemc/Mecanum-Wheels-Robot)

---

<p align="center">
  <em>Part of the Mecanum Wheels Mobile Robot — Engineering Thesis, Poznań University of Technology, 2025</em>
</p>
