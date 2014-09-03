/******************************************************************************************
 * 文件:    cut/status.c
 *
 * 描述:    生产状态的显示及刷新
 *
 * 作者:    Jun
 *
 * 创建:    2013-2-15
******************************************************************************************/
#define  CUT_STATUS_C
#include <FamesOS.h>
#include "common.h"

extern int font_big1, font_big2;

/*static*/ gui_widget * l__prod_count, * l__prod_meter, * l__prod_progress;
/*static*/ gui_widget * l__remain_count, * l__remain_meter, * l__remain_time;
/*static*/ gui_widget * l__bad_count, * l__bad_meter, * l__bad_percent;
/*static*/ gui_widget * l__real_speed, * l__average_speed, * l__speed_limit;
/*static*/ gui_widget * l__set_len, * l__actual_len, * l__len_diff;

static gui_widget * main_status_screen;
static gui_widget * main_today_data_screen;

#define PROD_BKCOLOR    136
#define PROD_COLOR      COLOR_YELLOW

#define REMAIN_BKCOLOR  66
#define REMAIN_COLOR    COLOR_GREEN

#define BAD_BKCOLOR     66
#define BAD_COLOR       6

#define SPEED_BKCOLOR   COLOR_BLUE
#define SPEED_COLOR     COLOR_CYAN

#define CUTLEN_BKCOLOR  0
#define CUTLEN_COLOR    COLOR_GRAY

#define TODAY_DATA_BK   136
#define TODAY_DATA_CL   COLOR_CYAN

#define X               8
#define Y               33
#define WIDTH           1004
#define HEIGHT          351

#define ADJUST_X        3 /* 用来调整左右两边的比例 */

#define DISTANCE        1
#define BORDER          5

/* LOGO部分 */
#define X_LOGO          (DISTANCE+BORDER)
#define Y_LOGO          (DISTANCE+BORDER)
#define W_LOGO          (600+ADJUST_X)
#define H_LOGO          56

/* 左半部分 */
#define X1              (DISTANCE+BORDER)
#define Y1              (Y_LOGO+H_LOGO+DISTANCE)
#define WIDTH1          (600+ADJUST_X)
#define HEIGHT1         140

/* 右半部分 */
#define X2              (X1+WIDTH1+DISTANCE+DISTANCE)
#define WIDTH2          (WIDTH-X2-DISTANCE-BORDER)
#define Y2              (Y_LOGO+HEIGHT2+DISTANCE)
#define HEIGHT2         112
#define Y3              (Y2+HEIGHT2+DISTANCE)
#define HEIGHT3         (HEIGHT-Y3-DISTANCE-BORDER)

#define W_TODAY_DATA    (WIDTH-X1-DISTANCE-BORDER)
#define Y_TODAY_DATA    (Y3+HEIGHT3+DISTANCE)
#define H_TODAY_DATA    ((HEIGHT+MAIN_ORDER_ITEM_HEIGHT)-Y_TODAY_DATA-DISTANCE-BORDER)

#define Y_BAD           Y_LOGO

#define LABEL_TEST_STYLE LABEL_STYLE_BORDER

void hook_font(KEYCODE key)
{
    if (key >= 'a' && key <= 'e') {
        set_status_font_big1(key - 'a');
        set_status_font_big2(key - 'a');
    }
}


/*-----------------------------------------------------------------------------------------
 *          
 *      生产画面(主版)
 * 
**---------------------------------------------------------------------------------------*/
void init_produce_screen(gui_widget * parent)
{
    gui_widget * widget, * main_form, * prod, * remain, * bad, * speed, * cutlen;

    main_form = gui_create_widget(GUI_WIDGET_FORM, X, Y, WIDTH, HEIGHT, 0, 0, 0,
                                  FORM_STYLE_CLIENT_BDR|FORM_STYLE_TRANSPARENT);
    if (!main_form)
        goto some_error;
    main_status_screen = main_form;
    gui_widget_link(parent, main_form);
    gui_form_init_private(main_form, 16);
    gui_form_set_caption(main_form, "status");

    /* 图框: 5大部分 */

    prod = gui_create_widget(GUI_WIDGET_FORM, X1, Y1, WIDTH1, HEIGHT1,
                             PROD_COLOR, PROD_BKCOLOR, 0, FORM_STYLE_CLIENT_BDR);
    if (!prod)
        goto some_error;
    gui_widget_link(main_form, prod);
    gui_form_init_private(prod, 32);
    gui_form_set_caption(prod, "良品张数");

    remain = gui_create_widget(GUI_WIDGET_FORM, X1, Y1+HEIGHT1+DISTANCE, WIDTH1, HEIGHT1,
                               REMAIN_COLOR, REMAIN_BKCOLOR, 0, FORM_STYLE_CLIENT_BDR);
    if (!remain)
        goto some_error;
    gui_widget_link(main_form, remain);
    gui_form_init_private(remain, 32);
    gui_form_set_caption(prod, "剩余张数");

    bad = gui_create_widget(GUI_WIDGET_FORM, X2, Y_BAD, WIDTH2, HEIGHT2,
                            BAD_COLOR, BAD_BKCOLOR, 0, FORM_STYLE_CLIENT_BDR);
    if (!bad)
        goto some_error;
    gui_widget_link(main_form, bad);
    gui_form_init_private(bad, 32);
    gui_form_set_caption(bad, "坏纸张数");

    speed = gui_create_widget(GUI_WIDGET_FORM, X2, Y2, WIDTH2, HEIGHT2,
                              SPEED_COLOR, SPEED_BKCOLOR, 0, FORM_STYLE_CLIENT_BDR);
    if (!speed)
        goto some_error;
    gui_widget_link(main_form, speed);
    gui_form_init_private(speed, 32);
    gui_form_set_caption(speed, "车速");

    cutlen = gui_create_widget(GUI_WIDGET_FORM, X2, Y3, WIDTH2, HEIGHT3,
                               CUTLEN_COLOR, CUTLEN_BKCOLOR, 0, FORM_STYLE_CLIENT_BDR);
    if (!cutlen)
        goto some_error;
    gui_widget_link(main_form, cutlen);
    gui_form_init_private(cutlen, 32);
    gui_form_set_caption(cutlen, "切长");

    /* 良品张数区域 */

    widget = gui_create_widget(GUI_WIDGET_LABEL, 6, 54, 120, 32, 
                               PROD_COLOR, PROD_BKCOLOR, font24, 0|LABEL_TEST_STYLE);
    if(!widget)
        goto some_error;
    gui_widget_link(prod, widget);
    gui_label_init_private(widget, 32);
    gui_label_set_text(widget, pick_string("良品张数:", "Produced:"));

    widget = gui_create_widget(GUI_WIDGET_LABEL, (116+ADJUST_X), 15, 472, 110, 
                               PROD_COLOR, 33+PROD_BKCOLOR, font_big1, LABEL_ALIGN_RIGHT|LABEL_TEST_STYLE);
    if(!widget)
        goto some_error;
    gui_widget_link(prod, widget);
    gui_label_init_private(widget, 32);
    gui_label_set_text(widget, "864193");
    l__prod_count = widget;

    widget = gui_create_widget(GUI_WIDGET_EDIT, 6, 130, (587+ADJUST_X), 32, 0, 136, font24, 0);
    if(!widget)
        goto some_error;
    gui_widget_link(prod, widget);
    gui_edit_init_private(widget, 128);
    gui_edit_set_text(widget, "生产米数: 862348937, 生产进度条asfddf4987");

    /* 剩余张数区域 */

    widget = gui_create_widget(GUI_WIDGET_LABEL, (108+ADJUST_X), 15, 480, 110, 
                               REMAIN_COLOR, REMAIN_BKCOLOR, font_big1, LABEL_ALIGN_RIGHT);
    if(!widget)
        goto some_error;
    gui_widget_link(remain, widget);
    gui_label_init_private(widget, 32);
    gui_label_set_text(widget, "123456");
    l__remain_count = widget;

    widget = gui_create_widget(GUI_WIDGET_EDIT, 6, 130, (587+ADJUST_X), 32, 0, 136, font24, 0);
    if(!widget)
        goto some_error;
    gui_widget_link(remain, widget);
    gui_edit_init_private(widget, 128);
    gui_edit_set_text(widget, "剩余米数: 8623489, 剩余时间: 00:00:30");

    /* 坏纸张数区域 */

    widget = gui_create_widget(GUI_WIDGET_LABEL, (120-ADJUST_X), 11, 240, 64,
                               BAD_COLOR, BAD_BKCOLOR, font_big2, LABEL_ALIGN_RIGHT);
    if(!widget)
        goto some_error;
    gui_widget_link(bad, widget);
    gui_label_init_private(widget, 8);
    gui_label_set_text(widget, "1239");
    l__bad_count = widget;

    widget = gui_create_widget(GUI_WIDGET_LABEL, 6, 79, (379-ADJUST_X), 28, 0, 33, font24, LABEL_STYLE_BORDER);
    if(!widget)
        goto some_error;
    gui_widget_link(bad, widget);
    gui_label_init_private(widget, 128);
    gui_label_set_text(widget, "坏纸米数: 8623489, 坏纸比例");

    /* 车速区域 */

    widget = gui_create_widget(GUI_WIDGET_LABEL, (120-ADJUST_X), 11, 240, 64,
                               SPEED_COLOR, SPEED_BKCOLOR, font_big2, LABEL_ALIGN_RIGHT);
    if(!widget)
        goto some_error;
    gui_widget_link(speed, widget);
    gui_label_init_private(widget, 8);
    gui_label_set_text(widget, "1239");
    l__real_speed = widget;

    widget = gui_create_widget(GUI_WIDGET_LABEL, 6, 79, (379-ADJUST_X), 28, 0, 33, font24, LABEL_STYLE_BORDER);
    if(!widget)
        goto some_error;
    gui_widget_link(speed, widget);
    gui_label_init_private(widget, 128);
    gui_label_set_text(widget, "平均车速: 8623489, 限速: 455");

    /* 切长区域 */

    widget = gui_create_widget(GUI_WIDGET_LABEL, (120-ADJUST_X), 11, 240, 64,
                               CUTLEN_COLOR, CUTLEN_BKCOLOR, font_big2, LABEL_ALIGN_RIGHT);
    if(!widget)
        goto some_error;
    gui_widget_link(cutlen, widget);
    gui_label_init_private(widget, 8);
    gui_label_set_text(widget, "8866");
    l__set_len = widget;

    widget = gui_create_widget(GUI_WIDGET_LABEL, 6, 79, (379-ADJUST_X), 28, 0, 33, font24, LABEL_STYLE_BORDER);
    if(!widget)
        goto some_error;
    gui_widget_link(cutlen, widget);
    gui_label_init_private(widget, 128);
    gui_label_set_text(widget, "实际长: 8623489, 误差: +455");

    /* 本日数据 */
    init_today_data_screen(main_form);

    /* 修改字体 */
    set_status_font_big1(config.font_big1);
    set_status_font_big2(config.font_big2);

    RegisterKeyHook(hook_font); /* 测试字体 */

    return;

some_error:
    sys_print("init_produce_screen(): failed to create widgets or window!\n");
    ExitApplication();
}

void init_today_data_screen(gui_widget * parent)
{
    gui_widget * form, * widget;

    form = gui_create_widget(GUI_WIDGET_FORM,
                             X1,
                             Y_TODAY_DATA,
                             W_TODAY_DATA,
                             H_TODAY_DATA,
                             TODAY_DATA_CL,
                             TODAY_DATA_BK,
                             0, FORM_STYLE_CLIENT_BDR);
    if (!form)
        goto some_error;
    main_today_data_screen = form;
    gui_hide_widget(form); /* 默认是不显示的 */
    gui_widget_link(parent, form);
    gui_form_init_private(form, 16);
    gui_form_set_caption(form, "today_data");

    widget = gui_create_widget(GUI_WIDGET_LABEL, 6, (H_TODAY_DATA-28)/2, (W_TODAY_DATA-12), 28, TODAY_DATA_CL, TODAY_DATA_BK, font24, LABEL_STYLE_BORDER);
    if(!widget)
        goto some_error;
    gui_widget_link(form, widget);
    gui_label_init_private(widget, 128);
    gui_label_set_text(widget, "生产总米数: 20000000   总坏纸米数: 200000(33%%)   平均车速: 127");

    return;

some_error:
    sys_print("init_today_data_screen(): failed to create widgets or window!\n");
    ExitApplication();
}

void __show_today_data_screen(int show)
{
    if (show) {
        gui_set_widget_dimension(main_status_screen, WIDTH, HEIGHT + MAIN_ORDER_ITEM_HEIGHT);
        gui_show_widget(main_today_data_screen);
    } else {
        gui_set_widget_dimension(main_status_screen, WIDTH, HEIGHT);
        gui_hide_widget(main_today_data_screen);
    }
}


/*-----------------------------------------------------------------------------------------
 *          
 *      生产画面(小版)
 * 
**---------------------------------------------------------------------------------------*/
void init_produce_screen_small(gui_widget * parent)
{
    parent = parent;
/*
    gui_widget * widget;

    widget = gui_create_widget(GUI_WIDGET_EDIT, 6, 25, 500, 80, 0, 0, font24, EDIT_ALIGN_CENTER);
    if(!widget)
        goto some_error;
    gui_widget_link(parent, widget);
    gui_edit_init_private(widget, 256);
    gui_edit_set_text(widget, "测试文本");

    return;

some_error:
    sys_print("init_produce_screen_small(): failed to create widgets or window!\n");
    ExitApplication();
*/
}

void set_status_font_big1(int font_index)
{
    font_index = get_font_big1(font_index);

    gui_set_widget_font(l__prod_count, font_index);
    gui_set_widget_font(l__remain_count, font_index);
}

void set_status_font_big2(int font_index)
{
    font_index = get_font_big2(font_index);

    gui_set_widget_font(l__bad_count, font_index);
    gui_set_widget_font(l__real_speed, font_index);
    gui_set_widget_font(l__set_len, font_index);
}


/*-----------------------------------------------------------------------------------------
 *          
 *      生产画面的刷新例程
 * 
**---------------------------------------------------------------------------------------*/



/*=========================================================================================
 * 
 * 本文件结束: cut/status.c
 * 
**=======================================================================================*/


