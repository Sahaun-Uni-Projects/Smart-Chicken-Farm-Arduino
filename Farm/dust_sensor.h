class DustSensor : public Component {
  int driverPin, measurePin;
  int samplingDelay;  // uS
  double voltage;
  
public:
  DustSensor(int driverPin, int measurePin, int samplingDelay)
    : driverPin(driverPin), measurePin(measurePin), samplingDelay(samplingDelay) {
    pinMode(driverPin, INPUT);
    pinMode(measurePin, OUTPUT);
  }

  virtual void stateOn() {
    digitalWrite(driverPin, LOW);
    delayMicroseconds(samplingDelay);
    
    voltage = analogRead(measurePin);

    digitalWrite(driverPin, HIGH);
  }

  double getDustDensity() {
    return 170. * (voltage * 5./1024. - 0.1);
  }
};
