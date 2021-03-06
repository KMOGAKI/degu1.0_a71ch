/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Linaro Limited
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

#include "py/mpconfig.h"
#if MICROPY_PY_ZEPHYR

#include <power.h>
#include <kernel.h>
#include <zephyr.h>
#include <misc/stack.h>

#include "py/runtime.h"

STATIC void mp_stack_dump(const struct k_thread *thread, void *user_data) {
	stack_analyze((char *)user_data, (char *)thread->stack_info.start,
		      thread->stack_info.size);
}

STATIC mp_obj_t mod_is_preempt_thread(void) {
    return mp_obj_new_bool(k_is_preempt_thread());
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_is_preempt_thread_obj, mod_is_preempt_thread);

STATIC mp_obj_t mod_suspend(mp_obj_t time_sec)
{
	int time_to_wake = mp_obj_get_int(time_sec);
	sys_pm_ctrl_enable_state(SYS_POWER_STATE_CPU_LPS_1);
	sys_pm_ctrl_enable_state(SYS_POWER_STATE_CPU_LPS_2);
	sys_set_power_state(SYS_POWER_STATE_CPU_LPS_2);
	k_sleep(K_SECONDS(time_to_wake));
    sys_pm_ctrl_disable_state(SYS_POWER_STATE_CPU_LPS_1);
	sys_pm_ctrl_disable_state(SYS_POWER_STATE_CPU_LPS_2);
	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_suspend_obj, mod_suspend);


STATIC mp_obj_t mod_powerdown(void)
{
    sys_pm_ctrl_enable_state(SYS_POWER_STATE_DEEP_SLEEP);
	sys_set_power_state(SYS_POWER_STATE_DEEP_SLEEP);
    sys_pm_ctrl_disable_state(SYS_POWER_STATE_DEEP_SLEEP);
	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_powerdown_obj, mod_powerdown);

STATIC mp_obj_t mod_current_tid(void) {
    return MP_OBJ_NEW_SMALL_INT(k_current_get());
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_current_tid_obj, mod_current_tid);

STATIC mp_obj_t mod_stacks_analyze(void) {
    k_thread_foreach(mp_stack_dump, NULL);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_stacks_analyze_obj, mod_stacks_analyze);

#ifdef CONFIG_NET_SHELL

//int net_shell_cmd_iface(int argc, char *argv[]);

STATIC mp_obj_t mod_shell_net_iface(void) {
    net_shell_cmd_iface(0, NULL);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_shell_net_iface_obj, mod_shell_net_iface);

#endif // CONFIG_NET_SHELL

STATIC const mp_rom_map_elem_t mp_module_time_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_zephyr) },
    { MP_ROM_QSTR(MP_QSTR_is_preempt_thread), MP_ROM_PTR(&mod_is_preempt_thread_obj) },
    { MP_ROM_QSTR(MP_QSTR_suspend), MP_ROM_PTR(&mod_suspend_obj) },
    { MP_ROM_QSTR(MP_QSTR_powerdown), MP_ROM_PTR(&mod_powerdown_obj) },
    { MP_ROM_QSTR(MP_QSTR_current_tid), MP_ROM_PTR(&mod_current_tid_obj) },
    { MP_ROM_QSTR(MP_QSTR_stacks_analyze), MP_ROM_PTR(&mod_stacks_analyze_obj) },

    #ifdef CONFIG_NET_SHELL
    { MP_ROM_QSTR(MP_QSTR_shell_net_iface), MP_ROM_PTR(&mod_shell_net_iface_obj) },
    #endif
};

STATIC MP_DEFINE_CONST_DICT(mp_module_time_globals, mp_module_time_globals_table);

const mp_obj_module_t mp_module_zephyr = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_time_globals,
};

#endif // MICROPY_PY_ZEPHYR
