/* This file is part of the OpenMV project.
 * Copyright (c) 2013-2018 Ibrahim Abdelkader <iabdalkader@openmv.io> & Kwabena W. Agyeman <kwagyeman@openmv.io>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 */
#ifndef __WIFI_DBG_H__
#define __WIFI_DBG_H__
typedef struct wifidbg_config {
    uint8_t unused;
}__attribute__((aligned(8))) wifidbg_config_t;
int wifidbg_init(wifidbg_config_t *config);
void wifidbg_dispatch();

#endif /* __WIFIDBG_H__ */
