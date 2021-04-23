#include "mcc_generated_files/mcc.h"
#include "max7219.h"

#include <stdio.h>

void main(void) {
  SYSTEM_Initialize();
  MAX7219_Init();
  
  MAX7219_SetEnabled(true);
  MAX7219_SetIntensity(4);
  
  uint8_t x = 0, y = 0;
  uint8_t s = 1;
  
  printf("MAX7219 Driver Chip Demo\n");

  while (1) {
    MAX7219_SetPixel(x, y, s);
    
    if(++x > 63) {
      x = 0;
      
      if(++y > 7) {
        y = 0;
        s = !s;
      }
    }
    
    MAX7219_Display();
    
   // __delay_ms(10);
  }
}
