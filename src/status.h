/******************************************************************************************
 * �ļ�:    cut/status.h
 *
 * ����:    ����״̬����ʾ��ˢ��
 *
 * ����:    Jun
 *
 * ����:    2013-2-15
******************************************************************************************/
#ifndef CUT_STATUS_DISPLAY_H
#define CUT_STATUS_DISPLAY_H


/*-----------------------------------------------------------------------------------------
 * 
 *      ��������
 * 
**---------------------------------------------------------------------------------------*/
void init_produce_screen(gui_widget * parent);
void init_produce_screen_small(gui_widget * parent);
void init_today_data_screen(gui_widget * parent);

void set_status_font_big1(int font_index);
void set_status_font_big2(int font_index);

void __show_today_data_screen(int show); /* ��show_today_data_screen()���� */



#endif /* #ifndef CUT_STATUS_DISPLAY_H */

/*=========================================================================================
 * 
 * ���ļ�����: cut/status.h
 * 
**=======================================================================================*/


