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

uint16_t posX = 400;
uint16_t posY = 400;

bool reset = 0;
bool pen = 0;

unsigned long lastTime = 0;

char buf[50];

void setup() { 

  Serial.begin(BAUDRATE);
  Serial.setTimeout(TIMEOUT);
  pinMode(SWA, INPUT_PULLUP);
  pinMode(SWB, INPUT_PULLUP);
}


void loop() {
  

  posX = constrain((knobX.read()/2)*10, 0, 790);
  posY = constrain((knobY.read()/2)*10, 0, 790);

  if (posX >= 790){
    posX = 790;
    knobX.write(159); 
  }
  if (posX <= 0){
    knobX.write(0);
    posX = 0;
  }
  if (posY >= 790){
    posY = 790;
    knobY.write(159);
  }
  if (posY <= 0){
    knobY.write(0);
    posY = 0;
  }
  

  if (!digitalRead(SWB)){
    delay(200);
    reset = 1;
    lastTime = millis();
    
  } else {
    if ((millis()-lastTime) > 1000) reset = 0;
  }
  
  // reset = 0;
  // else reset = 0;

  if (!digitalRead(SWA)) {
    delay(200);
    pen = !pen;
  }

  sprintf(buf, "{x = %u, y = %u, LBut = %u, RBut = %u}", posX, posY, pen, reset);
  Serial.println(buf);
}
