#include "auxiliary_rgb.h"

static uint8_t rgb_raw_hid_buffer[64];

void rgb_raw_hid_receive(uint8_t *data, uint8_t length)
{
    for (size_t i = 0; i < 32; i++) {
        rgb_raw_hid_buffer[i] = data[i];
        /* code */
    }
    rgb_raw_hid_send(rgb_raw_hid_buffer, 64);
}
