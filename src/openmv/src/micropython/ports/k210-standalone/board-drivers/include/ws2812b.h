#if 0
#ifndef _WS2812B_H
#define _WS2812B_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * LED control:
 * WS2812B_SetLedRGB(); // First LED (Connected to MCU)
 * WS2812B_SetLedRGB(); // Second LED
 * WS2812B_SetLedRGB(); // Third LED (If exist)
 * WS2812B_TxRes()      // Send reset signal
 *
 * WS2812B's protocol:
 * The data is sent in a sequence containing 24 of those bits – 8 bits for each color
 * Highest bit first, followed by a low “reset” pulse of at least 50µs.
 * 0 is encoded as: T0H = 0.4uS ToL = 0.85uS (Tolerance 0.15uS)
 * 1 is encoded as: T1H = 0.85uS T1L = 0.4uS (Tolerance 0.15uS)
 * A valid reset: Hold data line low for at least 50µs.
 *
 *                                                           |-->Update color
 *                                _________________ | >50uS  |
 * START_______IDLE_______________|1st LED|2nd LED|___Reset_______IDLE_____________
 **/

void WS2812B_SetLedRGB(uint8_t r, uint8_t g, uint8_t b,int gpio_num);
void WS2812B_TxRes(int gpio_num);
void  WS2812B_SetLednRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t num,int gpio_num);
void init_nop_cnt(void);

#ifdef __cplusplus
}
#endif

#endif /* _WS2812B_H */
#endif

#ifndef _WS2812_H
#define _WS2812_H
#include "stddef.h"
#include "dmac.h"
#include "stdbool.h"

typedef struct _WS2812_DATA{
	uint32_t blue : 8;
	uint32_t red : 8;
	uint32_t green : 8;
	uint32_t reserved : 8;
} ws2812_data __attribute__((packed, aligned(4)));

typedef struct _WS2812_INFO{
	size_t ws_num;
	ws2812_data *ws_buf;
} ws2812_info;

//void ws2812_send_data(uint32_t SPINUM, enum dmac_channel_number DMAC_NUM,uint32_t ws_cnt, uint32_t *ws_data);
bool ws2812_send_data(uint32_t SPINUM, dmac_channel_number_t DMAC_NUM, ws2812_info *ws);
ws2812_info *ws2812_get_buf(uint32_t num);
bool ws2812_release_buf(ws2812_info *ws);
bool ws2812_clear(ws2812_info *ws);
bool ws2812_set_data(ws2812_info *ws, uint32_t num, uint8_t r, uint8_t g, uint8_t b);

void ws2812_run(int mode);
void ws2812_init();

#endif

