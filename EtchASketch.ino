#include <Arduino.h>
#include <Encoder.h>

#define BAUDRATE 9600
#define TIMEOUT 1

#define SWA 7
#define DTA 2
#define CLKA 6

#define SWB 5
#define DTB 3
#define CLKB 4

Encoder knobX(DTA, CLKA);
Encoder knobY(DTB, CLKB);

void setup() { 

	Serial.begin(BAUDRATE);
  Serial.setTimeout(TIMEOUT);
  pinMode(SWA, INPUT_PULLUP);
  pinMode(SWB, INPUT_PULLUP);
}

uint16_t posX = 400;
uint16_t posY = 400;

bool reset = 0;
bool pen = 0;

char buf[50];

void loop() {
  
  posX = constrain(knobX.read(), 0, 800);
  posY = constrain(knobY.read(), 0, 800);

  if (!digitalRead(SWB)) reset = 1;
  else reset = 0;

  if (!digitalRead(SWA)) {
    delay(100);
    pen = !pen;
  }

  sprintf(buf, "{x = %u, y = %u, LBut = %u, RBut = %u}", posX, posY, pen, reset);
  Serial.println(buf);
}
