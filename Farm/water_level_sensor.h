class WaterLevelSensor : public Component {
  static const int MAX_LEVEL = 550;
  int pin, level;
  
public:
  WaterLevelSensor(int pin) : pin(pin) {
    pinMode(pin, INPUT);
  }

  virtual void stateOn() {
    setLevel(analogRead(pin));
  }

  void setLevel(int _level) {
    level = std::min(_level, MAX_LEVEL);
  }

  int getLevel() {
    return level;
  }
};
