/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
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

#include <stdio.h>
#include <string.h>

#include "py/mphal.h"
#include "py/runtime.h"
#include "py/obj.h"
#include "py/objtype.h"
#include "py/objstr.h"
#include "py/objint.h"

#include <string.h>
#include "lcd.h"
#include "nt35310.h"
#include "sleep.h"
#include "gpiohs.h"
#include "fpioa.h"
#include "dmac.h"
#include "sysctl.h"

int lcd_init_flag = 0;

typedef struct _machine_nt35310_obj_t {
    mp_obj_base_t base;
	unsigned short color[14];

} machine_nt35310_obj_t;

const mp_obj_type_t machine_nt35310_type;

static void io_mux_init(void)
{
    /* Init DVP IO map and function settings */
    fpioa_set_function(11, FUNC_CMOS_RST);
    fpioa_set_function(13, FUNC_CMOS_PWDN);
    fpioa_set_function(14, FUNC_CMOS_XCLK);
    fpioa_set_function(12, FUNC_CMOS_VSYNC);
    fpioa_set_function(17, FUNC_CMOS_HREF);
    fpioa_set_function(15, FUNC_CMOS_PCLK);
    fpioa_set_function(10, FUNC_SCCB_SCLK);
    fpioa_set_function(9, FUNC_SCCB_SDA);

    /* Init SPI IO map and function settings */
    fpioa_set_function(8, FUNC_GPIOHS0 + DCX_GPIONUM);
    fpioa_set_function(6, FUNC_SPI0_SS3);
    fpioa_set_function(7, FUNC_SPI0_SCLK);

    sysctl_set_spi0_dvp_data(1);
}

static void io_set_power(void)
{
    /* Set dvp and spi pin to 1.8V */
    sysctl_set_power_mode(SYSCTL_POWER_BANK1, SYSCTL_POWER_V18);
    sysctl_set_power_mode(SYSCTL_POWER_BANK2, SYSCTL_POWER_V18);
}

STATIC mp_obj_t machine_init_helper(machine_nt35310_obj_t *self) {
    /* LCD init */
    io_mux_init();
    io_set_power();
    plic_init();
    
    printf("LCD init\n");
    lcd_init();
    lcd_set_direction(DIR_YX_LRUD);
    lcd_clear(BLACK);
    lcd_init_flag = 1;
    return mp_const_none;
}

STATIC mp_obj_t machine_draw_picture(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
		machine_nt35310_obj_t* self = pos_args[0];
		enum {
			ARG_x,
			ARG_y,
			ARG_width,
			ARG_height,
			ARG_buf
		};
		static const mp_arg_t allowed_args[] = {
			{ MP_QSTR_x, 		 MP_ARG_INT, {.u_int = 0} },
			{ MP_QSTR_y,		 MP_ARG_INT, {.u_int = 0} },
			{ MP_QSTR_width,	 MP_ARG_INT, {.u_int = 0} },
			{ MP_QSTR_height, 	 MP_ARG_INT, {.u_int = 0} },
			{ MP_QSTR_buf, 		 MP_ARG_OBJ, {.u_obj = mp_const_none} },
		};
	mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args-1, pos_args+1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
	uint16_t x = args[ARG_x].u_int;
	uint16_t y = args[ARG_y].u_int;
	uint16_t width = args[ARG_width].u_int;
	uint16_t height = args[ARG_height].u_int;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[ARG_buf].u_obj, &bufinfo, MP_BUFFER_READ);
	uint32_t *ptr = (uint32_t *)bufinfo.buf;
	lcd_draw_picture( x,  y,  width, height, ptr);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_draw_picture_obj, 1,machine_draw_picture);


STATIC mp_obj_t machine_draw_picture_default(machine_nt35310_obj_t *self_in,mp_obj_t buf) {
	machine_nt35310_obj_t* self = self_in;

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_READ);
	uint32_t *ptr = (uint32_t *)bufinfo.buf;
	lcd_draw_picture( 0,  0,  320, 240 , ptr);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(machine_draw_picture_default_obj,machine_draw_picture_default);



STATIC mp_obj_t machine_draw_string(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
	machine_nt35310_obj_t* self = pos_args[0];
	enum {
		ARG_x,
		ARG_y,
		ARG_str,
		ARG_color
	};
	static const mp_arg_t allowed_args[] = {
		{ MP_QSTR_x, 		 MP_ARG_INT, {.u_int = 0} },
		{ MP_QSTR_y,		 MP_ARG_INT, {.u_int = 0} },
		{ MP_QSTR_str, 		 MP_ARG_OBJ, {.u_obj = mp_const_none} },
		{ MP_QSTR_color,	 MP_ARG_INT, {.u_int = 0} },
	};
	mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args-1, pos_args+1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
	
	uint16_t x = args[ARG_x].u_int;
	uint16_t y = args[ARG_y].u_int;
	if(args[ARG_str].u_obj == mp_const_none)
	{
		printf("[lichee error]:please enter a string");
		return mp_const_none;
	}
    mp_buffer_info_t bufinfo;
    mp_obj_str_get_buffer(args[ARG_str].u_obj, &bufinfo, MP_BUFFER_READ);
	char *ptr =bufinfo.buf;
	uint16_t color = args[ARG_color].u_int;
	if(color > 13)
	{
		printf("[lichee error]:please enter a right color\n");
		return mp_const_none;
	}
	lcd_draw_string( x, y,ptr,self->color[color]);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_draw_string_obj, 1,machine_draw_string);

STATIC mp_obj_t machine_clear(mp_obj_t self_in,mp_obj_t color) {
	machine_nt35310_obj_t* self = self_in;
	int color_num = 0;
	if (MP_OBJ_IS_SMALL_INT(color)) 
        color_num = MP_OBJ_SMALL_INT_VALUE(color);
	else
	{
		printf("[lichee error]:type error\n");
		return mp_const_none;
	}
	if(color_num > 13)
	{
		printf("[lichee error]:please enter a right color\n");
		return mp_const_none;
	}
	lcd_clear(self->color[color_num]);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(machine_clear_obj, machine_clear);


STATIC mp_obj_t machine_init(mp_obj_t self_in) {
	machine_nt35310_obj_t* self = self_in;
    return machine_init_helper(self);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_init_obj, machine_init);

STATIC mp_obj_t machine_make_new() {
    
    machine_nt35310_obj_t *self = m_new_obj(machine_nt35310_obj_t);
    self->base.type = &machine_nt35310_type;

    return self;
}

STATIC const mp_rom_map_elem_t pyb_nt35310_locals_dict_table[] = {
    // instance methods
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&machine_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_picture), MP_ROM_PTR(&machine_draw_picture_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_picture_default), MP_ROM_PTR(&machine_draw_picture_default_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_string), MP_ROM_PTR(&machine_draw_string_obj) },
    { MP_ROM_QSTR(MP_QSTR_clear), MP_ROM_PTR(&machine_clear_obj) },
};

STATIC MP_DEFINE_CONST_DICT(pyb_nt35310_ocals_dict, pyb_nt35310_locals_dict_table);

const mp_obj_type_t machine_nt35310_type = {
    { &mp_type_type },
    .name = MP_QSTR_nt35310,
    .make_new = machine_make_new,
    .locals_dict = (mp_obj_dict_t*)&pyb_nt35310_ocals_dict,
};


