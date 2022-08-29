class Led : public Component {
  int pin, value;
  
public:
  Led(int pin) : pin(pin) {
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
