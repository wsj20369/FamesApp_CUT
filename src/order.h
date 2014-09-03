/*************************************************************************************
 * �ļ�:    order.h
 *
 * ˵��:    CUT�����ļ��ṹ
 *
 * ����:    Jun
 *
 * ʱ��:    2011-2-28, 2012-10-29 (���������˰�, ����ʱ������.....)
 *
 * �汾:    V0.1.2(3)
*************************************************************************************/
#ifndef FAMES_ORDER_H
#define FAMES_ORDER_H

/*------------------------------------------------------------------------------------
 *
 *          ϵͳ֧�ֵ���󶩵���
 *
**----------------------------------------------------------------------------------*/
#define  NUM_OF_ORDERS  1000

/*------------------------------------------------------------------------------------
 *                          �����ļ�������ݽṹ
 * 
 * �����ļ��ɶ����ļ�ͷ�붩����¼���:
 *
 *    1: �����ļ�ͷ: 
 *       1) MAGIC    �����ļ���ʶ��, ����MAGIC��ȷ�϶����ļ��Ƿ���Ч
 *       2) VERSION  �汾��, ���������ڲ�ͬ�汾֮��ת������
 *       3) ORDERS   ��ǰ�Ķ�������
 *       4) WORKNO   ��������, ������Ĺ���, ���ڼ�¼����
 *       5) HARDNO   ��¼���, ��ʾ�����������Ӧ�ļ�¼�ڶ����ļ��е�ʵ��λ��(�������)
 *       6) USEDMAP  ��Чλͼ, �����ÿһ��λ��ʾ���ڶ����ļ�����Ӧ������¼��λ��
 *                   �Ƿ���Ч, ��Ч����˼���Ǹ�λ�ô���һ�ʶ���, 1=��Ч
 *      
 *    2: ������¼:
 *       ������λ��ο���ṹ�嶨��, ���ļ�����, ������¼���Ӷ����ļ�ͷ�洢,
 *       ���汾֧�����1000�ʶ���
 *
 * ����Ϊ�����ļ�ͷ��������¼�ṹ�Ķ���:
**----------------------------------------------------------------------------------*/
typedef struct order_header_s {             /* �����ļ�ͷ�ṹ---------------------- */
    INT32U  MAGIC;                          /* ��ʶ��                               */
    INT16U  VERSION;                        /* �汾��(1~1000)                       */
    INT16U  ORDERS;                         /* ��ǰ�Ķ�������                       */
    INT16U  WORKNO[NUM_OF_ORDERS];          /* ��������                             */ 
    INT16U  HARDNO[NUM_OF_ORDERS];          /* ��¼���                             */
    INT16U  USEDMAP[(NUM_OF_ORDERS+15)/16]; /* �����ļ�����Чλͼ                   */
} order_header_t;                           /* ------------------------------------ */

typedef struct order_record_s {             /* ������¼�ṹ------------------------ */
    INT16U  WORKNO;                         /* ����                                 */
    INT08S  ORDERNO[10];                    /* ������                               */
    INT16U  CUT_LENGTH;                     /* �趨����(�г�)                       */
    INT16U  CUT_LENGTH_INCH;                /* �趨����(�г�), Ӣ��                 */
    INT16U  WIDTH;                          /* ֽ��(����)                           */
    INT32U  NUMBER;                         /* �趨����                             */
    INT16S  UNIT;                           /* ����λ                               */
    INT16U  MODIFIED;                       /* ���޸ı��                           */
} order_struct_t;                           /* ------------------------------------ */

typedef struct order_record_s order_struct;

#define INIT_order_struct { 0, "", 0, 0, 0, 0, 20, }
#define DFLT_order_struct { 0, "--", 1200, 0, 1200, 100, 20, }

#if sizeof(order_header_t) > 32767
#error The struct "order_header" is too large (size > 32K)
#endif
#if (sizeof(order_struct_t)&0x1)!=0
#error The size of struct order_struct is not right, please check it!
#endif


/*-----------------------------------------------------------------------------------
 * 
 *      ��������
 * 
**---------------------------------------------------------------------------------*/
void   InitOrderEnv(void);
void   OpenOrderFile(void);
void   CloseOrderFile(void);
int    FindOrder(INT16U workno);
int    LoadOrder(order_struct * dest, int index, int num);
BOOL   GetOrderForView(order_struct * dest, int index);
BOOL   GetOrderForOrdChg(order_struct * dest, int index);
BOOL   DeleteOrder(int index);
BOOL   InsertOrder(order_struct * order);
BOOL   MoveOrder(INT16U old_wkno, INT16U new_wkno);
BOOL   ZapOrder(void);
BOOL   PackWorkNo(int interval);
int    get_order_nr(void);
void   make_order_info(char * order_info_buf, order_struct * order);
INT16U get_first_workno(void);
INT16U get_last_workno(void);
BOOL   similar_order(order_struct * order1, order_struct * order2);


#endif /* #ifndef FAMES_ORDER_H */

/*====================================================================================
 * 
 * ���ļ�����: order.h
 * 
**==================================================================================*/


