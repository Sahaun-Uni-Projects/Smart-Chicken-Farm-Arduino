class PhotoResistor : public Component {
  int pin, value;
  
public:
  PhotoResistor(int pin) : pin(pin) {
      pinMode(pin, INPUT);
  }

  virtual void stateOn() { setValue(analogRead(pin)); }

  void setValue(int _value) {
    value = _value;
  }

  int getValue() {
    return value;
  }
};
