/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Mutex.
 *
 */
#include "mutex.h"

// This is a standard implementation of mutexs on ARM processors following the ARM guide.
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHEJCHB.html

void mutex_init(mutex_t *mutex)
{
    return;
}

void mutex_lock(mutex_t *mutex, uint32_t tid)
{
    return;
}


int mutex_try_lock(mutex_t *mutex, uint32_t tid)
{
    return 1;
}


void mutex_unlock(mutex_t *mutex, uint32_t tid)
{
    return;
}

