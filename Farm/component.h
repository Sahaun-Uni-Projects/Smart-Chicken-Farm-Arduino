class Component {
protected:
  STATE state = ON;
  
public:
  virtual void setup() {}
  virtual void stateDisabled() {}
  virtual void stateOff() {}
  virtual void stateOn() {}

  virtual void loop() {
    switch (state) {
      case DISABLED: stateDisabled(); break;
      case OFF: stateOff(); break;
      case ON: stateOn(); break;
      default: break;
    }
  }

  STATE getState() {
    return state;
  }

  void setState(STATE _state) {
    state = _state;
  }
};
