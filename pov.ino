void ledsOut(int w) {
  PORTC = sBuffer[w]; // for faster pins 1-6
  digitalWrite(pin7, getBitValue(w, 7)); // because we don't have a PORT big enough
  digitalWrite(pin8, getBitValue(w, 8));
}

void ledsClear() {
  PORTC = 0x00;
  digitalWrite(pin7, 0);
  digitalWrite(pin8, 0);
}

int getBitValue(int8_t num, int b) {
  if (b > 0 && b <= 8)
    return ( (sBuffer[num] >> (b - 1)) & 1 );
  else
    return 0;
}

void setBit(int8_t x, int y, bool val){
  sBuffer[x] ^= (-val ^ sBuffer[x]) & (1 << y);
}

void drawBuffer() {
  const static int displaywidth = 127;
  //if ((micros() - stepTimer) >= stepLength) {
 //   stepTimer = micros();
    armLocation++;
    if (armLocation <  pSettings[1]) {                                        //      \       |  draws this section     /
      // ledsOut(map(w, 0, p0, ps, width-1));
      ledsOut( ( armLocation * (displaywidth + 1 - pSettings[0]) / pSettings[1] + pSettings[0]));
    }
    else if ((armLocation > pSettings[2]) && (armLocation < pSettings[3])) {  //      \  draws all in reverse from here->/
      //ledsOut((w-p1)*(0-width-1)/(p2-p1)+width-1);
      ledsOut(map(armLocation, pSettings[2], pSettings[3], displaywidth, 0));
    }
    else if ((armLocation > pSettings[4]) && (armLocation < pSettings[5])) {  //       \ draws here |                      /
      ledsOut((armLocation - pSettings[4]) * (pSettings[0]) / (pSettings[5] - pSettings[4]));
      //ledsOut(map(w,pSettings[4],pSettings[5], 0, pSettings[0]));
    }
    else  {
      ledsClear();
    }
//  }
  //if (armLocation >= pSettings[5]){
  //  Timer1.stop();
  //}
}

void trigger() {
  static uint16_t trigLowRange = 40000, trigHighRange = 50000;
  static int32_t lastTrig = 0, lastStartTrig = 0;
  int32_t k = micros() - lastTrig;
  if ((k > trigLowRange) && (k < trigHighRange)) {
    stepTimer = micros();
    freq = (freq + stepTimer - lastStartTrig) >> 1;
    stepLength = freq >> 10;
    lastStartTrig = stepTimer;
   // if (armLocation > 920){
    //  pSettings[5] -=  ((int16_t)pSettings[5]- (int16_t)armLocation)>>2;
   // }
    frame();
    armLocation = 0;
    Timer1.initialize(124);//stepLength+10);//124);//(uint32_t)stepLength);

  }
  lastTrig = micros();
}

void serialConfig() {
  static int curFig = 0;
  static bool textIn = false;
 
  static int cIndex = 0;
  if (Serial.available() > 0) {
    //debug = Serial.parseInt();
    char inb = Serial.read();
    if (textIn == true) {
      if (inb == '`') {
        clearBuffer();
        drawString(0, (uint8_t*)cText);
       // memset(cText, 0, sizeof(cText));
        cIndex = 0;
        textIn = false;
      }
      else {
        cText[cIndex] = inb;
        cIndex++;
      }
    }
    else {
      switch (inb) {
        case 'q': // Lower config setting index
          curFig--;
          curFig = constrain(curFig, 0, 6);
          printConfig(curFig);
          break;
        case 'w': // Raise config setting index
          curFig++;
          curFig = constrain(curFig, 0, 6);
          printConfig(curFig);
          break;
        case 'a': // Lower current config setting index value
          pSettings[curFig]--;
          printConfig(curFig);
          break;
        case 's': // Raise current config setting index value
          pSettings[curFig]++;
          printConfig(curFig);
          break;
        case '`': // Begin  text input, send another ` to end
          memset(cText,0x00,sizeof(cText));
          textIn = true;
          break;
        case 'r': // Change scrolling state
          scrollDir++;
          if (scrollDir >= 5) scrollDir = 0;
          break;
        case 'c': // useful for calibrating points
          testPattern();
          break;
        case 'g':
         //clearBuffer();
          //char cbuffer [8];
         // itoa(debug, cbuffer, 10);
          drawString(50, (uint8_t*)cText);
          gplaying ^= 1;
          break;
      }
    }
  }
}

void printConfig(int cf) {
  Serial.print(cf);
  Serial.print (" : ");
  Serial.println(pSettings[cf]);
}


