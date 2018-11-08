
#include <stdio.h>
#include <stdint.h>

#include "py/nlr.h"
#include "py/runtime.h"
#include "modmachine.h"
#include "mphalport.h"

#include "ws2812b.h"
#include "sleep.h"
#include "gpiohs.h"
#include "sysctl.h"

typedef struct _machine_ws2812_obj_t {
    mp_obj_base_t base;
} machine_ws2812_obj_t;

const mp_obj_type_t machine_ws2812_type;

STATIC mp_obj_t machine_init(mp_obj_t self_in) {
	ws2812_init();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_init_obj, machine_init);

STATIC mp_obj_t machine_red(mp_obj_t self_in) {
	machine_ws2812_obj_t* self = self_in;
    ws2812_run(1);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_red_obj, machine_red);

STATIC mp_obj_t machine_green(mp_obj_t self_in) {
	machine_ws2812_obj_t* self = self_in;
    ws2812_run(2);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_green_obj, machine_green);


STATIC mp_obj_t machine_blue(mp_obj_t self_in) {
	machine_ws2812_obj_t* self = self_in;
    ws2812_run(3);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_blue_obj, machine_blue);

STATIC mp_obj_t machine_off(mp_obj_t self_in) {
	machine_ws2812_obj_t* self = self_in;
    ws2812_run(0);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_off_obj, machine_off);



STATIC mp_obj_t machine_make_new() {
    
    machine_ws2812_obj_t *self = m_new_obj(machine_ws2812_obj_t);
    self->base.type = &machine_ws2812_type;

    return self;
}

STATIC const mp_rom_map_elem_t pyb_ws2812_locals_dict_table[] = {
    // instance methods
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&machine_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_red), MP_ROM_PTR(&machine_red_obj) },
    { MP_ROM_QSTR(MP_QSTR_green), MP_ROM_PTR(&machine_green_obj) },
    { MP_ROM_QSTR(MP_QSTR_blue), MP_ROM_PTR(&machine_blue_obj) },
    { MP_ROM_QSTR(MP_QSTR_off), MP_ROM_PTR(&machine_off_obj) },
};

STATIC MP_DEFINE_CONST_DICT(pyb_ws2812_ocals_dict, pyb_ws2812_locals_dict_table);

const mp_obj_type_t machine_ws2812_type = {
    { &mp_type_type },
    .name = MP_QSTR_ws2812,
    .make_new = machine_make_new,
    .locals_dict = (mp_obj_dict_t*)&pyb_ws2812_ocals_dict,
};

