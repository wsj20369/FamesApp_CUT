/******************************************************************************************
 * �ļ�:    cut/cut_core.c
 *
 * ����:    ���л������㷨
 *
 * ����:    Jun
 *
 * ʱ��:    2013-2-15
******************************************************************************************/


struct cut_run_state_s running_status;



#if 0

#define  SLC_SEND_ORDER_C
#include <FamesOS.h>
#include "common.h"

/*-----------------------------------------------------------------------------------------
 *          
 *      �ؼ�����������
 * 
**---------------------------------------------------------------------------------------*/
gui_widget        * send_screen   = NULL;       /* ���ؼ�      */
static gui_widget * status_bar    = NULL;       /* ״̬��      */
static gui_widget * send_message  = NULL;       /* �͵���Ϣ    */

gui_widget * slc_send_movement    = NULL;       /* ������      */

extern BMPINFO  icon;                           /* ͼ��        */

#define  ___bkcolor  236                        /* ����ɫ      */


/*-----------------------------------------------------------------------------------------
 *          
 *      ����Ķ���(���ʼ��)
 * 
**---------------------------------------------------------------------------------------*/
void init_send_screen(void)
{
    int x, y, width, height;
    COLOR bkcolor;

    x = 220;
    y = 296;
    width = 500;
    height = 168;
    
    bkcolor = ___bkcolor;

    /* ������   */
    send_screen = gui_create_widget(GUI_WIDGET_FORM, x, y, width, height, 0, bkcolor, 1, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE);
    if(!send_screen)
        goto some_error;
    gui_form_init_private(send_screen, 128);
    gui_form_set_icon(send_screen, &icon);
    gui_form_set_caption(send_screen, pick_string("��������", "Send Order"));
    
    /* ������   */
    status_bar = gui_create_widget(GUI_WIDGET_LABEL, 5, (height-35), (width-10), 30, 0, bkcolor, 1, LABEL_STYLE_CLIENT_BDR);
    if(!status_bar)
        goto some_error;
    gui_widget_link(send_screen, status_bar);
    gui_label_init_private(status_bar, 128);
    gui_label_set_text(status_bar, "");

    /* �͵���Ϣ */
    send_message = gui_create_widget(GUI_WIDGET_LABEL, 75, 50, 256, 52, 0, bkcolor, font24, 0);
    if(!send_message)
        goto some_error;
    gui_widget_link(send_screen, send_message);
    gui_label_init_private(send_message, 64);
    gui_label_set_text(send_message, "");

    /* ......   */
    slc_send_movement = gui_create_widget(GUI_WIDGET_LABEL, 333, 50, 96, 52, 0, bkcolor, font24, 0);
    if(!slc_send_movement)
        goto some_error;
    gui_widget_link(send_screen, slc_send_movement);
    gui_label_init_private(slc_send_movement, 16);
    gui_label_set_text(slc_send_movement, "");

    return;

some_error:
    sys_print("init_send_screen(): failed to create widgets!\n");
    ExitApplication();
}

/*-----------------------------------------------------------------------------------------
 * ����:    slc_send_order()
 *
 * ����:    slc_index       ��ѹ�����, 1 = SLC1, 2 = SLC2
 *          order           ��������
 *          no_control      1 = �����͵���̧���ź��������ź�, 0 = ����
 *
 * ����:    ���Ͷ�������ѹ��(�趨ֵ)
 *
 * ˵��:    ���ڴ˺����п��ܱ��������ͬʱִ��, ������Ҫ���⴦��(___lock)
**---------------------------------------------------------------------------------------*/
BOOL slc_send_order(int slc_index, order_struct * order, int no_control)
{
}


/*-----------------------------------------------------------------------------------------
 * ����:    set_plc_on_startup()
 *
 * ����:    ��ѹ������ʱ��PLC���趨
**---------------------------------------------------------------------------------------*/
void set_plc_on_startup(int slc_index)
{
    INT32S tmp;
    extern struct cut_config_s config;

    tmp = (INT32S)(config.slc[slc_index-1].slc_spg);
    ___slc_plc_rw_ensure(slc_index, FATEK_PLC_WRITE_DR, PLC_ADDR_SPG, &tmp, 1);
    tmp = (INT32S)(config.slc[slc_index-1].misc.yx_big);
    ___slc_plc_rw_ensure(slc_index, FATEK_PLC_WRITE_DR, PLC_ADDR_YXBIG, &tmp, 1);
    tmp = (INT32S)(config.slc[slc_index-1].misc.yx_small);
    ___slc_plc_rw_ensure(slc_index, FATEK_PLC_WRITE_DR, PLC_ADDR_YXSMALL, &tmp, 1);

    slc_clear_fix_ok(slc_index); /* ����ʱ�����λ����ź� */
}

#endif
/*=========================================================================================
 * 
 * ���ļ�����: cut/cut_core.c
 * 
**=======================================================================================*/


