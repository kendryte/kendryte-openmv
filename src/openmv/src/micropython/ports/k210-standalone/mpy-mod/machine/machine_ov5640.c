/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * Development of the code in this file was sponsored by Microbric Pty Ltd
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013-2015 Damien P. George
 * Copyright (c) 2016 Paul Sokolovsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/objtype.h"
#include "mphalport.h"
#include "plic.h"
#include "sysctl.h"
#include "ov5640.h"
#include "dvp.h"
#include "fpioa.h"
#include "lcd.h"
#include "sysctl.h"
#include "nt35310.h"
#include "systick.h"

#define TIMER_INTR_SEL TIMER_INTR_LEVEL
#define TIMER_DIVIDER  8

// TIMER_BASE_CLK is normally 80MHz. TIMER_DIVIDER ought to divide this exactly
#define TIMER_SCALE    (TIMER_BASE_CLK / TIMER_DIVIDER)

#define TIMER_FLAGS    0
struct dvp_buf
{
	uint32_t* addr[2];
	uint8_t buf_used[2];
	uint8_t buf_sel;
};
typedef struct _machine_ov5640_obj_t {
	mp_obj_base_t base;
	uint32_t active;
	uint16_t device_id;
	uint16_t manuf_id;
	struct dvp_buf buf;
	void* ptr;
    //mp_uint_t repeat;//timer mode
} machine_ov5640_obj_t;

const mp_obj_type_t machine_ov5640_type;

#define K210_DEBUG 0
#if K210_DEBUG==1
#define debug_print(x,arg...) printf("[lichee_debug]"x,##arg)
#else 
#define debug_print(x,arg...) 
#endif
void _ndelay(uint32_t ms)
{
    uint32_t i;

    while (ms && ms--)
    {
        for (i = 0; i < 25; i++)
            __asm__ __volatile__("nop");
    }
}
#if 0
static int dvp_irq(void *ctx)
{

	machine_ov5640_obj_t* self = ctx;
	
	if (dvp_get_interrupt(DVP_STS_FRAME_FINISH)) {
		debug_print("Enter finish dvp_irq\n");
		dvp_clear_interrupt(DVP_STS_FRAME_START | DVP_STS_FRAME_FINISH);
		self->buf.buf_used[self->buf.buf_sel] = 1;
		self->buf.buf_sel ^= 0x01;
		dvp_set_display_addr((uint32_t)self->buf.addr[self->buf.buf_sel]);	
	} else {
		debug_print("Enter start dvp_irq\n");
		dvp_clear_interrupt(DVP_STS_FRAME_START);
			if (self->buf.buf_used[self->buf.buf_sel] == 0)
			{
				dvp_start_convert();
			}
	}
	debug_print("self->buf.buf_used[0] = %d\n",self->buf.buf_used[0]);
	debug_print("self->buf.buf_used[1] = %d\n",self->buf.buf_used[1]);
	debug_print("self->buf.buf_sel = %d\n",self->buf.buf_sel);
	return 0;
}
#endif
STATIC void machine_ov5640_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    machine_ov5640_obj_t *self = self_in;

    mp_printf(print, "ov5640(%p) ", self);
    mp_printf(print, "ov5640 active = %d, ", self->active);
    mp_printf(print, "manuf_id=%d, ",self->manuf_id);
    mp_printf(print, "device_id=%d", self->device_id);
}

STATIC mp_obj_t machine_ov5640_make_new() {
    
    machine_ov5640_obj_t *self = m_new_obj(machine_ov5640_obj_t);
    self->base.type = &machine_ov5640_type;

    return self;
}

STATIC void machine_ov5640_disable(machine_ov5640_obj_t *self) {
	sysctl_clock_disable(SYSCTL_CLOCK_DVP);
	plic_irq_disable(IRQN_DVP_INTERRUPT);
	plic_irq_deregister(IRQN_DVP_INTERRUPT);
	dvp_config_interrupt(DVP_CFG_START_INT_ENABLE | DVP_CFG_FINISH_INT_ENABLE, 0);
	free(self->ptr);
	self->active = 0;
}

#if 0
static void dvp_io_init(void)
{
	/* Init DVP IO map and function settings */
	fpioa_set_function(11, FUNC_CMOS_RST);//15 RESET#
	fpioa_set_function(13, FUNC_CMOS_PWDN);//17 PWDN
	fpioa_set_function(14, FUNC_CMOS_XCLK);//20 XCLK
	fpioa_set_function(12, FUNC_CMOS_VSYNC);//18 VSYNC
	fpioa_set_function(17, FUNC_CMOS_HREF);//19 HREF
	fpioa_set_function(15, FUNC_CMOS_PCLK);//21 PCLK
//	fpioa_set_function(24, FUNC_CMOS_D0);
//	fpioa_set_function(25, FUNC_CMOS_D1);
//	fpioa_set_function(26, FUNC_CMOS_D2);
//	fpioa_set_function(27, FUNC_CMOS_D3);
//	fpioa_set_function(28, FUNC_CMOS_D4);
//	fpioa_set_function(29, FUNC_CMOS_D5);
//	fpioa_set_function(33, FUNC_CMOS_D6);
//	fpioa_set_function(35, FUNC_CMOS_D7);
	fpioa_set_function(10, FUNC_SCCB_SCLK);//22 SIO_C
	fpioa_set_function(9, FUNC_SCCB_SDA);//23 SIO_D

}
#endif

uint32_t g_lcd_gram0[38400] __attribute__((aligned(64)));
uint32_t g_lcd_gram1[38400] __attribute__((aligned(64)));

volatile uint8_t g_dvp_finish_flag;
volatile uint8_t g_ram_mux;

extern int lcd_init_flag;
typedef void (*on_irq_dvp_callbck)();

static on_irq_dvp_callbck on_irq_dvp_callbck_func = NULL;

static void dvp_lcd_show()
{
        timer_set_enable(TIMER_DEVICE_0, TIMER_CHANNEL_0, 0);
        while (g_dvp_finish_flag == 0)
            ;
        g_dvp_finish_flag = 0;
        /* display pic*/
        g_ram_mux ^= 0x01;
        lcd_draw_picture(0, 0, 320, 240, g_ram_mux ? g_lcd_gram0 : g_lcd_gram1);
}

static void dvp_show_start()
{
    timer_init(TIMER_DEVICE_0);
    timer_set_interval(TIMER_DEVICE_0, TIMER_CHANNEL_0, 100);
    timer_set_irq(TIMER_DEVICE_0, TIMER_CHANNEL_0, dvp_lcd_show, 1);//1 is lowest priority
    timer_set_enable(TIMER_DEVICE_0, TIMER_CHANNEL_0, 1);
}

static int on_irq_dvp(void* ctx)
{
    if (dvp_get_interrupt(DVP_STS_FRAME_FINISH))
    {
        /* switch gram */
        
        dvp_set_display_addr(g_ram_mux ? (uint32_t)g_lcd_gram0 : (uint32_t)g_lcd_gram1);

        dvp_clear_interrupt(DVP_STS_FRAME_FINISH);
        g_dvp_finish_flag = 1;
        
        if(on_irq_dvp_callbck_func && lcd_init_flag)
            on_irq_dvp_callbck_func();
    }
    else
    {
        dvp_start_convert();
        dvp_clear_interrupt(DVP_STS_FRAME_START);
    }

    return 0;
}

static void dvp_lcd_show_register()
{
    on_irq_dvp_callbck_func = dvp_show_start;
}

STATIC mp_obj_t machine_ov5640_init_helper(machine_ov5640_obj_t *self) {
    /* DVP init */
    printf("DVP init\n");
    
    dvp_init(16);
    dvp_enable_burst();
    dvp_set_output_enable(0, 1);
    dvp_set_output_enable(1, 1);
    dvp_set_image_format(DVP_CFG_RGB_FORMAT);
    dvp_set_image_size(320, 240);
    ov5640_init();

    //dvp_set_ai_addr((uint32_t)0x40600000, (uint32_t)0x40612C00, (uint32_t)0x40625800);
    dvp_set_display_addr((uint32_t)g_lcd_gram0);
    dvp_config_interrupt(DVP_CFG_START_INT_ENABLE | DVP_CFG_FINISH_INT_ENABLE, 0);
    dvp_disable_auto();

    /* DVP interrupt config */
    printf("DVP interrupt config\n");
    plic_set_priority(IRQN_DVP_INTERRUPT, 2);
    plic_irq_register(IRQN_DVP_INTERRUPT, on_irq_dvp, NULL);
    plic_irq_enable(IRQN_DVP_INTERRUPT);

    /* enable global interrupt */
    sysctl_enable_irq();

    /* system start */
    printf("system start\n");
    g_ram_mux = 0;
    g_dvp_finish_flag = 0;
    dvp_clear_interrupt(DVP_STS_FRAME_START | DVP_STS_FRAME_FINISH);
    dvp_config_interrupt(DVP_CFG_START_INT_ENABLE | DVP_CFG_FINISH_INT_ENABLE, 1);

    self->active = 1;
    return mp_const_none;
}


STATIC mp_obj_t machine_ov5640_deinit(mp_obj_t self_in) {
    machine_ov5640_disable(self_in);

    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_ov5640_deinit_obj, machine_ov5640_deinit);

STATIC mp_obj_t machine_ov5640_get_image(mp_obj_t self_in,mp_obj_t buf) {
    machine_ov5640_obj_t* self = self_in;
//	uint32_t length = 0;
//	uint8_t* buf_image = 0;
	//mp_obj_list_get(buf,&length,&item);
	mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_WRITE);
	while(self->buf.buf_used[self->buf.buf_sel] == 0 )_ndelay(50);
	memcpy(bufinfo.buf, self->buf.addr[self->buf.buf_sel], bufinfo.len);
	self->buf.buf_used[self->buf.buf_sel] = 0;
	//printf("[lichee]:get image!\n");
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_2(machine_ov5640_get_images_obj, machine_ov5640_get_image);


STATIC mp_obj_t machine_ov5640_init(mp_obj_t self_in) {
	machine_ov5640_obj_t* self = self_in;
    return machine_ov5640_init_helper(self);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_ov5640_init_obj, machine_ov5640_init);

STATIC mp_obj_t machine_ov5640_lcdshow(mp_obj_t self_in) {
    dvp_lcd_show_register();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_ov5640_lcdshow_obj, machine_ov5640_lcdshow);


STATIC const mp_rom_map_elem_t machine_ov5640_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&machine_ov5640_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&machine_ov5640_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&machine_ov5640_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcdshow), MP_ROM_PTR(&machine_ov5640_lcdshow_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_image), MP_ROM_PTR(&machine_ov5640_get_images_obj) },
};

STATIC MP_DEFINE_CONST_DICT(machine_ov5640_locals_dict, machine_ov5640_locals_dict_table);

const mp_obj_type_t machine_ov5640_type = {
    { &mp_type_type },
    .name = MP_QSTR_ov5640,
    .print = machine_ov5640_print,
    .make_new = machine_ov5640_make_new,
    .locals_dict = (mp_obj_t)&machine_ov5640_locals_dict,
};
