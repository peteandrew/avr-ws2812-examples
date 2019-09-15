//
// Based on code from https://www.embeddedrelated.com/showarticle/528.php
// 8MHz internal osc
//

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include <avr/pgmspace.h>

#define NUM_WS2812        10
#define NUM_LEDS          (NUM_WS2812*3)
#define NUM_COLOURS       8

const uint8_t ledColours[NUM_WS2812][3] PROGMEM =
{
  {125,0,0},
  {0,125,0},
  {0,0,125},
  {125,125,125},
  {255,0,0},
  {0,255,0},
  {0,0,255},
  {255,255,255}
};


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

  uint8_t curr_colour_idx = 0;

  DDRB = 1;   // bit 0 is our output

  while(1)
  {
    output_grb(buf, sizeof(buf));
   
    memset(buf, 0, sizeof(buf));

    for (uint8_t curr_led_idx = 0; curr_led_idx < NUM_WS2812; curr_led_idx++) {
      set_color(
        buf,
        curr_led_idx,
        pgm_read_byte(&ledColours[curr_colour_idx][0]),
        pgm_read_byte(&ledColours[curr_colour_idx][1]),
        pgm_read_byte(&ledColours[curr_colour_idx][2])
      );
    }

    curr_colour_idx++;
    if (curr_colour_idx == NUM_COLOURS) {
      curr_colour_idx = 0;
    }

    _delay_ms(500);
  }
}
