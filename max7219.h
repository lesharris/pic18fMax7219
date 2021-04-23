#ifndef MAX7219_H
#define MAX7219_H

#include "mcc_generated_files/spi1.h"

void MAX7219_Init();
void MAX7219_SetEnabled(uint8_t enabled);
void MAX7219_SetIntensity(uint8_t level);
void MAX7219_SetPixel(int16_t x, int16_t y, uint8_t enabled);
uint8_t MAX7219_GetPixel(int16_t x, int16_t y);
void MAX7219_SetColumn(int16_t x, uint8_t value);
void MAX7219_SetDecode(uint8_t mask);
void MAX7219_SetScanLimit(uint8_t level);

void MAX7219_Display();
void MAX7219_DisplayRow(uint8_t row);
void MAX7219_Clear();

#endif // MAX7219_H
