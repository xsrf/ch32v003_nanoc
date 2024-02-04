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

    // 200ms Blinking for regular LED and SK6812 RGB LED
    while(1)
    {
        h++;
        // Regular LED and RGB LED ON
        buffer[0] = h%3 == 0 ? 32 : 0;
        buffer[1] = h%3 == 1 ? 32 : 0;
        buffer[2] = h%3 == 2 ? 32 : 0;
        WS2812BSimpleSend( PORT, PIN, buffer, sizeof(buffer) ); // Will end with PIN LOW
        Delay_Ms(1);
        PORT->BSHR = 1<<(PIN+0); // Set PIN HIGH
        Delay_Ms(199);

        // Regular LED and RGB LED OFF
        buffer[0] = 0;
        buffer[1] = 0;
        buffer[2] = 0;
        PORT->BSHR = 1<<(PIN+16); // Set PIN LOW
        Delay_Ms(1); // Must start with PIN LOW!
        WS2812BSimpleSend( PORT, PIN, buffer, sizeof(buffer) ); // Will end with PIN LOW
        //PORT->BSHR = 1<<(PIN+16); // Set PIN LOW
        Delay_Ms(199);
    }
}