#include <TimerOne.h>


#define bufferWidth 256
#define pin7 10
#define pin8 11
#define triggerPin  2

volatile uint16_t armLocation = 0;
volatile uint32_t stepTimer = micros();
volatile int16_t stepLength = 1;
volatile int16_t  debug = 0;
 //  4 \ 3           5 | 0                       1 / 2  Location and order of pSettings points
static int16_t pSettings[6] = {50, 241, 321, 714, 792, 949};
static uint32_t freq = 110000; // starts close, but freq is automatically averaged to be correct
static uint8_t scrollDir = 0;

/* Serial Commands
      `change display text` with ` before and after
        r to scroll text
       c for textPattern useful for configuring pSettings
       q / w to lower/raise current pSettings index for editing
       a / s to lower/raise current pSettings index's value
       g display debug value
*/
bool gplaying = false;
uint16_t gtime = 0;
 static char cText[64] = "  Conway'sGameOfLife!";//"I love you Ashley :)";//"0123456789abcdefghij";//"hey /r/arduino";
void setup() {
//  cText[] = "hey /r/arduino";
  clearBuffer();
  drawString( 0, (uint8_t*)cText);
  //drawString( 25, (uint8_t*)"hey /r/arduino");
  //testPattern();
  pinMode(triggerPin, INPUT);
  attachInterrupt(0, trigger, RISING);
  Serial.setTimeout(50);
  Serial.begin(115200);
  Timer1.attachInterrupt(drawBuffer);
}

void frame() {
  gtime++;
 if (scrollDir == 1) scrollLeft();
 else if (scrollDir == 2) scrollRight();
 else if (scrollDir == 3) scrollUp();
 else if (scrollDir == 4) scrollDown();
}

void loop() {
  serialConfig();

  gol();
 
}
