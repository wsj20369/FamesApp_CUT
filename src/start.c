/*************************************************************************************
 * �ļ�:    start.c
 *
 * ˵��:    Ӧ�ó������
 *
 * ����:    Jun
 *
 * ʱ��:    2012-10-25
*************************************************************************************/
#define  APP_START_C
#include <FamesOS.h>
#include "common.h"

/*------------------------------------------------------------------------------------
 * ����:    startup_init()
 *
 * ����:    ��ʼ��ʼ��, ���ڴ�ϵͳ��ص�һЩ��Դ
**----------------------------------------------------------------------------------*/
void startup_init(void)
{
    OpenConsole();
}

/*------------------------------------------------------------------------------------
 * ����:    sk_refresh_screen()
 *
 * ����:    ˢ����Ļ
**----------------------------------------------------------------------------------*/
void sk_refresh_screen(void)
{
    gui_window_t * win;    
    gui_for_each_window(win) {
        gui_refresh_window(win);
    }
}

/*------------------------------------------------------------------------------------
 * ����:    start()
 *
 * ����:    SLC��ʼ����, ��ʼ�����ṩ�û���SLC�Ŀ���
**----------------------------------------------------------------------------------*/
void __task start(void * data)
{
    extern struct cut_config_s config;

    data = data;

    startup_init();

    RegisterSpecialKey(INSERT, sk_refresh_screen); /* INSERT: ˢ�µ�ǰ��Ļ */

    read_config();
    read_tmp_config();
    language_initialize(config.language);

    if (register_service_init() == fail) {
        sys_print("Fatal Error: Register Service Init Failed!!!\n");
        ExitApplication();
    }

    InitOrderEnv();
    /* slc_control_service_initialize(); ��Ҫʵ��CUT��Ӧ�İ汾 */

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

    active_main_screen(); /* ��ʾ������ */

    start_main_loop();    /* ������ѭ��, �˺����������� */
}

/*------------------------------------------------------------------------------------
 * ����: TaskSwitchHook()
 *
 * ˵��: �����л�����
 *
 * �ر�: �˺����������жϷ������֮��,Ӧ�ر�ע����ִ��Ч��
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
 *  ȡϵͳ��ʱ������
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
 * ���ļ�����: start.c
 * 
**==================================================================================*/

