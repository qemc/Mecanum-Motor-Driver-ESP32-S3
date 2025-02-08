#include "Service.h"

const String SET_PID_COMMAND = "SET_PID_MOTOR_";
const String SET_SPEED_COMMAND = "SET_SPEED_MOTOR_";

Service::Service(UARTHandler* uart, MotorDriver** motors, uint8_t motorCount) {
    this->uart = uart;
    this->motors = motors;
    this->motorCount = motorCount;
}

void Service::begin() {

    uart->begin();  

    motors[0]->setup(0);
    motors[1]->setup(1);
    motors[2]->setup(2);
    motors[3]->setup(3);
}

void Service::handleCommands() {

    String command;

    if (uart->receiveData(command)) {

        if (command.startsWith(SET_SPEED_COMMAND)) {

            if (!uart->validateSpeedFormat(command)) {

                uart->sendAcknowledgment("Invalid speed command format");
                return;
            }

            int motorNumber = uart->extractMotorNumber(command, SET_SPEED_COMMAND);
            uart->extractSpeedFromCommand(command);
            motors[motorNumber]->updateTargetRPM(uart->getSpeed());
            
            uart->sendAcknowledgment("SET_SPEED executed");

        } 
        else if (command.startsWith(SET_PID_COMMAND)) {

            if (!uart->validatePIDFormat(command)) {

                uart->sendAcknowledgment("Invalid PID command format");
                return;
            }

            uart->parsePIDCommand(command);
            int motorNumber = uart->extractMotorNumber(command, SET_PID_COMMAND);

            if (motorNumber >= 0 && motorNumber < 4) {

                float Kp = uart->getKp();
                float Ki = uart->getKi();
                float Kd = uart->getKd();

                motors[motorNumber]->setPID(Kp, Ki , Kd);
                uart->sendAcknowledgment("SET_PID for Motor " + String(motorNumber) + " executed");

            } 
            else {
                uart->sendAcknowledgment("Invalid motor number");
            }
        } 
        else {
            uart->sendAcknowledgment("Unknown Command");
        }
    }
}

void Service::updateMotors() {

    this->handleCommands();

    motors[0]->updateMotor();
    auto data_motor0 = motors[0]->prepData();
    uart->sendData(data_motor0); 

    motors[1]->updateMotor();
    auto data_motor1 = motors[1]->prepData();
    uart->sendData(data_motor1); 

    motors[2]->updateMotor();
    auto data_motor2 = motors[2]->prepData();
    uart->sendData(data_motor2); 

    motors[3]->updateMotor();
    auto data_motor3 = motors[3]->prepData();
    uart->sendData(data_motor3); 
}
