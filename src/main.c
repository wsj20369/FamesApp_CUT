/******************************************************************************************
 * 文件:    cut/main.c
 *
 * 说明:    CUT主界面
 *
 * 作者:    Jun
 *
 * 时间:    2012-11-27
******************************************************************************************/
#define  CUT_MAIN_C
#include <includes.h>
#include "common.h"

/*-----------------------------------------------------------------------------------------
 *          
 *      主画面中的控件及其它定义
 * 
**---------------------------------------------------------------------------------------*/
#define        main_buttons_max  6       /* 功能按钮个数         */

gui_window_t * main_window = NULL;
gui_widget   * main_screen = NULL;       /* 主界面的主控件       */
gui_widget   * main_status_bar;          /* 主界面状态条         */
gui_widget   * main_cpu_status;          /* CPU使用率的显示      */
gui_widget   * main_buttons[main_buttons_max]={NULL,}; /* 8个命令按钮 */

gui_widget   * company_picture;          /* 一张图片             */

gui_widget   * main_order_view;          /* 订单视图控件         */

gui_widget   * main_order_query_view;    /* 订单查询控件         */
gui_window_t * main_order_query_window;

gui_widget   * dashedline_in_main;       /* 一条横虚线           */

gui_widget   * current_date_time;        /* 时间与日期           */

gui_widget   * system_running_anim;      /* 系统运行标记         */

gui_widget   * driver_name_label;        /* 驱动器名称           */

gui_window_t * system_about_window;      /* 系统关于窗口         */


extern BMPINFO icon, company_bmp;        /* 图标与图片           */



/*-----------------------------------------------------------------------------------------
 *          
 *      辅助声明
 * 
**---------------------------------------------------------------------------------------*/
void init_order_view_private(void);
void init_order_query_private(void);
void set_buttons_caption_to_default(void);
void set_buttons_caption_to_alt(void);
void set_buttons_caption_to_ctrl(void);
void set_buttons_caption_to_shift(void);
void set_buttons_caption_to_orderquery(void);

extern button_cmd_t cmd_query[];

/*-----------------------------------------------------------------------------------------
 *          
 *      主画面颜色
 * 
**---------------------------------------------------------------------------------------*/
static char  ___color_order_title[4] = "251";    /* 主画面订单区域 */
static char  ___bkcolor_order_title[4] = "37";
static char  ___color_order_data[4] = "70";
static char  ___bkcolor_order_data[4] = "236";

static char  ___color_order_query[4] = "189";    /* 订单查询区域 */
static char  ___bkcolor_order_query[4] = "70";

static char  ___color_buttons[4] = "71";         /* 主画面按钮颜色 */
static char  ___bkcolor_buttons[4] = "2";

       char  ___bkcolor_other_dialog[4] = "236";

static int   BUTTON_COLOR    =  COLOR_WHITE;
static int   BUTTON_BKCOLOR  =  2;


/*-----------------------------------------------------------------------------------------
 *          
 *      主画面的定义(或初始化), 界面元素从下到上安排将会显示的更快
 * 
**---------------------------------------------------------------------------------------*/
void init_main_screen(void)
{
    int i, x, y, width, height;
    char ___s[128];

    load_string(___color_order_title,    4, "color_order_title");
    load_string(___bkcolor_order_title,  4, "bkcolor_order_title");
    load_string(___color_order_data,     4, "color_order_data");
    load_string(___bkcolor_order_data,   4, "bkcolor_order_data");

    load_string(___color_order_query,    4, "color_order_query");
    load_string(___bkcolor_order_query,  4, "bkcolor_order_query");
    
    load_string(___color_buttons,        4, "color_buttons");
    load_string(___bkcolor_buttons,      4, "bkcolor_buttons");

    load_string(___bkcolor_other_dialog, 4, "bkcolor_other_dialog");

    BUTTON_COLOR = atoi(___color_buttons);
    BUTTON_BKCOLOR = atoi(___bkcolor_buttons);    

    /* 主界面       */
    main_screen = gui_create_widget(GUI_WIDGET_FORM, 1, 1, 1021, 765, 0, 0, font16, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE);
    if(!main_screen)
        goto some_error;
    main_window = gui_create_window(main_screen);
    gui_widget_link(NULL, main_screen);      /* 设置主界面背景 */
    gui_form_init_private(main_screen, 128);
    gui_form_set_icon(main_screen, &icon);
    STRCPY(___s, "Application Demonstration");
    load_string(___s, sizeof(___s), "main_title");
    gui_form_set_caption(main_screen, ___s);

    /* 时间与日期   */
    current_date_time = gui_create_widget(GUI_WIDGET_LABEL, 838, 9, 172, 16,  COLOR_WHITE, WIDGET_COLOR_TITLE_XP, 1, LABEL_STYLE_TRANSPARENT);
    if(!current_date_time)
        goto some_error;
    gui_widget_link(main_screen, current_date_time);
    gui_label_init_private(current_date_time, 32);
    gui_label_set_text(current_date_time, "");

    init_produce_screen(main_screen);

    /* 一条虚线     */
    dashedline_in_main = gui_create_widget(GUI_WIDGET_DASHEDLINE, 10, 385, 998, 5, 73, 0, 0, DASHED_STYLE_CENTER);
    if(!dashedline_in_main)
        goto some_error;
    gui_widget_link(main_screen, dashedline_in_main);
    gui_dashedline_init_private(dashedline_in_main);
    gui_dashedline_set_param(dashedline_in_main, 970, 3, 0xE0); /* 0xE4 is good */

    /* 订单区域     */
    main_order_view = gui_create_widget(GUI_WIDGET_VIEW, 11, 392, 996, 256, atoi(___color_order_title), atoi(___bkcolor_order_title),
                                        font24, VIEW_STYLE_FIELDS_TITLE|VIEW_STYLE_MARK_BAR);
    if(!main_order_view)
        goto some_error;
    init_order_view_private();
    gui_view_set_dashed(main_order_view, 127, 0);
    gui_widget_link(main_screen, main_order_view);

    /* 功能按钮     */
    x = 12; y = 655; width = 160; height = 64;
    for(i=0; i<main_buttons_max; i++){
        if(i == 2 || i == 4)
            x += 12;
        main_buttons[i] = gui_create_widget(GUI_WIDGET_BUTTON, x, y, width, height, BUTTON_COLOR, BUTTON_BKCOLOR, font24, BUTTON_STYLE_CLIENT_BDR|BUTTON_STYLE_MODAL_FRAME);
        if(!main_buttons[i])
            goto some_error;
        gui_widget_link(main_screen, main_buttons[i]);
        gui_button_init_private(main_buttons[i], 32);
        gui_button_set_caption(main_buttons[i], "");
        x += (width+2);
    }
    
    /* 工具条       */
    main_status_bar = gui_create_widget(GUI_WIDGET_LABEL, 7, 725, 581, 30, 0, 0, font16, LABEL_STYLE_CLIENT_BDR);
    if(!main_status_bar)
        goto some_error;
    gui_widget_link(main_screen, main_status_bar);
    gui_label_init_private(main_status_bar, 100);
    STRCPY(___s, "Welcome to FamesOS!");
    load_string(___s, sizeof(___s), "first_note");
    gui_label_set_text(main_status_bar, ___s);

    /* 驱动器名称   */
    driver_name_label = gui_create_widget(GUI_WIDGET_LABEL, 752, 725, 100, 30, 0, 0, font16, LABEL_STYLE_CLIENT_BDR|LABEL_ALIGN_CENTER);
    if(!driver_name_label)
        goto some_error;
    gui_widget_link(main_screen, driver_name_label);
    gui_label_init_private(driver_name_label, 32);
    sprintf(___s, "%s", get_driver_types_name(config.driver_type));
    gui_label_set_text(driver_name_label, ___s);

    /* CUT运行状态  */
    system_running_anim = gui_create_widget(GUI_WIDGET_LABEL, 852, 725, 28, 30, 0, 0, font16, LABEL_STYLE_CLIENT_BDR|LABEL_ALIGN_CENTER);
    if(!system_running_anim)
        goto some_error;
    gui_widget_link(main_screen, system_running_anim);
    gui_label_init_private(system_running_anim, 8);
    gui_label_set_text(system_running_anim, "");

    /* CPU使用率    */
    main_cpu_status = gui_create_widget(GUI_WIDGET_LABEL, 880, 725, 132, 30, 0, 0, font16, LABEL_STYLE_CLIENT_BDR|LABEL_ALIGN_CENTER);
    if(!main_cpu_status)
        goto some_error;
    gui_widget_link(main_screen, main_cpu_status);
    gui_label_init_private(main_cpu_status, 32);
    gui_label_set_text(main_cpu_status, "");

    #if 0
    /* 公司标志     */
    company_picture = gui_create_widget(GUI_WIDGET_PICTURE, 9, 34, 1000, 72, 0, 0, 0, PICTURE_STYLE_TRANSPARENT|PICTURE_STYLE_NO_BORDER);
    if(!company_picture)
        goto some_error;
    gui_widget_link(main_screen, company_picture);
    gui_picture_init_private(company_picture);
    gui_picture_set_picture(company_picture, &company_bmp);
    #endif

    show_today_data_screen(config.today_data_show);

    return;

some_error:
    sys_print("init_main_screen(): failed to create widgets for main screen!\n");
    ExitApplication();
}


void init_order_query_window(void)
{
    gui_widget * form;
    int x, y, w, h;

    x = 6;
    y = 32;
    w = 1009;
    h = 620;

    form = gui_create_widget(GUI_WIDGET_FORM, x, y, w, h, 0, 0, 0, 0);
    if(!form)
        goto some_error;
    gui_widget_link(NULL, form);
    gui_form_init_private(form, 32);
    gui_form_set_caption(form, "");

    init_produce_screen_small(form);

    main_order_query_view = gui_create_widget(GUI_WIDGET_VIEW, (12-x), (40-y), 997, 605, atoi(___color_order_title), atoi(___bkcolor_order_title),
                                              font24, VIEW_STYLE_FIELDS_TITLE|VIEW_STYLE_MARK_BAR|VIEW_STYLE_STATISTIC_BAR);
    if(!main_order_query_view)
        goto some_error;
    init_order_query_private();
    gui_view_set_dashed(main_order_query_view, 127, 0);
    gui_widget_link(form, main_order_query_view);

    main_order_query_window = gui_create_window(form);
    if(!main_order_query_window)
        goto some_error;

    return;

some_error:
    sys_print("init_order_query_window(): failed to create widgets for main screen!\n");
    ExitApplication();
}

void init_about_screen(void)
{
    gui_widget * widget;
    gui_widget * sys_monitor;

    widget = gui_create_widget(GUI_WIDGET_FORM, 0, 0, 1024, 768, 3, CLRSCR_COLOR2, 0, FORM_STYLE_NO_BORDER);  
    if(!widget)
        goto some_error;
    gui_form_init_private(widget, 32);
    gui_form_set_caption(widget, "About System");

    sys_monitor = gui_create_widget(GUI_WIDGET_SYS_MNTR, 100, 86, 1, 1, COLOR_YELLOW, CLRSCR_COLOR, 0, SYS_MNTR_STYLE_NO_BORDER);
    if(!sys_monitor)
        goto some_error;
    gui_sys_mntr_init_private(sys_monitor);
    gui_widget_link(widget, sys_monitor);

    system_about_window = gui_create_window(widget);
    if(!system_about_window)
        goto some_error;

    return;

some_error:
    sys_print("init_about_screen(): failed to create widgets for main screen!\n");
    ExitApplication();
}

/* 本日生产数据的显示 */
void show_today_data_screen(int show)
{
    __show_today_data_screen(show);

    if (show) {
        gui_set_widget_location(dashedline_in_main, 10, 427); /* XXX: 这些数值须与上面的一致 */
        gui_set_widget_location(main_order_view, 11, 434);
        gui_set_widget_dimension(main_order_view, 996, 214);

    } else {
        gui_set_widget_location(dashedline_in_main, 10, 385);
        gui_set_widget_location(main_order_view, 11, 392);
        gui_set_widget_dimension(main_order_view, 996, 256);
    }

    gui_refresh_widget(main_screen);    /* 刷新主画面 */
}


/*-----------------------------------------------------------------------------------------
 *          
 *      几个辅助函数
 * 
**---------------------------------------------------------------------------------------*/
void set_buttons_caption(INT08S *captions[])
{
    int i;
    
    FamesAssert(captions);
    if(!captions)
        return;

    for(i=0; i<main_buttons_max; i++){
        if(captions[i])
            gui_button_set_caption(main_buttons[i], captions[i]);
    }
}

void active_main_screen(void)
{
    gui_show_window(main_window);
}

void message(INT08S * s)
{
    if(!main_status_bar || !s)
        return;
    gui_label_set_text(main_status_bar, s);
}

static void ___alert(INT08S * zh_str, INT08S * en_str)
{
    INT08S * s;

    s = pick_string(zh_str, en_str);
    
    if(!main_status_bar || !s)
        return;
    gui_label_set_text(main_status_bar, s);
    gui_set_widget_bkcolor(main_status_bar, COLOR_WARNING);
    waitkey(1000L);
    gui_set_widget_bkcolor(main_status_bar, 0);
}

/*-----------------------------------------------------------------------------------------
 *          
 *      主画面之按键例程的定义
 * 
**---------------------------------------------------------------------------------------*/
void cmd_main_dummy(void * data)
{
    data = data;
    message(pick_string(" *** 该版本中无此功能 ***", "Not Supported"));
}

static BOOL check_password(INT32U password)
{
    BOOL retval;
    
    retval = check_password_dialog(264, 190, password);

    return retval;
}

static BOOL check_confirm(char * caption, char * text)
{
    BOOL retval;

    retval = check_yes_no_dialog(264, 296, caption, text);

    return retval;
}

/* 计数复位, 目前只在启动时执行一次, 将来可将其作为一个按键功能由用户随时执行(须考虑其安全性及合理性) */
void cmd_reset_count(void * data)
{
    BOOL retval;

    data = data;

    retval = check_yes_no_dialog(264, 296, pick_string("计数复位", "Reset Counter"),
                                           pick_string("要 \"清除\" 当前所有的计数吗?","Clear all the counter data?"));
    if (retval) {
        /* TODO: 这里需要加入计数复位代码... */
    }
}

void cmd_input_order(void * data)
{
    data = data;
    order_edit();
}

void cmd_query_order(void * data)
{
    void ____set_buttoncmds_comment_query(void);

    data = data;
    gui_show_window(main_order_query_window);
    set_buttons_caption_to_orderquery();
    ____set_buttoncmds_comment_query();

    do_buttoncmds(cmd_query, main_buttons, main_buttons_max, 1,
        BUTTON_STYLE_CLIENT_BDR | BUTTON_STYLE_MODAL_FRAME, 
        BUTTON_STYLE_CLIENT_BDR | BUTTON_STYLE_MODAL_FRAME, putkey_to_add_dec);

    set_buttons_caption_to_default();
    gui_hide_window(main_order_query_window);
}
void cmd_change_order(void * data)
{
}
void cmd_stop_change(void * data)
{
}
void cmd_stop_count(void * data)
{
}
void cmd_additional(void * data)
{
}

void cmd_move_order(void * data)
{
    int  view_flag = (int)data;
    gui_widget * view = NULL;

    switch (view_flag) {
        case 1:
            view = main_order_view;
            break;
        case 2:
            view = main_order_query_view;
            break;
        default:
            ___alert("Err: cmd_move_order(): 参数错误!",
                     "Err: cmd_move_order(): Invalid data!");
            return;
    }

    if(get_order_nr() <= 0){
        ___alert("当前没有订单, 无法调单!", "No Order to move!");
        return;
    }
    move_order(view);
}

void cmd_delete_order(void * data)
{
    BOOL retval;
    int  view_flag = (int)data;
    gui_widget * view = NULL;

    switch (view_flag) {
        case 1:
            view = main_order_view;
            break;
        case 2:
            view = main_order_query_view;
            break;
        default:
            ___alert("Err: cmd_delete_order(): 参数错误!",
                     "Err: cmd_delete_order(): Invalid data!");
            return;
    }

    if(get_order_nr() <= 0){
        ___alert("没有订单了, 无法删除!", "No Order to delete!");
        return;
    }

    retval = check_yes_no_dialog(264, 296, pick_string("删除", "Delete"),
                                           pick_string("要 \"删除\" 这笔订单吗?","Delete this order?"));
    if (retval)
        DeleteOrder(gui_view_get_selected(view));
}


void cmd_clear_order(void * data)
{
    BOOL retval;

    data = data;
    if(get_order_nr() <= 0){
        ___alert("没有订单了, 不需要清单!", "No Order to clear!");
        return;
    }
    retval = check_yes_no_dialog(264, 296, pick_string("清单", "Clear"),
                                           pick_string("要 \"删除\" 所有订单吗?", "Clear All the orders?"));
    if (retval)
        ZapOrder();
}

void cmd_pack_workno(void * data)
{
    data = data;
    if(get_order_nr() <= 0){
        ___alert("当前没有订单, 无法重排!", "No Order to Resort");
        return;
    }
    if(check_confirm(pick_string("重排", "Resort"), 
                     pick_string("要 \"重排\" 工号吗?(Y/N)", "Confirm to Resort?(Y/N)"))){
        if(!PackWorkNo(10))
            ___alert("工号重排失败!", "Failed to Resort!");
    }
}

static void setup_super_param(void * data)
{
    data = data;

    if(check_password(PASSWORD_SUPER)){
        do_setup_super_param();
    }
    set_buttons_caption_to_default();
}

static void setup_generic_param(void * data)
{
    data = data;

    if(check_password(PASSWORD_PARAM)){
        do_setup_generic_param();
    }
    set_buttons_caption_to_default();
}

static void setup_spdlmt_tbl(void * data)
{
    data = data;

    if(check_password(PASSWORD_PARAM)){
        do_setup_spdlmt_tbl();
    }
    set_buttons_caption_to_default();
}

static void setup_driver_param(void * data)
{
    data = data;

    if(check_password(PASSWORD_DRIVER)){
        do_setup_driver_params();
    }
    set_buttons_caption_to_default();
}

static void cmd_common_order_browse(gui_widget * _view, void * _key)
{
    KEYCODE key;

    FamesAssert(_view != NULL);

    message(pick_string(
                "查看订单的命令有: Up, Down, PgUp, PgDn, Home, End", 
                "Cmds to View Orders: Up, Down, PgUp, PgDn, Home, End"));

    key = (KEYCODE)(INT32U)_key;
    switch(key){
        case UP:
            gui_view_move_up(_view);
            break;
        case DOWN:
            gui_view_move_down(_view);
            break;
        case PGUP:
            gui_view_page_up(_view);
            break;
        case PGDN:
            gui_view_page_down(_view);
            break;
        case HOME:
            gui_view_goto_top(_view);
            break;
        case END:
            gui_view_goto_bottom(_view);
            break;
        default:
            break;
    }
}

static void cmd_main_browse(void * data)
{
    cmd_common_order_browse(main_order_view, data);
}

static void cmd_query_browse(void * data)
{
    cmd_common_order_browse(main_order_query_view, data);
}

void cmd_main_ID(void * data)
{
    check_register_dialog(256, 180, (int)data);
    set_buttons_caption_to_default();
}

void cmd_main_VER(void * data)
{
    char str[64];

    data = data; /* suppress compiler warnings */

    sprintf(str, "%s: %s",
            pick_string("系统版本号", "System Version"),
            get_version_string());
    message(str);
}

void cmd_main_SYS(void * data)
{
    data = data;
    gui_show_window(system_about_window);
    while(NONE_KEY == waitkey(0L));
    clrkey();
    gui_hide_window(system_about_window);
    set_buttons_caption_to_default();
}

void cmd_main_PALETTE(void * data)
{
    extern gui_window_t * palette_window;
    data = data;
    gui_show_window(palette_window);
    while(NONE_KEY == waitkey(0L));
    clrkey();
    set_buttons_caption_to_default();
    gui_hide_window(palette_window);
}

void cmd_main_QUIT(void * data)
{
    data = data;

    if(check_confirm(pick_string(" >>> 退出 <<<", ">>> Quit <<<"), 
                     pick_string("要 \"退出\" 横切机系统吗?(Y/N)", "Confirm to \"Quit\" program?(Y/N)"))) {
        /* 退出时, 要做些什么, 比如: 关闭订单文件...
        do_quit_routines();
        */
        ExitApplication();
    }
    set_buttons_caption_to_default();
}

void cmd_main_ALT(void * data)
{
    data = data;
    set_buttons_caption_to_alt();
}

void cmd_main_CTRL(void * data)
{
    data = data;
    set_buttons_caption_to_ctrl();
}

void cmd_main_SHIFT(void * data)
{
    data = data;
    set_buttons_caption_to_shift();
}

/*-----------------------------------------------------------------------------------------
 *          
 *      主画面之按键的定义
 * 
**---------------------------------------------------------------------------------------*/
INT08S *main_default_functions_zh[] = {
        "[F1]输单", "[F2]查询",
        "[F5]停换", "[F6]停计",
        "[F7]换单", "[F9]追加", NULL,
};
INT08S *main_default_functions_en[] = {
        "[F1]Input", "[F2]Query",
        "[F5]StpChg", "[F6]StpCnt",
        "[F7]Change", "[F9]Additi", NULL,
};
INT08S *main_ctrl_functions_zh[] = {
        "", "", "", "", "[F10]重送", "[END]退出", NULL,
};
INT08S *main_ctrl_functions_en[] = {
        "", "", "", "", "[F10]ReSend", "[END]QUIT", NULL,
};
INT08S *main_alt_functions_zh[] = {
        "[F1]参数", "[F2]限速表", "[F3]驱动器", "[F5]高级", "", "[F8]系统", NULL,
};
INT08S *main_alt_functions_en[] = {
        "[F1]Setup", "[F2]SpdLmt", "[F3]Driver", "[F5]Super", "", "[F8]System", NULL,
};
INT08S *main_shift_functions_zh[] = {
        "", "", "", "[F6]版本", "[F7]ID", "[F8]系统", NULL,
};
INT08S *main_shift_functions_en[] = {
        "", "", "", "[F6]Ver", "[F7]ID", "[F8]System", NULL,
};

INT08S *order_query_functions_zh[] = {
        "[F3]删除", "[F4]调单",
        "[F5]清单", "[F6]重排",
        "[F9]备份", "[F10]回调", NULL,
};
INT08S *order_query_functions_en[] = {
        "[F3]Delete", "[F4]Move",
        "[F5]Clear",  "[F6]ReNumbr",
        "[F9]Backup", "[F10]Recall", NULL,
};

INT08S *order_query_alt_funcs_zh[] = {
        "", "", "", "", "", "", NULL,
};
INT08S *order_query_alt_funcs_en[] = {
        "", "", "", "", "", "", NULL,
};

INT08S ** main_default_functions = main_default_functions_zh;
INT08S ** main_ctrl_functions    = main_ctrl_functions_zh;
INT08S ** main_alt_functions     = main_alt_functions_zh;
INT08S ** main_shift_functions   = main_shift_functions_zh;
INT08S ** order_query_functions  = order_query_functions_zh;
INT08S ** order_query_alt_funcs  = order_query_alt_funcs_zh;

void set_buttons_caption_to_default(void)
{
    int i;

    message(pick_string("按下CTRL, ALT或SHIFT可查看更多命令",
                        "Press CTRL, ALT or SHIFT, there will be more functions"));

    for(i=0; i<main_buttons_max; i++){
        gui_set_widget_color(main_buttons[i], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[i], BUTTON_BKCOLOR);
    }
    if(tmp_config.stop_change){
        gui_set_widget_color(main_buttons[3], COLOR_WHITE);
        gui_set_widget_bkcolor(main_buttons[3], COLOR_RED);
    }
    if(tmp_config.stop_count){
        gui_set_widget_color(main_buttons[4], COLOR_WHITE);
        gui_set_widget_bkcolor(main_buttons[4], COLOR_RED);
    }

    set_buttons_caption(main_default_functions);
}

void set_buttons_caption_to_alt(void)
{
    int i;
    for(i=0; i<main_buttons_max; i++){
        gui_set_widget_color(main_buttons[i], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[i], BUTTON_BKCOLOR);
    }
    set_buttons_caption(main_alt_functions);
}

void set_buttons_caption_to_ctrl(void)
{
    int i;
    for(i=0; i<main_buttons_max; i++){
        gui_set_widget_color(main_buttons[i], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[i], BUTTON_BKCOLOR);
    }
    set_buttons_caption(main_ctrl_functions);
}

void set_buttons_caption_to_shift(void)
{
    int i;
    for(i=0; i<main_buttons_max; i++){
        gui_set_widget_color(main_buttons[i], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[i], BUTTON_BKCOLOR);
    }
    set_buttons_caption(main_shift_functions);
}

void set_buttons_caption_to_orderquery(void)
{
    int i;

    message(pick_string("订单查询", "Order Query"));

    for(i=0; i<main_buttons_max; i++){
        gui_set_widget_color(main_buttons[i], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[i], CLRSCR_COLOR);
    }

    set_buttons_caption(order_query_functions);
}

void cmd_main_NONE(void * data)
{
    data = data;
    set_buttons_caption_to_default();
}

static char __input_order_comment_zh[] = "输入一笔新的订单, 但如果工号已经存在, 那就成为修改了!";
static char __input_order_comment_en[] = "Input a Order and Insert it or Modify it if S/N already exist!";

button_cmd_t cmd_main[] = {
    #define __(x) (void*)(INT32U)(x)
   /* 主功能键     */
    { F1,          0, cmd_input_order,      NULL,      __input_order_comment_zh },
    { F2,          0, cmd_query_order,      NULL,      "查询所有订单, 并对其处理" },
    { F5,          0, cmd_stop_change,      NULL,      "停止换单" },
    { F6,          0, cmd_stop_count,       NULL,      "停止计数, 计数将计入不良品" },
    { F7,          0, cmd_change_order,     NULL,      "强制换单到下一笔" },
    { F9,          0, cmd_additional,       NULL,      "追加一笔临时订单" },
    { ENTER,       0, cmd_input_order,      NULL,      __input_order_comment_zh },
   /* 订单操作     */
    { F3,          0, cmd_delete_order,     __(1),     "删除当前订单!" },
    { F4,          0, cmd_move_order,       __(1),     "调整订单的生产顺序" },
   /* NONE_KEY     */
    { NONE_KEY,    0, cmd_main_NONE,        NULL,      "" },
   /* CTRL-F#      */
    { CTRL,        0, cmd_main_CTRL,        NULL,      "" },
    { CTRL_END,    0, cmd_main_QUIT   ,     NULL,      "退出横切机程序" },
   /* ALT-F#       */
    { ALT,         0, cmd_main_ALT,         NULL,      "" },
    { ALT_F1,      0, setup_generic_param,  NULL,      "横切机普通参数的设置" },
    { ALT_F2,      0, setup_spdlmt_tbl,     NULL,      "限速表设定" },
    { ALT_F3,      0, setup_driver_param,   NULL,      "驱动器参数的设置" },
    { ALT_F5,      0, setup_super_param,    NULL,      "高级参数的设置, 只有管理员才可修改" },
    { ALT_F8,      0, cmd_main_SYS,         NULL,      "System Monitor" },
   /* SHIFT-F#     */
    { SHIFT,       0, cmd_main_SHIFT,       NULL,      "" },
    { SHIFT_F6,    0, cmd_main_VER,         NULL,      "" },
    { SHIFT_F7,    0, cmd_main_ID,          __(0),     "System ID/SN" },
    { SHIFT_F8,    0, cmd_main_SYS,         NULL,      "System Monitor" },
   /* 注册退出键   */
    { RS_EXIT_KEY, 0, cmd_main_ID,          __(1),     "System ID/SN" },
   /* 订单浏览     */
    { UP,          0, cmd_main_browse,      __(UP),    "" },
    { DOWN,        0, cmd_main_browse,      __(DOWN),  "" },
    { PGUP,        0, cmd_main_browse,      __(PGUP),  "" },
    { PGDN,        0, cmd_main_browse,      __(PGDN),  "" },
    { HOME,        0, cmd_main_browse,      __(HOME),  "" },
    { END,         0, cmd_main_browse,      __(END),   "" },
   /* KEYCMD的功能 */
    { TAB,         0, NULL,                 NULL,      "" },
    { LEFT,        0, NULL,                 NULL,      "" },
    { RIGHT,       0, NULL,                 NULL,      "" },
   /* 测试用       */
    { SHIFT|CTRL,  0, cmd_main_SYS,         NULL,      "" },
    { 0x1900,      0, cmd_main_PALETTE,     NULL,      "ALT-P, Display System Palette" },
    { ESC,         0, NULL,                 NULL,      "提示: 如果您累了, 就要休息休息! 祝您工作顺利, 笑口常开!" },
   /* 结束标志     */
    { NONEKEY, },
    #undef __
};

static void ____set_buttoncmds_comment(void)
{
    /* 设置英文版本的注解字符串 */
    #define ____do_set(key, en_str) \
                buttoncmd_set_comment(cmd_main, key, pick_string(NULL, en_str))

    ____do_set(F1,     __input_order_comment_en);
    ____do_set(F2,     "Query Orders & Process");
    ____do_set(F5,     "Change to Next order");
    ____do_set(F6,     "Stop Auto Change-Order");
    ____do_set(F7,     "Stop Count");
    ____do_set(F9,     "Apply a additional temporary order");
    ____do_set(ENTER,  __input_order_comment_en);

    ____do_set(CTRL_END, "Exit Program!");

    ____do_set(ALT_F1, "General Parameters Setup");
    ____do_set(ALT_F2, "Speed Limit Table Setup");
    ____do_set(ALT_F3, "Driver specific Parameters Setup");
    ____do_set(ALT_F5, "Super Parameters Setup");

    ____do_set(ESC,    "Good luck!");

    ____do_set(F3,     "Delete the selected order");
    ____do_set(F4,     "Resort order");

    #undef ____do_set
}

static void cmd_query_NONE(void * data)
{
    data = data;
    set_buttons_caption(order_query_functions);
}

static void cmd_query_ALT(void * data)
{
    data = data;
    set_buttons_caption(order_query_alt_funcs);
}

button_cmd_t cmd_query[] = {
    #define __(x) (void*)(INT32U)(x)
   /* 主功能键     */
    { F3,          0, cmd_delete_order,     __(2),     "删除当前订单!" },
    { F4,          0, cmd_move_order,       __(2),     "调整订单的生产顺序" },
    { F5,          0, cmd_clear_order,      NULL,      "清除所有订单" },
    { F6,          0, cmd_pack_workno,      NULL,      "重新整理订单工号" },
    { F9,          0, cmd_change_order,     NULL,      "对所有订单做一个备份" },
    { F10,         0, cmd_additional,       NULL,      "把订单备份加到订单表中" },
   /* 状态键       */
    #if 0
    { NONE_KEY,    0, cmd_query_NONE,       NULL,      "" },
    { ALT,         0, cmd_query_ALT,        NULL,      "" },
    #endif
   /* 输单(修改)   */
    { F1,          0, cmd_input_order,      NULL,      __input_order_comment_zh },
    { ENTER,       0, cmd_input_order,      NULL,      __input_order_comment_zh },
   /* 订单浏览     */
    { UP,          0, cmd_query_browse,     __(UP),    "" },
    { DOWN,        0, cmd_query_browse,     __(DOWN),  "" },
    { PGUP,        0, cmd_query_browse,     __(PGUP),  "" },
    { PGDN,        0, cmd_query_browse,     __(PGDN),  "" },
    { HOME,        0, cmd_query_browse,     __(HOME),  "" },
    { END,         0, cmd_query_browse,     __(END),   "" },
   /* KEYCMD的功能 */
    { TAB,         0, NULL,                 NULL,      "" },
    { LEFT,        0, NULL,                 NULL,      "" },
    { RIGHT,       0, NULL,                 NULL,      "" },
    { ENTER,       0, NULL,                 NULL,      "" },
   /* 退出键       */
    { ESC,         1, NULL            ,     NULL,      "" },
   /* 结束标志     */
    { NONEKEY, },
    #undef __
};

static void ____set_buttoncmds_comment_query(void)
{
    /* 设置英文版本的注解字符串 */
    #define ____do_set(key, en_str) \
                buttoncmd_set_comment(cmd_query, key, pick_string(NULL, en_str))

    ____do_set(F3,     "Delete the selected order");
    ____do_set(F4,     "Resort order");
    ____do_set(F5,     "Clear all the orders");
    ____do_set(F6,     "Repack work numbers");
    ____do_set(F9,     "Order Backup");
    ____do_set(F10,    "Append the Backup to Orders");

    ____do_set(F1,     __input_order_comment_en);
    ____do_set(ENTER,  __input_order_comment_en);

    #undef ____do_set
}

/*-----------------------------------------------------------------------------------------
 *          
 *      定单显示区域(main_order_view)的相关定义
 * 
**---------------------------------------------------------------------------------------*/
enum order_view_fields_id {
    __id_sn,
    __id_wkno,
    __id_odrno,
    __id_setlen,
    __id_count,
    __id_unit,
    __id_spdlmt, /* 限速 */
    __id_expect, /* 预期完成时间 */
    __id_dummy,  /* 空的, 不显示实际内容 */
};

static view_fields_t order_view_fields[] =
{
#define ____style        0
#define ____draw_style   DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER
    #if 0
    { "订单序号",   __id_sn,     9,  10,  ____style,  ____draw_style,  "", },
    #endif
    { "工号",       __id_wkno,   4,  6,   ____style,  ____draw_style,  "", },
    { "订单号",     __id_odrno,  8,  12,  ____style,  ____draw_style,  "", },
    { "设定长",     __id_setlen, 6,  15,  ____style,  ____draw_style,  "", },
    { "张数",       __id_count,  6,  15,  ____style,  ____draw_style,  "", },
    { "捆",         __id_unit,   3,  6,   ____style,  ____draw_style,  "", },
    { "限速",       __id_spdlmt, 4,  8,   ____style,  ____draw_style,  "", },
    { "预期完成",   __id_expect, 9,  14,  ____style,  ____draw_style,  "", },
    { NULL, }
#undef ____style
#undef ____draw_style
};

struct __order_old_buf { /* 各个字符串的长度不可超过上面结构中的bytes_for_width字段 */
    char workno[6];
    char orderno[12];
    char cutlen[12];
    char number[12];
    char unit[6];
    char spdlimit[8];
    char finishtime[14];
};

static int order_get_max_index(void)
{
    return (get_order_nr()-1);
}

static void show_order_record(int index, int row,
                              int x, int y, int width_zoom, int height, int x_border,
                              COLOR color, COLOR bkcolor, int font,
                              COLOR marker_color, COLOR marker_bkcolor,
                              void *old, int  *fields_width, INT16U option)
{
    struct __order_old_buf * record_buf_old;
    order_struct __order;
    int i, j, to_left, width;
    char ___s[128];
    COLOR special_color = COLOR_YELLOW;
        
    FamesAssert(old);

    if(!old)
        return;

    record_buf_old = (struct __order_old_buf *)old;
    i = row;
    to_left = width_zoom;
    
    if(index <= order_get_max_index()){
        if(!GetOrderForView(&__order, index)) {
            j = 0; /* 在工号上显示<Err> */
            draw_font_for_widget(x+3, y, (fields_width[j]-to_left)-6, height, "<Err>", 
                                 record_buf_old->workno, marker_color, marker_bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
            return fail;
        }
        /* 工号 */
        j = 0;
        sprintf(___s, "%d", __order.WORKNO);
        draw_font_for_widget(x+3, y, (fields_width[j]-to_left)-6, height, ___s, 
                             record_buf_old->workno, marker_color, marker_bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* 订单号 */
        j++;
        __order.ORDERNO[8] = 0;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, __order.ORDERNO, 
                             record_buf_old->orderno, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* 设定长 */
        j++;
        sprintf(___s, "%u", __order.CUT_LENGTH);
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, ___s, 
                             record_buf_old->cutlen, special_color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* 张数 */
        j++;
        sprintf(___s, "%ld", __order.NUMBER);
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, ___s, 
                             record_buf_old->number, special_color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* 捆单位 */
        j++;
        sprintf(___s, "%d", __order.UNIT);
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, ___s, 
                             record_buf_old->unit, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* 限速 */
        j++;
        sprintf(___s, "%d", get_speed_limit(__order.CUT_LENGTH));
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, ___s, 
                             record_buf_old->spdlimit, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* 预期完成时间 */
        j++;
        sprintf(___s, "%s", "08:08:08");
        width = fields_width[j]-to_left;
        if (x + width > x_border)
            width = x_border - x;
        draw_font_for_widget(x, y, width, height, ___s, 
                             record_buf_old->finishtime, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
    } else {
        /* 工号 */
        j = 0;
        draw_font_for_widget(x+3, y, (fields_width[j]-to_left)-6, height, "", 
                             record_buf_old->workno, marker_color, marker_bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* 订单号 */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->orderno, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* 设定长 */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->cutlen, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* 张数 */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->number, color, bkcolor, font, DRAW_OPT_FIL_BG);
        x += fields_width[j];
        /* 捆单位 */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->unit, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* 限速 */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->spdlimit, color, bkcolor, font, DRAW_OPT_FIL_BG);
        x += fields_width[j];
        /* 预期完成时间 */
        j++;
        width = fields_width[j]-to_left;
        if (x + width > x_border)
            width = x_border - x;
        draw_font_for_widget(x, y, width, height, "", 
                             record_buf_old->finishtime, color, bkcolor, font, DRAW_OPT_FIL_BG);
        x += fields_width[j];
    }
}

static void show_statistics( int index,
                             int x, int y, int width, int height,
                             int color, int bkcolor, int font,
                             INT08S *comment, INT08S *old, INT16U option)
{
    char ___s[96];

    comment = comment;
    option  = option;
    
    sprintf(___s,
            pick_string(" 订单总数: %d 笔, 第 %d 笔", 
                        " Orders Nr: %d, Order #%d"),
            get_order_nr(), index+1);

    draw_font_for_widget(x, y, width, height, ___s, old, color, bkcolor, font, DRAW_OPT_FIL_BG);
}

static BOOL order_view_get_data(int index, int field_id, char *buf, int buf_len, INT16U option)
{
    order_struct __order;
    BOOL retval;
    char ___s[256];

    FamesAssert(buf);
    if(!buf)
        return fail;

    option = option;

    if(!GetOrderForView(&__order, index))
        return fail;
        
    if(index <= order_get_max_index()){
        retval = ok;
        switch(field_id){
            case __id_sn:
                if (index == 0) {
                    sprintf(___s, pick_string("当前订单", "Current"));
                } else {
                    sprintf(___s, pick_string("第%3d笔", "Order#%d"), index+1);
                }
                break;
            case __id_wkno:
                sprintf(___s, "%d", __order.WORKNO);
                break;
            case __id_odrno:
                sprintf(___s, "%s", __order.ORDERNO);
                break;
            case __id_setlen:
                sprintf(___s, "%u", __order.CUT_LENGTH);
                break;
            case __id_count:
                sprintf(___s, "%ld", __order.NUMBER);
                break;
            case __id_unit:
                sprintf(___s, "%d", __order.UNIT);
                break;
            case __id_spdlmt:
                sprintf(___s, "%d", get_speed_limit(__order.CUT_LENGTH));
                break;
            case __id_expect:
                sprintf(___s, "%s", "08:08:08");
                break;
            case __id_dummy:
                sprintf(___s, "");
                break;
            default:
                retval = fail;
                break;
        }
    } else {
        retval = fail;
    }

    if(retval){
        if(buf_len >= 256)
            buf_len = 255;
        ___s[buf_len] = 0;
        STRCPY(buf, ___s);
    }

    return retval;
}

static BOOL order_view_is_writable(int index, int field_id, INT16U option)
{
    switch (field_id) {
        case __id_odrno:
        case __id_setlen:
        case __id_count:
        case __id_unit:
            return YES;
        default:
            return NO;
    }
}

static BOOL order_view_set_data(int index, int field_id, char *buf, int buf_len, KEYCODE key, INT16U option)
{
    /*
     * 本来要实现在订单查询画面中, 直接在VIEW下编辑订单的功能的,
     * 不过, 发现不太好实现, 因为order_view_get_data()是从订单数据库中
     * 直接取得的数据, 用户在输入新的数据时会自动被order_view_get_data()
     * 所覆盖, 当然, 使用一些技巧还是可以做出来的, 不过感觉后来这个功能
     * 其实也没有多少实用价值, 所以还是不要做它了.
     *
     * 此函数保留为空, VIEW下直接编辑订单的功能没有实现! 2013-03-17
    */
    return ok;
}

static void notifier_on_changed(int index, int prev_index, INT16U option)
{
    #if 0
    order_struct __order;

    option = option;
    prev_index = prev_index;

    if(index < 0){
        index = -1;
    } else {
        if(!GetOrderForView(&__order, index))
            index = -1;
    }

    if(index == -1){
        /* 无订单时 */
    } else {
        /* 有订单时 */
    }
    #endif
}

void init_order_view_private(void)
{
    int i;
    /*
    printf("height_per_row = %4d\r", height_per_row);
    */
    i = 0;
    order_view_fields[i++].caption = pick_string("工号",     "WorkNo");
    order_view_fields[i++].caption = pick_string("订单号",   "OrderNo");
    order_view_fields[i++].caption = pick_string("设定长",   "SetLength");
    order_view_fields[i++].caption = pick_string("张数",     "Count");
    order_view_fields[i++].caption = pick_string("捆",       "UNIT");
    order_view_fields[i++].caption = pick_string("限速",     "SpdLmt");
    order_view_fields[i++].caption = pick_string("预期完成", "ExpectTime");

    gui_view_init_private( main_order_view, 
                           order_view_fields, 
                           order_get_max_index,
                           order_view_get_data,
                           NULL,
                           NULL,
                           show_order_record,
                           pick_string(" 统计:", "Stat:"), 
                           show_statistics, 
                           notifier_on_changed,
                           atoi(___color_order_data), /* 123, */
                           atoi(___bkcolor_order_data), /* WIDGET_BKCOLOR+65, */
                           MAIN_ORDER_ITEM_HEIGHT, /*height_per_row*/
                           1 /* alloc_memory */
                         );
}

/*-----------------------------------------------------------------------------------------
 *          
 *      定单查询区域(main_order_query_view)的相关定义
 * 
**---------------------------------------------------------------------------------------*/
static view_fields_t order_query_fields[] =
{
#define ____style        0
#define ____draw_style   DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER
    #if 0
    { "订单序号",   __id_sn,     9,  10,  ____style,  ____draw_style,  "", },
    #endif
    { "工号",       __id_wkno,   4,  6,   ____style,  ____draw_style,  "", },
    { "订单号",     __id_odrno,  8,  12,  ____style,  ____draw_style,  "", },
    { "设定长",     __id_setlen, 6,  15,  ____style,  ____draw_style,  "", },
    { "张数",       __id_count,  6,  15,  ____style,  ____draw_style,  "", },
    { "捆",         __id_unit,   3,  6,   ____style,  ____draw_style,  "", },
    { "限速",       __id_spdlmt, 4,  8,   ____style,  ____draw_style,  "", },
    { "",           __id_dummy,  9,  14,  ____style,  ____draw_style,  "", },
    { NULL, }
#undef ____style
#undef ____draw_style
};

static void show_query_statistics( int index,
                             int x, int y, int width, int height,
                             int color, int bkcolor, int font,
                             INT08S *comment, INT08S *old, INT16U option)
{
    char ___s[96];
    int  order_nr;

    comment = comment;
    option  = option;

    order_nr = get_order_nr();
    
    if (order_nr > 0) {
        sprintf(___s,
                pick_string(" 订单总数: %d 笔, 第 %d 笔", 
                            " Orders Nr: %d, Order #%d"),
                order_nr, index+1);
    } else {
        sprintf(___s,
                pick_string(" 没有订单", 
                            " No Order"));
    }

    draw_font_for_widget(x, y, width, height, ___s, old, color, bkcolor, font, DRAW_OPT_FIL_BG);
}

void init_order_query_private(void)
{
    int i;
    /*
    printf("height_per_row = %4d\r", height_per_row);
    */
    i = 0;
    order_query_fields[i++].caption = pick_string("工号",     "WorkNo");
    order_query_fields[i++].caption = pick_string("订单号",   "OrderNo");
    order_query_fields[i++].caption = pick_string("设定长",   "SetLength");
    order_query_fields[i++].caption = pick_string("张数",     "Count");
    order_query_fields[i++].caption = pick_string("捆",       "UNIT");
    order_query_fields[i++].caption = pick_string("限速",     "SpdLmt");

    gui_view_init_private( main_order_query_view, 
                           order_query_fields, 
                           order_get_max_index,
                           order_view_get_data,
                           order_view_set_data,
                           order_view_is_writable,
                           NULL,
                           pick_string(" 统计:", "Stat:"), 
                           show_query_statistics, 
                           NULL,
                           atoi(___color_order_query),
                           atoi(___bkcolor_order_query),
                           35, /*height_per_row*/
                           1 /* alloc_memory */
                         );
}


/*-----------------------------------------------------------------------------------------
 *          
 *      主画面下的定时器(DPC)
 * 
**---------------------------------------------------------------------------------------*/
void dpc_for_main(void * d, INT16S nr)
{
    char ___s[32];
    struct time tm;
    struct date dt;
    char * cpu_used_format;

    d = d; nr = nr;

    cpu_used_format = pick_string("CPU使用率: %d%%", "CPU-USAGE: %d%%");

    if(current_date_time){ /* 日期与时间的刷新 */
        lock_kernel();
        gettime(&tm);
        getdate(&dt);
        unlock_kernel();
        sprintf(___s, "%d/%02d/%02d  %02d:%02d %02d", dt.da_year, dt.da_mon, dt.da_day, tm.ti_hour, tm.ti_min, tm.ti_sec);
        if(tm.ti_hund < 50)
            ___s[14] = ':';
        else 
            ___s[14] = ' ';        
        gui_label_set_text(current_date_time, ___s);
    }
    if(main_cpu_status){ /* CPU使用率的显示 */
        lock_kernel();
        sprintf(___s, cpu_used_format, CPU_USED);
        unlock_kernel();
        gui_label_set_text(main_cpu_status, ___s);
    }
    if(system_running_anim){ /* 系统运行状态 */
        static char status[] = {'-', '/', '|', '\\'};
        static int  idx = 0;
        char str[4];
        str[0] = status[idx];
        str[1] = 0;
        idx = (idx + 1) % sizeof(status);
        gui_label_set_text(system_running_anim, str);
    }
    if(1){ /* 闪烁指定的颜色 */
        static int __t = 0, __cntr = 0;
        __cntr++;
        if(__cntr >= 2){
            __cntr = 0;
            if(__t){
                gdi_set_palette(COLOR_BLINK, 0x3F, 0x0, 0x0);
            } else {
                gdi_set_palette(COLOR_BLINK, 0x0, 0x3F, 0x0);
            }
            __t = !__t;
        }
    }
    if(1){ /* 闪烁警告色 */
        static int __t = 0, __cntr = 0;
        __cntr++;
        if(__cntr >= 2){
            __cntr = 0;
            if(__t){
                gdi_set_palette(COLOR_WARNING, 0x35, 0x34,0x32);
            } else {
                gdi_set_palette(COLOR_WARNING, 0x3F, 0x0, 0x0);
            }
            __t = !__t;
        }
    }
    if(1){ /* 闪烁警告色(236) */
        static int __t = 0, __cntr = 0;
        __cntr++;
        if(__cntr >= 2){
            __cntr = 0;
            if(__t){
                gdi_set_palette(COLOR_WARNING_236, 0x20, 0x28,0x30);
            } else {
                gdi_set_palette(COLOR_WARNING_236, 0x3F, 0x0, 0x0);
            }
            __t = !__t;
        }
    }

    if(0){
        static int count = 0, delay = 0;
        char tmp[32];
        extern gui_widget * l__prod_count;
        extern gui_widget * l__remain_count;
        if (delay++ == 0) {
            INT16toSTR(tmp, count, CHG_OPT_DEC|CHG_OPT_END);
            gui_label_set_text(l__prod_count, tmp);
            INT16toSTR(tmp, (32000-count), CHG_OPT_DEC|CHG_OPT_END);
            gui_label_set_text(l__remain_count, tmp);
            count++;
            if(count > 32000)
                count = 0;
        }
        if (delay >= 3)
            delay = 0;
    }
}

/*-----------------------------------------------------------------------------------------
 * 
 *      主循环, 不返回
 * 
**---------------------------------------------------------------------------------------*/
void start_main_loop(void)
{
    int i = 0;
    order_struct order;
    order_struct default_order = DFLT_order_struct;

    TimerSet(TimerForMainScreen, 100L, TIMER_TYPE_AUTO, dpc_for_main, NULL);
    
    TaskDelay(300); /* 用0.3秒等待主画面的显示, 应该是足够了吧 */

    #if 0
    if (config.send_order_on_startup > _SendOrderMode_None) {
        if(GetOrderForView(&order, i++)) { /* 有订单数据 */
            if (config.send_order_on_startup > 10)
                slc_send_order(pending_slc, &order, 0); /* 发送订单+启动 */
            else
                slc_send_order(pending_slc, &order, 1); /* 发送订单但不启动 */
        } else { /* 在没有订单的情况下... */
            switch (config.send_order_on_startup) {
                case _SendOrderMode_SendLastRunning:
                    slc_send_order(pending_slc, &slc->working, 1); /* 发送关机前订单但不启动 */
                    break;
                case _SendOrderMode_SendLastRunning_Control:
                    slc_send_order(pending_slc, &slc->working, 0); /* 发送关机前订单+启动 */
                    break;
                case _SendOrderMode_SendDefault:
                    slc_send_order(pending_slc, &default_order, 1); /* 发送默认订单但不启动 */
                    break;
                case _SendOrderMode_SendDefault_Control:
                    slc_send_order(pending_slc, &default_order, 0); /* 发送默认订单+启动 */
                    break;
                default:
                    break; /* 若是其它值, 且又订单为空, 那就什么也不发送了 */
            }
        }
    }
    #endif

    main_default_functions = pick_string(main_default_functions_zh, main_default_functions_en);
    main_ctrl_functions    = pick_string(main_ctrl_functions_zh,    main_ctrl_functions_en);
    main_alt_functions     = pick_string(main_alt_functions_zh,     main_alt_functions_en);
    main_shift_functions   = pick_string(main_shift_functions_zh,   main_shift_functions_en);
    order_query_functions  = pick_string(order_query_functions_zh,  order_query_functions_en);
    order_query_alt_funcs  = pick_string(order_query_alt_funcs_zh,  order_query_alt_funcs_en);

    ____set_buttoncmds_comment();

    for(;;){
        set_buttons_caption_to_default();
        if (config.reset_onstartup) {
            cmd_reset_count(NULL); /* 复位计数 */
        }
        do_buttoncmds(cmd_main, main_buttons, main_buttons_max, 1,
            BUTTON_STYLE_CLIENT_BDR | BUTTON_STYLE_MODAL_FRAME, 
            BUTTON_STYLE_CLIENT_BDR | BUTTON_STYLE_MODAL_FRAME, putkey_to_add_dec);
    }
}

/*=========================================================================================
 * 
 * 本文件结束: cut/main.c
 * 
**=======================================================================================*/

