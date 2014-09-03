/*************************************************************************************
 * 文件:    common.h
 *
 * 作者:    Jun
 *
 * 时间:    2012-10-25
*************************************************************************************/
#ifndef FAMES_APP_COMMON_H
#define FAMES_APP_COMMON_H

#include <string.h>

#include "cfg_defs.h"
#include "reg_core.h"
#include "cut_reg.h"
#include "passwd.h"
#include "yes_no.h"
#include "order.h"
#include "config.h"
#include "tmp_cfg.h"
#include "keycmd.h"
#include "add_sub.h"
#include "setup.h"
#include "ord_edit.h"
#include "cut_ctrl.h"
#include "language.h"
#include "status.h"

/*lint -e806*/

void quit(void);

long get_free_mem(void);

void app_init_gui(void);
void example_init_xms(void);
void early_loads(void);

int get_font_big1(int id);
int get_font_big2(int id);

void init_welcome_screen(void);
void welcome_start(void);
void welcome_ended(void);
void startup_message(INT08S * s);

void show_palette_init(void);

void start_main_loop(void);

void init_main_screen(void);
void init_order_query_window(void);
void active_main_screen(void);
void message(INT08S * s);
void set_buttons_caption(INT08S *captions[]);

void show_today_data_screen(int show);

void init_about_screen(void);

extern int font16, font48, font24;

extern struct cut_config_s config;

INT16U get_version(void);
STRING get_version_string(void);

struct bits16{
    int b0:1;
    int b1:1;
    int b2:1;
    int b3:1;
    int b4:1;
    int b5:1;
    int b6:1;
    int b7:1;
    int b8:1;
    int b9:1;
    int b10:1;
    int b11:1;
    int b12:1;
    int b13:1;
    int b14:1;
    int b15:1;
};

struct bits32{
    int b0:1;
    int b1:1;
    int b2:1;
    int b3:1;
    int b4:1;
    int b5:1;
    int b6:1;
    int b7:1;
    int b8:1;
    int b9:1;
    int b10:1;
    int b11:1;
    int b12:1;
    int b13:1;
    int b14:1;
    int b15:1;
    int b16:1;
    int b17:1;
    int b18:1;
    int b19:1;
    int b20:1;
    int b22:1;
    int b23:1;
    int b24:1;
    int b25:1;
    int b26:1;
    int b27:1;
    int b28:1;
    int b29:1;
    int b30:1;
    int b31:1;
};


#endif /* #ifndef FAMES_APP_COMMON_H */

/*====================================================================================
 * 
 * 本文件结束: common.h
 * 
**==================================================================================*/

