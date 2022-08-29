class Buzzer : public Component {
  int pin, value;
  
public:
  Buzzer(int pin) : pin(pin) {
      pinMode(pin, OUTPUT);
  }

  void play(int freq, int dur = -1) {
    if (dur == -1) tone(pin, freq);
      else tone(pin, freq, dur);
  }

  void stop() {
    noTone(pin);
  }

  void setValue(int _value) {
    value = _value;
    digitalWrite(pin, (value == 0) ? LOW : HIGH);
  }

  int getValue() {
    return value;
  }
};
