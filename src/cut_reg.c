/******************************************************************************************
 * �ļ�:    cut/cut_reg.c
 *
 * ����:    ע��Ի���
 *
 * ����:    Jun
 *
 * ����:    2012-8-20, 2012-10-29
******************************************************************************************/
#define  CUT_REGISTER_C
#include <FamesOS.h>
#include "common.h"

/*-----------------------------------------------------------------------------------------
 *          
 *      �ؼ�����������
 * 
**---------------------------------------------------------------------------------------*/
static gui_window_t * register_window = NULL;

static gui_widget   * register_dialog = NULL;     /* ���ؼ� */

static gui_widget   * esc_button = NULL;
static gui_widget   * reg_button = NULL;

static gui_widget   * id_text    = NULL;
static gui_widget   * sn_text    = NULL;
static gui_widget   * id_label   = NULL;
static gui_widget   * sn_label   = NULL;


static int _____has_been_registered = 0;

extern BMPINFO icon; /* ͼ�� */
extern gui_window_t * main_window; /* �����洰�� */

extern char  ___bkcolor_other_dialog[];

static int __register_callbacks(int cmd, void * data);
static void __event_handler(int is_registered);

/*-----------------------------------------------------------------------------------------
 *          
 *      ע��Ի���Ĵ���
 * 
**---------------------------------------------------------------------------------------*/
void init_register_dialog(void)
{
    gui_widget * widget;
    gui_widget * form;
    int x, y, width, height, font, __t;
    COLOR bkcolor;
    
    x = 0;
    y = 0;
    width = 480;
    height = 202;
    font = font16;

    bkcolor = atoi(___bkcolor_other_dialog);

    /* ������ */
    widget = gui_create_widget(GUI_WIDGET_FORM, x, y, width, height, 0, bkcolor, font16, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE);
    form = widget;
    if(!widget)
        goto some_error;
    register_dialog = widget;
    register_window = gui_create_window(widget);
    gui_form_init_private(widget, 64);
    gui_form_set_icon(widget, &icon);
    gui_form_set_caption(widget, "-/-");

    #define ____text_x  68
    #define ____text_y  52
    #define ____text_h  30
    #define ____t_style EDIT_ALIGN_CENTER
    
    /* ����ID�� */
    widget = gui_create_widget(GUI_WIDGET_LABEL, ____text_x+0, ____text_y+0, 28, ____text_h, 0, 0, font, 0);
    if(!widget)
        goto some_error;
    gui_widget_link(form, widget);
    gui_label_init_private(widget, 32); /* �����32���ַ�����Ҫ��;����ʾע��ʧ��ʱ���˳���Ϣ */
    gui_label_set_text(widget, "ID");
    id_label = widget;
    widget = gui_create_widget(GUI_WIDGET_EDIT, ____text_x+32, ____text_y+0, 290, ____text_h, 0, WIDGET_BKCOLOR, font, ____t_style|EDIT_STYLE_READONLY);
    if(!widget)
        goto some_error;
    gui_widget_link(form, widget);
    gui_edit_init_private(widget, ID_MAX_LEN);
    gui_edit_set_text(widget, "");
    id_text = widget;

    /* ע����SN */
    widget = gui_create_widget(GUI_WIDGET_LABEL, ____text_x+0, ____text_y+45, 28, ____text_h, 0, 0, font, 0);
    if(!widget)
        goto some_error;
    gui_widget_link(form, widget);
    gui_label_init_private(widget, 8);
    gui_label_set_text(widget, "SN");
    sn_label = widget;
    widget = gui_create_widget(GUI_WIDGET_EDIT, ____text_x+32, ____text_y+45, 290, ____text_h, 0, 0, font, ____t_style);
    if(!widget)
        goto some_error;
    gui_widget_link(form, widget);
    gui_edit_init_private(widget, SN_MAX_LEN);
    gui_edit_set_text(widget, "N/A");
    sn_text = widget;

    #define ____button_style BUTTON_STYLE_CLIENT_BDR

    /* ���ذ�ť */
    __t  = (width-180);
    __t /= 3;
    widget = gui_create_widget(GUI_WIDGET_BUTTON, (__t-20), (height-56), 132, 36, 0, 0, font16, ____button_style);
    if(!widget)
        goto some_error;
    esc_button = widget;
    gui_widget_link(form, widget);
    gui_button_init_private(widget, 32);
    gui_button_set_caption(widget, pick_string("����[ESC]", "ESC"));

    /* ע�ᰴť */
    __t += 112;
    widget = gui_create_widget(GUI_WIDGET_BUTTON, (width-__t), (height-56), 132, 36, 0, 0, font16, ____button_style);
    if(!widget)
        goto some_error;
    reg_button = widget;
    gui_widget_link(form, widget);
    gui_button_init_private(widget, 32);
    gui_button_set_caption(widget, pick_string("ע��[F10]", "REG[F10]"));

    #undef ____button_style

    /* ע��RegisterService�ص�...... */
    register_service_request(RS_REQUEST_REGISTER_CALLBACK, __register_callbacks);

    return;

some_error:
    sys_print("init_register_dialog(): failed to create widgets!\n");
    ExitApplication();
}

/*-----------------------------------------------------------------------------------------
 * ����:    __waitkey_for_key()
 *
 * ����:    �ȴ��账��İ���
**---------------------------------------------------------------------------------------*/
static KEYCODE __waitkey_for_key(INT32U timeout)
{
    KEYCODE key;
    int  wait = 1;

    do {
        key = waitkey(timeout);
        switch (key) {
            case ENTER:
            case SPACE:
            case ESC:
            case TAB:
            case LEFT:
            case RIGHT:
            case UP:
            case DOWN:
            case SHIFT_TAB:
            case F10:
                wait = 0;
                break;
            default:
                break;
        }
    } while (wait);

    return key;
}

static void __select_button_state(int index)
{
    if (index == 1) {
        gui_set_widget_style(esc_button, BUTTON_STYLE_CLIENT_BDR|BUTTON_STYLE_PRESSED);
        gui_set_widget_style(reg_button, BUTTON_STYLE_CLIENT_BDR);
    } else {
        gui_set_widget_style(reg_button, BUTTON_STYLE_CLIENT_BDR|BUTTON_STYLE_PRESSED);
        gui_set_widget_style(esc_button, BUTTON_STYLE_CLIENT_BDR);
    }
}

/*-----------------------------------------------------------------------------------------
 * ����:    check_register_dialog
 *
 * ����:    ϵͳע�ἰID����ʾ
**---------------------------------------------------------------------------------------*/
void check_register_dialog(int x, int y, int need_quit)
{
    int loop, index;
    int input_sn;
    KEYCODE key;
    char user_input_sn[SN_MAX_LEN];
    char zh_title[8], en_title[12];

    #define ____button_style BUTTON_STYLE_CLIENT_BDR

    gui_set_widget_location(register_dialog, x, y);
    gui_show_window(register_window);

    /* ��ʾĬ�ϱ���..... Note: �޸�ʱ��ע������Ĵ�С....... */
    zh_title[0] = 0xd7; /* "ע��" */
    zh_title[1] = 0xa2;
    zh_title[2] = 0xb2;
    zh_title[3] = 0xe1;
    zh_title[4] = 0x0;
    en_title[0] = 'R';  /* "Register" */
    en_title[1] = 'e';
    en_title[2] = 'g';
    en_title[3] = 'i';
    en_title[4] = 's';
    en_title[5] = 't';
    en_title[6] = 'e';
    en_title[7] = 'r';
    en_title[8] = '\0';
    gui_form_set_caption(register_dialog, pick_string(zh_title, en_title));

    /* ��ID����Ч��SN............... */
    gui_edit_set_text(id_text, "--/--");
    gui_edit_set_text(sn_text, "--/--");
    register_service_request(RS_REQUEST_CHECK, NULL);
    register_service_request(RS_REQUEST_GET_ID, NULL);
    register_service_request(RS_REQUEST_GET_SN, NULL);

    _____has_been_registered = 0; /* prepare for need_quit */

    index = 1; /* Ĭ���Ƿ��� */
    __select_button_state(index);

    for (loop=1; loop;) {
        input_sn = 0;
        key = __waitkey_for_key(0L);
        switch (key) {
            case ESC:
                loop = 0;
                index = 1;
                break;
            case ENTER:
            case SPACE:
                if (index == 1) {
                    loop = 0;
                    break;
                }
                /* if (index == 2) fall through */
            case F10:
                index = 2;
                input_sn = 1;
                gui_form_set_caption(register_dialog, pick_string(zh_title, en_title));
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
                if (index == 1) {
                    index = 2;
                } else {
                    index = 1;
                }
                break;
            default:
                break;
        }

        __select_button_state(index);

        while (input_sn) { /* ����ע���� */
            user_input_sn[0] = 0;
            key = gui_edit_input(sn_text, user_input_sn, 23, 0);
            switch (key) {
                case ESC: /* return to caller */
                    loop = 0;
                    input_sn = 0;
                    break;
                case ENTER:
                case F10: /* ��SNע�� */
                    {
                        char title_zh[20], title_en[20];
                        title_zh[0]  = 0xd7; /* "ע����, ���Ժ�..." */
                        title_zh[1]  = 0xa2;
                        title_zh[2]  = 0xb2;
                        title_zh[3]  = 0xe1;
                        title_zh[4]  = 0xd6;
                        title_zh[5]  = 0xd0;
                        title_zh[6]  = 0x2c;
                        title_zh[7]  = 0x20;
                        title_zh[8]  = 0xc7;
                        title_zh[9]  = 0xeb;
                        title_zh[10] = 0xc9;
                        title_zh[11] = 0xd4;
                        title_zh[12] = 0xba;
                        title_zh[13] = 0xee;
                        title_zh[14] = 0x2e;
                        title_zh[15] = 0x2e;
                        title_zh[16] = 0x2e;
                        title_zh[17] = 0x0;
                        title_en[0]  = 'R'; /* "Registering..." */
                        title_en[1]  = 'e';
                        title_en[2]  = 'g';
                        title_en[3]  = 'i';
                        title_en[4]  = 's';
                        title_en[5]  = 't';
                        title_en[6]  = 'e';
                        title_en[7]  = 'r';
                        title_en[8]  = 'i';
                        title_en[9]  = 'n';
                        title_en[10] = 'g';
                        title_en[11] = '.';
                        title_en[12] = '.';
                        title_en[13] = '.';
                        title_en[14] = '\0';
                        gui_form_set_caption(register_dialog, pick_string(title_zh, title_en));
                        TaskSleep(1000UL);
                        register_service_request(RS_REQUEST_SN, user_input_sn);
                        input_sn = 0;
                    }
                    break;
                default: /* ������? ��������� */
                    break;
            }
            if (input_sn == 0) {
                index = 1; /* Ĭ��Ϊ���� */
                __select_button_state(index);
            }
        }
    }

    if (need_quit && _____has_been_registered == 0) {
        char quit_title_zh[16], quit_title_en[12];

        quit_title_zh[0] = 0xd5; /* �����˳�... */
        quit_title_zh[1] = 0xfd;
        quit_title_zh[2] = 0xd4;
        quit_title_zh[3] = 0xda;
        quit_title_zh[4] = 0xcd;
        quit_title_zh[5] = 0xcb;
        quit_title_zh[6] = 0xb3;
        quit_title_zh[7] = 0xf6;
        quit_title_zh[8] = 0xd6;
        quit_title_zh[9] = 0xd0;
        quit_title_zh[10] = 0x2e;
        quit_title_zh[11] = 0x2e;
        quit_title_zh[12] = 0x2e;
        quit_title_zh[13] = 0x0;
        quit_title_en[0] = 'Q'; /* Quiting... */
        quit_title_en[1] = 'u';
        quit_title_en[2] = 'i';
        quit_title_en[3] = 't';
        quit_title_en[4] = 'i';
        quit_title_en[5] = 'n';
        quit_title_en[6] = 'g';
        quit_title_en[7] = '.';
        quit_title_en[8] = '.';
        quit_title_en[9] = '.';
        quit_title_en[10] = '\0';

        gui_hide_widget(esc_button);
        gui_hide_widget(reg_button);
        gui_hide_widget(id_text);
        gui_hide_widget(sn_text);
        gui_hide_widget(id_label);
        gui_hide_widget(sn_label);
        /* ��id_label��ʾ�˳���Ϣ */
        gui_label_set_text(id_label, pick_string(quit_title_zh, quit_title_en));
        gui_set_widget_font(id_label, font24);
        gui_set_widget_location(id_label, 68, 42);
        gui_set_widget_dimension(id_label, 342, 142);
        gui_set_widget_color(id_label, COLOR_WARNING); /* COLOR_WARNING */
        gui_set_widget_style(id_label, LABEL_ALIGN_CENTER); /* ����, �ޱ߿� */
        gui_show_widget(id_label);
        TaskSleep(800UL);
        waitkey(1200UL);
        quit(); /* �˳�SLC���� */
    }

    gui_hide_window(register_window);
}

/*-----------------------------------------------------------------------------------------
 * ����:    __register_callbacks
 *
 * ����:    ע�������֮�ص�����
**---------------------------------------------------------------------------------------*/
static int __register_callbacks(int cmd, void * data)
{
    char id[ID_MAX_LEN + 2]; /* +2���ڴ���ַ�'\0' */
    char sn[SN_MAX_LEN + 2];
    char * s;
    int  len, i_data = fail;

    s = data;
    if (!s)
        s = "< Null >";
    len = STRLEN(s);

    switch (cmd) {
        case RS_REQUEST_GET_ID:
            if (len >= ID_MAX_LEN)
                len = ID_MAX_LEN;
            MEMCPY(id, s, len);
            id[len] = 0;
            gui_edit_set_text(id_text, id);
            break;
        case RS_REQUEST_GET_SN:
            if (len >= SN_MAX_LEN)
                len = SN_MAX_LEN;
            MEMCPY(sn, s, len);
            sn[len] = 0;
            gui_edit_set_text(sn_text, sn);
            break;
        case RS_REQUEST_CHECK:
            i_data = (int)data;
            _____has_been_registered = i_data;
            if (i_data) {
                char ok_title_zh[10], ok_title_en[16];
                ok_title_zh[0] = 0xd2; /* ��ע��! */
                ok_title_zh[1] = 0xd1;
                ok_title_zh[2] = 0xd7;
                ok_title_zh[3] = 0xa2;
                ok_title_zh[4] = 0xb2;
                ok_title_zh[5] = 0xe1;
                ok_title_zh[6] = 0x21;
                ok_title_zh[7] = 0x0;
                ok_title_en[0] = 'R'; /* Registered! */
                ok_title_en[1] = 'e';
                ok_title_en[2] = 'g';
                ok_title_en[3] = 'i';
                ok_title_en[4] = 's';
                ok_title_en[5] = 't';
                ok_title_en[6] = 'e';
                ok_title_en[7] = 'r';
                ok_title_en[8] = 'e';
                ok_title_en[9] = 'd';
                ok_title_en[10] = '!';
                ok_title_en[11] = '\0';
                gui_form_set_caption(register_dialog, pick_string(ok_title_zh, ok_title_en));
            } else {
                char fail_title_zh[10], fail_title_en[16];
                fail_title_zh[0] = 0xce; /* δע��! */
                fail_title_zh[1] = 0xb4;
                fail_title_zh[2] = 0xd7;
                fail_title_zh[3] = 0xa2;
                fail_title_zh[4] = 0xb2;
                fail_title_zh[5] = 0xe1;
                fail_title_zh[6] = 0x21;
                fail_title_zh[7] = 0x0;
                fail_title_en[0] = 'U'; /* UnRegistered! */
                fail_title_en[1] = 'n';
                fail_title_en[2] = 'R';
                fail_title_en[3] = 'e';
                fail_title_en[4] = 'g';
                fail_title_en[5] = 'i';
                fail_title_en[6] = 's';
                fail_title_en[7] = 't';
                fail_title_en[8] = 'e';
                fail_title_en[9] = 'r';
                fail_title_en[10] = 'e';
                fail_title_en[11] = 'd';
                fail_title_en[12] = '!';
                fail_title_en[13] = '\0';
                gui_form_set_caption(register_dialog, pick_string(fail_title_zh, fail_title_en));
            }
            break;
        case RS_REQUEST_SN:
            i_data = (int)data;
            _____has_been_registered = i_data;
            if (i_data) {
                char ok_title_zh[12], ok_title_en[16];
                ok_title_zh[0] = 0xd7; /* ע��ɹ�! */
                ok_title_zh[1] = 0xa2;
                ok_title_zh[2] = 0xb2;
                ok_title_zh[3] = 0xe1;  
                ok_title_zh[4] = 0xb3;
                ok_title_zh[5] = 0xc9;
                ok_title_zh[6] = 0xb9;
                ok_title_zh[7] = 0xa6;
                ok_title_zh[8] = 0x21;
                ok_title_zh[9] = 0x0;
                ok_title_en[0] = 'R'; /* Register OK! */
                ok_title_en[1] = 'e';
                ok_title_en[2] = 'g';
                ok_title_en[3] = 'i';
                ok_title_en[4] = 's';
                ok_title_en[5] = 't';
                ok_title_en[6] = 'e';
                ok_title_en[7] = 'r';
                ok_title_en[8] = ' ';
                ok_title_en[9] = 'O';
                ok_title_en[10] = 'K';
                ok_title_en[11] = '!';
                ok_title_en[12] = '\0';
                gui_form_set_caption(register_dialog, pick_string(ok_title_zh, ok_title_en));
            } else {
                char fail_title_zh[12], fail_title_en[24];
                fail_title_zh[0] = 0xd7; /* ע��ʧ��! */
                fail_title_zh[1] = 0xa2;
                fail_title_zh[2] = 0xb2;
                fail_title_zh[3] = 0xe1;
                fail_title_zh[4] = 0xca;
                fail_title_zh[5] = 0xa7;
                fail_title_zh[6] = 0xb0;
                fail_title_zh[7] = 0xdc;
                fail_title_zh[8] = 0x21;
                fail_title_zh[9] = 0x0;
                fail_title_en[0] = 'F'; /* Failed to Register! */
                fail_title_en[1] = 'a';
                fail_title_en[2] = 'i';
                fail_title_en[3] = 'l';
                fail_title_en[4] = 'e';
                fail_title_en[5] = 'd';
                fail_title_en[6] = ' ';
                fail_title_en[7] = 't';
                fail_title_en[8] = 'o';
                fail_title_en[9] = ' ';
                fail_title_en[10] = 'R';
                fail_title_en[11] = 'e';
                fail_title_en[12] = 'g';
                fail_title_en[13] = 'i';
                fail_title_en[14] = 's';
                fail_title_en[15] = 't';
                fail_title_en[16] = 'e';
                fail_title_en[17] = 'r';
                fail_title_en[18] = '!';
                fail_title_en[19] = '\0';
                gui_form_set_caption(register_dialog, pick_string(fail_title_zh, fail_title_en));
            }
            break;
        case RS_REQUEST_EVENT:
            i_data = (int)data;
            _____has_been_registered = i_data;
            __event_handler(i_data);
            break;
        default:
            break;
    }

    return ok;
}

/*-----------------------------------------------------------------------------------------
 * ����:    __event_handler
 *
 * ����:    RegisterService�¼�����(RS_REQUEST_EVENT)
**---------------------------------------------------------------------------------------*/
static void __event_handler(int is_registered)
{
    static unsigned int prompt_1st = 0, prompt_2ed = 0;
    INT32U seconds;
    static INT32U wait_time = CONFIG_REG_PROMPT_TIME2;
    gui_window_t * current_root;
    prepare_atomic() /* ����ֵĶ���, �Ǻ�, ע����仰��û�д�� */

    if (is_registered)
        return;

    in_atomic();
    seconds = SecondsFromStart;
    out_atomic();
    seconds /= 1000UL;  /* ���� => �� */

    if ((prompt_1st == 0) && (seconds >= CONFIG_REG_PROMPT_TIME1)) { /* ��һ������(��һ��) */
        current_root = gui_get_top_window();

        if (current_root == main_window) { /* ֻ������������ʾ */
            prompt_1st = 1;
            clrkey();
            putkey(ESC);
            putkey(ESC);
            putkey(ESC);
            putkey(ESC);
            putkey(ESC);
            putkey(ESC);
            putkey(SHIFT_F7); /* ����ע�� */
        }

        return;
    }

    if (prompt_2ed < 5) { /* �ڶ�������, �ӵ�3�ο�ʼ�ͻᷢ���˳�����, 10����һ�� */
        if (seconds >= wait_time) {
            current_root = gui_get_top_window();

            if (current_root == main_window) { /* ֻ������������ʾ */
                prompt_2ed++;
                wait_time = seconds + CONFIG_REG_PROMPT_TIMEx; /* �´�������10����֮�� */
                clrkey();
                putkey(ESC);
                putkey(ESC);
                putkey(ESC);
                putkey(ESC);
                putkey(ESC);
                putkey(ESC);
                if (prompt_2ed >= 3) { 
                    /* ��3�μ��Ժ������, ����ע��ʧ��, ��ô���˳����� */
                    putkey(RS_EXIT_KEY);
                } else {
                    putkey(SHIFT_F7); /* ����ע�� */
                }
            }
        }

        return;
    }

    /* ��ô������ѻ��ǲ�ע��, ̫������, ������� */
    if (seconds >= wait_time) { /* 10����֮��, �Ͽ�PLC������ */
        /* do something... */
    }

    return;
}


/*=========================================================================================
 * 
 * ���ļ�����: cut/cut_reg.c
 * 
**=======================================================================================*/


