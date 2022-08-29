class PirSensor : public Component {
  int pin, value;
  
public:
  PirSensor(int pin) : pin(pin) {
      pinMode(pin, INPUT);
  }

  virtual void stateOn() {
    value = digitalRead(pin);
    setValue(value);
  }

  void setValue(int _value) {
    value = _value;
  }

  int getValue() {
    return value;
  }
};
