#include "ArduinoSTL.h"
#include "state.h"
#include "component.h"
#include "input.h"
#include "led.h"
#include "fan.h"
#include "water_pump.h"
#include "servo_motor.h"
#include "lcd_display.h"
#include "humiture_sensor.h"
#include "photo_resistor.h"
#include "water_level_sensor.h"

#define HUMIDITY_THRESHOLD 65.
#define TEMP_HOT 91     // F
#define DARKNESS_THRESHOLD 500

Keypad4x4 *input;
LcdDisplay *screen;
Led *darkLight1, *darkLight2, *heatLight;
Fan *coolerFan, *humidityFan;
HumitureSensor *dhtSensor;
PhotoResistor *lightSensor;
WaterLevelSensor *waterLevelSensor;
WaterPump *waterPump;

std::vector<Component*> components;
int FLAG;

void init_components() {
  input = new Keypad4x4(new byte[4]{46, 47, 48, 49}, new byte[4]{50, 51, 52, 53}, 10);
  screen = new LcdDisplay(A3, A2, A1, A0, A4);
  darkLight1 = new Led(22);
  darkLight2 = new Led(23);
  heatLight = new Led(24);
  humidityFan = new Fan(26);
  coolerFan = new Fan(27);
  dhtSensor = new HumitureSensor(30);
  lightSensor = new PhotoResistor(A8);
  waterLevelSensor = new WaterLevelSensor(A9);
  waterPump = new WaterPump(32);

  components = {
    input, screen, darkLight1, darkLight2, heatLight,
    humidityFan, coolerFan,
    dhtSensor, lightSensor,
    waterLevelSensor, waterPump,
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
  
  if (input->keyPressed()) {
    switch (input->getKey()) {
      case 'C':
        FLAG |= 1;
        input->bufferClear();
      break;
      case 'D':
        FLAG |= 1;
        char ch = input->bufferPop();
      break;
      default: break;
    }
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
