/*************************************************************************************
 * 文件:    start.c
 *
 * 说明:    应用程序入口
 *
 * 作者:    Jun
 *
 * 时间:    2012-10-25
*************************************************************************************/
#define  APP_START_C
#include <FamesOS.h>
#include "common.h"

/*------------------------------------------------------------------------------------
 * 函数:    startup_init()
 *
 * 描述:    启始初始化, 用于打开系统相关的一些资源
**----------------------------------------------------------------------------------*/
void startup_init(void)
{
    OpenConsole();
}

/*------------------------------------------------------------------------------------
 * 函数:    sk_refresh_screen()
 *
 * 描述:    刷新屏幕
**----------------------------------------------------------------------------------*/
void sk_refresh_screen(void)
{
    gui_window_t * win;    
    gui_for_each_window(win) {
        gui_refresh_window(win);
    }
}

/*------------------------------------------------------------------------------------
 * 函数:    start()
 *
 * 描述:    SLC启始任务, 初始化并提供用户对SLC的控制
**----------------------------------------------------------------------------------*/
void __task start(void * data)
{
    extern struct cut_config_s config;

    data = data;

    startup_init();

    RegisterSpecialKey(INSERT, sk_refresh_screen); /* INSERT: 刷新当前屏幕 */

    read_config();
    read_tmp_config();
    language_initialize(config.language);

    if (register_service_init() == fail) {
        sys_print("Fatal Error: Register Service Init Failed!!!\n");
        ExitApplication();
    }

    InitOrderEnv();
    /* slc_control_service_initialize(); 需要实现CUT对应的版本 */

    app_init_gui();

    #if 0
    {
        char  ___s[64];
        welcome_start();
        ___s[0] = 0;
        load_string(___s, sizeof(___s), "welcome_started");
        startup_message(___s);
        welcome_ended();
    }
    #endif

    active_main_screen(); /* 显示主画面 */

    start_main_loop();    /* 启动主循环, 此函数永不返回 */
}

/*------------------------------------------------------------------------------------
 * 函数: TaskSwitchHook()
 *
 * 说明: 任务切换钩子
 *
 * 特别: 此函数运行在中断服务程序之中,应特别注意其执行效率
**----------------------------------------------------------------------------------*/
void apical TaskSwitchHook(void)
{
}

void quit(void)
{
    ExitApplication();
}

long get_free_mem(void)
{
    long mem;

    lock_kernel();
    mem = (long)coreleft();
    unlock_kernel();

    return mem;
}

/*------------------------------------------------------------------------------------
 *  取系统的时间日期
**----------------------------------------------------------------------------------*/
void GetDateTime (INT08S *s)
{
    struct time now;
    struct date today;

    lock_kernel();
    gettime(&now);
    getdate(&today);
    unlock_kernel();
    sprintf(s, "%02d-%02d-%02d  %02d:%02d:%02d",
            today.da_mon,
            today.da_day,
            today.da_year,
            now.ti_hour,
            now.ti_min,
            now.ti_sec);
}


/*====================================================================================
 * 
 * 本文件结束: start.c
 * 
**==================================================================================*/

