#include "custom_char.h"

// Code from:
// http://ohm.bu.edu/~pbohn/Cell_Stretcher/Firmware/Programs_for_AVR-MT-C/test_programs/cellstretcher_firmware/lcd_lib.c
void LCDdefinechar(const uint8_t *pc,uint8_t char_code){
    uint8_t a, pcc;
    uint16_t i;
    a=(char_code<<3)|0x40;
    for (i=0; i<8; i++){
        pcc=pgm_read_byte(&pc[i]);
        LCD_WriteCommand(a++);
        LCD_WriteData(pcc);
    }
}
