/**************************************************************************************
 * 文件:    gui.c
 *
 * 说明:    图形用户界面初始化
 *
 * 作者:    Jun
 *
 * 时间:    2012-10-25
**************************************************************************************/
#define  APP_GUI_C
#include <FamesOS.h>
#include "common.h"

void app_init_gui(void)
{
    early_loads();
    
    init_welcome_screen();              /* 欢迎画面 */
    show_palette_init();                /* 调色板 */
    init_main_screen();                 /* 主画面 */
    init_order_query_window();          /* 订单查询画面 */
    init_input_order_screen();          /* 输单画面 */
    init_move_order_dialog();           /* 调单画面 */
    init_generic_param_dlg();           /* 普通参数 */
    init_super_param_dlg();             /* 高级参数 */
    init_spdlmt_setup_dlg();            /* 限速表 */
    init_driver_setup_dlg();            /* 驱动器参数 */
    init_add_sub_prompt();              /* 张数加减画面 */
    init_password_dialog();             /* 密码输入窗口 */
    init_register_dialog();             /* 注册窗口 */
    init_yes_or_no_dialog();            /* Y/N确认窗口 */
    init_about_screen();                /* 系统信息画面 */

    gui_desktop_set_color(COLOR_BLACK);
    gui_desktop_enable();

    StartGUI();
    return;
}


/*=====================================================================================
 * 
 * 本文件结束: gui.c
 * 
**===================================================================================*/

