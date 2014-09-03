/*************************************************************************************
 * �ļ�:    cut/cut_ctrl.c
 *
 * ����:    ���л���������
 *
 * ����:    Jun
 *
 * ʱ��:    2011-4-8, 2012-10-29
*************************************************************************************/
#define  CUT_CONTROL_C
#include <includes.h>
#include "common.h"


/*------------------------------------------------------------------------------------
 * ����:    CUT_control_service()
 *
 * ����:    CUT����Ҫ���ƹ���(CUT������)
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
    OrdChg_Flag = 0; /* ������־��λ */

  
    /*
    ** �������� ----------------------------------------------------------------
    */
    if((OrdChg_Flag == 1)||(OrdChg_Flag == 2)){
        int  i, last, next;
        os_mutex_lock(ord_chg_lock);
        if(get_order_nr()>=0){
            DeleteOrder(0); /* ɾ����һ�ʶ��� */
            last = OrdChg_Flag; /* �ղž��ڹ����Ļ��� */
            next = OrdChg_Flag; /* ����ѡ�еĻ���     */
            i = 0;
            if((OrdChg_Flag == 1) && (config.slc_used == 3)){
                next = 2;
                i = 1; /* �ӵ�2�ʵ���ʼ��� */
            }
            if((OrdChg_Flag == 2) && (config.slc_used == 3)){
                next = 1;
                i = 1; /* �ӵ�2�ʵ���ʼ��� */
            }
            for(; i < 5; i++){
                if(!GetOrderForOrdChg(&order, i))
                    break;
                if(similar_order(&order, &(config.slc[last-1].working)))
                    break; /* ��ղ����ĵ�һ��, ����Ҫ�ٷ����� */
                if(config.slc_used == 3){
                    if(similar_order(&order, &(config.slc[next-1].working)))
                        continue; /* ������Ҫ�����Ķ���һ��, ���� */
                }
                /* 
                 * ������, �����Ѿ��ҵ���һ����Ҫ���͵Ķ���, ��ʵ�Ӧ�ñ����͵� "�ղ��ڹ���, ����������, ����Ҫ����׼��" �Ļ�����
                 * Ҳ����last!
                */
                if(!slc_send_order(last, &order, 0)){
                    /* ����ʧ�� */
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
 * ����:    CUT_control_service_initialize()
 *
 * ����:    CUT��������ʼ��
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
 * ���ļ�����: cut/cut_ctrl.c
 * 
**==================================================================================*/

