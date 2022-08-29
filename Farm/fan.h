class Fan : public Component {
  int pin, value;
  
public:
  Fan(int pin) : pin(pin) {
    pinMode(pin, OUTPUT);
  }

  void setValue(int _value) {
    value = _value;
    digitalWrite(pin, (value == 0) ? LOW : HIGH);
  }

  int getValue() {
    return value;
  }
};
