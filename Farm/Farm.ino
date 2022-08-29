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

#define DUST_THRESHOLD 400.
#define WATER_THRESHOLD 500
#define PUMP_THRESHOLD 500
#define TEMP_HOT 92     // F
#define DARKNESS_THRESHOLD 530

Keypad4x4 *input;
LcdDisplay *screen;
Led *darkLight1, *darkLight2, *heatLight, *motionLight;
Fan *coolerFan, *dustFan;
HumitureSensor *dhtSensor;
PhotoResistor *lightSensor;
WaterLevelSensor *waterLevelSensor;
Buzzer *motionBuzzer;
WaterPump *waterPump;
PirSensor *motionSensor;
ServoMotor *doorServo, *foodServo;
DustSensor *dustSensor;

std::vector<Component*> components;
int FLAG, doorAngleValue, foodAngleValue, counter=0;
String tempValue, pirValue, ldrValue, waterValue, dustValue, doorValue, foodValue;

void init_components() {
  input = new Keypad4x4(new byte[4]{46, 47, 48, 49}, new byte[4]{50, 51, 52, 53}, 3);
  screen = new LcdDisplay(A3, A2, A1, A0, A4);
  darkLight1 = new Led(22);
  darkLight2 = new Led(23);
  heatLight = new Led(24);
  motionLight = new Led(44);
  dustFan = new Fan(26);
  coolerFan = new Fan(27);
  dhtSensor = new HumitureSensor(30);
  lightSensor = new PhotoResistor(A8);
  waterLevelSensor = new WaterLevelSensor(A9);
  waterPump = new WaterPump(32);
  motionSensor = new PirSensor(40);
  motionBuzzer = new Buzzer(42);
  doorServo = new ServoMotor(36, 90);
  foodServo = new ServoMotor(38, 0);
  dustSensor = new DustSensor(33, A10, 280);

  components = {
    input, screen, darkLight1, darkLight2, heatLight,
    dustFan, coolerFan,
    dhtSensor, lightSensor,
    waterLevelSensor, waterPump,
    motionSensor, motionLight, motionBuzzer,
    doorServo, foodServo,
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
}

void ev_step() {
  dustFan->setValue((int)(dustSensor->getDustDensity() > DUST_THRESHOLD));

  int dark = (lightSensor->getValue() > DARKNESS_THRESHOLD);
  darkLight1->setValue(dark);
  darkLight2->setValue(dark);

  int hot = (dhtSensor->getTemperature() >= TEMP_HOT);
  heatLight->setValue(!hot);
  coolerFan->setValue(hot);

  waterPump->setValue(waterLevelSensor->getLevel() < WATER_THRESHOLD);

  if (motionSensor->getValue() == 1) {
    //motionBuzzer->play(1000, 1000);
    motionLight->setValue(1);
  } else {
    //motionBuzzer->stop();
    motionLight->setValue(0);
  }
  
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
      
      if (len == 3) {
        //cmd.remove(len-1, 1);
        Serial.print("Executing command: ");
        Serial.println(cmd);

        if (cmd.equals("999")) {
          if (motionSensor->getState() == DISABLED) {
            Serial.println("PIR Sensor enabled");
            motionSensor->setState(ON);
          } else {
            Serial.println("PIR Sensor disabled");
            motionSensor->setState(DISABLED);
          }
        }
      }
    } else if (in == 'A') {
      doorAngleValue = 1;
    } else if (in == 'B') {
      doorAngleValue = 2;
    }else if (in == '9') {
      foodAngleValue = 1;
    } else if (in == '6') {
      foodAngleValue = 2;
    }
  }
  
  if(doorAngleValue == 1) {
    doorServo->approach(doorServo->getAngle(), 90, 15);
  }
  else if(doorAngleValue == 2)
  {
    doorServo->approach(doorServo->getAngle(), 0, 15);
  }

  if(foodAngleValue == 1) {
    foodServo->approach(foodServo->getAngle(), 0, 15);
  }
  else if(foodAngleValue == 2)
  {
    foodServo->approach(foodServo->getAngle(), 90, 15);
  }
}

void ev_step_end() {}

void ev_draw_begin() {}

void ev_draw() {
 
  screen->drawTextGeneral(10, 5, "SmartChickenFarm", 3, c_red);
  screen->drawTextGeneral(10, 30, "190104019 RAHIM", 2, c_blue);
  screen->drawTextGeneral(10, 50, "190104020 SAJID", 2, c_blue);
  screen->drawTextGeneral(10, 70, "190104021 SOHOM", 2, c_blue);
  screen->drawTextGeneral(10, 90, "190104024 MAMUN", 2, c_blue);
  screen->drawTextGeneral(10, 100, "----------------------", 2, c_black);

    if(counter == 0)
    {
      //tempValue, pirValue, ldrValue, waterValue, dustValue, doorValue, foodValue;
        
      tempValue = "Temperature: ";
      if(dhtSensor->getTemperature() >= TEMP_HOT)
        tempValue += "HOT";
      else
        tempValue += "COLD"; 

      pirValue = "Motion: ";
      if(motionSensor->getValue() == 0)
        pirValue += "FALSE";
      else
        pirValue += "TRUE"; 


      ldrValue = "Lights: ";
      if(lightSensor->getValue() >= DARKNESS_THRESHOLD)
        ldrValue += "ON";
      else
        ldrValue += "OFF"; 

        
      waterValue = "Water pump: ";
      if(waterLevelSensor->getLevel() >= WATER_THRESHOLD)
        waterValue += "OFF";
      else
        waterValue += "ON";

      
      dustValue = "Dust level: ";
      if(dustSensor->getDustDensity() >= DUST_THRESHOLD)
        dustValue += "HIGH";
      else
        dustValue += "LOW";


      doorValue = "Door value: ";
      doorValue += doorServo->getAngle();

      foodValue = "Food value: ";
      foodValue += foodServo->getAngle();

      screen->drawTextGeneral(10, 110, tempValue, 2, c_black);
      screen->drawTextGeneral(10, 130, pirValue, 2, c_black);
      screen->drawTextGeneral(10, 150, ldrValue, 2, c_black);
      screen->drawTextGeneral(10, 170, waterValue, 2, c_black);
      screen->drawTextGeneral(10, 190, dustValue, 2, c_black);
      //screen->drawTextGeneral(10, 210, doorValue, 2, c_black);
      //screen->drawTextGeneral(10, 230, foodValue, 2, c_black);
    }
    counter++;
    if(counter == 5)
    {
      counter=0;
      screen->drawTextGeneral(10, 110, tempValue, 2, c_yellow);
      screen->drawTextGeneral(10, 130, pirValue, 2, c_yellow);
      screen->drawTextGeneral(10, 150, ldrValue, 2, c_yellow);
      screen->drawTextGeneral(10, 170, waterValue, 2, c_yellow);
      screen->drawTextGeneral(10, 190, dustValue, 2, c_yellow);
      //screen->drawTextGeneral(10, 210, doorValue, 2, c_yellow);
      //screen->drawTextGeneral(10, 230, foodValue, 2, c_yellow);
    }
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
