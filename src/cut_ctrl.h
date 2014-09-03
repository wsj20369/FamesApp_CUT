/*************************************************************************************
 * 文件:    cut/cut_ctrl.h
 *
 * 描述:    横切机控制例程
 *
 * 作者:    Jun
 *
 * 时间:    2011-4-8, 2012-10-29
*************************************************************************************/
#ifndef CUT_CONTROL_H
#define CUT_CONTROL_H


/*------------------------------------------------------------------------------------
 * 
 *      函数声明
 * 
**----------------------------------------------------------------------------------*/
void CUT_control_service_initialize(void);

#if 0
BOOL slc_send_order(int slc_index, order_struct * order, int no_control);
void reset_kl_on_quit(void);
void slc_read_act_value(int slc_index);
void slc_send_kl_act_value(int slc_index);
void slc_read_regress_value(int slc_index);
void slc_send_regress_value(int slc_index);
void slc_read_kl_unit_value(int slc_index);
void slc_send_kl_unit_value(int slc_index);
void slc_read_kl_fix_value(int slc_index);
void slc_send_kl_fix_value(int slc_index);
void slc_set_yx(int slc_index, int yx);
void slc_send_deep_value(int slc_index, int deep);
void slc_send_fan_value(int slc_index, int fan);
void slc_kl_down_set(int slc_index);
void slc_kl_down_reset(int slc_index);
void slc_kl_up_set(int slc_index);
void slc_kl_up_reset(int slc_index);
void slc_send_start(int slc_index);
void slc_clear_fix_ok(int slc_index);
void set_plc_on_startup(int slc_index);

void ___slc_plc_rw(int slc_index, int cmd, void * addr, void * associated, int number);
void ___slc_plc_rw_ensure(int slc_index, int cmd, void * addr, void * associated, int number);
#endif

#endif /* #ifndef CUT_CONTROL_H */

/*====================================================================================
 * 
 * 本文件结束: cut/cut_ctrl.h
 * 
**==================================================================================*/

