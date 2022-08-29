#include <Servo.h>

class ServoMotor : public Component {
  int pin, angle;
  Servo servo;
  
public:
  ServoMotor(int pin) : pin(pin) {}

  void setup() {
    servo.attach(pin);
  }

  int getAngle() {
    return angle;
  }

  void setAngle(int ang) {
    angle = ang;
    servo.write(angle);
  }
};
