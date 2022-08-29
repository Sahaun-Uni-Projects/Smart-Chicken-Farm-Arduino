class WaterLevelSensor : public Component {
  static const int MAX_LEVEL = 550;
  int pin, level;
  
public:
  WaterLevelSensor(int pin) : pin(pin) {
      pinMode(pin, INPUT);
  }

  virtual void stateOff() {
    setLevel(analogRead(pin));
    if (level < MAX_LEVEL) setState(ON);
  }

  virtual void stateOn() {
    setLevel(analogRead(pin));
    if (level >= MAX_LEVEL) setState(OFF);
  }

  void setLevel(int _level) {
    level = std::max(_level, MAX_LEVEL);
  }

  int getLevel() {
    return level;
  }
};
