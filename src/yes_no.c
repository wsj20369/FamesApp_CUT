/******************************************************************************************
 * 文件:    cut/yes_no.c
 *
 * 描述:    确认对话框
 *
 * 作者:    Jun
 *
 * 创建:    2011-3-31, 2012-10-29
******************************************************************************************/
#define  CUT_YES_NO_C
#include <FamesOS.h>
#include "common.h"

/*-----------------------------------------------------------------------------------------
 *          
 *      确认画面中的控件及其它定义
 * 
**---------------------------------------------------------------------------------------*/
static gui_window_t * yes_no_window = NULL;
static gui_widget   * yes_no_dialog = NULL;     /* 主控件 */

static gui_widget   * no_button  = NULL;
static gui_widget   * yes_button = NULL;
static gui_widget   * text_label = NULL;

extern BMPINFO icon; /* 图标 */

extern char  ___bkcolor_other_dialog[];


/*-----------------------------------------------------------------------------------------
 *          
 *      确认对话框的创建
 * 
**---------------------------------------------------------------------------------------*/
void init_yes_or_no_dialog(void)
{
    gui_widget * widget;
    gui_widget * form;
    int x, y, width, height, font, __t;
    COLOR bkcolor;

    x = 0;
    y = 0;
    width = 480;
    height = 160;
    font = 1;

    bkcolor = atoi(___bkcolor_other_dialog);

    /* 主窗体 */
    widget = gui_create_widget(GUI_WIDGET_FORM, x, y, width, height, 0, bkcolor, font, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE);
    form = widget;
    if(!widget)
        goto some_error;
    yes_no_dialog = widget;
    yes_no_window = gui_create_window(yes_no_dialog);
    if (!yes_no_window)
        goto some_error;
    gui_form_init_private(widget, 64);
    gui_form_set_icon(widget, &icon);

    /* YES按钮 */
    __t  = (width-180);
    __t /= 3;
    widget = gui_create_widget(GUI_WIDGET_BUTTON, (__t-20), (height-56), 132, 36, 0, 0, font, 0);
    if(!widget)
        goto some_error;
    yes_button = widget;
    gui_widget_link(form, widget);
    gui_button_init_private(widget, 32);
    gui_button_set_caption(widget, pick_string("确认[Y]", "OK.[Y]"));

    /* NO按钮 */
    __t += 112;
    widget = gui_create_widget(GUI_WIDGET_BUTTON, (width-__t), (height-56), 132, 36, 0, 0, font, 0);
    if(!widget)
        goto some_error;
    no_button = widget;
    gui_widget_link(form, widget);
    gui_button_init_private(widget, 32);
    gui_button_set_caption(widget, pick_string("取消[N]", "No.[N]"));

    /* 提示信息 */
    widget = gui_create_widget(GUI_WIDGET_LABEL, 6, (height-128)/2+32, (width-24), 30, 0, bkcolor, font24, LABEL_ALIGN_CENTER);
    if(!widget)
        goto some_error;
    text_label = widget;
    gui_widget_link(form, widget);
    gui_label_init_private(widget, 256);
    gui_label_set_text(widget, "");

    return;

some_error:
    sys_print("init_yes_or_no_dialog(): failed to create widgets or window!\n");
    ExitApplication();
}

/*-----------------------------------------------------------------------------------------
 * 函数:    __waitkey_for_YN()
 *
 * 描述:    等待按键, 用于YES/NO按钮
**---------------------------------------------------------------------------------------*/
static KEYCODE __waitkey_for_YN(INT32U timeout)
{
    KEYCODE key;
    int  wait = 1;

    do {
        key = waitkey(timeout);
        switch(key){
            case ENTER:
            case SPACE:
            case ESC:
            case TAB:
            case LEFT:
            case RIGHT:
            case UP:
            case DOWN:
            case SHIFT_TAB:
            case 'Y':
            case 'y':
            case 'N':
            case 'n':
                wait = 0;
                break;
            default:
                break;
        }
    } while(wait);

    return key;
}

/*-----------------------------------------------------------------------------------------
 * 函数:    check_yes_no_dialog
 *
 * 描述:    确认YES/NO对话框
**---------------------------------------------------------------------------------------*/
BOOL check_yes_no_dialog(int x, int y, char * caption, char * text)
{
    BOOL retval;
    int  loop, index;
    KEYCODE key;
    static int lock = 0;
    #define ____button_style BUTTON_STYLE_CLIENT_BDR

    os_mutex_lock(lock);

    if(!caption)
        caption = "CUT";
    if(!text)
        text = pick_string("确认吗?", "Can you Confirm?");

    gui_set_widget_style(yes_button, ____button_style);
    gui_set_widget_style(no_button, ____button_style|BUTTON_STYLE_PRESSED);
    gui_label_set_text(text_label, text);
    gui_set_widget_location(yes_no_dialog, x, y);
    gui_form_set_caption(yes_no_dialog, caption);
    gui_show_window(yes_no_window);

    index = 2;  /* 默认值为NO */
    retval = fail;

    for(loop=1; loop;){
        key = __waitkey_for_YN(0L);
        switch(key){
            case ENTER:
            case SPACE:
                loop = 0;
                if(index == 1)
                    retval = ok;
                else
                    retval = fail;
                break;
            case LEFT:
            case UP:
                index = 1;
                break;
            case RIGHT:
            case DOWN:
                index = 2;
                break;
            case TAB:
            case SHIFT_TAB:
                if(index == 1){
                    index = 2;
                } else {
                    index = 1;
                }
                break;
            case ESC:
            case 'N':
            case 'n':
                loop = 0;
                retval = fail;
                break;
            case 'Y':
            case 'y':
                loop = 0;
                retval = ok;
                break;
            default:
                break;
        }
        if(loop){
            if(index == 2){
                gui_set_widget_style(no_button, ____button_style|BUTTON_STYLE_PRESSED);
                gui_set_widget_style(yes_button, ____button_style);
            } else {
                gui_set_widget_style(yes_button, ____button_style|BUTTON_STYLE_PRESSED);
                gui_set_widget_style(no_button, ____button_style);
            }
        }
    }

    gui_hide_window(yes_no_window);

    os_mutex_unlock(lock);

    return retval;
}


/*=========================================================================================
 * 
 * 本文件结束: cut/yes_no.c
 * 
**=======================================================================================*/


