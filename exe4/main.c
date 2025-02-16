#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int fall_red = 0;
volatile int rise_green = 0;
volatile int cnt_r = 0;
volatile int cnt_g = 0;


void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) {
    if (gpio == BTN_PIN_R) {
      fall_red = 1;
      cnt_r++;
    }
  } else if (events == 0x8) {
    if (gpio == BTN_PIN_G) {
      rise_green = 1;
      cnt_g++;
    }
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  // callback led r (first)
  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
    &btn_callback);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  // callback led g (nao usar _with_callback)
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true);

  gpio_init(LED_PIN_R);
  gpio_set_dir(LED_PIN_R, GPIO_OUT);

  gpio_init(LED_PIN_G);
  gpio_set_dir(LED_PIN_G, GPIO_OUT);

  while (true) {
    if (fall_red) {
      if (cnt_r % 2 == 0) {
        gpio_put(LED_PIN_R, 0);
      } else {
        gpio_put(LED_PIN_R, 1);
      }
      fall_red = 0;
    } else if (rise_green) {
      if (cnt_g % 2 == 0) {
        gpio_put(LED_PIN_G, 0);
      } else {
        gpio_put(LED_PIN_G, 1);
      }
      rise_green = 0;
    }
  }
}
