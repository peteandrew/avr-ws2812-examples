//
// WS2812 driving code from https://www.embeddedrelated.com/showarticle/528.php
// HSV to RGB conversion code from http://www.vagrearg.org/content/hsvrgb
// 8MHz internal osc
//

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "fast_hsv2rgb.h"

#define NUM_WS2812        1
#define NUM_LEDS          (NUM_WS2812*3)

typedef struct RgbColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RgbColor;

typedef struct HsvColor
{
    uint16_t h;
    uint8_t s;
    uint8_t v;
} HsvColor;

// declaration of our ASM function
extern void output_grb(uint8_t * ptr, uint16_t count);

void set_color(uint8_t * p_buf, uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
  uint16_t index = 3*led;
  p_buf[index++] = g;
  p_buf[index++] = r;
  p_buf[index] = b;  
}

int main(void)
{
  uint8_t buf[NUM_LEDS];

  HsvColor hsv;
  hsv.h = 30;
  hsv.s = HSV_SAT_MAX;
  hsv.v = 100;

  RgbColor rgb;

  uint8_t up = 1;

  DDRB = 1;   // bit 0 is our output

  while(1)
  {
    output_grb(buf, sizeof(buf));
   
    memset(buf, 0, sizeof(buf));

    fast_hsv2rgb_32bit(hsv.h, hsv.s, hsv.v, &rgb.r, &rgb.g, &rgb.b);

    set_color(
      buf,
      0,
      rgb.r,
      rgb.g,
      rgb.b
    );

    if (up) {
      hsv.v += 5;
      if (hsv.v == 255) up = 0;
    } else {
      hsv.v--;
      if (hsv.v == 0) up = 1;
    }

    _delay_ms(10);
  }
}
