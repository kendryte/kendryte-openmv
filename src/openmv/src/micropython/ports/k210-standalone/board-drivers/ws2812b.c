#if 0

#include <stdint.h>
#include "ws2812b.h"
#include "sleep.h"
#include "gpiohs.h"
#include "sysctl.h"


// Change this according to FPIOA, GPIO, and hardware configuration
#define WS2812B_PIN_CLR(gpio_num)                gpiohs_set_pin(gpio_num,GPIO_PV_LOW);                                   
#define WS2812B_PIN_SET(gpio_num)                gpiohs_set_pin(gpio_num,GPIO_PV_HIGH);  																


volatile static uint16_t ns_300_cnt = 0, ns_800_cnt = 0;

void init_nop_cnt(void)
{
	uint32_t cpu_freq = sysctl_clock_get_freq(SYSCTL_CLOCK_CPU);

	float per_nop_ns = 0.0;

	cpu_freq /= 1000000;
	per_nop_ns = 1000.0 / cpu_freq;

	printf("per_nop_ns:%f cpu_freq:%dMhz\r\n",per_nop_ns,cpu_freq);

	ns_300_cnt = 300.0 / per_nop_ns;
	ns_800_cnt = 800.0 / per_nop_ns;

	printf("ns_300_cnt:%d ns_800_cnt:%d\r\n",ns_300_cnt,ns_800_cnt);
}


// The duration / pulse width depends on the CPU clock frequency!
static inline void WS2812B_Tx0(int gpio_num)
{
	WS2812B_PIN_SET(gpio_num);
	//13--350ns
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");


	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

	WS2812B_PIN_CLR(gpio_num);
	//45--850ns
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

		asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
}

static inline void WS2812B_Tx1(int gpio_num)
{
	WS2812B_PIN_SET(gpio_num);
	//45--850ns
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");


	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	WS2812B_PIN_CLR(gpio_num);
	//13--450ns
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

		asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
}

void WS2812B_TxRes(int gpio_num)
{
	WS2812B_PIN_CLR(gpio_num);
	usleep(60);
}


void WS2812B_SetLedRGB(uint8_t r, uint8_t g, uint8_t b,int gpio_num)
{
	uint64_t frameBuf = (g << 16) | (r << 8) | b;

	for (uint64_t mask = 0x800000; mask > 0; mask >>= 1) {
		if (frameBuf & mask)
		{
			WS2812B_Tx1(gpio_num);
		}
		else
		{
			WS2812B_Tx0(gpio_num);
		}
	}
}

void  WS2812B_SetLednRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t num,int gpio_num)
{
	uint64_t frameBuf = (g << 16) | (r << 8) | b;
	uint8_t cnt;

	for(cnt=0;cnt<num;cnt++)
	{
		for (uint64_t mask = 0x800000; mask > 0; mask >>= 1)
		{
			if (frameBuf & mask)
				WS2812B_Tx1(gpio_num);
			else
				WS2812B_Tx0(gpio_num);
		}
	}
}
#endif

#include "spi.h"
#include "dmac.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "sysctl.h"
#include <string.h>
#include <stdbool.h>
#include "ws2812b.h"
#include "fpioa.h"

#define configASSERT( x ) if( ( x ) == 0 ) {                           \
    printf("(%s:%d) %s", __FILE__, __LINE__, #x);					   \
    for( ;; );                                                         \
}


#define		SPI_MAX_NUM		4

extern volatile spi_t *const spi[4];

int spi_clk_init_ws2812(uint8_t spi_bus){
	configASSERT(spi_bus < SPI_MAX_NUM && spi_bus != 2);
	sysctl_clock_enable(SYSCTL_CLOCK_SPI0 + spi_bus);
	sysctl_clock_set_threshold(SYSCTL_THRESHOLD_SPI0 + spi_bus, 0);
	return 0;
}

int spi_master_config_ws2812(uint8_t spi_bus){
	configASSERT(spi_bus < SPI_MAX_NUM && spi_bus != 2);

	uint8_t dfs_offset, frf_offset,tmod_offset;
	switch(spi_bus){
		case 0:
		case 1:
			dfs_offset = 16;
			frf_offset = 21;
			tmod_offset = 8;
			break;
		case 2:
			configASSERT(!"Spi Bus 2 Not Support!");
			break;
		case 3:
		default:
			dfs_offset = 0;
			frf_offset = 22;
			tmod_offset = 10;
			break;
	}

	volatile spi_t *spi_adapter = spi[spi_bus];

	uint32_t freq_spi_src = sysctl_clock_get_freq(SYSCTL_CLOCK_SPI0 + spi_bus);
	uint32_t div = freq_spi_src / 2500000;//8000000;		//set 8M clk
	spi_adapter->baudr = div;

//	spi_adapter->baudr = 0x0A;//0x02;//0x0A;
	spi_adapter->imr = 0x00;
	spi_adapter->dmacr = 0x00;
	spi_adapter->dmatdlr = 0x10;
	spi_adapter->dmardlr = 0x00;
	spi_adapter->ser = 0x00;
	spi_adapter->ssienr = 0x00;
	spi_adapter->ctrlr0 = (0 << 6) | (0 << frf_offset) | (31 << dfs_offset) | (0x01 << tmod_offset);
	spi_adapter->spi_ctrlr0 = 0;
	return 0;
}

int spi_init_ws2812(uint8_t spi_bus){
	configASSERT(spi_bus < SPI_MAX_NUM && spi_bus != 2);
	spi_clk_init_ws2812(spi_bus);
	spi_master_config_ws2812(spi_bus);
	return 0;
}

int spi_normal_send_dma_ws2812(dmac_channel_number_t channel_num, uint8_t spi_bus, uint32_t chip_sel,
						void *tx_buff, uint32_t tx_len)
{
	configASSERT(spi_bus < SPI_MAX_NUM && spi_bus != 2);
	volatile spi_t *spi_handle = spi[spi_bus];

	spi_handle->dmacr = 0x2;	//enable dma transmit
	spi_handle->ssienr = 0x01;

	dmac_enable();
	sysctl_dma_select(channel_num, SYSCTL_DMA_SELECT_SSI0_TX_REQ + spi_bus * 2);
//	dmac_set_single_mode(channel_num, buf, (void *)(&spi_handle->dr[0]), DMAC_ADDR_INCREMENT, DMAC_ADDR_NOCHANGE,
//								DMAC_MSIZE_4, DMAC_TRANS_WIDTH_32, tx_len);

	dmac->channel[channel_num].dar = (uint64_t)(&spi_handle->dr[0]);
	dmac->channel[channel_num].ctl = (((uint64_t)1 << 47) | ((uint64_t)15 << 48) | ((uint64_t)1 << 38) | ((uint64_t)15 << 39) | ((uint64_t)3 << 18) | ((uint64_t)3 << 14) | ((uint64_t)2 << 11) | ((uint64_t)2 << 8) | ((uint64_t)1 << 6));
	dmac->channel[channel_num].cfg = (((uint64_t)1 << 49) | ((uint64_t)1 << 32));
	dmac->channel[channel_num].sar = (uint64_t)tx_buff;
	dmac->channel[channel_num].block_ts = tx_len - 1;
	dmac->channel[channel_num].intstatus_en = 0xFFFFFFFF;
	dmac->channel[channel_num].intclear = 0xFFFFFFFF;
	dmac->chen = 0x0101 << channel_num;

	spi_handle->ser = chip_sel;
	while (!(readq(&dmac->channel[channel_num].intstatus) & 0x2))
			;


	while ((spi_handle->sr & 0x05) != 0x04)
		;
	spi_handle->ser = 0x00;
	spi_handle->ssienr = 0x00;
	return 0;
}

uint32_t spi_get_rate_ws2812(uint8_t spi_bus){
	volatile spi_t *spi_adapter = spi[spi_bus];
	uint32_t freq_spi_src = sysctl_clock_get_freq(SYSCTL_CLOCK_SPI0 + spi_bus);
	return freq_spi_src / spi_adapter->baudr;
}

ws2812_info *ws2812_get_buf(uint32_t num){
	ws2812_info *ws = malloc(sizeof(ws2812_info));
	if(ws == NULL){
		return NULL;
	}
	ws->ws_num = num;
	ws->ws_buf = malloc(num * sizeof(ws2812_data));
	if(ws->ws_buf == NULL){
		free(ws);
		return false;
	}
	memset(ws->ws_buf, 0, num * sizeof(ws2812_data));
	return ws;
}

bool ws2812_release_buf(ws2812_info *ws){
	if(ws == NULL){
		return false;
	}

	if(ws->ws_buf == NULL){
		return false;
	}

	free(ws->ws_buf);
	free(ws);
	return true;
}

bool ws2812_clear(ws2812_info *ws){
	if(ws == NULL){
		return false;
	}

	if(ws->ws_buf == NULL){
		return false;
	}

	memset(ws->ws_buf, 0, ws->ws_num * sizeof(ws2812_data));

	return true;
}

bool ws2812_set_data(ws2812_info *ws, uint32_t num, uint8_t r, uint8_t g, uint8_t b){
	if(ws == NULL){
		return false;
	}

	if(ws->ws_buf == NULL){
		return false;
	}

	if(num >= ws->ws_num){
		return false;
	}

	(ws->ws_buf + num)-> red = r;
	(ws->ws_buf + num)-> green = g;
	(ws->ws_buf + num)-> blue = b;

	return true;
}

bool ws2812_send_data(uint32_t SPINUM, dmac_channel_number_t DMAC_NUM, ws2812_info *ws){

	uint32_t longbit;
	uint32_t shortbit;
	uint32_t resbit;

	if(ws == NULL){
		return false;
	}

	if(ws->ws_buf == NULL){
		return false;
	}

	size_t ws_cnt = ws->ws_num;
	uint32_t *ws_data =(uint32_t *)ws->ws_buf;

	spi_init_ws2812(SPINUM);
//	spi_master_config(SPINUM, SPI_Mode_0, SPI_FF_Standard, 32);

	uint32_t freq_spi = spi_get_rate_ws2812(SPINUM);
	double clk_time = 1e9 / freq_spi;	// ns per clk

	uint32_t longtime = 850 / clk_time * clk_time;
	if(longtime < 700)
		longbit = 850 / clk_time + 1;
	else
		longbit = 850 / clk_time;

	uint32_t shortime = 400 / clk_time * clk_time;
	if(shortime < 250)
		shortbit = 400 / clk_time + 1;
	else
		shortbit = 400 / clk_time;

	resbit = (400000 / clk_time);	// > 300us

	uint32_t spi_send_cnt = (((ws_cnt * 24 * (longbit + shortbit) + resbit) / 8) + 7) / 4; //加最后的reset
	uint32_t reset_cnt = (resbit + 7) / 8 / 4;		//先reset
	uint32_t *tmp_spi_data = malloc((spi_send_cnt + reset_cnt) * 4);
	if(tmp_spi_data == NULL)
		return false;

	memset(tmp_spi_data, 0, (spi_send_cnt + reset_cnt) * 4);
	uint32_t *spi_data = tmp_spi_data;

	spi_data += reset_cnt;
	int pos = 31;
	uint32_t long_cnt = longbit;
	uint32_t short_cnt = shortbit;
	for(int i = 0; i < ws_cnt; i++){

		for(uint32_t mask = 0x800000; mask > 0; mask >>= 1){

			if(ws_data[i] & mask){

				while(long_cnt--){
					*(spi_data) |= (1 << (pos--));
					if(pos < 0){
						spi_data++;
						pos = 31;
					}
				}
				long_cnt = longbit;

				while(short_cnt--){
					*(spi_data) &= ~(1 << (pos--));
					if(pos < 0){
						spi_data ++;
						pos = 31;
					}
				}
				short_cnt = shortbit;

			}else{
				while(short_cnt--){
					*(spi_data) |= (1 << (pos--));
					if(pos < 0){
						spi_data ++;
						pos = 31;
					}
				}
				short_cnt = shortbit;

				while(long_cnt--){
					*(spi_data) &= ~(1 << (pos--));
					if(pos < 0){
						spi_data++;
						pos = 31;
					}
				}
				long_cnt = longbit;


			}
		}
	}
	spi_normal_send_dma_ws2812(DMAC_NUM, SPINUM, 1, tmp_spi_data, spi_send_cnt + reset_cnt);
	free(tmp_spi_data);
	return true;
}

void ws2812_init()
{
    fpioa_set_function(40, FUNC_SPI1_D0);
}

//0:off  1:red  2:green  3:blue
void ws2812_run(int mode)
{
    //sys_clock_init();
    //      fpioa_set_function(30, FUNC_SPI0_SCLK);
    //      fpioa_set_function(32, FUNC_SPI0_SS0);
    //fpioa_set_function(40, FUNC_SPI1_D0);

    ws2812_info *info = ws2812_get_buf(1);//only one led

    switch(mode)
    {
        case 0:
            ws2812_set_data(info, 0, 0, 0, 0);
            ws2812_send_data(1, 0, info);
            break;
        case 1:
            ws2812_set_data(info, 0, 150, 0, 0);
            ws2812_send_data(1, 0, info);
            break;
        case 2:
            ws2812_set_data(info, 0, 0, 150, 0);
            ws2812_send_data(1, 0, info);
            break;
        case 3:
            ws2812_set_data(info, 0, 0, 0, 150);
            ws2812_send_data(1, 0, info);
            break;
        default:
            break;
    }

    ws2812_release_buf(info);
}


