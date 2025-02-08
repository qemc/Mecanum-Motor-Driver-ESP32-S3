#include "UARTHandler.h"

UARTHandler::UARTHandler() {

    inputBuffer = "";
    motor_number = -1;
    speed  = 0;
    Kp = Ki = Kd = 0.0;

}

void UARTHandler::begin() {

    Serial.begin(115200);

}

// zczytywanie pełnych lini z portu szeregowego
bool UARTHandler::receiveData(String &command) {

    while (Serial.available() > 0) {

        char incomingChar = Serial.read();

        if (incomingChar == '\n') {  

            command = inputBuffer;
            inputBuffer = "";

            return true;

        } else {

            inputBuffer += incomingChar;
        }
    }
    return false;
}


void UARTHandler::parsePIDCommand(const String &command) {

    extractPIDFromCommand(command);
    sendAcknowledgment("SET_PID executed");

}

void UARTHandler::extractSpeedFromCommand(const String &command) {

    speed = static_cast<int>(extractValue(command, "RPM:"));

    sendAcknowledgment("SET_SPEED executed");

}

void UARTHandler::extractPIDFromCommand(const String &command) {

    Kp = extractValue(command, "Kp:");
    Ki = extractValue(command, "Ki:");
    Kd = extractValue(command, "Kd:");

}

int UARTHandler::extractMotorNumber(const String &command, const String &prefix) {


    if(command.startsWith(prefix)) {

        motor_number = command.substring(prefix.length(), command.indexOf(";")).toInt();
        return motor_number;
    }

    return -1;
}



float UARTHandler::extractValue(const String &command, const String &key) {

    int keyIndex = command.indexOf(key);

    if (keyIndex == -1){

        sendAcknowledgment( key + " not found in command, value set to 0");
        return 0.0;
    }

    keyIndex += key.length();
    int endIndex = command.indexOf(';', keyIndex);

    if (endIndex == -1) {

        sendAcknowledgment("not proper format of the command, value set to 0");
        return 0.0;

    }
    return command.substring(keyIndex, endIndex).toFloat();
}

bool UARTHandler::validateSpeedFormat(const String &command) {

    return command.startsWith("SET_SPEED_MOTOR_") &&
           command.indexOf("RPM:") != -1;
}

bool UARTHandler::validatePIDFormat(const String &command) {

    return command.startsWith("SET_PID_MOTOR_") &&
           command.indexOf("Kp:") != -1 &&
           command.indexOf("Ki:") != -1 &&
           command.indexOf("Kd:") != -1;
}



void UARTHandler::sendAcknowledgment(const String &message) {

    Serial.println("ACK: " + message);

}

void UARTHandler::sendData(String &data) {

    static unsigned long lastSendTime = 0;  
    unsigned long currentTime = millis();   

    if (currentTime - lastSendTime >= 100) {  

        Serial.println(data);                
        lastSendTime = currentTime;          
    
    }
}

int UARTHandler::getSpeed() const { return speed; }


float UARTHandler::getKp() const { return Kp; }
float UARTHandler::getKi() const { return Ki; }
float UARTHandler::getKd() const { return Kd; }
