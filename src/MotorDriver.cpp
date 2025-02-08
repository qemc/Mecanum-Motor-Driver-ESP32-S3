#include "MotorDriver.h"

// statyczna tablica przechowująca wskaźniki do zainicjalizowanych obiektów, 
// z góry wiadomo, ze bedzie ich 4
MotorDriver* MotorDriver::instances[4] = {nullptr, nullptr, nullptr, nullptr};

MotorDriver::MotorDriver(uint8_t pwmPin, uint8_t dir1Pin, uint8_t dir2Pin, uint8_t encAPin, uint8_t encBPin) {
    
    this->pwmPin = pwmPin;
    this->dir1Pin = dir1Pin;
    this->dir2Pin = dir2Pin;
    this->encAPin = encAPin;
    this->encBPin = encBPin;
    this->encoderCount = 0;

    this->lastMotorUpdateTime = 0;
    this->lastCurrentRPMUpdateTime = 0;

    this->currentRPM = 0;
    this->direction = 0;
    this->maxRPM = 100;

    this->Kp = 3.8;
    this->Ki = 0.33;
    this->Kd = 0.17;


    this->maxRPM=100;
    this->minPWM=50;
    this->minRPM=15;
    
    this->integral = 0;
    this->lastError = 0;
    this-> pwmValue = 0;
    this->targetRPM = 0;
    this->absTargetRPM = 0;
    this->proportion = 0;
}

void MotorDriver::setup(int instanceIndex){

  
    pinMode(dir1Pin, OUTPUT);
    pinMode(dir2Pin, OUTPUT);

    pinMode(encAPin, INPUT_PULLUP);
    pinMode(encBPin, INPUT_PULLUP);

    pinMode(pwmPin, OUTPUT);
    this->instance = instanceIndex;

    // przypisanie numeru silnika do danego obiektu
    instances[instanceIndex] = this;

    // Przypisanie funckji przerwania do odpowiedniego pinu enkodera
    switch (instanceIndex) {
    case 0:
        attachInterrupt(digitalPinToInterrupt(encAPin), encoderInterruptHandler0, RISING);
        break;
    case 1:
        attachInterrupt(digitalPinToInterrupt(encAPin), encoderInterruptHandler1, RISING);
        break;
    case 2:
        attachInterrupt(digitalPinToInterrupt(encAPin), encoderInterruptHandler2, RISING);
        break;
    case 3:
        attachInterrupt(digitalPinToInterrupt(encAPin), encoderInterruptHandler3, RISING);
        break;
    }
}

void MotorDriver::encoderInterruptHandler0() {

    // jeśli silnik został zainiclalizowany i wywołało sie przerwanie na pinie enkoder, 
    // zwiększamy jego licznik
    if (instances[0]) {
        instances[0]->encoderCount += 1;
    }
}

void MotorDriver::encoderInterruptHandler1() {
    if (instances[1]) {
        instances[1]->encoderCount += 1;
    }
}

void MotorDriver::encoderInterruptHandler2() {
    if (instances[2]) {
        instances[2]->encoderCount += 1;
    }
}

void MotorDriver::encoderInterruptHandler3() {
    if (instances[3]) {
        instances[3]->encoderCount += 1;
    }
}
void MotorDriver::restartIntergal(){
    this -> integral = 0;
}
void MotorDriver::updateCurrentRPM(){

    unsigned long currentTime = millis();
    unsigned long deltaTime = currentTime - lastCurrentRPMUpdateTime;

    if (deltaTime >= 100) { 
        noInterrupts();          
        long counts = encoderCount;
        encoderCount = 0;
        interrupts();

        currentRPM = (counts * 60.0) / (ppr * (deltaTime / 1000.0));
        lastCurrentRPMUpdateTime = currentTime;
    }
}

void MotorDriver::updateTargetRPM(int8_t rpm) {

    this->proportion = rpm; 

    if (proportion > 0) {
        direction = 1;  
        digitalWrite(dir1Pin, HIGH);
        digitalWrite(dir2Pin, LOW);
    } else if (proportion < 0) {
        direction = -1;  
        digitalWrite(dir1Pin, LOW);
        digitalWrite(dir2Pin, HIGH);
    } else {
        direction = 0;
        digitalWrite(dir1Pin, LOW);
        digitalWrite(dir2Pin, LOW); 
        targetRPM = 0;      
        restartIntergal();        
        return;
    }
    
    int speed_to_set = constrain(abs(proportion), 0, 100);
    targetRPM = abs(proportion);
}

void MotorDriver::setPWM(uint8_t pwm) {

    if (proportion == 0) { 
        analogWrite(pwmPin, 0);
        digitalWrite(dir1Pin, LOW);
        digitalWrite(dir2Pin, LOW);
    } else {
        pwmValue = constrain(pwm, minPWM, 255);
        analogWrite(pwmPin, pwmValue);
    }
}

void MotorDriver::PIDcontroller() {

    float error = abs(targetRPM) - currentRPM;

    float proportional = Kp * error;


    integral += error;
    integral = constrain(integral, -500.0, 500.0);


    float derivative = error - lastError;
    derivative = constrain(derivative, -50.0, 50.0);
    float pidOutput = proportional + Ki * integral + Kd * derivative;

    if(proportion!=0){

        pwmValue = constrain((int)abs(pidOutput), minPWM, 255);    
    }
    
    setPWM(pwmValue);
    lastError = error;
}


void MotorDriver::updateMotor() {

    unsigned long currentTime = millis();
    unsigned long deltaTime = currentTime - lastMotorUpdateTime;

    updateCurrentRPM();

    if (deltaTime >= 100) {

        lastMotorUpdateTime = currentTime;

        PIDcontroller();
    }

}

void MotorDriver::setPID(float Kp, float Ki, float Kd){

    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}

String MotorDriver::prepData() {

    String data = "MOTOR_DATA" + String(instance) + ";";
    data += "currentRPM:" + String(currentRPM) + ";";
    data += "targetRPM:" + String(targetRPM) + ";";
    data += "errorPID:" + String(targetRPM - currentRPM) + ";";
    data += "PWMvalue:" + String(pwmValue) + ";";
    data += "Direction:" + String(direction) + ";";
    data += "EncoderCounts:" + String(encoderCount) + ";";
    data += "Kp:" + String(Kp) + ";";
    data += "Kd:" + String(Kd) + ";";
    data += "Ki:" + String(Ki) + ";";

    return data;
}

