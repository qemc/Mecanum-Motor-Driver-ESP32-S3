#ifndef SERVICE_H
#define SERVICE_H

#include "UARTHandler.h"
#include "MotorDriver.h"

class Service {
private:
    UARTHandler* uart;             // Pointer to UARTHandler instance
    MotorDriver** motors;          // Array of MotorDriver instances
    uint8_t motorCount;            // Number of motors

public:
    // Constructor
    Service(UARTHandler* uart, MotorDriver** motors, uint8_t motorCount);

    // Initialization
    void begin();

    // Main command handler
    void handleCommands();

    // Motor control loop
    void updateMotors();
};

#endif
