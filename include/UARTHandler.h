#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include <Arduino.h>

class UARTHandler {
private:
    String inputBuffer;  
    String commandType;  

    float Kp, Ki, Kd;       
    int motor_number;
    int speed;  

    void extractPIDFromCommand(const String &command);
    float extractValue(const String &command, const String &key);

    
public:


    UARTHandler();

    void begin();  
    bool receiveData(String &command);
    void sendAcknowledgment(const String &message);
    void sendData(String &data);

    bool validateSpeedFormat(const String &command);
    bool validatePIDFormat(const String &command);

    int extractMotorNumber(const String &command, const String &prefix);
    void extractSpeedFromCommand(const String &command);
    void parsePIDCommand(const String &command);

    int getSpeed() const;

    float getKp() const;
    float getKi() const;
    float getKd() const;
};

#endif
