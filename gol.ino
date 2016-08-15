#define gwidth 127
#define gheight 7

int16_t sumNeighbors(int16_t x, int8_t y) {
  int16_t total = 0, tx;
  int8_t ym1,  yp1; 
  uint8_t *tB;

  // Y axis wrapping logic
  if (y == 0)ym1 = gheight;
  else ym1 = y - 1;
  if ( y == gheight) yp1 = 0;
  else yp1 = y+1;

  // Add neighbors from column to the left
  if (x == 0) tx = gwidth;
  else tx = x-1;  
  *tB = sBuffer[tx];
  total += ((*tB>>ym1)&1) + ((*tB>>y)&1) + ((*tB>>yp1)&1);

  // Add neighbors from above and below in center column
  *tB = sBuffer[x];
  total += ((*tB>>ym1)&1) + ((*tB>>yp1)&1);

  // Add neighbors from column to the right
  if (x == gwidth) tx = 0;
  else (tx = x+1);
  *tB = sBuffer[tx];
  total +=((*tB >>ym1)&1) + ((*tB >>y)&1) + ((*tB >>yp1)&1);
  
  return total;
}

uint16_t gplay() {
  uint8_t tBuffer[bufferWidth] = {0x00};
  memcpy(tBuffer,sBuffer,bufferWidth);
  int16_t x = gwidth+1;
  while(x--){
    int16_t y = gheight+1;
    while(y--){
      uint16_t neighbors = sumNeighbors(x,y);
      if ((neighbors < 2) || ( neighbors >3)) tBuffer[x] &= ~(1 <<y);
      if ((neighbors == 3) ) tBuffer[x] |= 1 << y;//|| (neighbors == 6)
    }
  }
  memcpy(sBuffer,tBuffer,bufferWidth);
}

void drawMissile(int x){
  sBuffer[x] =   0b00011000;
  sBuffer[x+1] = 0b00110000;
  sBuffer[x+2] = 0b00001000;
}

 void gol(){
  static int redrawCounter = 0;
  const uint16_t redrawTime = 270;
  if ((gplaying)&&(gtime>0)){
    gplay();
    redrawCounter++;
    gtime = 0;
    if (redrawCounter % 3 == 0){scrollLeftVis();}
    if (redrawCounter % 4 == 0) {scrollUp();}
   // if (redrawCounter % 18 == 0) {drawMissile(random(1,300));}
    if (redrawCounter > redrawTime){
      redrawCounter = 0;
      drawString( 0, (uint8_t*)cText);
      delay(1600);
    }
  }
  }

uint8_t ballX=30, ballY=3;
uint8_t dx=4, dy=1;
void bplay() {
  //clearBuffer();
  sBuffer[ballX] &= ~(1 << ballY);
  ballX += dx;
  ballY += dy;
  if ((ballX<0) || (ballX > 127)){
    dx *= -1;
    ballX += dx;
  }
   if ((ballY<0) || (ballY > 7)){
    dy *= -1;
    ballY += dy;
   }
 // setBit(ballX/100, ballY/100,1);
  sBuffer[ballX] |= 1 << ballY;
 //  Serial.print(ballX);
 //  Serial.print(" ");
//  Serial.println(ballY);
}


