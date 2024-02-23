

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <HX711_ADC.h>

#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define THRESHOLD 2
#define THRESHOLD1 0.2
const int scale_DOUT_PIN = 3 ;
const int scale_SCK_PIN = 2;
long t;
float i;
bool Power = 0;
bool tared = false;
HX711_ADC scale(scale_DOUT_PIN, scale_SCK_PIN);

void setup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.display();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Welcome :)");
  display.print(" pls Wait.");
  display.display();
  Serial.begin(115200);
  scale.begin();
  long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  scale.start(stabilizingtime, _tare);
  scale.setCalFactor(22069.43);
}

void loop() {
  display.setCursor(0, 0);
  display.setTextSize(3);
  const int serialPrintInterval = 0;
  static boolean newDataReady = 0;
  if (scale.update()) newDataReady = true;
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      i = scale.getData();
      Serial.println(i, 4);
      newDataReady = 0;
      t = millis();
    }
  }
  if (i > THRESHOLD)Power = 1;
  if (i < THRESHOLD1) Power = 0;
  switch (Power) {
    case 1:
      display.clearDisplay();
      display.setTextSize(5);
      display.println(i, 1);
      display.display();
      display.clearDisplay();
      tared = false;
      break;
    case 0:
      //if (tared == false && i == 0.00f) {
        //delay(1000);
        //scale.tare();
        tared = true;
      //}
      display.clearDisplay();
      display.setTextSize(3);
      display.write("Step On");
      display.display();
      break;
  }
}
