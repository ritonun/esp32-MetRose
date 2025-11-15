#include "esp32WS2811.h"
#include "Colour.h"
#include <memory>

static WS2811* strip = nullptr;

extern "C" {

void cpp_led_init(int count) {
    strip = new WS2811(5, count, RMT_CHANNEL_0);      
    strip->begin();
    strip->clearAll();
    strip->show();
}

void cpp_led_setPixel(int index, uint8_t r, uint8_t g, uint8_t b) {
    if (!strip) return;
    strip->setPixel(index, r, g, b);
}

void cpp_led_setAll(uint8_t r, uint8_t g, uint8_t b) {
    if (!strip) return;
    strip->setAll(r, g, b);
}

void cpp_led_clearAll() {
    if (!strip) return;
    strip->clearAll();
}

void cpp_led_show() {
    if (!strip) return;
    strip->show();
}

}
