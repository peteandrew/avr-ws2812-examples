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

  uint8_t colour[3] = {125,0,50};

  uint8_t curr_led = 0;
  uint8_t up = 1;

  DDRB = 1;   // bit 0 is our output

  while(1)
  {
    output_grb(buf, sizeof(buf));
   
    memset(buf, 0, sizeof(buf));

    set_color(
      buf,
      curr_led,
      pgm_read_byte(&colour[0]),
      pgm_read_byte(&colour[1]),
      pgm_read_byte(&colour[2])
    );

    if (up) {
      curr_led++;
      if (curr_led == NUM_WS2812 - 1) {
        up = 0;
      }
    } else {
      curr_led--;
      if (curr_led == 0) {
        up = 1;
      }
    }

    _delay_ms(100);
  }
}
