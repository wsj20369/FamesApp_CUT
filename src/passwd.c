/******************************************************************************************
 * �ļ�:    cut/passwd.c
 *
 * ����:    ��������(���)
 *
 * ����:    Jun
 *
 * ����:    2011-3-31, 2012-10-29
******************************************************************************************/
#define  CUT_PASSWORD_C
#include <FamesOS.h>
#include "common.h"

/*-----------------------------------------------------------------------------------------
 *          
 *      �ؼ�������
 * 
**---------------------------------------------------------------------------------------*/
gui_widget * password_dialog = NULL;    /* ���ؼ�  */
gui_window_t * password_window = NULL;

enum {
    __passwd_id,
};

/* ��������Ի���Ķ��� */
DECLARE_INPUT_DIALOG_START(___password_dialog)
INPUT_DIALOG_ITEM(" ����������:  ", __passwd_id,  16,  32,  55,  346, 28, 1, EDIT_STYLE_PASSWORD, "����������, ���߰�ESC����")
INPUT_DIALOG_SET(___password_dialog, "����������", NULL, 0, 0, 480, 193, 0, 236, 1, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE)
DECLARE_INPUT_DIALOG_ENDED(___password_dialog);

/*-----------------------------------------------------------------------------------------
 *          
 *      ��������Ի���ĳ�ʼ��
 * 
**---------------------------------------------------------------------------------------*/
void init_password_dialog(void)
{
    extern char  ___bkcolor_other_dialog[];

    ___password_dialog.bkcolor = atoi(___bkcolor_other_dialog);
    
    input_dialog_preset_title(&___password_dialog, pick_string("����������", "Password"));
    input_dialog_preset_item_name(&___password_dialog, __passwd_id, pick_string(" ����������:  ", " Enter Password:  "));
    input_dialog_preset_item_comment(&___password_dialog, __passwd_id, 
                                     pick_string("����������, ���߰�ESC����", "Enter Password or Press ESC to Return"));
    password_dialog = input_dialog_initialize(&___password_dialog);
    if (!password_dialog)
        goto some_error;
    password_window = gui_create_window(password_dialog);
    if (!password_window)
        goto some_error;
    input_dialog_set_buttons_caption(&___password_dialog, 
                                     pick_string("ȷ��[F10]", "OK.[F10]"), 
                                     pick_string("ȡ��[ESC]", "No.[ESC]"));
    input_dialog_set_ok_comment(&___password_dialog, 
                                     pick_string("ȷ��, �밴F10��س���", "Press F10/Enter to Confirm"));
    input_dialog_set_cancel_comment(&___password_dialog, 
                                     pick_string("ȡ��, �밴ESC��س���", "Press ESC/Enter to Cancel"));

    return;

some_error:
    sys_print("init_password_dialog(): failed to create widgets or window!\n");
    ExitApplication();
}

void password_prepare(int id, char * buf, void * data, INT16U opt)
{
    FamesAssert(buf);
    if(!buf)
        return;

    data = data;
    opt  = opt;
    
    switch(id){
        case __passwd_id:
            break;
        default:
            break;
    }

    return;
}

int password_entered(int id, char * buf, void * data, KEYCODE key)
{
    INT32U * ___d;
    FamesAssert(buf);
    FamesAssert(data);
    if(!buf || !data)
        return 1; /* ���������Ч, ��ô����Ϊ������������Ч�� */

    key = key;
    
    switch(id){
        case __passwd_id:
            ___d = (INT32U *)data;
            *___d = STRtoINT32(buf, CHG_OPT_DEC);
            if(key == ENTER){
                putkey(F10);
            }
            break;
        default:
            break;
    }

    return 1;
}

static void ____alert(char * s)
{
    input_dialog_alert(&___password_dialog, s, COLOR_WARNING_236);
}


/*-----------------------------------------------------------------------------------------
 * ����:    check_password_dialog()
 *
 * ����:    �������벢���
**---------------------------------------------------------------------------------------*/
BOOL check_password_dialog(int x, int y, INT32U password)
{
    BOOL retval;
    int  loop;
    INT32U user_passwd = 0L;
    static int lock = 0;

    os_mutex_lock(lock);

    gui_set_widget_location(password_dialog, x, y);
    gui_show_window(password_window);

    for(loop=1; loop;){
        retval = input_dialog_method(&___password_dialog, password_prepare, password_entered, (void *)&user_passwd, 0);
        if(retval){
            if(__encrypt(user_passwd) != password){
                ____alert(pick_string("*** �� �� �� �� ***", "*** Wrong Password ***"));
                continue;
            } else {
                loop = 0;
                retval = ok;
            }
        } else {
            loop = 0;
        }
    }

    gui_hide_window(password_window);

    os_mutex_unlock(lock);

    return retval;
}


/*=========================================================================================
 * 
 * ���ļ�����: cut/passwd.c
 * 
**=======================================================================================*/


