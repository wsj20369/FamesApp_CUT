/*************************************************************************************
 * 文件:    cut/welcome.c
 *
 * 说明:    欢迎界面
 *
 * 作者:    Jun
 *
 * 时间:    2011-2-19, 2012-10-29
*************************************************************************************/
#define  CUT_WELCOME_C
#include <includes.h>
#include "common.h"

/*------------------------------------------------------------------------------------
 * 
 *  启始画面中的控件
 * 
**----------------------------------------------------------------------------------*/
gui_widget * welcome_root;   /* 欢迎界面的主控件 */
gui_widget * welcome_tupian;
gui_widget * welcome_msg;
gui_widget * welcome_status_bar;
gui_widget * welcome_button1, * welcome_button2;

gui_window_t * welcome_window;

extern BMPINFO welcome_picture, icon;

/*------------------------------------------------------------------------------------
 * 函数:    init_welcome_screen()
 *
 * 描述:    启始画面初始化
**----------------------------------------------------------------------------------*/
void init_welcome_screen(void)
{
    char __version[100];
    char ___s[128];

    sprintf(__version, "OS: %s-%s   CUT: %s", 
                        os_get_name(),
                        os_get_version_string(),
                        get_version_string());

    welcome_root       = gui_create_widget(GUI_WIDGET_FORM,    1,   1,  1021, 765, 0, 0, 0, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE);
    welcome_tupian     = gui_create_widget(GUI_WIDGET_PICTURE, 71,  70,  880, 450, 0, 0, 0, PICTURE_STYLE_NO_BORDER);
    welcome_msg        = gui_create_widget(GUI_WIDGET_LABEL,   7,   725,1006, 30,  0, 0, 1, LABEL_STYLE_CLIENT_BDR);
    welcome_button1    = gui_create_widget(GUI_WIDGET_BUTTON,  236, 590, 180, 42,  0, 0, 1, BUTTON_STYLE_FOCUS_BORDER | BUTTON_STYLE_CLIENT_BDR);
    welcome_button2    = gui_create_widget(GUI_WIDGET_BUTTON,  585, 590, 180, 42,  0, 0, 1, BUTTON_STYLE_CLIENT_BDR);
    welcome_status_bar = gui_create_widget(GUI_WIDGET_LABEL,   8,   705, 900, 20,  4, 0, 0, 0);
    if(!welcome_root ||
       !welcome_tupian ||
       !welcome_msg ||
       !welcome_button1 ||
       !welcome_button2 ||
       !welcome_status_bar){
        sys_print("init_welcome_screen(): failed to create widgets for welcome!\n");
        quit();
    }
    gui_widget_link(NULL, welcome_root);
    gui_form_init_private(welcome_root, 64);
    gui_form_set_icon(welcome_root, &icon);
    STRCPY(___s, "Welcome");
    load_string(___s, sizeof(___s), "welcome_title");
    gui_form_set_caption(welcome_root, ___s);

    gui_widget_link(welcome_root, welcome_msg);
    gui_label_init_private(welcome_msg, 100);  /* 最多50个汉字 */
    
    gui_widget_link(welcome_root, welcome_tupian);
    gui_picture_init_private(welcome_tupian);
    gui_picture_set_picture(welcome_tupian, &welcome_picture);
    
    gui_widget_link(welcome_root, welcome_button1);
    gui_button_init_private(welcome_button1, 32);
    gui_button_set_caption(welcome_button1, pick_string("启动中...", "Starting..."));
        
    gui_widget_link(welcome_root, welcome_button2);
    gui_button_init_private(welcome_button2, 32);
    gui_button_set_caption(welcome_button2, pick_string("[ESC] 退出", "[ESC] Exit"));

    gui_widget_link(welcome_root, welcome_status_bar);
    gui_label_init_private(welcome_status_bar, 100);
    gui_label_set_text(welcome_status_bar, __version);

    welcome_window = gui_create_window(welcome_root);
}

void welcome_start(void)
{
    gui_show_window(welcome_window);
}

void welcome_ended(void)
{
    KEYCODE key;
    int     enter;
    int     pressed;
    INT16U  style;

    enter = 1;
    pressed = 1;

    gui_button_set_caption(welcome_button1, pick_string("[ENTER] 确定", "[ENTER] OK"));

    for(;;){
        key = waitkey(0L);
        switch(key){
            case ENTER:
                if(enter)
                    goto out;
                else
                    quit();
                break;
            case ESC:
                quit();
                break;
            case LEFT:
                if(enter)
                    continue;
                enter = 1;
                break;
            case RIGHT:
                if(!enter)
                    continue;
                enter = 0;
                break;
            case TAB:
                enter = !enter;
                break;
            default:
                continue;
        }
        if(pressed)
            style = BUTTON_STYLE_PRESSED;
        else 
            style = 0;
        if(enter){
            gui_set_widget_style(welcome_button1, BUTTON_STYLE_FOCUS_BORDER | BUTTON_STYLE_CLIENT_BDR | style);
            gui_set_widget_style(welcome_button2, BUTTON_STYLE_CLIENT_BDR);
        } else {
            gui_set_widget_style(welcome_button2, BUTTON_STYLE_FOCUS_BORDER | BUTTON_STYLE_CLIENT_BDR | style);
            gui_set_widget_style(welcome_button1, BUTTON_STYLE_CLIENT_BDR);
        }
    }
    
out:
    gui_hide_window(welcome_window);
    return;
}

void startup_message(INT08S * s)
{
    char a[80];
    
    if(!welcome_msg || !s)
        return;
    sprintf(a, ">>>%s", s);
    gui_label_set_text(welcome_msg, a);
    TaskSleep(10L);
}


/*====================================================================================
 * 
 * 本文件结束: cut/welcome.c
 * 
**==================================================================================*/


