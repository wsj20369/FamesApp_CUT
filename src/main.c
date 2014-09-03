/******************************************************************************************
 * �ļ�:    cut/main.c
 *
 * ˵��:    CUT������
 *
 * ����:    Jun
 *
 * ʱ��:    2012-11-27
******************************************************************************************/
#define  CUT_MAIN_C
#include <includes.h>
#include "common.h"

/*-----------------------------------------------------------------------------------------
 *          
 *      �������еĿؼ�����������
 * 
**---------------------------------------------------------------------------------------*/
#define        main_buttons_max  6       /* ���ܰ�ť����         */

gui_window_t * main_window = NULL;
gui_widget   * main_screen = NULL;       /* ����������ؼ�       */
gui_widget   * main_status_bar;          /* ������״̬��         */
gui_widget   * main_cpu_status;          /* CPUʹ���ʵ���ʾ      */
gui_widget   * main_buttons[main_buttons_max]={NULL,}; /* 8�����ť */

gui_widget   * company_picture;          /* һ��ͼƬ             */

gui_widget   * main_order_view;          /* ������ͼ�ؼ�         */

gui_widget   * main_order_query_view;    /* ������ѯ�ؼ�         */
gui_window_t * main_order_query_window;

gui_widget   * dashedline_in_main;       /* һ��������           */

gui_widget   * current_date_time;        /* ʱ��������           */

gui_widget   * system_running_anim;      /* ϵͳ���б��         */

gui_widget   * driver_name_label;        /* ����������           */

gui_window_t * system_about_window;      /* ϵͳ���ڴ���         */


extern BMPINFO icon, company_bmp;        /* ͼ����ͼƬ           */



/*-----------------------------------------------------------------------------------------
 *          
 *      ��������
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
 *      ��������ɫ
 * 
**---------------------------------------------------------------------------------------*/
static char  ___color_order_title[4] = "251";    /* �����涩������ */
static char  ___bkcolor_order_title[4] = "37";
static char  ___color_order_data[4] = "70";
static char  ___bkcolor_order_data[4] = "236";

static char  ___color_order_query[4] = "189";    /* ������ѯ���� */
static char  ___bkcolor_order_query[4] = "70";

static char  ___color_buttons[4] = "71";         /* �����水ť��ɫ */
static char  ___bkcolor_buttons[4] = "2";

       char  ___bkcolor_other_dialog[4] = "236";

static int   BUTTON_COLOR    =  COLOR_WHITE;
static int   BUTTON_BKCOLOR  =  2;


/*-----------------------------------------------------------------------------------------
 *          
 *      ������Ķ���(���ʼ��), ����Ԫ�ش��µ��ϰ��Ž�����ʾ�ĸ���
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

    /* ������       */
    main_screen = gui_create_widget(GUI_WIDGET_FORM, 1, 1, 1021, 765, 0, 0, font16, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE);
    if(!main_screen)
        goto some_error;
    main_window = gui_create_window(main_screen);
    gui_widget_link(NULL, main_screen);      /* ���������汳�� */
    gui_form_init_private(main_screen, 128);
    gui_form_set_icon(main_screen, &icon);
    STRCPY(___s, "Application Demonstration");
    load_string(___s, sizeof(___s), "main_title");
    gui_form_set_caption(main_screen, ___s);

    /* ʱ��������   */
    current_date_time = gui_create_widget(GUI_WIDGET_LABEL, 838, 9, 172, 16,  COLOR_WHITE, WIDGET_COLOR_TITLE_XP, 1, LABEL_STYLE_TRANSPARENT);
    if(!current_date_time)
        goto some_error;
    gui_widget_link(main_screen, current_date_time);
    gui_label_init_private(current_date_time, 32);
    gui_label_set_text(current_date_time, "");

    init_produce_screen(main_screen);

    /* һ������     */
    dashedline_in_main = gui_create_widget(GUI_WIDGET_DASHEDLINE, 10, 385, 998, 5, 73, 0, 0, DASHED_STYLE_CENTER);
    if(!dashedline_in_main)
        goto some_error;
    gui_widget_link(main_screen, dashedline_in_main);
    gui_dashedline_init_private(dashedline_in_main);
    gui_dashedline_set_param(dashedline_in_main, 970, 3, 0xE0); /* 0xE4 is good */

    /* ��������     */
    main_order_view = gui_create_widget(GUI_WIDGET_VIEW, 11, 392, 996, 256, atoi(___color_order_title), atoi(___bkcolor_order_title),
                                        font24, VIEW_STYLE_FIELDS_TITLE|VIEW_STYLE_MARK_BAR);
    if(!main_order_view)
        goto some_error;
    init_order_view_private();
    gui_view_set_dashed(main_order_view, 127, 0);
    gui_widget_link(main_screen, main_order_view);

    /* ���ܰ�ť     */
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
    
    /* ������       */
    main_status_bar = gui_create_widget(GUI_WIDGET_LABEL, 7, 725, 581, 30, 0, 0, font16, LABEL_STYLE_CLIENT_BDR);
    if(!main_status_bar)
        goto some_error;
    gui_widget_link(main_screen, main_status_bar);
    gui_label_init_private(main_status_bar, 100);
    STRCPY(___s, "Welcome to FamesOS!");
    load_string(___s, sizeof(___s), "first_note");
    gui_label_set_text(main_status_bar, ___s);

    /* ����������   */
    driver_name_label = gui_create_widget(GUI_WIDGET_LABEL, 752, 725, 100, 30, 0, 0, font16, LABEL_STYLE_CLIENT_BDR|LABEL_ALIGN_CENTER);
    if(!driver_name_label)
        goto some_error;
    gui_widget_link(main_screen, driver_name_label);
    gui_label_init_private(driver_name_label, 32);
    sprintf(___s, "%s", get_driver_types_name(config.driver_type));
    gui_label_set_text(driver_name_label, ___s);

    /* CUT����״̬  */
    system_running_anim = gui_create_widget(GUI_WIDGET_LABEL, 852, 725, 28, 30, 0, 0, font16, LABEL_STYLE_CLIENT_BDR|LABEL_ALIGN_CENTER);
    if(!system_running_anim)
        goto some_error;
    gui_widget_link(main_screen, system_running_anim);
    gui_label_init_private(system_running_anim, 8);
    gui_label_set_text(system_running_anim, "");

    /* CPUʹ����    */
    main_cpu_status = gui_create_widget(GUI_WIDGET_LABEL, 880, 725, 132, 30, 0, 0, font16, LABEL_STYLE_CLIENT_BDR|LABEL_ALIGN_CENTER);
    if(!main_cpu_status)
        goto some_error;
    gui_widget_link(main_screen, main_cpu_status);
    gui_label_init_private(main_cpu_status, 32);
    gui_label_set_text(main_cpu_status, "");

    #if 0
    /* ��˾��־     */
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

/* �����������ݵ���ʾ */
void show_today_data_screen(int show)
{
    __show_today_data_screen(show);

    if (show) {
        gui_set_widget_location(dashedline_in_main, 10, 427); /* XXX: ��Щ��ֵ���������һ�� */
        gui_set_widget_location(main_order_view, 11, 434);
        gui_set_widget_dimension(main_order_view, 996, 214);

    } else {
        gui_set_widget_location(dashedline_in_main, 10, 385);
        gui_set_widget_location(main_order_view, 11, 392);
        gui_set_widget_dimension(main_order_view, 996, 256);
    }

    gui_refresh_widget(main_screen);    /* ˢ�������� */
}


/*-----------------------------------------------------------------------------------------
 *          
 *      ������������
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
 *      ������֮�������̵Ķ���
 * 
**---------------------------------------------------------------------------------------*/
void cmd_main_dummy(void * data)
{
    data = data;
    message(pick_string(" *** �ð汾���޴˹��� ***", "Not Supported"));
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

/* ������λ, Ŀǰֻ������ʱִ��һ��, �����ɽ�����Ϊһ�������������û���ʱִ��(�뿼���䰲ȫ�Լ�������) */
void cmd_reset_count(void * data)
{
    BOOL retval;

    data = data;

    retval = check_yes_no_dialog(264, 296, pick_string("������λ", "Reset Counter"),
                                           pick_string("Ҫ \"���\" ��ǰ���еļ�����?","Clear all the counter data?"));
    if (retval) {
        /* TODO: ������Ҫ���������λ����... */
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
            ___alert("Err: cmd_move_order(): ��������!",
                     "Err: cmd_move_order(): Invalid data!");
            return;
    }

    if(get_order_nr() <= 0){
        ___alert("��ǰû�ж���, �޷�����!", "No Order to move!");
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
            ___alert("Err: cmd_delete_order(): ��������!",
                     "Err: cmd_delete_order(): Invalid data!");
            return;
    }

    if(get_order_nr() <= 0){
        ___alert("û�ж�����, �޷�ɾ��!", "No Order to delete!");
        return;
    }

    retval = check_yes_no_dialog(264, 296, pick_string("ɾ��", "Delete"),
                                           pick_string("Ҫ \"ɾ��\" ��ʶ�����?","Delete this order?"));
    if (retval)
        DeleteOrder(gui_view_get_selected(view));
}


void cmd_clear_order(void * data)
{
    BOOL retval;

    data = data;
    if(get_order_nr() <= 0){
        ___alert("û�ж�����, ����Ҫ�嵥!", "No Order to clear!");
        return;
    }
    retval = check_yes_no_dialog(264, 296, pick_string("�嵥", "Clear"),
                                           pick_string("Ҫ \"ɾ��\" ���ж�����?", "Clear All the orders?"));
    if (retval)
        ZapOrder();
}

void cmd_pack_workno(void * data)
{
    data = data;
    if(get_order_nr() <= 0){
        ___alert("��ǰû�ж���, �޷�����!", "No Order to Resort");
        return;
    }
    if(check_confirm(pick_string("����", "Resort"), 
                     pick_string("Ҫ \"����\" ������?(Y/N)", "Confirm to Resort?(Y/N)"))){
        if(!PackWorkNo(10))
            ___alert("��������ʧ��!", "Failed to Resort!");
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
                "�鿴������������: Up, Down, PgUp, PgDn, Home, End", 
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
            pick_string("ϵͳ�汾��", "System Version"),
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

    if(check_confirm(pick_string(" >>> �˳� <<<", ">>> Quit <<<"), 
                     pick_string("Ҫ \"�˳�\" ���л�ϵͳ��?(Y/N)", "Confirm to \"Quit\" program?(Y/N)"))) {
        /* �˳�ʱ, Ҫ��Щʲô, ����: �رն����ļ�...
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
 *      ������֮�����Ķ���
 * 
**---------------------------------------------------------------------------------------*/
INT08S *main_default_functions_zh[] = {
        "[F1]�䵥", "[F2]��ѯ",
        "[F5]ͣ��", "[F6]ͣ��",
        "[F7]����", "[F9]׷��", NULL,
};
INT08S *main_default_functions_en[] = {
        "[F1]Input", "[F2]Query",
        "[F5]StpChg", "[F6]StpCnt",
        "[F7]Change", "[F9]Additi", NULL,
};
INT08S *main_ctrl_functions_zh[] = {
        "", "", "", "", "[F10]����", "[END]�˳�", NULL,
};
INT08S *main_ctrl_functions_en[] = {
        "", "", "", "", "[F10]ReSend", "[END]QUIT", NULL,
};
INT08S *main_alt_functions_zh[] = {
        "[F1]����", "[F2]���ٱ�", "[F3]������", "[F5]�߼�", "", "[F8]ϵͳ", NULL,
};
INT08S *main_alt_functions_en[] = {
        "[F1]Setup", "[F2]SpdLmt", "[F3]Driver", "[F5]Super", "", "[F8]System", NULL,
};
INT08S *main_shift_functions_zh[] = {
        "", "", "", "[F6]�汾", "[F7]ID", "[F8]ϵͳ", NULL,
};
INT08S *main_shift_functions_en[] = {
        "", "", "", "[F6]Ver", "[F7]ID", "[F8]System", NULL,
};

INT08S *order_query_functions_zh[] = {
        "[F3]ɾ��", "[F4]����",
        "[F5]�嵥", "[F6]����",
        "[F9]����", "[F10]�ص�", NULL,
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

    message(pick_string("����CTRL, ALT��SHIFT�ɲ鿴��������",
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

    message(pick_string("������ѯ", "Order Query"));

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

static char __input_order_comment_zh[] = "����һ���µĶ���, ����������Ѿ�����, �Ǿͳ�Ϊ�޸���!";
static char __input_order_comment_en[] = "Input a Order and Insert it or Modify it if S/N already exist!";

button_cmd_t cmd_main[] = {
    #define __(x) (void*)(INT32U)(x)
   /* �����ܼ�     */
    { F1,          0, cmd_input_order,      NULL,      __input_order_comment_zh },
    { F2,          0, cmd_query_order,      NULL,      "��ѯ���ж���, �����䴦��" },
    { F5,          0, cmd_stop_change,      NULL,      "ֹͣ����" },
    { F6,          0, cmd_stop_count,       NULL,      "ֹͣ����, ���������벻��Ʒ" },
    { F7,          0, cmd_change_order,     NULL,      "ǿ�ƻ�������һ��" },
    { F9,          0, cmd_additional,       NULL,      "׷��һ����ʱ����" },
    { ENTER,       0, cmd_input_order,      NULL,      __input_order_comment_zh },
   /* ��������     */
    { F3,          0, cmd_delete_order,     __(1),     "ɾ����ǰ����!" },
    { F4,          0, cmd_move_order,       __(1),     "��������������˳��" },
   /* NONE_KEY     */
    { NONE_KEY,    0, cmd_main_NONE,        NULL,      "" },
   /* CTRL-F#      */
    { CTRL,        0, cmd_main_CTRL,        NULL,      "" },
    { CTRL_END,    0, cmd_main_QUIT   ,     NULL,      "�˳����л�����" },
   /* ALT-F#       */
    { ALT,         0, cmd_main_ALT,         NULL,      "" },
    { ALT_F1,      0, setup_generic_param,  NULL,      "���л���ͨ����������" },
    { ALT_F2,      0, setup_spdlmt_tbl,     NULL,      "���ٱ��趨" },
    { ALT_F3,      0, setup_driver_param,   NULL,      "����������������" },
    { ALT_F5,      0, setup_super_param,    NULL,      "�߼�����������, ֻ�й���Ա�ſ��޸�" },
    { ALT_F8,      0, cmd_main_SYS,         NULL,      "System Monitor" },
   /* SHIFT-F#     */
    { SHIFT,       0, cmd_main_SHIFT,       NULL,      "" },
    { SHIFT_F6,    0, cmd_main_VER,         NULL,      "" },
    { SHIFT_F7,    0, cmd_main_ID,          __(0),     "System ID/SN" },
    { SHIFT_F8,    0, cmd_main_SYS,         NULL,      "System Monitor" },
   /* ע���˳���   */
    { RS_EXIT_KEY, 0, cmd_main_ID,          __(1),     "System ID/SN" },
   /* �������     */
    { UP,          0, cmd_main_browse,      __(UP),    "" },
    { DOWN,        0, cmd_main_browse,      __(DOWN),  "" },
    { PGUP,        0, cmd_main_browse,      __(PGUP),  "" },
    { PGDN,        0, cmd_main_browse,      __(PGDN),  "" },
    { HOME,        0, cmd_main_browse,      __(HOME),  "" },
    { END,         0, cmd_main_browse,      __(END),   "" },
   /* KEYCMD�Ĺ��� */
    { TAB,         0, NULL,                 NULL,      "" },
    { LEFT,        0, NULL,                 NULL,      "" },
    { RIGHT,       0, NULL,                 NULL,      "" },
   /* ������       */
    { SHIFT|CTRL,  0, cmd_main_SYS,         NULL,      "" },
    { 0x1900,      0, cmd_main_PALETTE,     NULL,      "ALT-P, Display System Palette" },
    { ESC,         0, NULL,                 NULL,      "��ʾ: ���������, ��Ҫ��Ϣ��Ϣ! ף������˳��, Ц�ڳ���!" },
   /* ������־     */
    { NONEKEY, },
    #undef __
};

static void ____set_buttoncmds_comment(void)
{
    /* ����Ӣ�İ汾��ע���ַ��� */
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
   /* �����ܼ�     */
    { F3,          0, cmd_delete_order,     __(2),     "ɾ����ǰ����!" },
    { F4,          0, cmd_move_order,       __(2),     "��������������˳��" },
    { F5,          0, cmd_clear_order,      NULL,      "������ж���" },
    { F6,          0, cmd_pack_workno,      NULL,      "��������������" },
    { F9,          0, cmd_change_order,     NULL,      "�����ж�����һ������" },
    { F10,         0, cmd_additional,       NULL,      "�Ѷ������ݼӵ���������" },
   /* ״̬��       */
    #if 0
    { NONE_KEY,    0, cmd_query_NONE,       NULL,      "" },
    { ALT,         0, cmd_query_ALT,        NULL,      "" },
    #endif
   /* �䵥(�޸�)   */
    { F1,          0, cmd_input_order,      NULL,      __input_order_comment_zh },
    { ENTER,       0, cmd_input_order,      NULL,      __input_order_comment_zh },
   /* �������     */
    { UP,          0, cmd_query_browse,     __(UP),    "" },
    { DOWN,        0, cmd_query_browse,     __(DOWN),  "" },
    { PGUP,        0, cmd_query_browse,     __(PGUP),  "" },
    { PGDN,        0, cmd_query_browse,     __(PGDN),  "" },
    { HOME,        0, cmd_query_browse,     __(HOME),  "" },
    { END,         0, cmd_query_browse,     __(END),   "" },
   /* KEYCMD�Ĺ��� */
    { TAB,         0, NULL,                 NULL,      "" },
    { LEFT,        0, NULL,                 NULL,      "" },
    { RIGHT,       0, NULL,                 NULL,      "" },
    { ENTER,       0, NULL,                 NULL,      "" },
   /* �˳���       */
    { ESC,         1, NULL            ,     NULL,      "" },
   /* ������־     */
    { NONEKEY, },
    #undef __
};

static void ____set_buttoncmds_comment_query(void)
{
    /* ����Ӣ�İ汾��ע���ַ��� */
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
 *      ������ʾ����(main_order_view)����ض���
 * 
**---------------------------------------------------------------------------------------*/
enum order_view_fields_id {
    __id_sn,
    __id_wkno,
    __id_odrno,
    __id_setlen,
    __id_count,
    __id_unit,
    __id_spdlmt, /* ���� */
    __id_expect, /* Ԥ�����ʱ�� */
    __id_dummy,  /* �յ�, ����ʾʵ������ */
};

static view_fields_t order_view_fields[] =
{
#define ____style        0
#define ____draw_style   DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER
    #if 0
    { "�������",   __id_sn,     9,  10,  ____style,  ____draw_style,  "", },
    #endif
    { "����",       __id_wkno,   4,  6,   ____style,  ____draw_style,  "", },
    { "������",     __id_odrno,  8,  12,  ____style,  ____draw_style,  "", },
    { "�趨��",     __id_setlen, 6,  15,  ____style,  ____draw_style,  "", },
    { "����",       __id_count,  6,  15,  ____style,  ____draw_style,  "", },
    { "��",         __id_unit,   3,  6,   ____style,  ____draw_style,  "", },
    { "����",       __id_spdlmt, 4,  8,   ____style,  ____draw_style,  "", },
    { "Ԥ�����",   __id_expect, 9,  14,  ____style,  ____draw_style,  "", },
    { NULL, }
#undef ____style
#undef ____draw_style
};

struct __order_old_buf { /* �����ַ����ĳ��Ȳ��ɳ�������ṹ�е�bytes_for_width�ֶ� */
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
            j = 0; /* �ڹ�������ʾ<Err> */
            draw_font_for_widget(x+3, y, (fields_width[j]-to_left)-6, height, "<Err>", 
                                 record_buf_old->workno, marker_color, marker_bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
            return fail;
        }
        /* ���� */
        j = 0;
        sprintf(___s, "%d", __order.WORKNO);
        draw_font_for_widget(x+3, y, (fields_width[j]-to_left)-6, height, ___s, 
                             record_buf_old->workno, marker_color, marker_bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* ������ */
        j++;
        __order.ORDERNO[8] = 0;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, __order.ORDERNO, 
                             record_buf_old->orderno, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* �趨�� */
        j++;
        sprintf(___s, "%u", __order.CUT_LENGTH);
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, ___s, 
                             record_buf_old->cutlen, special_color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* ���� */
        j++;
        sprintf(___s, "%ld", __order.NUMBER);
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, ___s, 
                             record_buf_old->number, special_color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* ����λ */
        j++;
        sprintf(___s, "%d", __order.UNIT);
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, ___s, 
                             record_buf_old->unit, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* ���� */
        j++;
        sprintf(___s, "%d", get_speed_limit(__order.CUT_LENGTH));
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, ___s, 
                             record_buf_old->spdlimit, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* Ԥ�����ʱ�� */
        j++;
        sprintf(___s, "%s", "08:08:08");
        width = fields_width[j]-to_left;
        if (x + width > x_border)
            width = x_border - x;
        draw_font_for_widget(x, y, width, height, ___s, 
                             record_buf_old->finishtime, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
    } else {
        /* ���� */
        j = 0;
        draw_font_for_widget(x+3, y, (fields_width[j]-to_left)-6, height, "", 
                             record_buf_old->workno, marker_color, marker_bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* ������ */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->orderno, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* �趨�� */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->cutlen, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* ���� */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->number, color, bkcolor, font, DRAW_OPT_FIL_BG);
        x += fields_width[j];
        /* ����λ */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->unit, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* ���� */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->spdlimit, color, bkcolor, font, DRAW_OPT_FIL_BG);
        x += fields_width[j];
        /* Ԥ�����ʱ�� */
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
            pick_string(" ��������: %d ��, �� %d ��", 
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
                    sprintf(___s, pick_string("��ǰ����", "Current"));
                } else {
                    sprintf(___s, pick_string("��%3d��", "Order#%d"), index+1);
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
     * ����Ҫʵ���ڶ�����ѯ������, ֱ����VIEW�±༭�����Ĺ��ܵ�,
     * ����, ���ֲ�̫��ʵ��, ��Ϊorder_view_get_data()�ǴӶ������ݿ���
     * ֱ��ȡ�õ�����, �û��������µ�����ʱ���Զ���order_view_get_data()
     * ������, ��Ȼ, ʹ��һЩ���ɻ��ǿ�����������, �����о������������
     * ��ʵҲû�ж���ʵ�ü�ֵ, ���Ի��ǲ�Ҫ������.
     *
     * �˺�������Ϊ��, VIEW��ֱ�ӱ༭�����Ĺ���û��ʵ��! 2013-03-17
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
        /* �޶���ʱ */
    } else {
        /* �ж���ʱ */
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
    order_view_fields[i++].caption = pick_string("����",     "WorkNo");
    order_view_fields[i++].caption = pick_string("������",   "OrderNo");
    order_view_fields[i++].caption = pick_string("�趨��",   "SetLength");
    order_view_fields[i++].caption = pick_string("����",     "Count");
    order_view_fields[i++].caption = pick_string("��",       "UNIT");
    order_view_fields[i++].caption = pick_string("����",     "SpdLmt");
    order_view_fields[i++].caption = pick_string("Ԥ�����", "ExpectTime");

    gui_view_init_private( main_order_view, 
                           order_view_fields, 
                           order_get_max_index,
                           order_view_get_data,
                           NULL,
                           NULL,
                           show_order_record,
                           pick_string(" ͳ��:", "Stat:"), 
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
 *      ������ѯ����(main_order_query_view)����ض���
 * 
**---------------------------------------------------------------------------------------*/
static view_fields_t order_query_fields[] =
{
#define ____style        0
#define ____draw_style   DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER
    #if 0
    { "�������",   __id_sn,     9,  10,  ____style,  ____draw_style,  "", },
    #endif
    { "����",       __id_wkno,   4,  6,   ____style,  ____draw_style,  "", },
    { "������",     __id_odrno,  8,  12,  ____style,  ____draw_style,  "", },
    { "�趨��",     __id_setlen, 6,  15,  ____style,  ____draw_style,  "", },
    { "����",       __id_count,  6,  15,  ____style,  ____draw_style,  "", },
    { "��",         __id_unit,   3,  6,   ____style,  ____draw_style,  "", },
    { "����",       __id_spdlmt, 4,  8,   ____style,  ____draw_style,  "", },
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
                pick_string(" ��������: %d ��, �� %d ��", 
                            " Orders Nr: %d, Order #%d"),
                order_nr, index+1);
    } else {
        sprintf(___s,
                pick_string(" û�ж���", 
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
    order_query_fields[i++].caption = pick_string("����",     "WorkNo");
    order_query_fields[i++].caption = pick_string("������",   "OrderNo");
    order_query_fields[i++].caption = pick_string("�趨��",   "SetLength");
    order_query_fields[i++].caption = pick_string("����",     "Count");
    order_query_fields[i++].caption = pick_string("��",       "UNIT");
    order_query_fields[i++].caption = pick_string("����",     "SpdLmt");

    gui_view_init_private( main_order_query_view, 
                           order_query_fields, 
                           order_get_max_index,
                           order_view_get_data,
                           order_view_set_data,
                           order_view_is_writable,
                           NULL,
                           pick_string(" ͳ��:", "Stat:"), 
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
 *      �������µĶ�ʱ��(DPC)
 * 
**---------------------------------------------------------------------------------------*/
void dpc_for_main(void * d, INT16S nr)
{
    char ___s[32];
    struct time tm;
    struct date dt;
    char * cpu_used_format;

    d = d; nr = nr;

    cpu_used_format = pick_string("CPUʹ����: %d%%", "CPU-USAGE: %d%%");

    if(current_date_time){ /* ������ʱ���ˢ�� */
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
    if(main_cpu_status){ /* CPUʹ���ʵ���ʾ */
        lock_kernel();
        sprintf(___s, cpu_used_format, CPU_USED);
        unlock_kernel();
        gui_label_set_text(main_cpu_status, ___s);
    }
    if(system_running_anim){ /* ϵͳ����״̬ */
        static char status[] = {'-', '/', '|', '\\'};
        static int  idx = 0;
        char str[4];
        str[0] = status[idx];
        str[1] = 0;
        idx = (idx + 1) % sizeof(status);
        gui_label_set_text(system_running_anim, str);
    }
    if(1){ /* ��˸ָ������ɫ */
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
    if(1){ /* ��˸����ɫ */
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
    if(1){ /* ��˸����ɫ(236) */
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
 *      ��ѭ��, ������
 * 
**---------------------------------------------------------------------------------------*/
void start_main_loop(void)
{
    int i = 0;
    order_struct order;
    order_struct default_order = DFLT_order_struct;

    TimerSet(TimerForMainScreen, 100L, TIMER_TYPE_AUTO, dpc_for_main, NULL);
    
    TaskDelay(300); /* ��0.3��ȴ����������ʾ, Ӧ�����㹻�˰� */

    #if 0
    if (config.send_order_on_startup > _SendOrderMode_None) {
        if(GetOrderForView(&order, i++)) { /* �ж������� */
            if (config.send_order_on_startup > 10)
                slc_send_order(pending_slc, &order, 0); /* ���Ͷ���+���� */
            else
                slc_send_order(pending_slc, &order, 1); /* ���Ͷ����������� */
        } else { /* ��û�ж����������... */
            switch (config.send_order_on_startup) {
                case _SendOrderMode_SendLastRunning:
                    slc_send_order(pending_slc, &slc->working, 1); /* ���͹ػ�ǰ������������ */
                    break;
                case _SendOrderMode_SendLastRunning_Control:
                    slc_send_order(pending_slc, &slc->working, 0); /* ���͹ػ�ǰ����+���� */
                    break;
                case _SendOrderMode_SendDefault:
                    slc_send_order(pending_slc, &default_order, 1); /* ����Ĭ�϶����������� */
                    break;
                case _SendOrderMode_SendDefault_Control:
                    slc_send_order(pending_slc, &default_order, 0); /* ����Ĭ�϶���+���� */
                    break;
                default:
                    break; /* ��������ֵ, ���ֶ���Ϊ��, �Ǿ�ʲôҲ�������� */
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
            cmd_reset_count(NULL); /* ��λ���� */
        }
        do_buttoncmds(cmd_main, main_buttons, main_buttons_max, 1,
            BUTTON_STYLE_CLIENT_BDR | BUTTON_STYLE_MODAL_FRAME, 
            BUTTON_STYLE_CLIENT_BDR | BUTTON_STYLE_MODAL_FRAME, putkey_to_add_dec);
    }
}

/*=========================================================================================
 * 
 * ���ļ�����: cut/main.c
 * 
**=======================================================================================*/

