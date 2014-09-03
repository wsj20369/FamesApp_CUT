/*************************************************************************************
 * 文件:    cut/cut_ctrl.c
 *
 * 描述:    横切机控制例程
 *
 * 作者:    Jun
 *
 * 时间:    2011-4-8, 2012-10-29
*************************************************************************************/
#define  CUT_CONTROL_C
#include <includes.h>
#include "common.h"


/*------------------------------------------------------------------------------------
 * 函数:    CUT_control_service()
 *
 * 描述:    CUT的主要控制过程(CUT服务器)
**----------------------------------------------------------------------------------*/

void __daemon CUT_control_service(void * data)
{
    #if 0
    struct slc_run_state_s *state, *state1, *state2, old_state[2];
    slc_descriptor_t * slc;
    int  slc_index, OrdChg_Flag;
    int  Start_Flag_delay[2], Start_off_time[2];
    int  Regu_Flag_delay[2], Regu_Off_delay[2]; /* Regu: Regulate */
    int  CimChg_Flag, CimChg_Step, CIM_COK, CIMUP;
    int  ORD_USE[2];
    order_struct order;

    data = data;

    MEMSET((INT08S*)state1,        0, sizeof(old_state[0]));
    MEMSET((INT08S*)state2,        0, sizeof(old_state[0]));
    MEMSET((INT08S*)&old_state[0], 0, sizeof(old_state[0]));
    MEMSET((INT08S*)&old_state[1], 0, sizeof(old_state[0]));
#endif

again_and_again:
    #if 0
    OrdChg_Flag = 0; /* 换单标志复位 */

  
    /*
    ** 换单过程 ----------------------------------------------------------------
    */
    if((OrdChg_Flag == 1)||(OrdChg_Flag == 2)){
        int  i, last, next;
        os_mutex_lock(ord_chg_lock);
        if(get_order_nr()>=0){
            DeleteOrder(0); /* 删除第一笔订单 */
            last = OrdChg_Flag; /* 刚才就在工作的机器 */
            next = OrdChg_Flag; /* 将被选中的机器     */
            i = 0;
            if((OrdChg_Flag == 1) && (config.slc_used == 3)){
                next = 2;
                i = 1; /* 从第2笔单开始检查 */
            }
            if((OrdChg_Flag == 2) && (config.slc_used == 3)){
                next = 1;
                i = 1; /* 从第2笔单开始检查 */
            }
            for(; i < 5; i++){
                if(!GetOrderForOrdChg(&order, i))
                    break;
                if(similar_order(&order, &(config.slc[last-1].working)))
                    break; /* 与刚才做的单一样, 不需要再发送了 */
                if(config.slc_used == 3){
                    if(similar_order(&order, &(config.slc[next-1].working)))
                        continue; /* 与马上要工作的订单一样, 跳过 */
                }
                /* 
                 * 到这里, 我们已经找到了一笔需要发送的订单, 这笔单应该被发送到 "刚才在工作, 而现在已起刀, 并需要作好准备" 的机器上
                 * 也就是last!
                */
                if(!slc_send_order(last, &order, 0)){
                    /* 发送失败 */
                }
                break;
            }
        }
        OrdChg_Flag = 0;
        os_mutex_unlock(ord_chg_lock);
    }
    #endif
    TaskDelay(50);

    goto again_and_again;
}

/*------------------------------------------------------------------------------------
 * 函数:    CUT_control_service_initialize()
 *
 * 描述:    CUT服务器初始化
**----------------------------------------------------------------------------------*/
void CUT_control_service_initialize(void)
{
    HANDLE task;

    task = TaskCreate(CUT_control_service, NULL, "cutter-ctrl-svc", NULL, 2048, PRIO_NORMAL, TASK_CREATE_OPT_NONE);
    if(task == InvalidHandle){
        sys_print("CUT_control_service_initialize() failed\n");
        ExitApplication();
    }
}


/*====================================================================================
 * 
 * 本文件结束: cut/cut_ctrl.c
 * 
**==================================================================================*/

