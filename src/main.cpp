#include <Arduino.h>
#include "Service.h"
#include "UARTHandler.h"
#include "MotorDriver.h"



//FL
MotorDriver motor0(8, 7, 6, 34, 33);
//FR
MotorDriver motor1(40, 39, 38, 13, 14);
//RL
MotorDriver motor2(2, 5, 4, 17, 18);
//RR
MotorDriver motor3(35, 37, 36, 15, 16);

// Tablica wskaźników do silników
MotorDriver* motors[] = {&motor0, &motor1, &motor2, &motor3};
// UART Handler
UARTHandler uart;
// Service zarządzający silnikami i komunikacją
Service service(&uart, motors, 4);

void setup() {
    service.begin();
}

void loop() {
    service.updateMotors();
}







// void setup() {
//     Serial.begin(115200);

//     // Inicjalizacja UART
//     uart.begin();

//     // Konfiguracja silników
//     motor0.setup(0);
//     motor1.setup(1);
//     motor2.setup(2);
//     motor3.setup(3);

//     Serial.println("System Initialized. Ready for commands.");
// }