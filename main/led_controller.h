#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#ifdef __cplusplus
extern "C" {
#endif

void led_manager_init(int led_count);
void setPixel(int index, uint8_t r, uint8_t g, uint8_t b);
void setAll(uint8_t r, uint8_t g, uint8_t b);
void clearAll();
void show();

#ifdef __cplusplus
}
#endif
