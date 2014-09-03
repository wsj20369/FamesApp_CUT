/******************************************************************************************
 * �ļ�:    add_sub.c
 *
 * ����:    �����Ӽ�����
 *
 * ����:    Jun
 *
 * ����:    2013-2-5
******************************************************************************************/
#define  ADD_SUB_PROMPT_C
#include <FamesOS.h>
#include "common.h"

/*-----------------------------------------------------------------------------------------
 *          
 *      �ؼ�����������
 * 
**---------------------------------------------------------------------------------------*/
static gui_window_t * add_sub_window = NULL; /* ���� */
static gui_widget   * add_sub_text = NULL;
static gui_widget   * add_sub_form = NULL;

static COLOR  color_for_text = COLOR_WHITE;

/*-----------------------------------------------------------------------------------------
 *          
 *      �����ʼ��
 * 
**---------------------------------------------------------------------------------------*/
void init_add_sub_prompt(void)
{
    gui_widget * widget;
    int x, y, width, height;
    COLOR color, bkcolor;
    char  ___color[6] = "254";
    char  ___bkcolor[6] = "66";
    char  ___x[6] = "352";
    char  ___y[6] = "260";

    load_string(___color, 6, "add_sub_color");
    load_string(___bkcolor, 6, "add_sub_bkcolor");
    load_string(___x, 6, "add_sub_x");
    load_string(___y, 6, "add_sub_y");
    x = atoi(___x);
    y = atoi(___y);
    width = 320;
    height = 80;

    color = atoi(___color);
    bkcolor = atoi(___bkcolor);

    /* ������ */
    widget = gui_create_widget(GUI_WIDGET_FORM, x, y, width, height, color, bkcolor, font16, FORM_STYLE_CLIENT_BDR);
    if(!widget)
        goto some_error;
    add_sub_form = widget;
    add_sub_window = gui_create_window(add_sub_form);
    if (!add_sub_window)
        goto some_error;
    gui_form_init_private(widget, 16);

    /* ��ʾ��Ϣ */
    widget = gui_create_widget(GUI_WIDGET_LABEL, 20, 25, 120, 30, color, bkcolor, font24, LABEL_ALIGN_LEFT);
    if(!widget)
        goto some_error;
    gui_widget_link(add_sub_form, widget);
    gui_label_init_private(widget, 32);
    gui_label_set_text(widget, pick_string("�����Ӽ�:", "Add/Sub:"));

    /* ������ʾ */
    widget = gui_create_widget(GUI_WIDGET_EDIT, 156, 15, 120, 48, color_for_text, COLOR_BLACK, font24, EDIT_ALIGN_CENTER);
    if(!widget)
        goto some_error;
    add_sub_text = widget;
    gui_widget_link(add_sub_form, widget);
    gui_edit_init_private(widget, 32);
    gui_edit_set_text(widget, "");

    return;

some_error:
    sys_print("init_add_sub_prompt(): failed to create widgets or window!\n");
    ExitApplication();
}

void add_sub_prompt_show(int num)
{
    INT08S buf[32];

    INT16toSTR(buf, num, CHG_OPT_DEC|CHG_OPT_END);
    gui_edit_set_text(add_sub_text, buf);
    gui_show_window(add_sub_window);
}

void add_sub_prompt_hide(void)
{
    gui_hide_window(add_sub_window);
}

/* buttoncmds_key_hook_fn */
KEYCODE putkey_to_add_dec(KEYCODE key)
{
    static int i = 0;
    static char buf[8];
    #define MAX_SIZE 4 /* ��������4λ, ��ȡֵ��ΧΪ: [1, 9999] */

    switch (key) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (i < MAX_SIZE) {
                buf[i] = (char)key;
                buf[i+1] = 0;
                i++;
            }
            if (STRtoINT16(buf, CHG_OPT_DEC) > 99) { /* ����99��, Ҫ��ɺ�ɫ, ����ʾ֮�� */
                if (add_sub_text->color != COLOR_RED)/* FIXME: ��Ӧ��ֱ�ӷ��ʽṹ������ĳ�Ա */
                    gui_set_widget_color(add_sub_text, COLOR_RED);
            }
            add_sub_prompt_show(STRtoINT16(buf, CHG_OPT_DEC));
            break;
        case DELETE:
        case BACKSPACE:
            if (i > 0) {
                i--;
                buf[i] = 0;
            }
            if (STRtoINT16(buf, CHG_OPT_DEC) <= 99) {
                if (add_sub_text->color != color_for_text)
                    gui_set_widget_color(add_sub_text, color_for_text);
            }
            if (i <= 0)
                add_sub_prompt_hide();
            else
                add_sub_prompt_show(STRtoINT16(buf, CHG_OPT_DEC));
            break;
        case '-':
            i = 0;
            buf[i] = 0;
            add_sub_prompt_hide();
            gui_set_widget_color(add_sub_text, color_for_text);
            break;
        case '+':
            i = 0;
            buf[i] = 0;
            add_sub_prompt_hide();
            gui_set_widget_color(add_sub_text, color_for_text);
            break;
        case ESC:  /* ESC���˳��Ӽ�����ģʽ */
        case SPACE:
            if (i > 0) {
                i = 0;
                buf[i] = 0;
                add_sub_prompt_hide();
                gui_set_widget_color(add_sub_text, color_for_text);
                break;
            }
            return key;
        default:
            if (i > 0) /* �ڼӼ�����ʱ, ������ֵ���� */
                break;
            return key;
    }

    return NONEKEY;
}

/*=========================================================================================
 * 
 * ���ļ�����: add_sub.c
 * 
**=======================================================================================*/


