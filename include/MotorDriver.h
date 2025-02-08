#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

class MotorDriver {
private:

    uint8_t pwmPin;
    uint8_t dir1Pin;
    uint8_t dir2Pin;
    uint8_t encAPin;
    uint8_t encBPin;

    int8_t direction; 

    float targetRPM;
    float absTargetRPM;
    float currentRPM;

    const unsigned int ppr = 546;

    int maxRPM = 100;
    int minPWM = 100;
    int minRPM = 40;
    
    volatile long encoderCount; 
    static MotorDriver* instances[4]; 

    unsigned long lastMotorUpdateTime;
    unsigned long lastCurrentRPMUpdateTime;

    float Kp, Ki, Kd; 
    float integral;
    float lastError;

    int pwmValue;
    int proportion;
    int instance;//number of the motor that class belongs to. Useful for data export


    static void encoderInterruptHandler0();
    static void encoderInterruptHandler1();
    static void encoderInterruptHandler2();
    static void encoderInterruptHandler3();

    void updateCurrentRPM(); //calculated using encoder pulses
    void PIDcontroller();//as an input goes targetRPM and currentRPM, output PWMValue

public:



    MotorDriver(uint8_t pwmPin, uint8_t dir1Pin, uint8_t dir2Pin, 
                uint8_t encAPin, uint8_t encBPin);

    void updateTargetRPM(int8_t proportion); 

    void setPID(float Kp, float Ki, float Kd); 
    
    void updateMotor();

    void setPWM(uint8_t pwm); 

    void setup(int instanceIndex);
    void restartIntergal();

    String prepData();
    
};


#endif
