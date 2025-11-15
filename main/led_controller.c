#include "led_controller.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "esp_log.h"

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Déclarations C++ (mais utilisables en C)
extern void cpp_led_init(int count);
extern void cpp_led_setPixel(int index, uint8_t r, uint8_t g, uint8_t b);
extern void cpp_led_setAll(uint8_t r, uint8_t g, uint8_t b);
extern void cpp_led_clearAll();
extern void cpp_led_show();

void led_manager_init(int count) { cpp_led_init(count); }
void setPixel(int index, uint8_t r, uint8_t g, uint8_t b) { cpp_led_setPixel(index,r,g,b); }
void clearAll() { cpp_led_clearAll(); }
void setAll(uint8_t r,uint8_t g,uint8_t b) { cpp_led_setAll(r,g,b); }
void show() { cpp_led_show(); }

#ifdef __cplusplus
}
#endif