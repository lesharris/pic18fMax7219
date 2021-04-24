#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "max7219.h"
#include "mcc_generated_files/mcc.h"

const static uint16_t ENABLE = 0x0C00;
const static uint16_t TEST = 0x0F00;
const static uint16_t INTENSITY = 0x0A00;
const static uint16_t SCAN_LIMIT = 0x0B00;
const static uint16_t DECODE = 0x0900;

static uint8_t framebuffer[64];

uint8_t *getBufferPtr(int16_t x, int16_t y);
void sendCommand(uint16_t command);

void MAX7219_Init() {
  MAX7219_Clear();

  MAX7219_SetEnabled(false);
  MAX7219_SetIntensity(3);
  sendCommand(TEST);
  sendCommand(DECODE);
  sendCommand(SCAN_LIMIT | 7);
}

void MAX7219_SetEnabled(uint8_t enabled) {
  sendCommand(ENABLE | (enabled ? 1 : 0));
}

void MAX7219_SetIntensity(uint8_t level) {
  if (level > 0x0F)
    level = 0x0F;

  sendCommand(INTENSITY | level);
}

void MAX7219_SetPixel(int16_t x, int16_t y, uint8_t enabled) {
  uint8_t *p = getBufferPtr(x, y);

  if (!p)
    return;

  uint16_t bt = 7 - (x & 7);

  if (enabled) {
    *p |= (1 << bt);
  } else {
    *p &= ~(1 << bt);
  }
}

uint8_t MAX7219_GetPixel(int16_t x, int16_t y) {
  uint8_t *p = getBufferPtr(x, y);

  if (!p)
    return false;

  uint16_t bt = 7 - (x & 7);

  return *p & (1 << bt);
}

void MAX7219_SetColumn(int16_t x, uint8_t value) {
  for (uint8_t y = 0; y < 8; ++y) {
    MAX7219_SetPixel(x, y, value & 1);
    value >>= 1;
  }
}

void MAX7219_SetDecode(uint8_t mask) {
  sendCommand(DECODE | mask);
}

void MAX7219_SetScanLimit(uint8_t limit) {
  if (limit > 7)
    limit = 7;

  sendCommand(SCAN_LIMIT | limit);
}

void MAX7219_Display() {
  for (uint8_t y = 0; y < 8; y++) {
    MAX7219_DisplayRow(y);
  }
}

void MAX7219_DisplayRow(uint8_t row) {
  SPI1_Open(SPI1_DEFAULT);
  CS_SetLow();

  uint8_t data = framebuffer[row];
  uint8_t cmd[] = {row + 1, data};
  SPI1_WriteBlock((void *) cmd, 2);

  CS_SetHigh();
  SPI1_Close();
}

void MAX7219_Clear() {
  memset(framebuffer, 0, 64);
}

void MAX7219_DrawSprite(const uint8_t *sprite, int16_t x, int16_t y, uint8_t width, uint8_t height)
{
  uint8_t mask = 0b10000000;
  
  for( int iy = 0; iy < height; iy++ )
  {
    for( int ix = 0; ix < width; ix++ )
    {
      MAX7219_SetPixel(x + ix, y + iy, (bool)(sprite[iy] & mask ));
      mask = mask >> 1;
    }
    mask = 0b10000000;
  }
}

void MAX7219_DrawString(const char *text, uint16_t len, int16_t x, int16_t y) {
    for( int idx = 0; idx < len; idx ++ )
  {
    int c = text[idx] - 32;

    // stop if char is outside visible area
    if( x + idx * 8  > 63 )
      return;

    // only draw if char is visible
    if( 8 + x + idx * 8 > 0 )
      MAX7219_DrawSprite( font[c], x + idx * 8, y, 8, 8 );
  }
}

uint8_t *getBufferPtr(int16_t x, int16_t y) {
  if (y < 0 || y > 7)
    return NULL;

  if (x < 0 || x > 7)
    return NULL;

  uint16_t byte = (uint16_t)x >> 3;

  return framebuffer + y + byte;
}

void sendCommand(uint16_t command) {
  uint8_t cmd = (command & 0xFF00) >> 8;
  uint8_t data = command & 0x00FF;
  uint8_t block[] = {cmd, data};

  SPI1_Open(SPI1_DEFAULT);
  CS_SetLow();
  SPI1_WriteBlock((void *) block, 2);
  CS_SetHigh();
  SPI1_Close();
}
