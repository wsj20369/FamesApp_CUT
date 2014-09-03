/******************************************************************************************
 * 文件:    cut/ord_edit.c
 *
 * 描述:    订单编辑(输单, 调单等)
 *
 * 作者:    Jun
******************************************************************************************/
#define  CUT_INPUT_ORDER_C
#include <FamesOS.h>
#include "common.h"

/*-----------------------------------------------------------------------------------------
 *          
 *      输单画面中的控件及其它定义
 * 
**---------------------------------------------------------------------------------------*/
static gui_widget   * input_screen = NULL;          /* 输单界面的主控件  */
static gui_window_t * input_order_window = NULL;

extern BMPINFO icon;                                /* 图标              */

extern char  ___bkcolor_other_dialog[];

enum {
    ORD_EDIT_WKNO,
    ORD_EDIT_ORDNO,
    ORD_EDIT_SETLEN,
    ORD_EDIT_COUNT,
    ORD_EDIT_UNIT,
};

/* 订单编辑对话框的定义 */
#define ____font CONFIG_FONT_24
DECLARE_INPUT_DIALOG_START(edit_order_dialog)
INPUT_DIALOG_ITEM("工号:  ", ORD_EDIT_WKNO,   5,  40,  42,  206, 36, ____font, 0, "")
INPUT_DIALOG_ITEM("订单号:", ORD_EDIT_ORDNO,  8,  276, 42,  206, 36, ____font, 0, "")
INPUT_DIALOG_ITEM("捆单位:", ORD_EDIT_UNIT,   3,  512, 42,  160, 36, ____font, 0, "")
INPUT_DIALOG_ITEM("设定长:", ORD_EDIT_SETLEN, 4,  40,  92,  206, 36, ____font, 0, "")
INPUT_DIALOG_ITEM("张数:  ", ORD_EDIT_COUNT,  6,  276, 92,  206, 36, ____font, 0, "")
INPUT_DIALOG_SET(edit_order_dialog, "订单编辑", &icon, 128, 408, 720, 216, 0, 236, CONFIG_FONT_16, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE)
DECLARE_INPUT_DIALOG_ENDED(edit_order_dialog);
#undef ____font


/*-----------------------------------------------------------------------------------------
 *          
 *      订单编辑对话框的初始化
 * 
**---------------------------------------------------------------------------------------*/
void init_input_order_screen(void)
{
    edit_order_dialog.bkcolor = atoi(___bkcolor_other_dialog);

    input_dialog_preset_title(&edit_order_dialog, pick_string("订单编辑", "Order Input/Edit"));
    input_dialog_preset_item_name(&edit_order_dialog, ORD_EDIT_WKNO,   pick_string("工号:  ", "S/N:   "));
    input_dialog_preset_item_name(&edit_order_dialog, ORD_EDIT_ORDNO,  pick_string("订单号:", "OrdNo: "));
    input_dialog_preset_item_name(&edit_order_dialog, ORD_EDIT_UNIT,   pick_string("捆单位:", "Unit:  "));
    input_dialog_preset_item_name(&edit_order_dialog, ORD_EDIT_SETLEN, pick_string("设定长:", "SetLen:"));
    input_dialog_preset_item_name(&edit_order_dialog, ORD_EDIT_COUNT,  pick_string("张数:  ", "Count: "));

    input_dialog_preset_item_comment(&edit_order_dialog, ORD_EDIT_WKNO, 
                                     pick_string("工号决定了生产的顺序, 0~9999", "Order S/N, 0~9999"));
    input_dialog_preset_item_comment(&edit_order_dialog, ORD_EDIT_ORDNO, 
                                     pick_string("订单号", "Order No."));
    input_dialog_preset_item_comment(&edit_order_dialog, ORD_EDIT_UNIT, 
                                     pick_string("每捆纸板的张数", "The Unit of Bundle"));
    input_dialog_preset_item_comment(&edit_order_dialog, ORD_EDIT_SETLEN, 
                                     pick_string("设定切长", "Set Cut Length"));
    input_dialog_preset_item_comment(&edit_order_dialog, ORD_EDIT_COUNT, 
                                     pick_string("裁切张数, (1 ~ 999999)", "Cut Number, (1 ~ 999999)"));

    input_screen = input_dialog_initialize(&edit_order_dialog);
    if (!input_screen)
        goto some_error;
    input_dialog_set_buttons_caption(&edit_order_dialog, 
                                     pick_string("确认[F10]", "OK.[F10]"), 
                                     pick_string("取消[ESC]", "No.[ESC]"));
    input_dialog_set_ok_comment(&edit_order_dialog, 
                                     pick_string("确认, 请按F10或回车键", "Press F10/Enter to Confirm"));
    input_dialog_set_cancel_comment(&edit_order_dialog, 
                                     pick_string("取消, 请按ESC或回车键", "Press ESC/Enter to Cancel"));

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
** 返回值: 0=内容无效, 1=内容有效, -1=所有内容须刷新
*/
int finish_edit(int id, char * buf, void * data, KEYCODE key)
{
    order_struct * order;
    struct order_edit_private * temp;
    int    i;

    FamesAssert(buf);
    FamesAssert(data);
    if(!buf || !data)
        return 1; /* 如果缓冲无效, 那么就认为其内容总是有效的 */

    if(key == ESC)
        return 1;

    temp = (struct order_edit_private *)data;
    order = &temp->order;

    switch (id) {
        case ORD_EDIT_WKNO:
            order->WORKNO = atoi(buf);
            i = FindOrder(order->WORKNO);
            if (i >= 0) { /* 定单已存在, 修改模式 */
                LoadOrder(order, i, 1);
                gui_form_set_caption(input_screen, pick_string("订单编辑 [订单已存在, 修改模式]", "Order Input/Edit [Order Exist]"));
                return -1;
            } else {
                gui_form_set_caption(input_screen, pick_string("订单编辑 [新订单]", "Order Input/Edit [New Order]"));
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
                sprintf(buf1, "切长不应该小于设定的最小值[%dmm]", config.min_cutlen);
                sprintf(buf2, "SetLen is out of range(No less than %dmm)", config.min_cutlen);
                ____alert(buf1, buf2);
                return 0;
            }
            break;
        case ORD_EDIT_COUNT:
            order->NUMBER = atol(buf);
            if (order->NUMBER < 1) {
                ____alert("数量不对, 至少应该有1张吧!", "Count is out of range, should be >= 1");
                return 0;
            }
            break;
        default:
            break;
    }

    return 1;
}

/*-----------------------------------------------------------------------------------------
 * 函数:    order_edit()
 *
 * 描述:    订单编辑
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
        gui_form_set_caption(input_screen, pick_string("订单编辑", "Order Input/Edit"));
        retval = input_dialog_method(&edit_order_dialog, prepare_edit, finish_edit, &order_for_edit, 0);
        if(retval){
            /* 检查工号是否有效 */
            workno = order_for_edit.order.WORKNO;
            /* FIXME: 这里也许用if(workno <= get_first_workno())才对,
             *        如果用户修改了生产订单的切长或其它字段, 有可能
             *        会引起系统异常, 但有时确实也有这种需要,
             * TODO:  将来再仔细的考虑这个问题吧! 2013-3-3, shujun
             */
            if(workno < get_first_workno()){
                ____alert("工号太小(应该大于第一笔单的工号)", "S/N is too small");
            } else {
                lock_kernel();
                retval = InsertOrder(&order_for_edit.order);
                unlock_kernel();
                if(!retval){
                    ____alert("插单失败!", "Failed to Insert!");
                    continue;
                }

                /* 准备输入下一笔订单 */
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
 *      调单画面中的控件及其它定义
 * 
**---------------------------------------------------------------------------------------*/
static gui_widget   * move_order_dialog = NULL;    /* 输单界面的主控件  */
static gui_window_t * move_order_window = NULL;

enum {
    __old_workno,
    __new_workno,
};

/* 调单对话框的定义 */
DECLARE_INPUT_DIALOG_START(__move_order_dialog)
INPUT_DIALOG_ITEM("当前工号: ", __old_workno, 4,  64,  42,  256, 28, 1, 0, "")
INPUT_DIALOG_ITEM("新的工号: ", __new_workno, 4,  64,  80,  256, 28, 1, 0, "")
INPUT_DIALOG_SET(__move_order_dialog, "调单", &icon, 230, 284, 560, 200, 0, 236, 1, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE)
DECLARE_INPUT_DIALOG_ENDED(__move_order_dialog);

/*-----------------------------------------------------------------------------------------
 *          
 *      调单对话框的初始化
 * 
**---------------------------------------------------------------------------------------*/
void init_move_order_dialog(void)
{
    __move_order_dialog.bkcolor = atoi(___bkcolor_other_dialog);
    input_dialog_preset_title(&__move_order_dialog, pick_string("调单", "Order Move"));
    input_dialog_preset_item_name(&__move_order_dialog, __old_workno, pick_string("当前工号: ", "Cur S/N: "));
    input_dialog_preset_item_name(&__move_order_dialog, __new_workno, pick_string("新的工号: ", "New S/N: "));
    input_dialog_preset_item_comment(&__move_order_dialog, __old_workno, 
                                     pick_string("\"当前工号\"可以选择将要移动的订单", "Current S/N indicates the Order to Move."));
    input_dialog_preset_item_comment(&__move_order_dialog, __new_workno, 
                                     pick_string("\"新的工号\"是订单将要移动到的位置", "New S/N indicates the Target Position to Move to."));
    move_order_dialog = input_dialog_initialize(&__move_order_dialog);
    if (!move_order_dialog)
        goto some_error;
    input_dialog_set_buttons_caption(&__move_order_dialog, 
                                     pick_string("确认[F10]", "OK.[F10]"), 
                                     pick_string("取消[ESC]", "No.[ESC]"));
    input_dialog_set_ok_comment(&__move_order_dialog, 
                                     pick_string("确认, 请按F10或回车键", "Press F10/Enter to Confirm"));
    input_dialog_set_cancel_comment(&__move_order_dialog, 
                                     pick_string("取消, 请按ESC或回车键", "Press ESC/Enter to Cancel"));

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
 * 函数:    move_order()
 *
 * 描述:    调单
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

    putkey(ENTER); /* 将光标跳到: "新的工号" */

    for (loop=1; loop;) {
        retval = input_dialog_method(&__move_order_dialog, ___prepare_move, ___finish_move, (void*)&t, 0);
        if (retval) {
            if (t.new_wkno == t.old_wkno) {
                input_dialog_alert(&__move_order_dialog, pick_string("目标工号与原来的相同, 请重新输入", "Target WorkNo should be different!"), COLOR_WARNING_236);
                continue;
            }
            i = FindOrder(t.old_wkno);
            if (i < 0) {
                input_dialog_alert(&__move_order_dialog, pick_string("订单不存在", "Order Not Exist!"), COLOR_WARNING_236);
                continue;
            }
            i = FindOrder(t.new_wkno);
            if (i >= 0) {
                input_dialog_alert(&__move_order_dialog, 
                                   pick_string("新的工号已经存在, 请另外输入一个", 
                                               "New S/N already Exist, Choose Another One!"), 
                                   COLOR_WARNING_236);
                continue;
            }
            if (t.new_wkno < get_first_workno()) {
                input_dialog_alert(&__move_order_dialog, 
                                   pick_string("新的工号太小, 请另外输入一个", 
                                               "New S/N is too small, Choose Another One!"),
                                   COLOR_WARNING_236);
                continue;
            }
            lock_kernel();
            retval = MoveOrder(t.old_wkno, t.new_wkno);
            unlock_kernel();
            if (!retval) {
                input_dialog_alert(&__move_order_dialog, pick_string("调单失败!", "Failed!"), COLOR_WARNING_236);
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
 * 本文件结束: cut/ord_edit.c
 * 
**=======================================================================================*/


