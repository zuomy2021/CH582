#pragma once

#include "CH58x_common.h"
#include "eeprom_partition_table.h"
#include "partition_table.h"

#ifdef BLE_ENABLE
#include "ble_config.h"
#endif

#ifdef ESB_ENABLE
#include "esb_config.h"
#endif

#if !defined(UINT8_MAX)
#define UINT8_MAX ((uint8_t)-1)
#endif

#if !defined(UINT32_MAX)
#define UINT32_MAX ((uint32_t)-1)
#endif

#ifdef DEBUG
#include "printf.h"
#define PLF_DEBUG DEBUG
#else
#define NO_PRINT
#endif

#ifdef BLE_ENABLE
#ifndef DCDC_ENABLE
#define DCDC_ENABLE 1
#endif
#ifndef HAL_SLEEP
#define HAL_SLEEP 1
#endif
#ifndef LSE_ENABLE
#define LSE_ENABLE 1
#endif
#ifndef QMK_TASK_INTERVAL_MAX
#define QMK_TASK_INTERVAL_MAX SYS_TICK_MS(15)
#endif
#ifndef QMK_TASK_INTERVAL_LED
#define QMK_TASK_INTERVAL_LED SYS_TICK_MS(3)
#endif
#ifndef BLE_SLOT_NUM
#define BLE_SLOT_NUM 4
#endif
#endif

#ifdef ESB_ENABLE
#if ESB_ENABLE == 1
#ifndef DCDC_ENABLE
#define DCDC_ENABLE 1
#endif
#ifndef HAL_SLEEP
#define HAL_SLEEP 1
#endif
#ifndef LSE_ENABLE
#define LSE_ENABLE 1
#endif
#ifndef QMK_TASK_INTERVAL_MAX
#define QMK_TASK_INTERVAL_MAX SYS_TICK_MS(20) // 1.25ms
#endif
#ifndef QMK_TASK_INTERVAL_LED
#define QMK_TASK_INTERVAL_LED QMK_TASK_INTERVAL_MAX
#endif
#elif ESB_ENABLE == 2
#ifndef DCDC_ENABLE
#define DCDC_ENABLE 0
#endif
#ifndef HAL_SLEEP
#define HAL_SLEEP 0
#endif
#if LSE_ENABLE
#undef LSE_ENABLE
#define LSE_ENABLE 0
#endif
#endif
#endif

#if FREQ_SYS == 80000000
#define Fsys CLK_SOURCE_PLL_80MHz
#elif FREQ_SYS == 60000000
#define Fsys CLK_SOURCE_PLL_60MHz
#elif FREQ_SYS == 48000000
#define Fsys CLK_SOURCE_PLL_48MHz
#elif FREQ_SYS == 40000000
#define Fsys CLK_SOURCE_PLL_40MHz
#elif FREQ_SYS == 32000000
#define Fsys CLK_SOURCE_PLL_32MHz
#elif FREQ_SYS == 24000000
#define Fsys CLK_SOURCE_PLL_24MHz
#elif FREQ_SYS == 16000000
#define Fsys CLK_SOURCE_HSE_16MHz
#elif FREQ_SYS == 8000000
#define Fsys CLK_SOURCE_HSE_8MHz
#elif FREQ_SYS == 4000000
#define Fsys CLK_SOURCE_HSE_4MHz
#elif FREQ_SYS == 2000000
#define Fsys CLK_SOURCE_HSE_2MHz
#elif FREQ_SYS == 1000000
#define Fsys CLK_SOURCE_HSE_1MHz
#else
#error "Illegal CPU clock!"
#endif

#if (defined RGBLIGHT_ENABLE || defined RGB_MATRIX_ENABLE) && FREQ_SYS <= 32000000
#error "CPU clock speed too low!"
#endif

#ifndef EXTRAKEY_ENABLE
#define EXTRAKEY_ENABLE
#endif

#ifndef SEND_STRING_ENABLE
#define SEND_STRING_ENABLE
#endif

#ifdef NKRO_ENABLE
// a special trick
#define PROTOCOL_LUFA
#define ENDPOINT_TOTAL_ENDPOINTS 8
#else
#ifdef FORCE_NKRO
#undef FORCE_NKRO
#endif
#endif

#if defined LSE_ENABLE && LSE_ENABLE
#define FREQ_RTC   32768
#define CLK_OSC32K 0
#ifdef WS2812_DRIVER_PWM
#warning "WS2812 PWM driver can not be used with LSE!"
#undef WS2812_DRIVER_PWM
#endif
#ifdef WS2812_PWM_DRIVER
#warning "WS2812 PWM driver can not be used with LSE!"
#undef WS2812_PWM_DRIVER
#endif
#else
#ifdef LSE_LOAD_CAPACITANCE
#undef LSE_LOAD_CAPACITANCE
#endif
#define FREQ_RTC 32000
#if ESB_ENABLE == 2
#define CLK_OSC32K 0x81
#else
#define CLK_OSC32K 1
#endif
#endif

#ifndef WS2812
#ifdef WS2812_EN_PIN
#undef WS2812_EN_PIN
#endif
#ifdef WS2812_EN_LEVEL
#undef WS2812_EN_LEVEL
#endif
#endif

#ifdef WS2812_DRIVER_SPI
#define RGB_DI_PIN A14
#elif defined WS2812_DRIVER_PWM
#if WS2812_PWM_DRIVER == 1
#define RGB_DI_PIN A10
#elif WS2812_PWM_DRIVER == 2
#define RGB_DI_PIN A11
#endif
#endif

#define MACRO2STR_(_macro) #_macro
#define MACRO2STR(_macro)  MACRO2STR_(_macro)

// ms to 1 tick (625us)
#define SYS_TICK_MS(x) MS1_TO_SYSTEM_TIME(x)
// ms to 1.25ms (connection interval)
#define MS_UNIT_1_25(x) ((int)(x / 1.25))

enum {
    PLATFORM_EVENT_MODE_SELECT = 1,
    PLATFORM_EVENT_INITIALIZE,
    PROTOCOL_EVENT_PRE_INIT,
    PROTOCOL_EVENT_RUN,
    USB_EVENT_START_INDEX = 10,
    BLE_EVENT_START_INDEX = 20,
    ESB_EVENT_START_INDEX = 30,
};

enum {
    kbd_protocol_start = 0,
    kbd_protocol_usb,
    kbd_protocol_ble,
    kbd_protocol_esb,
    kbd_protocol_max,
};

#if BLE_SLOT_NUM > 16
#error "Too many BLE slots! Cap: 16"
#endif

#if !defined USB_ENABLE && !defined BLE_ENABLE && !defined ESB_ENABLE
#error "No interface enabled!"
#endif
