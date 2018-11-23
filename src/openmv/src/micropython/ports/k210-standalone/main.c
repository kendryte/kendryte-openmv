/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//#include <stdint.h>
#include <stdio.h>
//#include <string.h>

#include "sleep.h"
#include "encoding.h"

#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "lib/utils/pyexec.h"
#include "fpioa.h"
#include "gpio.h"
#include "lib/mp-readline/readline.h"
#include "timer.h"
#include "sysctl.h"
#include "w25qxx.h"
#include "plic.h"
#include "uarths.h"
#include "lcd.h"
#include "spiffs-port.h"
#include "framebuffer.h"
#include "py_image.h"
#include "fb_alloc.h"
#include <malloc.h>
#define UART_BUF_LENGTH_MAX 269
#define MPY_HEAP_SIZE 1 * 1024 * 1024
extern int mp_hal_stdin_rx_chr(void);


static char *stack_top;
#if MICROPY_ENABLE_GC
static char heap[MPY_HEAP_SIZE];
#endif

void do_str(const char *src, mp_parse_input_kind_t input_kind);

//CPU core 400M Hz (PLL0_OUTPUT_FREQ/2)
#define PLL0_OUTPUT_FREQ 800000000UL
#define PLL1_OUTPUT_FREQ 160000000UL
#define PLL2_OUTPUT_FREQ 45158400UL

static void pll_init()
{
    
    sysctl_clock_set_clock_select(SYSCTL_CLOCK_SELECT_ACLK, SYSCTL_SOURCE_IN0);

	sysctl_pll_enable(SYSCTL_PLL0);
    sysctl_pll_set_freq(SYSCTL_PLL0,  PLL0_OUTPUT_FREQ);
    //while (sysctl_pll_is_lock(SYSCTL_PLL0) == 0)
    //    sysctl_pll_clear_slip(SYSCTL_PLL0);
    //msdelay(100);
    sysctl_clock_enable(SYSCTL_CLOCK_PLL0);
    sysctl_clock_set_clock_select(SYSCTL_CLOCK_SELECT_ACLK, SYSCTL_SOURCE_PLL0);
    //printf("PLL0 output test:(real)%ld  (measure)%ld\n", PLL0_OUTPUT_FREQ, PLL0_OUTPUT_FREQ / 16);

    sysctl_pll_enable(SYSCTL_PLL1);
    sysctl_pll_set_freq(SYSCTL_PLL1,  PLL1_OUTPUT_FREQ);
    //while (sysctl_pll_is_lock(SYSCTL_PLL1) == 0)
	//	sysctl_pll_clear_slip(SYSCTL_PLL1);
    
    //msdelay(100);
    
    sysctl_clock_enable(SYSCTL_CLOCK_PLL1);
    //printf("PLL1 output test:(real)%ld  (measure)%ld\n", PLL1_OUTPUT_FREQ, PLL1_OUTPUT_FREQ / 16);

    sysctl_pll_enable(SYSCTL_PLL2);
    sysctl_pll_set_freq(SYSCTL_PLL2,  PLL2_OUTPUT_FREQ);
    //while (sysctl_pll_is_lock(SYSCTL_PLL2) == 0)
	//	sysctl_pll_clear_slip(SYSCTL_PLL2);
    
    //msdelay(100);
    sysctl_clock_enable(SYSCTL_CLOCK_PLL2);
    //printf("PLL2 output test:(real)%ld  (measure)%ld\n", PLL2_OUTPUT_FREQ, PLL2_OUTPUT_FREQ / 16);
}

#if 0
extern uint32_t g_lcd_gram0[38400];
extern uint32_t g_lcd_gram1[38400];

extern volatile uint8_t g_dvp_finish_flag;
extern volatile uint8_t g_ram_mux;

static void dvp_lcd_show()
{
    /* ai cal finish*/
    if (g_dvp_finish_flag != 0)
    {
        g_dvp_finish_flag = 0;
        /* display pic*/
        g_ram_mux ^= 0x01;
        lcd_draw_picture(0, 0, 320, 240, g_ram_mux ? g_lcd_gram0 : g_lcd_gram1);
    }
}
#endif
void mpy_main(void)
{
    uint64_t core_id = current_coreid();
    if (core_id == 0)
    {
        /*
        sysctl_pll_set_freq(SYSCTL_PLL0,320000000);
		sysctl_pll_enable(SYSCTL_PLL1);
		sysctl_pll_set_freq(SYSCTL_PLL1,160000000);
		printf("[lichee]:pll0 freq:%d\r\n",sysctl_clock_get_freq(SYSCTL_CLOCK_PLL0));
		printf("[lichee]:pll1 freq:%d\r\n",sysctl_clock_get_freq(SYSCTL_CLOCK_PLL1));
		*/
		pll_init();
		sysctl->power_sel.power_mode_sel6 = 1;
		sysctl->power_sel.power_mode_sel7 = 1;

		uarths_init();
        uint8_t manuf_id, device_id;
		while (1) {
			w25qxx_init(3);
			w25qxx_read_id(&manuf_id, &device_id);
			if (manuf_id != 0xFF && manuf_id != 0x00 && device_id != 0xFF && device_id != 0x00)
			    break;
		}
		w25qxx_enable_quad_mode();
        printf("flash init:manuf_id:0x%02x,device_id:0x%02x\n", manuf_id, device_id);
		my_spiffs_init();
	    int stack_dummy;
	    stack_top = (char*)&stack_dummy;
	    #if MICROPY_ENABLE_GC
	    gc_init(heap, heap + sizeof(heap));
	    #endif
	    mp_init();
	    readline_init0();
	    readline_process_char(27);
		fb_alloc_init0();
		printf("_fb_alloc_point_to:%x\n",_fballoc);
		memset(MAIN_FB(), 0, sizeof(*MAIN_FB()));
		MAIN_FB()->pixels = (uint8_t *)g_framebuffer;
		printf("_fb_base_point_to:%x\n",MAIN_FB()->pixels);
		
	    pyexec_frozen_module("boot.py");
        #if MICROPY_REPL_EVENT_DRIVEN
            pyexec_event_repl_init();
            char c = 0;
            for (;;) {
                //int cnt = read_ringbuff(&c,1);
                //if(cnt==0){continue;}
                c = (char)uarths_getc();
                if(c == 0xff)
                {
                    //dvp_lcd_show();
                    continue;
                }
                if(pyexec_event_repl_process_char(c)) {
                    break;
                }
                //dvp_lcd_show();
            }
	    #else
	        pyexec_friendly_repl();
	    #endif
	    mp_deinit();
	    msleep(1);
	    printf("prower off\n");
	    return;
    }
    while (1);
}

void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, MP_EMIT_OPT_NONE, true);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}

void nlr_jump_fail(void *val) {
    while (1);
}

void gc_collect(void) {
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    void *dummy;
    gc_collect_start();
    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info();
}

#if !MICROPY_DEBUG_PRINTERS
// With MICROPY_DEBUG_PRINTERS disabled DEBUG_printf is not defined but it
// is still needed by esp-open-lwip for debugging output, so define it here.
#include <stdarg.h>
int mp_vprintf(const mp_print_t *print, const char *fmt, va_list args);
int DEBUG_printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = mp_vprintf(MICROPY_DEBUG_PRINTER, fmt, ap);
    va_end(ap);
    return ret;
}
#endif


