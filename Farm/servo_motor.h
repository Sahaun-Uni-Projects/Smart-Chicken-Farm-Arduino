#include <Servo.h>

class ServoMotor : public Component {
  int pin, angle, target, speed;
  Servo servo;
  
public:
  ServoMotor(int pin) : pin(pin) {}

  void setup() {
    servo.attach(pin);
    setAngle(0);
  }

  int getAngle() {
    return angle;
  }

  void setAngle(int ang) {
    angle = ang;
    servo.write(angle);
  }

  void approach(int from, int to, int spd) {
    // Good enough for 180-degree servo motors
    int diff = to-from;
    from = (diff >= 0) ? std::min(from+spd, to) : std::max(from-spd, to);
    setAngle(from);
  }
};
