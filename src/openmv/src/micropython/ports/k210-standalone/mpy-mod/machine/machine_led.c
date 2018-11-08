
#include <stdio.h>
#include <stdint.h>

#include "py/nlr.h"
#include "py/runtime.h"
#include "modmachine.h"
#include "mphalport.h"

#include "led.h"

typedef struct _machine_led_obj_t {
    mp_obj_base_t base;
} machine_led_obj_t;

const mp_obj_type_t machine_led_type;

STATIC mp_obj_t machine_init(mp_obj_t self_in) {
	led_init();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_init_obj, machine_init);

STATIC mp_obj_t machine_led_1_on(mp_obj_t self_in) {
	machine_led_obj_t* self = self_in;
    led_OnOff(0,1);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_led_1_on_obj, machine_led_1_on);

STATIC mp_obj_t machine_led_1_off(mp_obj_t self_in) {
	machine_led_obj_t* self = self_in;
    led_OnOff(0,0);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_led_1_off_obj, machine_led_1_off);

STATIC mp_obj_t machine_led_2_on(mp_obj_t self_in) {
	machine_led_obj_t* self = self_in;
    led_OnOff(1,1);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_led_2_on_obj, machine_led_2_on);

STATIC mp_obj_t machine_led_2_off(mp_obj_t self_in) {
	machine_led_obj_t* self = self_in;
    led_OnOff(1,0);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_led_2_off_obj, machine_led_2_off);



STATIC mp_obj_t machine_make_new() {
    
    machine_led_obj_t *self = m_new_obj(machine_led_obj_t);
    self->base.type = &machine_led_type;

    return self;
}

STATIC const mp_rom_map_elem_t pyb_led_locals_dict_table[] = {
    // instance methods
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&machine_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_left_on), MP_ROM_PTR(&machine_led_1_on_obj) },
    { MP_ROM_QSTR(MP_QSTR_left_off), MP_ROM_PTR(&machine_led_1_off_obj) },
    { MP_ROM_QSTR(MP_QSTR_right_on), MP_ROM_PTR(&machine_led_2_on_obj) },
    { MP_ROM_QSTR(MP_QSTR_right_off), MP_ROM_PTR(&machine_led_2_off_obj) },
};

STATIC MP_DEFINE_CONST_DICT(pyb_led_ocals_dict, pyb_led_locals_dict_table);

const mp_obj_type_t machine_led_type = {
    { &mp_type_type },
    .name = MP_QSTR_led,
    .make_new = machine_make_new,
    .locals_dict = (mp_obj_dict_t*)&pyb_led_ocals_dict,
};

