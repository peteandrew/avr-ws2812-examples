//
// WS2812 driving code from https://www.embeddedrelated.com/showarticle/528.php
// HSV to RGB conversion code from http://www.vagrearg.org/content/hsvrgb
// 8MHz internal osc
//

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "fast_hsv2rgb.h"

#define NUM_WS2812        10
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
  uint8_t vals[NUM_WS2812] = {0,25,50,75,100,125,150,175,200,225};
  uint8_t ups[NUM_WS2812] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  HsvColor hsv;
  hsv.h = 30;
  hsv.s = HSV_SAT_MAX;

  RgbColor rgb;

  DDRB = 1;   // bit 0 is our output

  while(1)
  {
    output_grb(buf, sizeof(buf));
   
    memset(buf, 0, sizeof(buf));

    for (uint8_t i = 0; i < NUM_WS2812; i++) {
      fast_hsv2rgb_32bit(hsv.h, hsv.s, vals[i], &rgb.r, &rgb.g, &rgb.b);

      set_color(
        buf,
        i,
        rgb.r,
        rgb.g,
        rgb.b
      );

      if (ups[i]) {
        vals[i] += 5;
        if (vals[i] == 255) ups[i] = 0;
      } else {
        vals[i] -= 5;
        if (vals[i] == 0) {
          ups[i] = 1;
          if (i == 0) {
            hsv.h += 5;
            if (hsv.h > HSV_HUE_MAX)
              hsv.h -= HSV_HUE_MAX;
          }
        }
      }
    }

    _delay_ms(10);
  }
}
