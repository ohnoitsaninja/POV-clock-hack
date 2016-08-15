uint8_t sBuffer[bufferWidth];

void testPattern() {
  int divr = 24;
  clearBuffer();
  for (int i = 0; i < divr; i++) {
    sBuffer[(128 * i) / divr] = 0xFF;
  }
}

void scrollLeft() {
  sBuffer[bufferWidth - 2] = sBuffer[0];
  memcpy(sBuffer , sBuffer + 1, bufferWidth - 2);
}

void scrollLeftVis() {
  int ts = sBuffer[0];
  memcpy(sBuffer , sBuffer + 1, 128);
  sBuffer[127] = ts;
}

void scrollUp(){
  uint8_t *t = &sBuffer[bufferWidth - 1];
  while (t != sBuffer) {
    bool tb = (*t >> 7) & 1;
    *t = *t << 1;
    *t ^= (-tb ^ *t) & (1 << 0);
    t--;
  }
}

void scrollDown(){
  uint8_t *t = &sBuffer[bufferWidth - 1];
  while (t != sBuffer) {
    bool tb = (*t >> 0) & 1;
    *t = *t >> 1;
    *t ^= (-tb ^ *t) & (1 << 7);
    t--;
  }
}

void scrollRight() {
  sBuffer[0] = sBuffer[bufferWidth - 2];
  uint8_t *t = &sBuffer[bufferWidth - 2];
  while (t != sBuffer) {
    *t = *(t - 1);
    t--;
  }
}

void clearBuffer() {
  memset(sBuffer, 0x00, sizeof(sBuffer));
}
