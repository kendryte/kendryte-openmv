/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Image Python module.
 *
 */
#ifndef __PY_IMAGE_H__
#define __PY_IMAGE_H__
// DISABLED #include <mp.h>
#include "imlib.h"

/*deepvision*/
#define FRAMEBUFFER_SIZE	((2*1024*1024)/4) //1MB

extern uint32_t g_lcd_gram0[38400] __attribute__((aligned(8)));
extern uint32_t g_lcd_gram1[38400] __attribute__((aligned(8)));
extern uint32_t g_framebuffer[FRAMEBUFFER_SIZE] __attribute__((aligned(8)));
extern uint32_t g_display_buff[38400] __attribute__((aligned(8)));
extern volatile uint8_t g_dvp_finish_flag;
extern volatile uint8_t g_ram_mux;

mp_obj_t py_image(int width, int height, int bpp, void *pixels);
mp_obj_t py_image_from_struct(image_t *img);
void *py_image_cobj(mp_obj_t img_obj);
int py_image_descriptor_from_roi(image_t *img, const char *path, rectangle_t *roi);
#endif // __PY_IMAGE_H__
