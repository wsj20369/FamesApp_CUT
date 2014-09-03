/*************************************************************************************
 * 文件:    cut/cut_core.h
 *
 * 描述:    CUT核心定义
 *
 * 作者:    Jun
 *
 * 时间:    2012-10-25
*************************************************************************************/
#ifndef CUT_CORE_H
#define CUT_CORE_H


/*------------------------------------------------------------------------------------
 * 
 *      横切机支持设定
 * 
**----------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------
 * 
 *      当前运行状态
 * 
**----------------------------------------------------------------------------------*/
struct cut_run_state_s {
    int   set_length;
    int   act_length;
    long  good;
    long  remain;
    long  total;
    int   width;
    int   bad;
    int   speed;
    long  elapsed_current;
    long  elapsed_total;
};

typedef struct cut_run_state_s cut_run_state_t;


extern struct cut_run_state_s running_status;



/*------------------------------------------------------------------------------------
 * 
 *      通用例程声明
 * 
**----------------------------------------------------------------------------------*/



#endif /* #ifndef CUT_CORE_H */

/*====================================================================================
 * 
 * 本文件结束: cut/cut_core.h
 * 
**==================================================================================*/

