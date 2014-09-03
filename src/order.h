/*************************************************************************************
 * 文件:    order.h
 *
 * 说明:    CUT订单文件结构
 *
 * 作者:    Jun
 *
 * 时间:    2011-2-28, 2012-10-29 (都快两年了啊, 真是时光如梭.....)
 *
 * 版本:    V0.1.2(3)
*************************************************************************************/
#ifndef FAMES_ORDER_H
#define FAMES_ORDER_H

/*------------------------------------------------------------------------------------
 *
 *          系统支持的最大订单数
 *
**----------------------------------------------------------------------------------*/
#define  NUM_OF_ORDERS  1000

/*------------------------------------------------------------------------------------
 *                          订单文件相关数据结构
 * 
 * 订单文件由订单文件头与订单记录组成:
 *
 *    1: 订单文件头: 
 *       1) MAGIC    订单文件标识字, 根据MAGIC可确认订单文件是否有效
 *       2) VERSION  版本号, 将来可用在不同版本之间转移数据
 *       3) ORDERS   当前的订单数量
 *       4) WORKNO   索引工号, 已排序的工号, 用于记录索引
 *       5) HARDNO   记录序号, 表示索引工号相对应的记录在订单文件中的实际位置(订单序号)
 *       6) USEDMAP  有效位图, 此域的每一个位表示了在订单文件中相应订单记录的位置
 *                   是否有效, 有效的意思是那个位置存有一笔订单, 1=有效
 *      
 *    2: 订单记录:
 *       具体栏位请参考其结构体定义, 在文件当中, 订单记录紧接订单文件头存储,
 *       本版本支持最多1000笔订单
 *
 * 下面为订单文件头、订单记录结构的定义:
**----------------------------------------------------------------------------------*/
typedef struct order_header_s {             /* 订单文件头结构---------------------- */
    INT32U  MAGIC;                          /* 标识字                               */
    INT16U  VERSION;                        /* 版本号(1~1000)                       */
    INT16U  ORDERS;                         /* 当前的订单数量                       */
    INT16U  WORKNO[NUM_OF_ORDERS];          /* 索引工号                             */ 
    INT16U  HARDNO[NUM_OF_ORDERS];          /* 记录序号                             */
    INT16U  USEDMAP[(NUM_OF_ORDERS+15)/16]; /* 订单文件中有效位图                   */
} order_header_t;                           /* ------------------------------------ */

typedef struct order_record_s {             /* 订单记录结构------------------------ */
    INT16U  WORKNO;                         /* 工号                                 */
    INT08S  ORDERNO[10];                    /* 定单号                               */
    INT16U  CUT_LENGTH;                     /* 设定长度(切长)                       */
    INT16U  CUT_LENGTH_INCH;                /* 设定长度(切长), 英制                 */
    INT16U  WIDTH;                          /* 纸幅(毫米)                           */
    INT32U  NUMBER;                         /* 设定数量                             */
    INT16S  UNIT;                           /* 捆单位                               */
    INT16U  MODIFIED;                       /* 已修改标记                           */
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
 *      函数声明
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
 * 本文件结束: order.h
 * 
**==================================================================================*/


