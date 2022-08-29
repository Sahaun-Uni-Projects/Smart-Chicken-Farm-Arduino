#include "ArduinoSTL.h"
#include "state.h"
#include "component.h"

#include "fan.h"
#include "led.h"
#include "input.h"
#include "buzzer.h"
#include "pir_sensor.h"
#include "water_pump.h"
#include "dust_sensor.h"
#include "lcd_display.h"
#include "servo_motor.h"
#include "photo_resistor.h"
#include "humiture_sensor.h"
#include "water_level_sensor.h"

#define HUMIDITY_THRESHOLD 65.
#define TEMP_HOT 91     // F
#define DARKNESS_THRESHOLD 500

Keypad4x4 *input;
LcdDisplay *screen;
Led *darkLight1, *darkLight2, *heatLight, *motionLight;
Fan *coolerFan, *humidityFan;
HumitureSensor *dhtSensor;
PhotoResistor *lightSensor;
WaterLevelSensor *waterLevelSensor;
Buzzer *motionBuzzer;
WaterPump *waterPump;
PirSensor *motionSensor;
ServoMotor *doorServo;
DustSensor *dustSensor;

std::vector<Component*> components;
int FLAG;

void init_components() {
  input = new Keypad4x4(new byte[4]{46, 47, 48, 49}, new byte[4]{50, 51, 52, 53}, 4);
  screen = new LcdDisplay(A3, A2, A1, A0, A4);
  darkLight1 = new Led(22);
  darkLight2 = new Led(23);
  heatLight = new Led(24);
  motionLight = new Led(44);
  humidityFan = new Fan(26);
  coolerFan = new Fan(27);
  dhtSensor = new HumitureSensor(30);
  lightSensor = new PhotoResistor(A8);
  waterLevelSensor = new WaterLevelSensor(A9);
  waterPump = new WaterPump(32);
  motionSensor = new PirSensor(40);
  motionBuzzer = new Buzzer(42);
  doorServo = new ServoMotor(36, 90);
  dustSensor = new DustSensor(33, A10, 280);

  components = {
    input, screen, darkLight1, darkLight2, heatLight,
    humidityFan, coolerFan,
    dhtSensor, lightSensor,
    waterLevelSensor, waterPump,
    motionSensor, motionLight, motionBuzzer,
    doorServo,
    dustSensor,
  };
}

void ev_create() {
  Serial.begin(9600);
  FLAG = 0;
  
  init_components();
  for (Component* comp : components) comp->setup();
}

void ev_step_begin() {
  for (Component* comp : components) comp->loop();
  
  //Serial.println(water_sensor->getLevel());
  //int a = water_sensor->getLevel();
  //Serial.println(a);

  //led->setValue(0);
  //if (ldr->getValue() > 500) led->setValue(1);
  //cout << "LDR : " << ldr->getValue() << "\n";

  //Serial.println(ldr->getValue());

  if (false && dhtSensor->canRead()) {
    Serial.print(F(" Humidity: "));
    Serial.print(dhtSensor->getHumidity());
    Serial.print(F("%  Temperature: "));
    Serial.print(dhtSensor->getTemperature(false));
    Serial.print(F("°C "));
    Serial.print(dhtSensor->getTemperature(true));
    Serial.print(F("°F  Heat index: "));
    Serial.print(dhtSensor->getHeatIndex(false));
    Serial.print(F("°C "));
    Serial.print(dhtSensor->getHeatIndex(true));
    Serial.println(F("°F"));
  }

  //Serial.println(waterLevelSensor->getLevel());
}

void ev_step() {
  humidityFan->setValue((int)(dhtSensor->getHumidity() > HUMIDITY_THRESHOLD));

  int dark = (lightSensor->getValue() > DARKNESS_THRESHOLD);
  darkLight1->setValue(dark);
  darkLight2->setValue(dark);

  int hot = (dhtSensor->getTemperature() >= TEMP_HOT);
  heatLight->setValue(!hot);
  coolerFan->setValue(hot);

  waterPump->setValue(waterLevelSensor->getLevel() > 650);

  if (motionSensor->getValue() == 1) {
    //motionBuzzer->play(1000, 1000);
    motionLight->setValue(1);
  } else {
    //motionBuzzer->stop();
    motionLight->setValue(0);
  }

  Serial.println(dustSensor->getDustDensity());
  
  int doorAngleValue = -1;
  
  if (input->keyPressed()) {
    Serial.print("Input Buffer : ");
    Serial.println(input->bufferGet());

    // switch-case does not work (!)
    
    char in = input->getKey();
    if (in == 'C') {
      FLAG |= 1;
      input->bufferClear();
    } else if (in == 'D') {
      String cmd = input->bufferGet();
      int len = cmd.length();
      if (len == 4) {
        cmd.remove(len-1, 1);
        Serial.print("Executing command: ");
        Serial.println(cmd);

        if (cmd.equals("AAA")) {
          if (motionSensor->getState() == DISABLED) {
            Serial.println("PIR Sensor enabled");
            motionSensor->setState(ON);
          } else {
            Serial.println("PIR Sensor disabled");
            motionSensor->setState(DISABLED);
          }
        }
      }
      //FLAG |= 1;
      //input->bufferPop();
    } else if (in == 'A') {
      //doorAngleValue = 1;
    } else if (in == 'B') {
      //doorAngleValue = 2;
    }
  }
  
  if(doorAngleValue == 1) {
    doorServo->approach(doorServo->getAngle(), 90, 15);
  }
  else if(doorAngleValue == 2)
  {
    doorServo->approach(doorServo->getAngle(), 0, 15);
  }
}

void ev_step_end() {}

void ev_draw_begin() {}

void ev_draw() {
  screen->drawTextGeneral(10, 10, "Smart", 4, c_red);
  screen->drawTextGeneral(10, 50, "Chicken Farm", 3, c_red);
  screen->drawTextGeneral(10, 80, "190104019 RAHIM", 3, c_blue);
  screen->drawTextGeneral(10, 110, "190104020 SAJID", 3, c_blue);
  screen->drawTextGeneral(10, 140, "190104021 SOHOM", 3, c_blue);
  screen->drawTextGeneral(10, 170, "190104024 MAMUN", 3, c_blue);
  screen->drawText(10, 210, input->bufferGet());
}

void ev_draw_end() {}

void setup() {
  ev_create();
}

void loop() {
  FLAG = 0;
  
  // Step
  ev_step_begin();
  ev_step();
  ev_step_end();

  // Clear Buffer
  if (FLAG & 1) screen->clearScreen();
  
  // Draw
  ev_draw_begin();
  ev_draw();
  ev_draw_end();
}
