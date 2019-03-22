#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define NUM_PIXEL 5
#define PIXEL_PIN 3
#define MAX_ADC 4096
#define HOLD_TIMER 30;

Adafruit_NeoPixel *strip;
int holdValue = 0;
int holdTimer = 0;

void mapColor(int input, u8 *red, u8 *green)
{
  if (input < NUM_PIXEL/2)
  {
    *green = 255;
    *red = (input * 255) / (NUM_PIXEL / 2);
  }
  else
  {
    *red = 255;
    input -= NUM_PIXEL / 2;
    *green = 255 - ((input * 255) / (NUM_PIXEL / 2));
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(D4, OUTPUT);

  strip = new Adafruit_NeoPixel(NUM_PIXEL, PIXEL_PIN, NEO_GRB | NEO_KHZ800);
  strip->begin();
  strip->show();
}

void loop() {
  // put your main code here, to run repeatedly:
  // digitalWrite(D4, 0);
  // delay(500);
  // digitalWrite(D4, 255);
  // delay(200);

  int value = analogRead(A0);
  value -= MAX_ADC / 2;
  if (value < 0)
    value *= -1;

  int valuePixel = (value * NUM_PIXEL) / (MAX_ADC / 2);
  u8 red = 0;
  u8 green = 0;

  for (int eachPixel = 0; eachPixel < NUM_PIXEL; ++eachPixel)
  {
    if (eachPixel < valuePixel)
      mapColor(eachPixel, &red, &green);
    else
    {
      red = 0;
      green = 0;
    }
      
    strip->setPixelColor(eachPixel, red, green, 0);
  }

  if (valuePixel > holdValue)
  {
    holdValue = valuePixel;
    holdTimer = HOLD_TIMER;
  }

  if (holdTimer > 0)
    holdTimer--;

  if (holdTimer == 0 && holdValue > 0)
  {
    holdValue--;
    holdTimer = HOLD_TIMER;
  }

  mapColor(holdValue, &red, &green);
  strip->setPixelColor(holdValue, red, green, 0);

  strip->show();

  delay(100);
}
