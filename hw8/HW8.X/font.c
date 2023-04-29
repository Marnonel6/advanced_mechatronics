#include "font.h"
#include <string.h>

// Draw single char to OLED display
void drawChar(char letter, char x, char y){
    for (int j = 0; j < 5; j++){
        char col = ASCII[letter-0x20][j];
        for (int i = 0; i < 8; i++){
            ssd1306_drawPixel(x+j, y+i, (col>>i)&0b1); // At this position switch off/on
        }
    }
}

// Draw string to OLED display
void drawString(char *message, char x, char y){
    int k = 0;
    while(message[k]!=0){
        drawChar(message[k], x+5*k, y);
        k++;
    }
}