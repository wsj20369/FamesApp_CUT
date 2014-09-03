/******************************************************************************************
 * 文件:    cut/status.h
 *
 * 描述:    生产状态的显示及刷新
 *
 * 作者:    Jun
 *
 * 创建:    2013-2-15
******************************************************************************************/
#ifndef CUT_STATUS_DISPLAY_H
#define CUT_STATUS_DISPLAY_H


/*-----------------------------------------------------------------------------------------
 * 
 *      函数声明
 * 
**---------------------------------------------------------------------------------------*/
void init_produce_screen(gui_widget * parent);
void init_produce_screen_small(gui_widget * parent);
void init_today_data_screen(gui_widget * parent);

void set_status_font_big1(int font_index);
void set_status_font_big2(int font_index);

void __show_today_data_screen(int show); /* 由show_today_data_screen()调用 */



#endif /* #ifndef CUT_STATUS_DISPLAY_H */

/*=========================================================================================
 * 
 * 本文件结束: cut/status.h
 * 
**=======================================================================================*/


