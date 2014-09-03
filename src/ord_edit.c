/******************************************************************************************
 * �ļ�:    cut/ord_edit.c
 *
 * ����:    �����༭(�䵥, ������)
 *
 * ����:    Jun
******************************************************************************************/
#define  CUT_INPUT_ORDER_C
#include <FamesOS.h>
#include "common.h"

/*-----------------------------------------------------------------------------------------
 *          
 *      �䵥�����еĿؼ�����������
 * 
**---------------------------------------------------------------------------------------*/
static gui_widget   * input_screen = NULL;          /* �䵥��������ؼ�  */
static gui_window_t * input_order_window = NULL;

extern BMPINFO icon;                                /* ͼ��              */

extern char  ___bkcolor_other_dialog[];

enum {
    ORD_EDIT_WKNO,
    ORD_EDIT_ORDNO,
    ORD_EDIT_SETLEN,
    ORD_EDIT_COUNT,
    ORD_EDIT_UNIT,
};

/* �����༭�Ի���Ķ��� */
#define ____font CONFIG_FONT_24
DECLARE_INPUT_DIALOG_START(edit_order_dialog)
INPUT_DIALOG_ITEM("����:  ", ORD_EDIT_WKNO,   5,  40,  42,  206, 36, ____font, 0, "")
INPUT_DIALOG_ITEM("������:", ORD_EDIT_ORDNO,  8,  276, 42,  206, 36, ____font, 0, "")
INPUT_DIALOG_ITEM("����λ:", ORD_EDIT_UNIT,   3,  512, 42,  160, 36, ____font, 0, "")
INPUT_DIALOG_ITEM("�趨��:", ORD_EDIT_SETLEN, 4,  40,  92,  206, 36, ____font, 0, "")
INPUT_DIALOG_ITEM("����:  ", ORD_EDIT_COUNT,  6,  276, 92,  206, 36, ____font, 0, "")
INPUT_DIALOG_SET(edit_order_dialog, "�����༭", &icon, 128, 408, 720, 216, 0, 236, CONFIG_FONT_16, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE)
DECLARE_INPUT_DIALOG_ENDED(edit_order_dialog);
#undef ____font


/*-----------------------------------------------------------------------------------------
 *          
 *      �����༭�Ի���ĳ�ʼ��
 * 
**---------------------------------------------------------------------------------------*/
void init_input_order_screen(void)
{
    edit_order_dialog.bkcolor = atoi(___bkcolor_other_dialog);

    input_dialog_preset_title(&edit_order_dialog, pick_string("�����༭", "Order Input/Edit"));
    input_dialog_preset_item_name(&edit_order_dialog, ORD_EDIT_WKNO,   pick_string("����:  ", "S/N:   "));
    input_dialog_preset_item_name(&edit_order_dialog, ORD_EDIT_ORDNO,  pick_string("������:", "OrdNo: "));
    input_dialog_preset_item_name(&edit_order_dialog, ORD_EDIT_UNIT,   pick_string("����λ:", "Unit:  "));
    input_dialog_preset_item_name(&edit_order_dialog, ORD_EDIT_SETLEN, pick_string("�趨��:", "SetLen:"));
    input_dialog_preset_item_name(&edit_order_dialog, ORD_EDIT_COUNT,  pick_string("����:  ", "Count: "));

    input_dialog_preset_item_comment(&edit_order_dialog, ORD_EDIT_WKNO, 
                                     pick_string("���ž�����������˳��, 0~9999", "Order S/N, 0~9999"));
    input_dialog_preset_item_comment(&edit_order_dialog, ORD_EDIT_ORDNO, 
                                     pick_string("������", "Order No."));
    input_dialog_preset_item_comment(&edit_order_dialog, ORD_EDIT_UNIT, 
                                     pick_string("ÿ��ֽ�������", "The Unit of Bundle"));
    input_dialog_preset_item_comment(&edit_order_dialog, ORD_EDIT_SETLEN, 
                                     pick_string("�趨�г�", "Set Cut Length"));
    input_dialog_preset_item_comment(&edit_order_dialog, ORD_EDIT_COUNT, 
                                     pick_string("��������, (1 ~ 999999)", "Cut Number, (1 ~ 999999)"));

    input_screen = input_dialog_initialize(&edit_order_dialog);
    if (!input_screen)
        goto some_error;
    input_dialog_set_buttons_caption(&edit_order_dialog, 
                                     pick_string("ȷ��[F10]", "OK.[F10]"), 
                                     pick_string("ȡ��[ESC]", "No.[ESC]"));
    input_dialog_set_ok_comment(&edit_order_dialog, 
                                     pick_string("ȷ��, �밴F10��س���", "Press F10/Enter to Confirm"));
    input_dialog_set_cancel_comment(&edit_order_dialog, 
                                     pick_string("ȡ��, �밴ESC��س���", "Press ESC/Enter to Cancel"));

    input_order_window = gui_create_window(input_screen);
    if (!input_order_window)
        goto some_error;

    return;

some_error:
    sys_print("init_input_order_screen(): failed to create widgets or window!\n");
    ExitApplication();
}

struct order_edit_private {
    order_struct order;
};

void prepare_edit(int id, char * buf, void * data, INT16U opt)
{
    order_struct * order;
    struct order_edit_private * temp;

    FamesAssert(buf);
    FamesAssert(data);
    if(!buf || !data)
        return;

    opt = opt;

    temp = (struct order_edit_private *)data;
    order = &temp->order;
    
    switch(id){
        case ORD_EDIT_WKNO:
            sprintf(buf, "%d", order->WORKNO);
            break;
        case ORD_EDIT_ORDNO:
            order->ORDERNO[8] = 0;
            sprintf(buf, "%s", order->ORDERNO);
            break;
        case ORD_EDIT_UNIT:
            sprintf(buf, "%d", order->UNIT);
            break;
        case ORD_EDIT_SETLEN:
            sprintf(buf, "%d", order->CUT_LENGTH);
            break;
        case ORD_EDIT_COUNT:
            sprintf(buf, "%ld", order->NUMBER);
            break;
        default:
            break;
    }

    return;
}

static void ____alert(char * zh_str, char * en_str)
{
    char * s;

    s = pick_string(zh_str, en_str);
    input_dialog_alert(&edit_order_dialog, s, COLOR_WARNING_236);
}

/***************************************************************************************
** ����ֵ: 0=������Ч, 1=������Ч, -1=����������ˢ��
*/
int finish_edit(int id, char * buf, void * data, KEYCODE key)
{
    order_struct * order;
    struct order_edit_private * temp;
    int    i;

    FamesAssert(buf);
    FamesAssert(data);
    if(!buf || !data)
        return 1; /* ���������Ч, ��ô����Ϊ������������Ч�� */

    if(key == ESC)
        return 1;

    temp = (struct order_edit_private *)data;
    order = &temp->order;

    switch (id) {
        case ORD_EDIT_WKNO:
            order->WORKNO = atoi(buf);
            i = FindOrder(order->WORKNO);
            if (i >= 0) { /* �����Ѵ���, �޸�ģʽ */
                LoadOrder(order, i, 1);
                gui_form_set_caption(input_screen, pick_string("�����༭ [�����Ѵ���, �޸�ģʽ]", "Order Input/Edit [Order Exist]"));
                return -1;
            } else {
                gui_form_set_caption(input_screen, pick_string("�����༭ [�¶���]", "Order Input/Edit [New Order]"));
            }
            break;
        case ORD_EDIT_ORDNO:
            buf[8] = 0;
            sprintf(order->ORDERNO, "%s", buf);
            break;
        case ORD_EDIT_UNIT:
            order->UNIT = atoi(buf);
            break;
        case ORD_EDIT_SETLEN:
            order->CUT_LENGTH = atoi(buf);
            if (order->CUT_LENGTH < config.min_cutlen) {
                char buf1[64], buf2[64];
                sprintf(buf1, "�г���Ӧ��С���趨����Сֵ[%dmm]", config.min_cutlen);
                sprintf(buf2, "SetLen is out of range(No less than %dmm)", config.min_cutlen);
                ____alert(buf1, buf2);
                return 0;
            }
            break;
        case ORD_EDIT_COUNT:
            order->NUMBER = atol(buf);
            if (order->NUMBER < 1) {
                ____alert("��������, ����Ӧ����1�Ű�!", "Count is out of range, should be >= 1");
                return 0;
            }
            break;
        default:
            break;
    }

    return 1;
}

/*-----------------------------------------------------------------------------------------
 * ����:    order_edit()
 *
 * ����:    �����༭
**---------------------------------------------------------------------------------------*/
static struct order_edit_private order_for_edit = { INIT_order_struct };

BOOL order_edit(void)
{
    BOOL retval;
    int  loop;
    INT16U workno;

    order_for_edit.order.WORKNO = 10 + get_last_workno();

    gui_show_window(input_order_window);

    for(loop=1; loop;){
        gui_form_set_caption(input_screen, pick_string("�����༭", "Order Input/Edit"));
        retval = input_dialog_method(&edit_order_dialog, prepare_edit, finish_edit, &order_for_edit, 0);
        if(retval){
            /* ��鹤���Ƿ���Ч */
            workno = order_for_edit.order.WORKNO;
            /* FIXME: ����Ҳ����if(workno <= get_first_workno())�Ŷ�,
             *        ����û��޸��������������г��������ֶ�, �п���
             *        ������ϵͳ�쳣, ����ʱȷʵҲ��������Ҫ,
             * TODO:  ��������ϸ�Ŀ�����������! 2013-3-3, shujun
             */
            if(workno < get_first_workno()){
                ____alert("����̫С(Ӧ�ô��ڵ�һ�ʵ��Ĺ���)", "S/N is too small");
            } else {
                lock_kernel();
                retval = InsertOrder(&order_for_edit.order);
                unlock_kernel();
                if(!retval){
                    ____alert("�嵥ʧ��!", "Failed to Insert!");
                    continue;
                }

                /* ׼��������һ�ʶ��� */
                order_for_edit.order.WORKNO = 10 + get_last_workno();
                order_for_edit.order.CUT_LENGTH = 0;
                order_for_edit.order.CUT_LENGTH_INCH = 0;
                order_for_edit.order.NUMBER = 0;
            }
        } else {
            loop = 0;
        }
    }

    gui_hide_window(input_order_window);

    return retval;
}

/*-----------------------------------------------------------------------------------------
 *          
 *      ���������еĿؼ�����������
 * 
**---------------------------------------------------------------------------------------*/
static gui_widget   * move_order_dialog = NULL;    /* �䵥��������ؼ�  */
static gui_window_t * move_order_window = NULL;

enum {
    __old_workno,
    __new_workno,
};

/* �����Ի���Ķ��� */
DECLARE_INPUT_DIALOG_START(__move_order_dialog)
INPUT_DIALOG_ITEM("��ǰ����: ", __old_workno, 4,  64,  42,  256, 28, 1, 0, "")
INPUT_DIALOG_ITEM("�µĹ���: ", __new_workno, 4,  64,  80,  256, 28, 1, 0, "")
INPUT_DIALOG_SET(__move_order_dialog, "����", &icon, 230, 284, 560, 200, 0, 236, 1, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE)
DECLARE_INPUT_DIALOG_ENDED(__move_order_dialog);

/*-----------------------------------------------------------------------------------------
 *          
 *      �����Ի���ĳ�ʼ��
 * 
**---------------------------------------------------------------------------------------*/
void init_move_order_dialog(void)
{
    __move_order_dialog.bkcolor = atoi(___bkcolor_other_dialog);
    input_dialog_preset_title(&__move_order_dialog, pick_string("����", "Order Move"));
    input_dialog_preset_item_name(&__move_order_dialog, __old_workno, pick_string("��ǰ����: ", "Cur S/N: "));
    input_dialog_preset_item_name(&__move_order_dialog, __new_workno, pick_string("�µĹ���: ", "New S/N: "));
    input_dialog_preset_item_comment(&__move_order_dialog, __old_workno, 
                                     pick_string("\"��ǰ����\"����ѡ��Ҫ�ƶ��Ķ���", "Current S/N indicates the Order to Move."));
    input_dialog_preset_item_comment(&__move_order_dialog, __new_workno, 
                                     pick_string("\"�µĹ���\"�Ƕ�����Ҫ�ƶ�����λ��", "New S/N indicates the Target Position to Move to."));
    move_order_dialog = input_dialog_initialize(&__move_order_dialog);
    if (!move_order_dialog)
        goto some_error;
    input_dialog_set_buttons_caption(&__move_order_dialog, 
                                     pick_string("ȷ��[F10]", "OK.[F10]"), 
                                     pick_string("ȡ��[ESC]", "No.[ESC]"));
    input_dialog_set_ok_comment(&__move_order_dialog, 
                                     pick_string("ȷ��, �밴F10��س���", "Press F10/Enter to Confirm"));
    input_dialog_set_cancel_comment(&__move_order_dialog, 
                                     pick_string("ȡ��, �밴ESC��س���", "Press ESC/Enter to Cancel"));

    move_order_window = gui_create_window(move_order_dialog);
    if (!move_order_window)
        goto some_error;

    return;

some_error:
    sys_print("init_move_order_dialog(): failed to create widgets or window!\n");
    ExitApplication();
}

struct ___struct {
    INT16U old_wkno;
    INT16U new_wkno;
};

static void ___prepare_move(int id, char * buf, void * data, INT16U opt)
{
    struct ___struct * t;
    
    FamesAssert(buf);
    FamesAssert(data);
    if(!buf || !data)
        return;

    opt = opt;

    t = (struct ___struct *)data;
    
    switch(id){
        case __old_workno:
            sprintf(buf, "%d", t->old_wkno);
            break;
        case __new_workno:
            sprintf(buf, "%d", t->new_wkno);
            break;
        default:
            break;
    }

    return;
}

static int ___finish_move(int id, char * buf, void * data, KEYCODE key)
{
    struct ___struct * t;
    
    FamesAssert(buf);
    FamesAssert(data);
    if(!buf || !data)
        return 1;

    if(key == ESC)
        return 1;

    t = (struct ___struct *)data;
    
    switch(id){
        case __old_workno:
            t->old_wkno = (INT16U)STRtoINT16(buf, CHG_OPT_DEC);
            break;
        case __new_workno:
            t->new_wkno = (INT16U)STRtoINT16(buf, CHG_OPT_DEC);
            if(key == ENTER){
                putkey(F10);
            }
            break;
        default:
            break;
    }

    return 1;
}

/*-----------------------------------------------------------------------------------------
 * ����:    move_order()
 *
 * ����:    ����
**---------------------------------------------------------------------------------------*/
BOOL move_order(gui_widget * order_view)
{
    BOOL retval;
    int  loop, i;
    struct ___struct t;
    order_struct order;

    FamesAssert(order_view);
    if (!order_view)
        return fail;

    t.old_wkno = get_first_workno();
    i = gui_view_get_selected(order_view);
    if (1 == LoadOrder(&order, i, 1)) {
        t.old_wkno = order.WORKNO;
    }
    t.new_wkno = t.old_wkno;
    
    gui_show_window(move_order_window);

    putkey(ENTER); /* ���������: "�µĹ���" */

    for (loop=1; loop;) {
        retval = input_dialog_method(&__move_order_dialog, ___prepare_move, ___finish_move, (void*)&t, 0);
        if (retval) {
            if (t.new_wkno == t.old_wkno) {
                input_dialog_alert(&__move_order_dialog, pick_string("Ŀ�깤����ԭ������ͬ, ����������", "Target WorkNo should be different!"), COLOR_WARNING_236);
                continue;
            }
            i = FindOrder(t.old_wkno);
            if (i < 0) {
                input_dialog_alert(&__move_order_dialog, pick_string("����������", "Order Not Exist!"), COLOR_WARNING_236);
                continue;
            }
            i = FindOrder(t.new_wkno);
            if (i >= 0) {
                input_dialog_alert(&__move_order_dialog, 
                                   pick_string("�µĹ����Ѿ�����, ����������һ��", 
                                               "New S/N already Exist, Choose Another One!"), 
                                   COLOR_WARNING_236);
                continue;
            }
            if (t.new_wkno < get_first_workno()) {
                input_dialog_alert(&__move_order_dialog, 
                                   pick_string("�µĹ���̫С, ����������һ��", 
                                               "New S/N is too small, Choose Another One!"),
                                   COLOR_WARNING_236);
                continue;
            }
            lock_kernel();
            retval = MoveOrder(t.old_wkno, t.new_wkno);
            unlock_kernel();
            if (!retval) {
                input_dialog_alert(&__move_order_dialog, pick_string("����ʧ��!", "Failed!"), COLOR_WARNING_236);
                continue;
            } else {
                loop = 0;
                retval = ok;
            }
        } else {
            loop = 0;
        }
    }

    gui_hide_window(move_order_window);

    return retval;
}


/*=========================================================================================
 * 
 * ���ļ�����: cut/ord_edit.c
 * 
**=======================================================================================*/


