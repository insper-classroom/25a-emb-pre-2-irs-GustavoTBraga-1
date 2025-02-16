#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

volatile int btn_fall = 0;
volatile int cnt = 0;

const int BTN_PIN_R = 28;
const int LED_PIN = 4;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    btn_fall = 1;
    cnt++;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
    BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
  
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  while (true) {
    if (btn_fall) {
      if (cnt % 2 == 0) {
        gpio_put(LED_PIN, 0);
      } else {
        gpio_put(LED_PIN, 1);
      }
      btn_fall = 0;
    }
  }
  return 0;
}
