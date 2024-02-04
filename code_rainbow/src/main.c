#include "ch32v003fun.h"
#include <stdio.h>
#include <string.h>

#define WS2812BSIMPLE_IMPLEMENTATION
#include <ws2812b_simple.h>

#define PORT GPIOA
#define PIN 2
#define DELAY 100

uint8_t buffer[3] = {255,0,255};
uint8_t h,r,g,b = 255;

void hue2rgb(uint8_t hue, uint8_t * r, uint8_t * g, uint8_t * b);

int main()
{
	SystemInit();
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO;

	// Disable USB by driving D- on C2 low - otherwise PC would report a broken device
    // Required if D- Pull-Up is soldered to 3V
    // Not required if D- Pull-Up is connected to C4
    GPIOC->CFGLR |= (GPIO_Speed_50MHz | GPIO_CNF_OUT_OD)<<(4*2); // C2 to open drain
    GPIOC->BSHR = 1<<(2+16); // Set C2 low

    // Rainbow
	while(1)
	{
		h++;
		hue2rgb(h,&r,&g,&b);
		buffer[0] = r;
		buffer[1] = g;
		buffer[2] = b;
		WS2812BSimpleSend( PORT, PIN, buffer, sizeof(buffer) );
		Delay_Ms(20);
	}
}





void hue2rgb(uint8_t hue, uint8_t * r, uint8_t * g, uint8_t * b) {
    // https://blog.adafruit.com/2012/03/14/constant-brightness-hsb-to-rgb-algorithm/
    const uint8_t step = 255/3;
    if(0 <= hue && hue < step) {
        *b = 0;
        *g = hue*3;
        *r = 255 - hue*3;
    } else 
    if(step <= hue && hue < step*2) {
        *r = 0;
        *b = (hue-step)*3;
        *g = 255 - (hue-step)*3;
    } else 
    if(step*2 <= hue && hue < step*3) {
        *g = 0;
        *r = (hue-step*2)*3;
        *b = 255 - (hue-step*2)*3;
    } else {
        *g = 0;
        *b = 0;
        *r = 255;
    }
}