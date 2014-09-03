/**************************************************************************************
 * �ļ�:    gui.c
 *
 * ˵��:    ͼ���û������ʼ��
 *
 * ����:    Jun
 *
 * ʱ��:    2012-10-25
**************************************************************************************/
#define  APP_GUI_C
#include <FamesOS.h>
#include "common.h"

void app_init_gui(void)
{
    early_loads();
    
    init_welcome_screen();              /* ��ӭ���� */
    show_palette_init();                /* ��ɫ�� */
    init_main_screen();                 /* ������ */
    init_order_query_window();          /* ������ѯ���� */
    init_input_order_screen();          /* �䵥���� */
    init_move_order_dialog();           /* �������� */
    init_generic_param_dlg();           /* ��ͨ���� */
    init_super_param_dlg();             /* �߼����� */
    init_spdlmt_setup_dlg();            /* ���ٱ� */
    init_driver_setup_dlg();            /* ���������� */
    init_add_sub_prompt();              /* �����Ӽ����� */
    init_password_dialog();             /* �������봰�� */
    init_register_dialog();             /* ע�ᴰ�� */
    init_yes_or_no_dialog();            /* Y/Nȷ�ϴ��� */
    init_about_screen();                /* ϵͳ��Ϣ���� */

    gui_desktop_set_color(COLOR_BLACK);
    gui_desktop_enable();

    StartGUI();
    return;
}


/*=====================================================================================
 * 
 * ���ļ�����: gui.c
 * 
**===================================================================================*/

