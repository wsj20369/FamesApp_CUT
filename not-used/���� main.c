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
#define  main_buttons_max  6            /* ���ܰ�ť����         */

gui_widget  * main_screen = NULL;       /* ����������ؼ�       */
gui_widget  * main_status_bar;          /* ������״̬��         */
gui_widget  * main_cpu_status;          /* CPUʹ���ʵ���ʾ      */
gui_widget  * main_buttons[main_buttons_max]={NULL,}; /* 8�����ť */

gui_widget  * slc_1_status;             /* ��1��״̬��ʾ        */
gui_widget  * slc_2_status;             /* ��2��״̬��ʾ        */

gui_widget  * cim_link_status;          /* ��������״̬����ʾ   */

gui_widget  * company_picture;          /* һ��ͼƬ             */

gui_widget  * main_order_view;          /* ������ͼ�ؼ�         */
gui_widget  * order_shouldbe_preview;   /* ����Ԥ���ؼ�         */

gui_widget  * dashedline_in_main;       /* һ��������           */

gui_widget  * current_date_time;        /* ʱ��������           */

gui_widget  * input_order_screen;       /* �䵥����             */
gui_widget  * move_order_screen;        /* ��������             */

gui_widget  * slc_monitor_screen;       /* SLC��ػ���          */

gui_widget  * slc_whet_screen;          /* SLCĥ������          */

gui_widget  * slc_lb_screen;            /* �����ǳ����         */

gui_widget  * slc_param_screen;         /* SLC��������          */

gui_widget  * slc_global_param;         /* SLCϵͳ����          */

gui_widget  * slc_malfunction_screen;   /* SLC���ϼ�⻭��      */

gui_widget  * cim_link_mntr_screen;     /* ���ߴ��ڼ��         */

gui_widget  * plc_monitor_screen;       /* PLC���              */

gui_widget  * plc_io_monitor_screen;    /* PLC-IO����         */

extern BMPINFO icon, company_bmp;       /* ͼ����ͼƬ           */

/*-----------------------------------------------------------------------------------------
 *          
 *      ������������
 * 
**---------------------------------------------------------------------------------------*/
void init_view_private(void);
void set_buttons_caption_to_default(void);
void set_buttons_caption_to_alt(void);
void set_buttons_caption_to_ctrl(void);
void set_buttons_caption_to_shift(void);

/*-----------------------------------------------------------------------------------------
 *          
 *      ��������ɫ
 * 
**---------------------------------------------------------------------------------------*/
static char  ___color_order_title[4] = "251";    /* �����涩������ */
static char  ___bkcolor_order_title[4] = "37";
static char  ___color_order_data[4] = "70";
static char  ___bkcolor_order_data[4] = "236";

static char  ___color_preview[4] = "5";          /* ������Ԥ������ */
static char  ___bkcolor_preview[4] = "236";

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
    int preview_id;
    char ___s[128];

    load_string(___color_order_title,    4, "color_order_title");
    load_string(___bkcolor_order_title,  4, "bkcolor_order_title");
    load_string(___color_order_data,     4, "color_order_data");
    load_string(___bkcolor_order_data,   4, "bkcolor_order_data");

    load_string(___color_preview,        4, "color_preview");
    load_string(___bkcolor_preview,      4, "bkcolor_preview");
    
    load_string(___color_buttons,        4, "color_buttons");
    load_string(___bkcolor_buttons,      4, "bkcolor_buttons");

    load_string(___bkcolor_other_dialog, 4, "bkcolor_other_dialog");

    BUTTON_COLOR = atoi(___color_buttons);
    BUTTON_BKCOLOR = atoi(___bkcolor_buttons);    

    /* ������       */
    main_screen = gui_create_widget(GUI_WIDGET_FORM, 1, 1, 1021, 765, 0, 0, font16, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE);
    if(!main_screen)
        goto some_error;
    gui_widget_link(NULL, main_screen);      /* ���������汳�� */
    gui_form_init_private(main_screen, 128);
    gui_form_set_icon(main_screen, &icon);
    STRCPY(___s, "Application Demonstration");
    load_string(___s, sizeof(___s), "slc_title");
    gui_form_set_caption(main_screen, ___s);
    
    /* ������       */
    main_status_bar = gui_create_widget(GUI_WIDGET_LABEL, 7, 725, 581, 30, 0, 0, 1, LABEL_STYLE_CLIENT_BDR);
    if(!main_status_bar)
        goto some_error;
    gui_widget_link(main_screen, main_status_bar);
    gui_label_init_private(main_status_bar, 100);
    STRCPY(___s, "Welcome to FamesOS!");
    load_string(___s, sizeof(___s), "first_note");
    gui_label_set_text(main_status_bar, ___s);

    /* ��������״̬ */
    cim_link_status = gui_create_widget(GUI_WIDGET_LABEL, 589, 725, 88, 30, 0, 0, 1, LABEL_STYLE_CLIENT_BDR);
    if(!cim_link_status)
        goto some_error;
    gui_widget_link(main_screen, cim_link_status);
    gui_label_init_private(cim_link_status, 32);
    gui_label_set_text(cim_link_status, "");
    
    /* ��1״̬      */
    slc_1_status = gui_create_widget(GUI_WIDGET_LABEL, 678, 725, 100, 30, 0, 0, 1, LABEL_STYLE_CLIENT_BDR);
    if(!slc_1_status)
        goto some_error;
    gui_widget_link(main_screen, slc_1_status);
    gui_label_init_private(slc_1_status, 32);
    gui_label_set_text(slc_1_status, "");
    
    /* ��2״̬      */
    slc_2_status = gui_create_widget(GUI_WIDGET_LABEL, 779, 725, 100, 30, 0, 0, 1, LABEL_STYLE_CLIENT_BDR);
    if(!slc_2_status)
        goto some_error;
    gui_widget_link(main_screen, slc_2_status);
    gui_label_init_private(slc_2_status, 32);
    gui_label_set_text(slc_2_status, "");

    /* CPUʹ����    */
    main_cpu_status = gui_create_widget(GUI_WIDGET_LABEL, 880, 725, 132, 30, 0, 0, 1, LABEL_STYLE_CLIENT_BDR|LABEL_ALIGN_CENTER);
    if(!main_cpu_status)
        goto some_error;
    gui_widget_link(main_screen, main_cpu_status);
    gui_label_init_private(main_cpu_status, 32);
    gui_label_set_text(main_cpu_status, "");

    /* ���ܰ�ť     */
    x = 12; y = 655; width = 120; height = 64;
    for(i=0; i<main_buttons_max; i++){
        if(i == 3 || i == 6)
            x += 10;
        main_buttons[i] = gui_create_widget(GUI_WIDGET_BUTTON, x, y, width, height, BUTTON_COLOR, BUTTON_BKCOLOR, 1, BUTTON_STYLE_CLIENT_BDR|BUTTON_STYLE_MODAL_FRAME);
        if(!main_buttons[i])
            goto some_error;
        gui_widget_link(main_screen, main_buttons[i]);
        gui_button_init_private(main_buttons[i], 32);
        gui_button_set_caption(main_buttons[i], "");
        x += (width+2);
    }
    
    /* ����Ԥ������ */
    preview_id = preview_initialize();
    if(!preview_id){
        sys_print("preview_initialize() failed\n");
        getch();
        ExitApplication();
    }
    order_shouldbe_preview = gui_create_widget(preview_id, 10, 375, 998, 273, atoi(___color_preview), atoi(___bkcolor_preview), font16, PREVIEW_STYLE_CLIENT_BDR|PREVIEW_STYLE_MODAL_FRAME);
    if(!order_shouldbe_preview)
        goto some_error;
    gui_widget_link(main_screen, order_shouldbe_preview);
    preview_init_private(order_shouldbe_preview);

    /* һ������     */
    dashedline_in_main = gui_create_widget(GUI_WIDGET_DASHEDLINE, 10, 369, 998, 5, 73, 0, 0, DASHED_STYLE_CENTER);
    if(!dashedline_in_main)
        goto some_error;
    gui_widget_link(main_screen, dashedline_in_main);
    gui_dashedline_init_private(dashedline_in_main);
    gui_dashedline_set_param(dashedline_in_main, 970, 3, 0xE0); /* 0xE4 is good */

    /* ��������     */
    main_order_view = gui_create_widget(GUI_WIDGET_VIEW, 10, 112, 998, 256, atoi(___color_order_title), atoi(___bkcolor_order_title), 1, VIEW_STYLE_FIELDS_TITLE|VIEW_STYLE_MARK_BAR);
    if(!main_order_view)
        goto some_error;
    gui_widget_link(main_screen, main_order_view);
    init_view_private();
    gui_view_set_dashed(main_order_view, 127, 0);
    
    /* ��˾��־     */
    company_picture = gui_create_widget(GUI_WIDGET_PICTURE, 9, 34, 1000, 72, 0, 0, 0, PICTURE_STYLE_TRANSPARENT|PICTURE_STYLE_NO_BORDER);
    if(!company_picture)
        goto some_error;
    gui_widget_link(main_screen, company_picture);
    gui_picture_init_private(company_picture);
    gui_picture_set_picture(company_picture, &company_bmp);

    /* ʱ��������   */
    current_date_time = gui_create_widget(GUI_WIDGET_LABEL, 838, 9, 172, 16,  COLOR_WHITE, WIDGET_COLOR_TITLE_XP, 1, LABEL_STYLE_TRANSPARENT);
    if(!current_date_time)
        goto some_error;
    gui_widget_link(main_screen, current_date_time);
    gui_label_init_private(current_date_time, 32);
    gui_label_set_text(current_date_time, "");

    /* �䵥����     */
    input_order_screen = init_input_order_screen();
    if(!input_order_screen){
        goto some_error;
    }
    gui_widget_link(main_screen, input_order_screen);
    
    /* ��������     */
    move_order_screen = init_move_order_dialog();
    if(move_order_screen){
        gui_widget_link(main_screen, move_order_screen);
    }

    /* ��ػ���     */
    slc_monitor_screen = init_slc_monitor_screen();
    if(!slc_monitor_screen){
        goto some_error;
    }

    /* ĥ������     */
    slc_whet_screen = init_whet_screen();
    if(!slc_whet_screen){
        goto some_error;
    }
    gui_hide_widget(slc_whet_screen);
    gui_widget_link(main_screen, slc_whet_screen);

    /* �����ǳ���� */
    slc_lb_screen = init_lb_screen();
    if(!slc_lb_screen){
        goto some_error;
    }
    gui_hide_widget(slc_lb_screen);
    gui_widget_link(main_screen, slc_lb_screen);

    /* �������� */
    slc_param_screen = init_slc_param_screen();
    if(!slc_param_screen){
        goto some_error;
    }

    /* ϵͳ���� */
    slc_global_param = init_slc_global_param_dlg();
    if(!slc_global_param)
        goto some_error;

    /* SLC���ϼ�� */
    slc_malfunction_screen = init_malfunction_screen();
    if(slc_malfunction_screen){
        gui_hide_widget(slc_malfunction_screen);
        gui_widget_link(main_screen, slc_malfunction_screen);
    }

    /* ���ߴ��ڼ�� */
    cim_link_mntr_screen = init_cim_link_monitor_screen();
    if(cim_link_mntr_screen){
        gui_widget_link(main_screen, cim_link_mntr_screen);
    }

    /* PLC��� */
    plc_monitor_screen = init_plc_monitor_screen();
    if(plc_monitor_screen){
        gui_hide_widget(plc_monitor_screen);
        gui_widget_link(main_screen, plc_monitor_screen);
    }

    /* PLC-IO���� */
    plc_io_monitor_screen = init_plc_io_monitor_screen();
    if(plc_io_monitor_screen){
        gui_hide_widget(plc_io_monitor_screen);
        gui_widget_link(main_screen, plc_io_monitor_screen);
    }

    init_password_dialog();   /* �������봰�� */
    init_register_dialog();   /* ע�ᴰ�� */
    init_yes_or_no_dialog();  /* Y/Nȷ�ϴ��� */

    return;

some_error:
    sys_print("init_main_screen(): failed to create widgets for main screen!\n");
    ExitApplication();
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
    gui_set_root_widget(main_screen);
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
    
    gui_hide_widget(order_shouldbe_preview);
    gui_widget_link(main_screen, password_dialog);
    retval = check_password_dialog(160, 432, password);
    gui_widget_unlink(main_screen, password_dialog);
    gui_show_widget(order_shouldbe_preview);

    return retval;
}

static BOOL check_confirm(char * caption, char * text)
{
    BOOL retval;

    gui_hide_widget(order_shouldbe_preview);
    gui_widget_link(main_screen, yes_no_dialog);
    retval = check_yes_no_dialog(160, 432, caption, text);
    gui_widget_unlink(main_screen, yes_no_dialog);
    gui_show_widget(order_shouldbe_preview);

    return retval;
}

void cmd_input_order(void * data)
{
    data = data;
    if(input_order_screen){
        gui_hide_widget(order_shouldbe_preview);
        gui_hide_widget(dashedline_in_main);
        gui_hide_widget(main_order_view);
        order_edit();
        gui_show_widget(main_order_view);
        gui_show_widget(dashedline_in_main);
        gui_show_widget(order_shouldbe_preview);
    } else {
        ___alert("���ش���: �����༭���治����!", "Fatal: No Input Screen!");
    }
}

void cmd_move_order(void * data)
{
    data = data;
    if(get_order_nr() <= 0){
        ___alert("��ǰû�ж���, �޷�����!", "No Order to move!");
        return;
    }
    if(move_order_screen){
        gui_hide_widget(order_shouldbe_preview);
        gui_hide_widget(dashedline_in_main);
        move_order();
        gui_show_widget(dashedline_in_main);
        gui_show_widget(order_shouldbe_preview);
    } else {
        ___alert("���ش���:�������治����!", "Fatal: No Order-Move Screen!");
    }
}

void cmd_remove_order(void * data)
{
    data = data;
    if(get_order_nr() <= 0){
        ___alert("��ǰû�ж���, �޷�ɾ��!", "No Order to Delete!");
        return;
    }
    if(check_confirm(pick_string("ɾ������","Delete"), 
                     pick_string("Ҫ��ɾ����������?(Y/N)", "Delete this Order?(Y/N)"))){
        DeleteOrder(gui_view_get_selected(main_order_view));
    }
}

void cmd_pack_workno(void * data)
{
    data = data;
    if(get_order_nr() <= 0){
        ___alert("��ǰû�ж���, �޷�����!", "No Order to Resort");
        return;
    }
    if(check_confirm(pick_string("����", "Resort"), 
                     pick_string("Ҫ�����š�������?(Y/N)", "Confirm to Resort?(Y/N)"))){
        if(!PackWorkNo(10))
            ___alert("��������ʧ��!", "Failed to Resort!");
    }
}

void cmd_main_SEND(void * data)
{
    int machine;
    char ___s[96];
    order_struct order;
    int old_x, old_y;
    extern gui_widget * send_screen;

    machine = (int)data;

    if((machine == 1) && (!( config.slc_used & 1))){
        ___alert("��[1]û��ʹ��, �޷�ǿ��!", "SLC-1 is not in use, No Sending!");
        return;
    }
    if((machine == 2) && (!( config.slc_used & 2))){
        ___alert("��[2]û��ʹ��, �޷�ǿ��!", "SLC-2 is not in use, No Sending!");
        return;
    }
    
    machine--;

    if(get_order_nr() <= 0){
        ___alert("��ǰû�ж���, ����ǿ��!", "No Order to Send!");
        return;
    }
    if(machine != 0 && machine != 1){
        ___alert("�������뷢�͵���̨������???", "Which machine???");
        return;
    }
    sprintf(___s, pick_string("Ҫ�����͡���������[%d]��?(Y/N)", "Confirm to Send Order to [%d] (Y/N)?"), (machine+1));
    if(check_confirm(pick_string("ǿ��", "Send"), ___s)){
        lock_kernel();
        LoadOrder(&order, gui_view_get_selected(main_order_view), 1);
        old_x = send_screen->rect.x;
        old_y = send_screen->rect.y;
        unlock_kernel();
        gui_set_widget_location(send_screen, 160, 429);
        slc_send_order(machine+1, &order, 0);
        gui_set_widget_location(send_screen, old_x, old_y);
    }
}

void cmd_main_MONITOR(void * data)
{
    int machine;

    machine = (int)data;

    if((machine == 1) && (!( config.slc_used & 1))){
        ___alert("��[1]û��ʹ��, �޷����!", "SLC-1 is not in use, No Monitoring!");
        return;
    }
    if((machine == 2) && (!( config.slc_used & 2))){
        ___alert("��[2]û��ʹ��, �޷����!", "SLC-2 is not in use, No Monitoring!");
        return;
    }

    if(slc_monitor_screen){
        enter_slc_monitor(machine);
        gui_set_root_widget(main_screen);
    } else {
        ___alert("���ش���: SLC��ػ��治����!", "Fatal: No Monitor Screen!");
    }
}

void cmd_main_PARAM(void * data)
{
    int machine;

    machine = (int)data;

    if((machine == 1) && (!( config.slc_used & 1))){
        ___alert("��[1]û��ʹ��, �޷��޸������!", "SLC-1 is not in use, No Setup!");
        return;
    }
    if((machine == 2) && (!( config.slc_used & 2))){
        ___alert("��[2]û��ʹ��, �޷��޸������!", "SLC-2 is not in use, No Setup!");
        return;
    }

    if(check_password(PASSWORD_SLC_PARAM)){
        if(slc_param_screen){
            enter_slc_param_setup(machine);
            gui_set_root_widget(main_screen);
        } else {
            ___alert("���ش���: SLC�������治����!", "Fatal: No Setup Screen!");
        }
    }
    set_buttons_caption_to_default();
}

void setup_slc_param(void * data)
{
    data = data;

    if(slc_global_param){
        if(check_password(PASSWORD_SYS_PARAM)){
            gui_set_root_widget(slc_global_param);
            setup_slc_global_param();
            gui_set_root_widget(main_screen);
        }
    } else {
        ___alert("���ش���: SLCϵͳ�������治����!", "Fatal: No SLC Setup Screen");
    }
    set_buttons_caption_to_default();
}

void cmd_main_WHET(void * data)
{
    int machine;

    machine = (int)data;

    if((machine == 1) && (!( config.slc_used & 1))){
        ___alert("��[1]û��ʹ��, �޷�����ĥ������!", "SLC-1 is not in use, No Whetting!");
        return;
    }
    if((machine == 2) && (!( config.slc_used & 2))){
        ___alert("��[2]û��ʹ��, �޷�����ĥ������!", "SLC-2 is not in use, No Whetting!");
        return;
    }

    if(slc_whet_screen){
        gui_hide_widget(order_shouldbe_preview);
        gui_hide_widget(dashedline_in_main);
        gui_hide_widget(main_order_view);
        setup_whet(machine);
        gui_show_widget(main_order_view);
        gui_show_widget(dashedline_in_main);
        gui_show_widget(order_shouldbe_preview);
    } else {
        ___alert("���ش���: SLCĥ�����治����!", "Fatal: No Whet Screen!");
    }
    set_buttons_caption_to_default();
}

void cmd_main_LB(void * data)
{
    data = data;

    if(slc_lb_screen){
        if(check_password(PASSWORD_LB_SETUP)){
            gui_hide_widget(order_shouldbe_preview);
            gui_hide_widget(dashedline_in_main);
            gui_hide_widget(main_order_view);
            gui_show_widget(slc_lb_screen);
            lb_setup();
            gui_hide_widget(slc_lb_screen);
            gui_show_widget(main_order_view);
            gui_show_widget(dashedline_in_main);
            gui_show_widget(order_shouldbe_preview);
        }
    } else {
        ___alert("���ش���: �����ǳ���治����!", "Fatal: No Flute Setup Screen!");
    }
    set_buttons_caption_to_default();
}


void cmd_main_browse(void * data)
{
    KEYCODE key;

    message(pick_string(
                "�鿴������������: Up, Down, PgUp, PgDn, Home, End", 
                "Cmds to View Orders: Up, Down, PgUp, PgDn, Home, End"));

    key = (KEYCODE)(INT32U)data;
    switch(key){
        case UP:
            gui_view_move_up(main_order_view);
            break;
        case DOWN:
            gui_view_move_down(main_order_view);
            break;
        case PGUP:
            gui_view_page_up(main_order_view);
            break;
        case PGDN:
            gui_view_page_down(main_order_view);
            break;
        case HOME:
            gui_view_goto_top(main_order_view);
            break;
        case END:
            gui_view_goto_bottom(main_order_view);
            break;
        default:
            break;
    }
}

void cmd_main_ID(void * data)
{
    gui_hide_widget(order_shouldbe_preview);
    gui_hide_widget(dashedline_in_main);
    gui_hide_widget(main_order_view);
    gui_widget_link(main_screen, register_dialog);
    check_register_dialog(256, 202, (int)data);
    set_buttons_caption_to_default();
    gui_widget_unlink(main_screen, register_dialog);
    gui_show_widget(main_order_view);
    gui_show_widget(dashedline_in_main);
    gui_show_widget(order_shouldbe_preview);
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
    extern gui_widget * about_system_root;
    data = data;
    gui_set_root_widget(about_system_root);
    while(NONE_KEY == waitkey(0L));
    clrkey();
    set_buttons_caption_to_default();
    gui_set_root_widget(main_screen);
}

void cmd_main_PALETTE(void * data)
{
    extern gui_widget * palette_screen;
    data = data;
    gui_set_root_widget(palette_screen);
    while(NONE_KEY == waitkey(0L));
    clrkey();
    set_buttons_caption_to_default();
    gui_set_root_widget(main_screen);
}

void cmd_main_QUIT(void * data)
{
    data = data;

    if(check_confirm(pick_string(" >>> �˳� <<<", ">>> Quit <<<"), 
                     pick_string("Ҫ���˳�����ѹ��������?(Y/N)", "Confirm to \"Quit\" program?(Y/N)"))){
#if CONFIG_NOACTION_ON_QUIT != 1
        /* ... */
        reset_kl_on_quit();
#endif
        ExitApplication();
    }
    set_buttons_caption_to_default();
}

void cmd_slc_MALF(void * data)
{
    data = data;
    if(slc_malfunction_screen){
        gui_hide_widget(order_shouldbe_preview);
        gui_hide_widget(dashedline_in_main);
        gui_hide_widget(main_order_view);
        gui_show_widget(slc_malfunction_screen);
        slc_malfunction_monitor();
        gui_hide_widget(slc_malfunction_screen);
        gui_show_widget(main_order_view);
        gui_show_widget(dashedline_in_main);
        gui_show_widget(order_shouldbe_preview);
    } else {
        ___alert("���ش���: SLC���ϼ�⻭�治����!", "Fatal: No Malfunction Screen!");
    }
    set_buttons_caption_to_default();
}

void cmd_cim_monitor(void * data)
{
    data = data;
    if(cim_link_mntr_screen){
        gui_hide_widget(order_shouldbe_preview);
        cim_link_monitor();
        gui_show_widget(order_shouldbe_preview);
    } else {
        ___alert("���ش���: ���߼�ػ��治����!", "Fatal: No CIM-LINK Monitor Screen!");
    }
    set_buttons_caption_to_default();
}

void cmd_plc_mntr(void * data)
{
    data = data;
    if(plc_monitor_screen){
        gui_hide_widget(order_shouldbe_preview);
        gui_hide_widget(dashedline_in_main);
        gui_hide_widget(main_order_view);
        gui_show_widget(plc_monitor_screen);
        plc_monitor();
        gui_hide_widget(plc_monitor_screen);
        gui_show_widget(main_order_view);
        gui_show_widget(dashedline_in_main);
        gui_show_widget(order_shouldbe_preview);
    } else {
        ___alert("���ش���: PLC��ػ��治����!", "Fatal: No PLC Monitor Screen!");
    }
    set_buttons_caption_to_default();
}

void cmd_plc_io_mntr(void * data)
{
    data = data;
    if(plc_io_monitor_screen){
        gui_hide_widget(order_shouldbe_preview);
        plc_io_monitor();
        gui_show_widget(order_shouldbe_preview);
    } else {
        ___alert("���ش���: I/O���ػ��治����!", "Fatal: No I/O Monitor Screen!");
    }
    set_buttons_caption_to_default();
}


void cmd_main_CIM(void * data)
{
    data = data;
    lock_kernel();
    tmp_config.cim_link = !tmp_config.cim_link;
    unlock_kernel();
    save_tmp_config();
    if(tmp_config.cim_link){
        open_cim_link();
        gui_set_widget_color(main_buttons[6], COLOR_BLACK);
        gui_set_widget_bkcolor(main_buttons[6], COLOR_GREEN);
    } else {
        close_cim_link();
        gui_set_widget_color(main_buttons[6], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[6], BUTTON_BKCOLOR);
    }
}

void cmd_main_KL_AUTO(void * data)
{
    data = data;
    lock_kernel();
    tmp_config.auto_kl = !tmp_config.auto_kl;
    unlock_kernel();
    save_tmp_config();
    if(tmp_config.auto_kl){
        gui_set_widget_color(main_buttons[2], COLOR_BLACK);
        gui_set_widget_bkcolor(main_buttons[2], COLOR_GREEN);
    } else {
        gui_set_widget_color(main_buttons[2], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[2], BUTTON_BKCOLOR);
    }
}

void cmd_main_YX_AUTO(void * data)
{
    data = data;
    lock_kernel();
    tmp_config.auto_yx = !tmp_config.auto_yx;
    unlock_kernel();
    save_tmp_config();
    if(tmp_config.auto_yx){
        gui_set_widget_color(main_buttons[3], COLOR_BLACK);
        gui_set_widget_bkcolor(main_buttons[3], COLOR_GREEN);
    } else {
        gui_set_widget_color(main_buttons[3], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[3], BUTTON_BKCOLOR);
    }
}

void cmd_main_FAN_AUTO(void * data)
{
    data = data;
    lock_kernel();
    tmp_config.auto_fan = !tmp_config.auto_fan;
    unlock_kernel();
    save_tmp_config();
    if(tmp_config.auto_fan){
        gui_set_widget_color(main_buttons[4], COLOR_BLACK);
        gui_set_widget_bkcolor(main_buttons[4], COLOR_GREEN);
    } else {
        gui_set_widget_color(main_buttons[4], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[4], BUTTON_BKCOLOR);
    }
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
#if CONFIG_FUNC_BUTTON_BIGFONT == 1

INT08S *main_default_functions_zh[] = {
        "�䵥 F1", "���� F2", "ɾ�� F3", "���� F4", "ǿ��1 F5", "ǿ��2 F6",
        "���1 F7", "���2 F8", NULL,
};
INT08S *main_default_functions_en[] = {
        "Input[F1]", "Move[F2]", "Del[F3]", "Resort[F4]", "M1-Sd[F5]", "M2-Sd[F6]",
        "M1-Mr[F7]", "M2-Mr[F8]", NULL,
};
INT08S *main_ctrl_functions_zh[] = {
        "����1 F1", "����2 F2", "",
        "ĥ��1 F4", "ĥ��2 F5", "",
        "���� F7", "�˳� END", NULL,
};
INT08S *main_ctrl_functions_en[] = {
        "Setup1[F1]", "Setup2[F2]", "",
        "Whet1[F4]",  "Whet2[F5]", "",
        "MIS[F7]", "Quit[END]", NULL,
};

#else

INT08S *main_default_functions_zh[] = {
        "�䵥[F1]", "����[F2]", "ɾ��[F3]", "����[F4]", "��1ǿ��[F5]", "��2ǿ��[F6]",
        "��1���[F7]", "��2���[F8]", NULL,
};
INT08S *main_default_functions_en[] = {
        "Input[F1]", "Move[F2]", "Delete[F3]", "Resort[F4]", "M1-Send[F5]", "M2-Send[F6]",
        "M1-Mntr[F7]", "M2-Mntr[F8]", NULL,
};
INT08S *main_ctrl_functions_zh[] = {
        "��1����[F1]", "��2����[F2]", "",
        "��1ĥ��[F4]", "��2ĥ��[F5]", "",
        "��������[F7]", "�˳�[END]", NULL,
};
INT08S *main_ctrl_functions_en[] = {
        "M1-Setup[F1]", "M2-Setup[F2]", "",
        "M1-Whet[F4]",  "M2-Whet[F5]", "",
        "CIM-Link[F7]", "Quit[END]", NULL,
};

#endif

INT08S *main_alt_functions_zh[] = {
        "ϵͳ����[F1]", "�����ǳ[F2]", "�Զ��µ�[F3]", "ѹ���Զ�[F4]", "�����Զ�[F5]", "", "", "ϵͳ[F8]", NULL,
};
INT08S *main_alt_functions_en[] = {
        "Setup[F1]", "Deep[F2]", "Auto-KL[F3]", "Auto-Prs[F4]", "Auto-Fan[F5]", "", "", "System[F8]", NULL,
};
INT08S *main_shift_functions_zh[] = {
        "PLC��д[F1]",  "I/O��[F2]", "���ϼ��[F3]", "���ߴ���[F4]", "", "�汾��[F6]", "ID [F7]", "ϵͳ[F8]", NULL,
};
INT08S *main_shift_functions_en[] = {
        "PLC-RW[F1]",  "I/O[F2]", "Fault[F3]", "Link-COM[F4]", "", "Version[F6]", "ID [F7]", "System[F8]", NULL,
};

INT08S **main_default_functions = main_default_functions_zh;
INT08S **main_ctrl_functions    = main_ctrl_functions_zh;
INT08S **main_alt_functions     = main_alt_functions_zh;
INT08S **main_shift_functions   = main_shift_functions_zh;

void set_buttons_caption_to_default(void)
{
    int i;
    message(pick_string("����CTRL, ALT��SHIFT�ɲ鿴��������",
                        "Press CTRL, ALT or SHIFT, there will be more functions"));
    for(i=0; i<main_buttons_max; i++){
        gui_set_widget_color(main_buttons[i], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[i], BUTTON_BKCOLOR);
        #if CONFIG_FUNC_BUTTON_BIGFONT == 1
        gui_set_widget_font(main_buttons[i], font24);
        #endif
    }
    set_buttons_caption(main_default_functions);
}

void set_buttons_caption_to_alt(void)
{
    int i;
    for(i=0; i<main_buttons_max; i++){
        gui_set_widget_color(main_buttons[i], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[i], BUTTON_BKCOLOR);
        #if CONFIG_FUNC_BUTTON_BIGFONT == 1
        gui_set_widget_font(main_buttons[i], font16);
        #endif
    }
    if(tmp_config.auto_kl){
        gui_set_widget_color(main_buttons[2], COLOR_BLACK);
        gui_set_widget_bkcolor(main_buttons[2], COLOR_GREEN);
    }
    if(tmp_config.auto_yx){
        gui_set_widget_color(main_buttons[3], COLOR_BLACK);
        gui_set_widget_bkcolor(main_buttons[3], COLOR_GREEN);
    }
    if(tmp_config.auto_fan){
        gui_set_widget_color(main_buttons[4], COLOR_BLACK);
        gui_set_widget_bkcolor(main_buttons[4], COLOR_GREEN);
    }
    set_buttons_caption(main_alt_functions);
}

void set_buttons_caption_to_ctrl(void)
{
    int i;
    for(i=0; i<main_buttons_max; i++){
        gui_set_widget_color(main_buttons[i], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[i], BUTTON_BKCOLOR);
        #if CONFIG_FUNC_BUTTON_BIGFONT == 1
        gui_set_widget_font(main_buttons[i], font24);
        #endif
    }
    if(tmp_config.cim_link){
        gui_set_widget_color(main_buttons[6], COLOR_BLACK);
        gui_set_widget_bkcolor(main_buttons[6], COLOR_GREEN);
    }
    set_buttons_caption(main_ctrl_functions);
}

void set_buttons_caption_to_shift(void)
{
    int i;
    for(i=0; i<main_buttons_max; i++){
        gui_set_widget_color(main_buttons[i], BUTTON_COLOR);
        gui_set_widget_bkcolor(main_buttons[i], BUTTON_BKCOLOR);
        #if CONFIG_FUNC_BUTTON_BIGFONT == 1
        gui_set_widget_font(main_buttons[i], font16);
        #endif
    }
    set_buttons_caption(main_shift_functions);
}

void cmd_main_NONE(void * data)
{
    data = data;
    set_buttons_caption_to_default();
}


button_cmd_t cmd_main[] = {
    #define __(x) (void*)(INT32U)(x)
   /* �����ܼ�     */
    { F1,          0, cmd_input_order,      NULL,      "����һ���µĶ���, ����������Ѿ�����, �Ǿͳ�Ϊ�޸���!" },
    { F2,          0, cmd_move_order,       NULL,      "��������������˳��" },
    { F3,          0, cmd_remove_order,     NULL,      "\"ɾ��\"���ѡ�еĶ���!!!" },
    { F4,          0, cmd_pack_workno,      NULL,      "�������ж����Ĺ���" },
    { F5,          0, cmd_main_SEND,        __(1),     "��ѡ�еĶ����͵���1" },
    { F6,          0, cmd_main_SEND,        __(2),     "��ѡ�еĶ����͵���2" },
    { F7,          0, cmd_main_MONITOR,     __(1),     "��ػ�1�ĵ���λ�ü�����״̬" },
    { F8,          0, cmd_main_MONITOR,     __(2),     "��ػ�2�ĵ���λ�ü�����״̬" },
   /* �������     */
    { UP,          0, cmd_main_browse,      __(UP),    "" },
    { DOWN,        0, cmd_main_browse,      __(DOWN),  "" },
    { PGUP,        0, cmd_main_browse,      __(PGUP),  "" },
    { PGDN,        0, cmd_main_browse,      __(PGDN),  "" },
    { HOME,        0, cmd_main_browse,      __(HOME),  "" },
    { END,         0, cmd_main_browse,      __(END),   "" },
   /* NONE_KEY     */
    { NONE_KEY,    0, cmd_main_NONE,        NULL,      "" },
   /* CTRL-F#      */
    { CTRL,        0, cmd_main_CTRL,        NULL,      "" },
    { CTRL_F1,     0, cmd_main_PARAM,       __(1),     "��1��������, ��Ҫ�ǵ�����λ����" },
    { CTRL_F2,     0, cmd_main_PARAM,       __(2),     "��2��������, ��Ҫ�ǵ�����λ����" },
    { CTRL_F3,     0, NULL            ,     NULL,      "" },
    { CTRL_F4,     0, cmd_main_WHET,        __(1),     "��1ĥ������" },
    { CTRL_F5,     0, cmd_main_WHET,        __(2),     "��2ĥ������" },
    { CTRL_F6,     0, NULL            ,     NULL,      "" },
    { CTRL_F7,     0, cmd_main_CIM,         NULL,      "�������߹���, ��ɫ��ʾ������" },
    { CTRL_END,    0, cmd_main_QUIT   ,     NULL,      "�˳���ѹ������" },
   /* ALT-F#       */
    { ALT,         0, cmd_main_ALT,         NULL,      "" },
    { ALT_F1,      0, setup_slc_param,      NULL,      "ϵͳ����������, ֻ�й���Ա�ſ��޸�" },
    { ALT_F2,      0, cmd_main_LB,          NULL,      "�����ǳ����" },
    { ALT_F3,      0, cmd_main_KL_AUTO,     NULL,      "��λ��ɺ�, �Ƿ�Ҫ�Զ��µ�, ����ɫΪ�Զ�" },
    { ALT_F4,      0, cmd_main_YX_AUTO,     NULL,      "ѹ����/�Զ��л�, ����ɫΪ�Զ�" },
    { ALT_F5,      0, cmd_main_FAN_AUTO,    NULL,      "������/�Զ��л�, ����ɫΪ�Զ�" },
    { ALT_F6,      0, NULL            ,     NULL,      "" },
    { ALT_F7,      0, NULL            ,     NULL,      "" },
    { ALT_F8,      0, cmd_main_SYS,         NULL,      "System Monitor" },
   /* SHIFT-F#     */
    { SHIFT,       0, cmd_main_SHIFT,       NULL,      "" },
    { SHIFT_F1,    0, cmd_plc_mntr,         NULL,      "PLC�ݴ����ļ��, ���ڵ���"  },
    { SHIFT_F2,    0, cmd_plc_io_mntr,      NULL,      "PLC I/O��ļ��, ���ڵ���[ESC����]" },
    { SHIFT_F3,    0, cmd_slc_MALF,         NULL,      "��ѹ�����ϼ��" },
    { SHIFT_F4,    0, cmd_cim_monitor,      NULL,      "����������ߴ������շ�������[ESC����]" },
    { SHIFT_F5,    0, NULL            ,     NULL,      "" },
    { SHIFT_F6,    0, cmd_main_VER,         NULL,      "" },
    { SHIFT_F7,    0, cmd_main_ID,          __(0),     "System ID/SN" },
    { SHIFT_F8,    0, cmd_main_SYS,         NULL,      "System Monitor" },
   /* ע���˳���   */
    { RS_EXIT_KEY, 0, cmd_main_ID,          __(1),     "System ID/SN" },
   /* ������       */
    { SHIFT|CTRL,  0, cmd_main_SYS,         NULL,      "" },
    { 0x1900,      0, cmd_main_PALETTE,     NULL,      "ALT-P, Display System Palette" },
    { ESC,         0, NULL            ,     NULL,      "��ʾ: ���������, ��Ҫ��Ϣ��Ϣ! ף������˳��, Ц�ڳ���!" },
   /* ������־     */
    { NONEKEY, },
    #undef __
};

static void ____set_buttoncmds_comment(void)
{
    /* ����Ӣ�İ汾��ע���ַ��� */
    #define ____do_set(key, en_str) \
                buttoncmd_set_comment(cmd_main, key, pick_string(NULL, en_str))

    ____do_set(F1, "Input a Order and Insert it or Modify it if S/N already exist!");
    ____do_set(F2, "Adjust the Sequence of Order");
    ____do_set(F3, "Delete Order Selected");
    ____do_set(F4, "Repack S/N");
    ____do_set(F5, "Send Order to M1");
    ____do_set(F6, "Send Order to M2");
    ____do_set(F7, "Monitor the State of M1");
    ____do_set(F8, "Monitor the State of M2");

    ____do_set(CTRL_F1, "M1 Parameters Setup");
    ____do_set(CTRL_F2, "M2 Parameters Setup");

    ____do_set(CTRL_F4, "M1 Whet Setup");
    ____do_set(CTRL_F5, "M2 Whet Setup");

    ____do_set(CTRL_F7, "Enable/Disable CIM-LINK");

    ____do_set(CTRL_END, "Exit Program!");

    ____do_set(ALT_F1, "SLC Global Parameters Setup");
    ____do_set(ALT_F2, "Flute Depth Setup");
    ____do_set(ALT_F3, "Auto K/L down when Fixed");
    ____do_set(ALT_F4, "Auto switch Press Type");
    
    ____do_set(SHIFT_F1, "PLC Monitor, Used for Debug!");
    ____do_set(SHIFT_F2, "PLC I/O Monitor, Used for Debug [ESC Return]");
    ____do_set(SHIFT_F3, "Malfunction View");
    ____do_set(SHIFT_F4, "Monitor the COM-PORT for CIM-LINK");

    ____do_set(ESC, "");
}


/*-----------------------------------------------------------------------------------------
 *          
 *      ������ʾ����(main_order_view)����ض���
 * 
**---------------------------------------------------------------------------------------*/
enum order_view_fields_id {
    __id_wkno,
    __id_odrno,
    __id_cuts,
    __id_data,
    __id_yx,
    __id_flute,
    __id_trim,
    __id_commt,
};

static view_fields_t order_view_fields[] =
{
#define ____style        0
#define ____draw_style   DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER
    { "����",           __id_wkno,  4,  6,   ____style,  ____draw_style,  "", },
    { "������",         __id_odrno, 8,  12,  ____style,  ____draw_style,  "", },
    { "��",             __id_cuts,  2,  5,   ____style,  ____draw_style,  "", },
    { "ѹ      ��      ��      ��", 
                        __id_data,  64, 66,  ____style,  DRAW_OPT_FIL_BG, "", },
    { "ѹ��",           __id_yx,    1,  6,   ____style,  ____draw_style,  "", },
    { "���",           __id_flute, 3,  6,   ____style,  ____draw_style,  "", },
    { "�ޱ�",           __id_trim,  1,  6,   ____style,  ____draw_style,  "", },
    { "��ע",           __id_commt, 7,  7,   ____style,  ____draw_style,  "", },
    { NULL, }
#undef ____style
#undef ____draw_style
};

#if 0  /* ��һ����ʾ�����ķ��� */
struct __order_old_buf { /* �����ַ����ĳ��Ȳ��ɳ�������ṹ�е�bytes_for_width�ֶ� */
    char workno[6];
    char orderno[12];
    char cuts[4];
    char speccut[70];
    char yx[4];
    char flute[6];
    char trim[4];
};

static void show_order_record(int index, int row, 
                              int x, int y, int width_zoom, int height, 
                              COLOR color, COLOR bkcolor, int font,
                              COLOR marker_color, COLOR marker_bkcolor,
                              void *old, int  *fields_width, INT16U option)
{
    struct __order_old_buf * record_buf_old;
    order_struct __order;
    int i, j, to_left;
    char ___s[8];
        
    FamesAssert(old);

    if(!old)
        return;

    record_buf_old = (struct __order_old_buf *)old;
    i = row;
    to_left = width_zoom;

    if(!GetOrderForView(&__order, index))
        return fail;
    
    if(index <= get_max_index()){
        /* ���� */
        j = 0;
        sprintf(___s, "%d", __order.WORKNO);       
        draw_font_for_widget(x+3, y, (fields_width[j]-to_left)-6, height, ___s, 
                             record_buf_old->workno, marker_color, marker_bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* ������ */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, __order.ORDERNO, 
                             record_buf_old->orderno, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* ���� */
        j++;
        sprintf(___s, "%d", __order.CUTS);       
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, ___s, 
                             record_buf_old->cuts, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* ѹ������ */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, __order.SPECCUT, 
                             record_buf_old->speccut, color, bkcolor, font, DRAW_OPT_FIL_BG);
        x += fields_width[j];
        /* ѹ�� */
        j++;
        sprintf(___s, "%d", __order.YX);       
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, ___s, 
                             record_buf_old->yx, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* ��� */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, __order.FLUTE, 
                             record_buf_old->flute, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* �ޱ� */
        j++;
        sprintf(___s, "%s", __order.TRIM?"��":"����");       
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, ___s, 
                             record_buf_old->trim, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
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
        /* ���� */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->cuts, color, bkcolor, font, DRAW_OPT_FIL_BG);
        x += fields_width[j];
        /* ѹ������ */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->speccut, color, bkcolor, font, DRAW_OPT_FIL_BG);
        x += fields_width[j];
        /* ѹ�� */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->yx, color, bkcolor, font, DRAW_OPT_FIL_BG);
        x += fields_width[j];
        /* ��� */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->flute, color, bkcolor, font, DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER);
        x += fields_width[j];
        /* �ޱ� */
        j++;
        draw_font_for_widget(x, y, fields_width[j]-to_left, height, "", 
                             record_buf_old->trim, color, bkcolor, font, DRAW_OPT_FIL_BG);
        x += fields_width[j];
    }
}
#endif

static void show_statistics( int index,
                             int x, int y, int width, int height,
                             int color, int bkcolor, int font,
                             INT08S *comment, INT08S *old, INT16U option)
{
    char ___s[96];

    comment = comment;
    option  = option;
    
    sprintf(___s, 
            pick_string(" ��������: %d ��, ��ǰ����: %d", 
                        " Orders Nr: %d, Current: %d"),
            get_order_nr(), index+1);

    draw_font_for_widget(x, y, width, height, ___s, old, color, bkcolor, font, DRAW_OPT_FIL_BG);
}

static int order_get_max_index(void)
{
    return (get_order_nr()-1);
}

static BOOL order_view_get_data(int index, int field_id, char *buf, int buf_len, INT16U option)
{
    order_struct __order;
    BOOL retval;
    char ___s[256];
    INT16U used;

    FamesAssert(buf);
    if(!buf)
        return fail;

    option  = option;

    if(!GetOrderForView(&__order, index))
        return fail;
        
    if(index <= order_get_max_index()){
        retval = ok;
        switch(field_id){
            case __id_wkno:
                sprintf(___s, "%d", __order.WORKNO);
                break;
            case __id_odrno:
                sprintf(___s, "%s", __order.ORDERNO);
                break;
            case __id_cuts:
                sprintf(___s, "%d", __order.CUTS);
                break;
            case __id_data:
                sprintf(___s, "%s", __order.SPECCUT);
                break;
            case __id_yx:
                sprintf(___s, "%s", get_yx_string(__order.YX));
                buf_len = STRLEN(___s);
                break;
            case __id_flute:
                sprintf(___s, "%s", __order.FLUTE);
                break;
            case __id_trim:
                sprintf(___s, "%s", __order.TRIM?pick_string("��", "YES"):pick_string("����", "NO"));
                buf_len = 4;
                break;
            case __id_commt:
                used = 0;
                if(config.slc_used & 1){
                    if(similar_order(&__order, &(config.slc[0].working)))
                        used |= 1;
                }
                if(config.slc_used & 2){
                    if(similar_order(&__order, &(config.slc[1].working)))
                        used |= 2;
                }
                switch(used){
                    case 1:
                        sprintf(___s, "[ 1 ]");
                        break;
                    case 2:
                        sprintf(___s, "[ 2 ]");
                        break;
                    case 3:
                        sprintf(___s, "[1][2]");
                        break;
                    default:
                        sprintf(___s, "");
                        break;
                }
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

static void notifier_on_changed(int index, int prev_index, INT16U option)
{
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
        preview_set_order(order_shouldbe_preview, NULL);
    } else {
        preview_set_order(order_shouldbe_preview, &__order);
    }    
}

void init_view_private(void)
{
    /*
    printf("height_per_row = %4d\r", height_per_row);
    */
    order_view_fields[0].caption = pick_string("����",   "S/N");
    order_view_fields[1].caption = pick_string("������", "OrderNo");
    order_view_fields[2].caption = pick_string("��",     "Cuts");
    order_view_fields[3].caption = pick_string("ѹ      ��      ��      ��", "S L C   D A T A");
    order_view_fields[4].caption = pick_string("ѹ��",   "Press");
    order_view_fields[5].caption = pick_string("���",   "Flute");
    order_view_fields[6].caption = pick_string("�ޱ�",   "Trim");
    order_view_fields[7].caption = pick_string("��ע",   "Used");

    gui_view_init_private( main_order_view, 
                           order_view_fields, 
                           order_get_max_index,
                           order_view_get_data,
                           NULL,
                           NULL,
                           NULL,
                           pick_string(" ͳ��:", "Stat:"), 
                           show_statistics, 
                           notifier_on_changed,
                           atoi(___color_order_data), /* 123, */
                           atoi(___bkcolor_order_data), /* WIDGET_BKCOLOR+65, */
                           42, /*height_per_row*/
                           1
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
    extern gui_widget * slc_mntr_cpu_status;
    extern gui_widget * slc_param_cpu_status;
    extern gui_widget * slc_send_movement;
    
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
    if(cim_link_status){ /* ��������״̬����ʾ */
        static int linked = 0;
        static int unlinked = 0;
        if(edge_up((tmp_config.cim_link), linked)){
            gui_set_widget_color(cim_link_status, 0);
            gui_set_widget_bkcolor(cim_link_status, 120);
            gui_set_widget_style(cim_link_status, LABEL_STYLE_CLIENT_BDR|LABEL_ALIGN_CENTER);
        }
        if(edge_dn((tmp_config.cim_link), unlinked)){
            gui_set_widget_color(cim_link_status, 0);
            gui_set_widget_bkcolor(cim_link_status, 0);
            gui_set_widget_style(cim_link_status, LABEL_STYLE_CLIENT_BDR|LABEL_ALIGN_CENTER);
        }
        if(tmp_config.cim_link){
            extern cim_link_type * cim_link_protocol;
            static int old = 0, cntr = 0;
            static int timeout = 0, blink = 0;
            int ___t;
            lock_kernel();
            if(cim_link_protocol){
                ___t = cim_link_protocol->rx_packets_nr;
            }
            unlock_kernel();
            timeout++;
            if(old != ___t){
                cntr++;
                timeout = 0;
                old = ___t;
                if(!blink){
                    gui_set_widget_color(cim_link_status, 0);
                    gui_set_widget_bkcolor(cim_link_status, COLOR_CIM_LINK_RX);
                    blink = 1;
                }
            }
            if(timeout >= 30){ /* ���յ���3���Ӽ�ֹͣ��˸ */
                timeout = 30;
                if(blink){
                    gui_set_widget_color(cim_link_status, 0);
                    gui_set_widget_bkcolor(cim_link_status, 120);
                }
                blink = 0;
            }
            lock_kernel();
            sprintf(___s, "-> %u", ___t);
            unlock_kernel();
        } else {
            lock_kernel();
            sprintf(___s, "-/-");
            unlock_kernel();
        }
        gui_label_set_text(cim_link_status, ___s);
    }
    if(slc_1_status){ /* ��1��״̬��ʾ */
        static int used_this = 0;
        static int unused_this = 0;
        extern int plc_tick[];
        extern int plc_connected[];
        static int old_tick = -1, cntr = 0;
        static int timeout = 0, disconnected = 0;
        static int kl_down = 0, kl_up = 0;
        struct slc_run_state_s * state;
        if(edge_up((config.slc_used & 0x1), used_this)){
            gui_set_widget_color(slc_1_status, 0);
            gui_set_widget_bkcolor(slc_1_status, 0);
            gui_set_widget_style(slc_1_status, LABEL_STYLE_CLIENT_BDR);
        }
        if(edge_dn((config.slc_used & 0x1), unused_this)){
            gui_set_widget_color(slc_1_status, 0);
            gui_set_widget_bkcolor(slc_1_status, 0);
            gui_set_widget_style(slc_1_status, LABEL_STYLE_CLIENT_BDR|LABEL_ALIGN_CENTER);
        }
        if(config.slc_used & 0x1){
            int ___t;
            lock_kernel();
            ___t = plc_tick[0];
            unlock_kernel();
            timeout++;
            if(old_tick != ___t){
                cntr++;
                timeout = 0;
                old_tick = ___t;
                if(disconnected){
                    gui_set_widget_color(slc_1_status, 0);
                    gui_set_widget_bkcolor(slc_1_status, 0);
                    kl_down = 0;
                    disconnected = 0;
                }
                state = &config.slc[0].state;
                if(edge_up((state->k_down || state->l_down), kl_down)){ /* �����±�ʾ�������� */
                    gui_set_widget_bkcolor(slc_1_status, COLOR_SLC_RUNNING);
                    save_running_slc(1);
                }
                if(edge_dn((state->k_down || state->l_down), kl_up)){
                    gui_set_widget_bkcolor(slc_1_status, 0);
                }
            }
            if(timeout >= CONFIG_PLC_CONNECT_TIMEOUT){ /* PLC���߳�ʱ��ʼ���� */
                timeout = CONFIG_PLC_CONNECT_TIMEOUT;
                if(!disconnected){
                    gui_set_widget_color(slc_1_status, 0);
                    gui_set_widget_bkcolor(slc_1_status, COLOR_WARNING);
                }
                disconnected = 1;
            }
            lock_kernel();
            if(disconnected)
                sprintf(___s, pick_string("[1]: ����!","[1]: Off!"));
            else
                sprintf(___s, "[1]: %u", cntr);
            plc_connected[0] = !disconnected;
            unlock_kernel();
        } else {
            lock_kernel();
            sprintf(___s, "--");
            unlock_kernel();
            timeout = 0;            
            disconnected = 0;
        }
        gui_label_set_text(slc_1_status, ___s);
    }
    if(slc_2_status){ /* ��2��״̬��ʾ */
        static int used_this = 0;
        static int unused_this = 0;
        extern int plc_tick[];
        extern int plc_connected[];
        static int old_tick = -1, cntr = 0;
        static int timeout = 0, disconnected = 0;
        static int kl_down = 0, kl_up = 0;
        struct slc_run_state_s * state;
        if(edge_up((config.slc_used & 0x2), used_this)){
            gui_set_widget_color(slc_2_status, 0);
            gui_set_widget_bkcolor(slc_2_status, 0);
            gui_set_widget_style(slc_2_status, LABEL_STYLE_CLIENT_BDR);
        }
        if(edge_dn((config.slc_used & 0x2), unused_this)){
            gui_set_widget_color(slc_2_status, 0);
            gui_set_widget_bkcolor(slc_2_status, 0);
            gui_set_widget_style(slc_2_status, LABEL_STYLE_CLIENT_BDR|LABEL_ALIGN_CENTER);
        }
        if(config.slc_used & 0x2){
            int ___t;
            lock_kernel();
            ___t = plc_tick[1];
            unlock_kernel();
            timeout++;
            if(old_tick != ___t){
                cntr++;
                timeout = 0;
                old_tick = ___t;
                if(disconnected){
                    gui_set_widget_color(slc_2_status, 0);
                    gui_set_widget_bkcolor(slc_2_status, 0);
                    kl_down = 0;
                    disconnected = 0;
                }
                state = &config.slc[1].state;
                if(edge_up((state->k_down || state->l_down), kl_down)){ /* �����±�ʾ�������� */
                    gui_set_widget_bkcolor(slc_2_status, COLOR_SLC_RUNNING);
                    save_running_slc(2);
                }
                if(edge_dn((state->k_down || state->l_down), kl_up)){
                    gui_set_widget_bkcolor(slc_2_status, 0);
                }
            }
            if(timeout >= CONFIG_PLC_CONNECT_TIMEOUT){ /* PLC���߳�ʱ��ʼ���� */
                timeout = CONFIG_PLC_CONNECT_TIMEOUT;
                if(!disconnected){
                    gui_set_widget_color(slc_2_status, 0);
                    gui_set_widget_bkcolor(slc_2_status, COLOR_WARNING);
                }
                disconnected = 1;
            }
            lock_kernel();
            if(disconnected)
                sprintf(___s, pick_string("[2]: ����!","[2]: Off!"));
            else
                sprintf(___s, "[2]: %u", cntr);
            plc_connected[1] = !disconnected;
            unlock_kernel();
        } else {
            lock_kernel();
            sprintf(___s, "--");
            unlock_kernel();
            timeout = 0;
            disconnected = 0;
        }
        gui_label_set_text(slc_2_status, ___s);
    }
    if(main_cpu_status){ /* CPUʹ���ʵ���ʾ */
        lock_kernel();
        sprintf(___s, cpu_used_format, CPU_USED);
        unlock_kernel();
        gui_label_set_text(main_cpu_status, ___s);
    }
    if(slc_mntr_cpu_status){ /* CPUʹ���ʵ���ʾ */
        lock_kernel();
        sprintf(___s, cpu_used_format, CPU_USED);
        unlock_kernel();
        gui_label_set_text(slc_mntr_cpu_status, ___s);
    }
    if(slc_param_cpu_status){ /* CPUʹ���ʵ���ʾ */
        lock_kernel();
        sprintf(___s, cpu_used_format, CPU_USED);
        unlock_kernel();
        gui_label_set_text(slc_param_cpu_status, ___s);
    }
    if(order_shouldbe_preview && main_order_view){ /* ����Ԥ����ˢ�� */
        notifier_on_changed(gui_view_get_selected(main_order_view), 0, 0);
    }
    if(1){ /* ����������ص���ɫ */
        static int __t = 0, __cntr = 0;
        __cntr++;
        if(__cntr >= 2){
            __cntr = 0;
            if(__t){
                gdi_set_palette(COLOR_CIM_LINK_RX, 0x35, 0x34,0x32);
            } else {
                gdi_set_palette(COLOR_CIM_LINK_RX, 0x30, 0x0, 0x38);
            }
            __t = !__t;
        }
    }
    if(1){ /* SLC��־ɫ */
        static int __t = 0, __cntr = 0;
        __cntr++;
        if(__cntr >= 5){
            __cntr = 0;
            if(__t){
                gdi_set_palette(COLOR_SLC_RUNNING, 0x35, 0x34,0x32);
            } else {
                gdi_set_palette(COLOR_SLC_RUNNING, 0x0, 0x38, 0x0);
            }
            __t = !__t;
        }
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
    if(slc_send_movement){ /* ���Ͷ���ʱ��С���� */
        static int __cntr = 0, i = 0;
        static char * ___str[] = { ".", "..", "...", "....", ".....", "......", "" };
        __cntr++;
        if(__cntr >= 3){
            __cntr = 0;
            gui_label_set_text(slc_send_movement, ___str[i++]);
            if(i >= 7)
                i = 0;
        }
    }
    if(1){ /* ��ʾSLC������Ϣ */
        INT32U malf_code;
        int i, has_malf;
        static int has_malf_old = 0;
        char ___s[64];
        extern INT32S slc_malfunction[];

        #define  NR_MALFUNCTION  32  /* ����32�ֹ��� */
        
        has_malf = 0;

        /* �ȿ���1�� */
        lock_kernel();
        malf_code = (INT32U)slc_malfunction[0];
        unlock_kernel();
        for(i = 0; i < NR_MALFUNCTION; i++){
            if(malf_code & 0x1L)
                break;
            malf_code >>= 1;
        }
        if(i < NR_MALFUNCTION){
            sprintf(___s, pick_string("��1�й���: ", "M-1 fault: "));
            slc_malfunction_get_comment(&___s[STRLEN(___s)], 50, (i+1));
            has_malf = 1;
        }
        /* �ٿ���2�� */
        lock_kernel();
        malf_code = (INT32U)slc_malfunction[1];
        unlock_kernel();
        for(i = 0; i < NR_MALFUNCTION; i++){
            if(malf_code & 0x1L)
                break;
            malf_code >>= 1;
        }
        if(i < NR_MALFUNCTION){
            sprintf(___s, pick_string("��2�й���: ", "M-2 fault: "));
            slc_malfunction_get_comment(&___s[STRLEN(___s)], 50, (i+1));
            has_malf = 1;
        }

        if(has_malf){
            if(!has_malf_old){
                gui_set_widget_bkcolor(main_status_bar, COLOR_WARNING);
            }
            message(___s);
        } else {
            if(has_malf_old){
                gui_set_widget_bkcolor(main_status_bar, 0);
                message("");
            }
        }
        has_malf_old = has_malf;
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
    int running_slc, pending_slc;
    order_struct order;
    order_struct default_order = DFLT_order_struct;
    slc_descriptor_t * slc;

    TimerSet(TimerForMainScreen, 100L, TIMER_TYPE_AUTO, dpc_for_main, NULL);
    
    TaskDelay(300); /* ��0.3��ȴ����������ʾ, Ӧ�����㹻�˰� */

    running_slc = tmp_config.running_slc;
    switch (running_slc) {
        case 2:
            running_slc = 2;
            pending_slc = 1;
            break;
        case 0:
        case 1:
        default: /* �������Чֵ, ��ô��Ĭ��Ϊ�ǻ�1������ */
            running_slc = 1;
            pending_slc = 2;
            break;
    }

    if(config.slc_used & running_slc){
        slc = &config.slc[running_slc-1];
        set_plc_on_startup(running_slc);
        if (config.send_order_on_startup > _SendOrderMode_None) {
            if(GetOrderForView(&order, i++)) { /* �ж������� */
                if (config.send_order_on_startup > 10)
                    slc_send_order(running_slc, &order, 0); /* ���Ͷ���+���� */
                else
                    slc_send_order(running_slc, &order, 1); /* ���Ͷ����������� */
            } else { /* ��û�ж����������... */
                switch (config.send_order_on_startup) {
                    case _SendOrderMode_SendLastRunning:
                        slc_send_order(running_slc, &slc->working, 1); /* ���͹ػ�ǰ������������ */
                        break;
                    case _SendOrderMode_SendLastRunning_Control:
                        slc_send_order(running_slc, &slc->working, 0); /* ���͹ػ�ǰ����+���� */
                        break;
                    case _SendOrderMode_SendDefault:
                        slc_send_order(running_slc, &default_order, 1); /* ����Ĭ�϶����������� */
                        break;
                    case _SendOrderMode_SendDefault_Control:
                        slc_send_order(running_slc, &default_order, 0); /* ����Ĭ�϶���+���� */
                        break;
                    default:
                        break; /* ��������ֵ, ���ֶ���Ϊ��, �Ǿ�ʲôҲ�������� */
                }
            }
        }
    }

    if(config.slc_used & pending_slc){
        slc = &config.slc[pending_slc-1];
        set_plc_on_startup(pending_slc);
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
    }

    main_default_functions = pick_string(main_default_functions_zh, main_default_functions_en);
    main_ctrl_functions    = pick_string(main_ctrl_functions_zh,    main_ctrl_functions_en);
    main_alt_functions     = pick_string(main_alt_functions_zh,     main_alt_functions_en);
    main_shift_functions   = pick_string(main_shift_functions_zh,   main_shift_functions_en);

    ____set_buttoncmds_comment();

    for(;;){
        set_buttons_caption_to_default();
        do_buttoncmds(cmd_main, main_buttons, 8, 1,
            BUTTON_STYLE_CLIENT_BDR | BUTTON_STYLE_MODAL_FRAME | BUTTON_STYLE_PRESSED, 
            BUTTON_STYLE_CLIENT_BDR | BUTTON_STYLE_MODAL_FRAME);
    }
}

/*=========================================================================================
 * 
 * ���ļ�����: slc/main.c
 * 
**=======================================================================================*/

