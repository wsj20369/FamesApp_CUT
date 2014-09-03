/*************************************************************************************
 * 文件:    order.c
 *
 * 说明:    订单处理
 *
 * 作者:    Jun
 *
 * 时间:    2011-3-26
 *          2012-10-28 为横切机修改
 *
 * 版本:    V0.1.3(3)
 *
 * 关于:    这个文件实现了订单处理(文件)的功能, 其主要目标为:
 *          1) 订单要以工号排序,并尽可能快速处理
 *          2) 订单文件中的实际顺序与逻辑顺序不同, 由索引实现两者的联系
 *          3) 用位图的方法实现订单文件中空位的查找,加快了插单等动作的速度
 *          4) 工号不可重复!
 *
 **** 历史 *******
 *
 * 1.       2012-10-28, 修改了订单文件的文件名及订单结构(为横切机上位机而改)
 *
 *
*************************************************************************************/
#define  FAMES_ORDER_C
#include <FamesOS.h>
#include "common.h"

/*------------------------------------------------------------------------------------
 * 
 *      文件名及指针
 * 
**----------------------------------------------------------------------------------*/
#define  ORDER_MAGIC      0xFF004F46L       /* 订单索引文件标识字"FO"              */
#define  ORDER_VERSION    1                 /* 订单文件版本号                      */

char   * order_fname =  "order.cut";        /* 订单文件名                          */

/*------------------------------------------------------------------------------------
 * 
 *      订单高速缓冲结构
 * 
**----------------------------------------------------------------------------------*/
struct order_cache_s {
    int  capacity;
    int  nr;
    int  first_index;
    int  need_sync;
    order_struct * orders;
};

static struct order_cache_s cache_for_view    = { /* 用于主画面显示(或浏览) */
        32, 0, 0, 1,
};
static struct order_cache_s cache_for_ord_chg = { /* 用于换单               */
        8, 0, 0, 1,
};

#define ___set_cache_need_sync() cache_for_view.need_sync = 1; \
                                 cache_for_ord_chg.need_sync = 1;


/*------------------------------------------------------------------------------------
 * 
 *      全局缓冲(订单文件头)
 * 
**----------------------------------------------------------------------------------*/
static order_header_t * order_header;        /* 订单文件头缓冲         */
static int              order_handle = -1;


/*------------------------------------------------------------------------------------
 * 函数:    InitOrderEnv()
 *
 * 描述:    初始化订单工作环境
**----------------------------------------------------------------------------------*/
void __exit OnExit_for_Order(void)
{
    void CloseOrderFile(void);

    CloseOrderFile();
}

void InitOrderEnv(void)
{
    void * buf;
    
    buf = mem_alloc((INT32U)sizeof(order_header_t));
    order_header = buf;
    if(!buf)
        goto error;
    buf = mem_alloc((INT32U)sizeof(order_struct) * cache_for_view.capacity);
    cache_for_view.orders = buf;
    if(!buf)
        goto error;
    buf = mem_alloc((INT32U)sizeof(order_struct) * cache_for_ord_chg.capacity);
    cache_for_ord_chg.orders = buf;
    if(!buf)
        goto error;
    if(!RegisterOnExit(OnExit_for_Order)){
        /* 注册OnExit例程失败, 这将导致程序退出时, 订单文件没有关闭的问题 */
        sys_print("RegisterOnExit(OnExit_for_Order) failed!\n");
    }
    OpenOrderFile();
    return;
    
error:
    sys_print("Fatal: memory allocation failed in InitOrderEnv()!\n");
    getch();
    ExitApplication();
}

/*------------------------------------------------------------------------------------
 * 函数:    OpenOrderFile()
 *
 * 描述:    打开订单文件
 *
 * 说明:    此函数会加载订单文件头
**----------------------------------------------------------------------------------*/
void OpenOrderFile(void)
{
    order_struct tor = INIT_order_struct;
    int  i;
    int  fd = -1;

    lock_kernel();
    fd=open(order_fname, O_RDWR|O_BINARY);
    if(fd<0){ /* 如果打开失败,则要尝试创建文件     */
        fd=open(order_fname, O_WRONLY|O_BINARY|O_CREAT, S_IREAD|S_IWRITE);
        if(fd>=0){ /* 写入新文件 */
            MEMSET((INT08S *)order_header, 0, sizeof(order_header_t));
            order_header->MAGIC = ORDER_MAGIC;
            order_header->VERSION = ORDER_VERSION;
            order_header->ORDERS = 0;
            lseek(fd, 0L, SEEK_SET);
            write(fd, (void *)order_header, sizeof(order_header_t));
            for(i=0; i<NUM_OF_ORDERS; i++){
                write(fd, (void *)&tor, sizeof(tor));
            }
            close(fd);
            fd=open(order_fname, O_RDWR|O_BINARY);
        } else {
            sys_print("Fatal: can not create order file!\n");
            getch();
            ExitApplication();
        }
    }
    if(fd>=0){
        if(read(fd, (void *)order_header, sizeof(order_header_t)) > 0){
            if(order_header->MAGIC != ORDER_MAGIC){
                sys_print("Error: order.slc has been corruptted!!!\n\n"
                          "  remove it manually, then it will be a new one!\n\n"
                          "Any key to exit\n");
                getch();
                ExitApplication();
            }
        }
    }
    order_handle = fd;
    unlock_kernel();
}

/*------------------------------------------------------------------------------------
 * 函数:    CloseOrderFile()
 *
 * 说明:    关闭订单文件
**----------------------------------------------------------------------------------*/
void CloseOrderFile(void)
{
    lock_kernel();
    if(order_handle >= 0)
        close(order_handle);
    order_handle = -1;
    unlock_kernel();
}

/*------------------------------------------------------------------------------------
 * 函数:    FindOrder()
 *
 * 描述:    按工号查找一笔订单(二分法查找)
 *
 * 输入:    workno 开始工号
 *
 * 返回:    订单逻辑索引号(-1=查找失败)
**----------------------------------------------------------------------------------*/
#if 1 /* 另一种算法, 来自Linux */
int ___find_order(INT16U workno, int start, int end)
{
    int ___t;
    
    while(start <= end){
        ___t = (end - start)/2 + start;

        if(order_header->WORKNO[___t] < workno)
            start = ___t + 1;
        else if(order_header->WORKNO[___t] > workno)
            end = ___t - 1;
        else
            return ___t;
    }

    return -1;
}
#else /* 以前的算法, 用到了递归 */
int ___find_order(INT16U workno, int start, int end)
{
    int ___t;
    
    if(end - start < 0)
        return -1;

    if(end - start <= 1){
        if(workno == order_header->WORKNO[start])
            return start;
        if(workno == order_header->WORKNO[end])
            return end;
        return -1;
    }

    ___t = (start + end)/2;

    if(workno == order_header->WORKNO[___t])
        return ___t;
    if(workno < order_header->WORKNO[___t])
        return ___find_order(workno, start, ___t);
    else
        return ___find_order(workno, ___t, end);
}
#endif

int FindOrder(INT16U workno)
{
    FamesAssert(order_header);
    
    if(!order_header)
        return -1;

    if(order_header->ORDERS <= 0)
        return -1;

    return ___find_order(workno, 0, (order_header->ORDERS-1));
}

/*------------------------------------------------------------------------------------
 * 函数:    FindOrderSlot()
 *
 * 描述:    按工号查找一个空位(二分法查找)
 *
 * 输入:    workno 开始工号
 *
 * 返回:    该工号应该占有的逻辑位置(-1表示该工号已存在)
**----------------------------------------------------------------------------------*/
int ___find_order_slot(INT16U workno, int start, int end)
{
    int ___t;
    
    if(start >= end)
        return (start+1);

    if(end - start == 1){
        if(workno == order_header->WORKNO[start])
            return -1;
        if(workno == order_header->WORKNO[end])
            return -1;
        if(workno < order_header->WORKNO[start])
            return start;
        return end;
    }

    ___t = (start + end)/2;

    if(workno == order_header->WORKNO[___t])
        return -1;
    else if(workno < order_header->WORKNO[___t])
        return ___find_order_slot(workno, start, ___t);
    else
        return ___find_order_slot(workno, ___t, end);
}

int FindOrderSlot(INT16U workno)
{
    FamesAssert(order_header);
    
    if(!order_header)
        return -2;

    if(order_header->ORDERS <= 0)
        return 0;

    return ___find_order_slot(workno, 0, (order_header->ORDERS));
}


/*------------------------------------------------------------------------------------
 * 函数:    LoadOrder()
 *
 * 描述:    读入一批连续的订单
 *
 * 输入:    dest   存放订单组缓冲的首指针
 *          workno 开始工号
 *          num    需读入的订单数
 *
 * 返回:    实际读到的订单数
**----------------------------------------------------------------------------------*/
void ___load_order(order_struct * dest, INT16U hardno)
{
    long offset;

    FamesAssert(hardno >= 0);
    FamesAssert(hardno < NUM_OF_ORDERS);
    if(hardno < 0 || hardno >= NUM_OF_ORDERS)
        return;

    offset =  (long)sizeof(order_header_t);
    offset += (long)sizeof(order_struct) * hardno;

    lock_kernel();
    lseek(order_handle, offset, SEEK_SET);
    read(order_handle, (void *)dest, sizeof(order_struct));
    unlock_kernel();
    return;
}

int LoadOrder(order_struct * dest, int index, int num)
{
    int i, j, n;

    FamesAssert(order_header);
    FamesAssert(dest);
    FamesAssert(index >= 0);
    FamesAssert(num>=1 && num<=NUM_OF_ORDERS);
    
    if(!order_header || !dest)
        return 0;
    if(index < 0 || num<1 || num>NUM_OF_ORDERS)
        return 0;
    if(index >= order_header->ORDERS)
        return 0;

    if(order_handle < 0)
        return 0;

    j = index;
    j += num;
    if(j > order_header->ORDERS)
        j = order_header->ORDERS;
    if(j > NUM_OF_ORDERS)
        j = NUM_OF_ORDERS;

    for(i=index,n=0; i<j; i++, n++){
        ___load_order(&dest[n], order_header->HARDNO[i]);
    }
    
    return n;
}

/*------------------------------------------------------------------------------------
 * 函数:    GetOrderForView()
 *
 * 说明:    为了主画面显示而读取订单
 *
 * 输入:    dest   订单资料将存放的位置
 *          index  索引项序号(0~INDEX_BUF.HEADER.ORDERS-1)
 *
 * 返回:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL GetOrderForView(order_struct * dest, int index)
{
    int  nr;
    BOOL retval;
    static int lock = 0;
    
    FamesAssert(order_header);
    FamesAssert(dest);
    FamesAssert(index >= 0);

    retval = fail;
    os_mutex_lock(lock);
    
    if(!order_header || !dest)
        goto out;
    if(index < 0)
        goto out;
    if(order_handle < 0)
        goto out;

    if(index >= order_header->ORDERS)
        goto out;

    if(cache_for_view.need_sync || 
       cache_for_view.nr == 0 || index < cache_for_view.first_index ||
       index >= cache_for_view.first_index + cache_for_view.nr){
        lock_kernel();
        nr = LoadOrder(cache_for_view.orders, index, cache_for_view.capacity);
        cache_for_view.first_index = index;
        cache_for_view.nr = nr;
        cache_for_view.need_sync = 0;
        unlock_kernel();
    }
    if(cache_for_view.nr == 0 || index < cache_for_view.first_index ||
       index >= cache_for_view.first_index + cache_for_view.nr){
        goto out;
    }
    lock_kernel();
    *dest = cache_for_view.orders[index - cache_for_view.first_index];
    unlock_kernel();

    retval = ok;

out:
    os_mutex_unlock(lock);
    
    return retval;
}

/*------------------------------------------------------------------------------------
 * 函数:    GetOrderForOrdChg()
 *
 * 说明:    与换单相关的订单读取
 *
 * 输入:    dest   订单资料将存放的位置
 *          index  索引项序号(0~INDEX_BUF.HEADER.ORDERS-1)
 *
 * 返回:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL GetOrderForOrdChg(order_struct * dest, int index)
{
    int  nr;
    BOOL retval;
    static int lock = 0;
    
    FamesAssert(order_header);
    FamesAssert(dest);
    FamesAssert(index >= 0);

    retval = fail;
    os_mutex_lock(lock);
    
    if(!order_header || !dest)
        goto out;
    if(index < 0)
        goto out;
    if(order_handle < 0)
        goto out;

    if(index >= order_header->ORDERS)
        goto out;

    if(cache_for_ord_chg.need_sync || 
       cache_for_ord_chg.nr == 0 || index < cache_for_ord_chg.first_index ||
       index >= cache_for_ord_chg.first_index + cache_for_ord_chg.nr){
        lock_kernel();
        nr = LoadOrder(cache_for_ord_chg.orders, index, cache_for_ord_chg.capacity);
        cache_for_ord_chg.first_index = index;
        cache_for_ord_chg.nr = nr;
        cache_for_ord_chg.need_sync = 0;
        unlock_kernel();
    }
    if(cache_for_ord_chg.nr == 0 || index < cache_for_ord_chg.first_index ||
       index >= cache_for_ord_chg.first_index + cache_for_ord_chg.nr){
        goto out;
    }
    lock_kernel();
    *dest = cache_for_ord_chg.orders[index - cache_for_ord_chg.first_index];
    unlock_kernel();

    retval = ok;
out:
    os_mutex_unlock(lock);
    
    return retval;
}

/*------------------------------------------------------------------------------------
 * 函数:    DeleteOrder()
 *
 * 描述:    删除一笔订单(按逻辑序号)
 *
 * 参数:    index 订单的逻辑序号, 0=第一笔
 *
 * 返回:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL DeleteOrder(int index)
{
    BOOL retval;
    int  i, nr;
    
    FamesAssert(order_header);
    FamesAssert(index >= 0);
    FamesAssert(index < NUM_OF_ORDERS);

    if(!order_header)
        return fail;
    if(index < 0)
        return fail;
    if(order_handle < 0)
        return fail;

    lock_kernel();
    nr = order_header->ORDERS;
    if(index >= nr || nr <= 0){
        retval = fail;
    } else {
        INT16U hardno;
        int    x, y;
        hardno = order_header->HARDNO[index];
        nr--; /* 这里已经减1了, 下面的for语句不可再减1, 比如: for(i=index; i<(nr-1); i++)是错误的 */
        for(i=index; i<nr; i++){
            order_header->HARDNO[i] = order_header->HARDNO[i+1];
            order_header->WORKNO[i] = order_header->WORKNO[i+1];
        }
        order_header->HARDNO[i] = 0;
        order_header->WORKNO[i] = 0;
        order_header->ORDERS--;
        x = (int)(hardno&0xf);
        y = (int)(hardno>>4);
        order_header->USEDMAP[y] &= (~BITOFF16[x]);
        lseek(order_handle, 0L, SEEK_SET);
        write(order_handle, (void *)order_header, sizeof(order_header_t));
        retval = ok;
        ___set_cache_need_sync();
    }
    unlock_kernel();

    return retval;
}

/*------------------------------------------------------------------------------------
 * 函数:    InsertOrder()
 *
 * 描述:    插入一笔订单(按工号)
 *
 * 参数:    order 新的订单
 *
 * 返回:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL InsertOrder(order_struct * order)
{
    BOOL retval;
    int  i, nr;
    int  x, y;
    int  index, hardno;
    INT16U ___t;
    
    FamesAssert(order_header);
    FamesAssert(order);

    if(!order_header || !order)
        return fail;
    if(order_handle < 0)
        return fail;

    hardno = -1;
    retval = fail;
    lock_kernel();
    nr = order_header->ORDERS;
    index = FindOrder(order->WORKNO);
    if(index >=0){ /* 此工号已存在: 修改订单 */
        hardno = order_header->HARDNO[index];
    } else { /* 此工号不存在, 找一个空位: 插入订单 */
        if(nr >= NUM_OF_ORDERS)
            goto out;
        index = FindOrderSlot(order->WORKNO);
        if(index < 0)
            goto out;
        /* 在订单索引区挪一个空位出来, index=目标空位的序号 */
        for(i=nr; i>index; i--){
            order_header->HARDNO[i] = order_header->HARDNO[i-1];
            order_header->WORKNO[i] = order_header->WORKNO[i-1];
        }
        /* 在订单数据区找一个空位 */
        for(y=0; y<sizeof(order_header->USEDMAP); y++){
            if(order_header->USEDMAP[y] != 0xFFFF)
                break;
        }
        FamesAssert(y<sizeof(order_header->USEDMAP));
        ___t = order_header->USEDMAP[y];
        ___t = ~___t; /* 按位取反 */
        for(x=0; x<16; x++){
            if(___t & BITOFF16[x])
                break;
        }
        order_header->USEDMAP[y] |= BITOFF16[x]; /* 位图置位 */
        /* 找到了... */
        hardno = (y*16)+x;
        if(i < 0)
            i = 0;
        order_header->HARDNO[i] = hardno;
        order_header->WORKNO[i] = order->WORKNO;
        order_header->ORDERS++;
    }
    if(hardno >= 0){
        /* 已经确定了数据区的位置, 现在就把订单保存到那里去... */
        long  offset;        
        offset =  (long)sizeof(order_header_t);
        offset += (long)sizeof(order_struct) * hardno;
        lseek(order_handle, offset, SEEK_SET);
        write(order_handle, (void *)order, sizeof(order_struct));
        lseek(order_handle, 0L, SEEK_SET);
        write(order_handle, (void *)order_header, sizeof(order_header_t));
        retval = ok;
        ___set_cache_need_sync();
    }
    
out:
    unlock_kernel();
    return retval;
}

/*------------------------------------------------------------------------------------
 * 函数:    MoveOrder()
 *
 * 描述:    调单(更改工号)
 *
 * 参数:    order    目标订单
 *          new_wkno 新的工号
 *
 * 返回:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL MoveOrder(INT16U old_wkno, INT16U new_wkno)
{
    int  i, nr, index;
    BOOL retval;
    INT16U hardno;

    FamesAssert(order_header);

    if(!order_header)
        return fail;
    if(order_handle < 0)
        return fail;

    retval = fail;
    lock_kernel();
    if(FindOrder(new_wkno) >= 0) /* 目标工号已存在 */
        goto out;

    index = FindOrder(old_wkno);
    if(index < 0) /* 没有这个订单 */
        goto out;

    nr = order_header->ORDERS;
    /* 从原来的位置取出来 */
    hardno = order_header->HARDNO[index];
    FamesAssert(hardno >= 0);
    FamesAssert(hardno < NUM_OF_ORDERS);
    if(hardno < 0 || hardno >= NUM_OF_ORDERS)
        goto out;
    for(i=index; i<(nr-1); i++){
        order_header->HARDNO[i] = order_header->HARDNO[i+1];
        order_header->WORKNO[i] = order_header->WORKNO[i+1];
    }
    order_header->HARDNO[i] = 0;
    order_header->WORKNO[i] = 0;
    /* 找到应该插入的位置... */
    order_header->ORDERS--;
    index = FindOrderSlot(new_wkno);
    order_header->ORDERS++;
    if(index < 0) /* ...没找到 */
        goto restore_header;
    /* 在订单索引区挪一个空位出来, index=目标空位的序号 */
    for(i=nr; i>index; i--){
        order_header->HARDNO[i] = order_header->HARDNO[i-1];
        order_header->WORKNO[i] = order_header->WORKNO[i-1];
    }
    order_header->HARDNO[i] = hardno;
    order_header->WORKNO[i] = new_wkno;
    lseek(order_handle, 0L, SEEK_SET);
    write(order_handle, (void *)order_header, sizeof(order_header_t));
    if(1){ /* 将新工号写入数据区 */
        long  offset;        
        offset =  (long)sizeof(order_header_t);
        offset += (long)sizeof(order_struct) * hardno;
        lseek(order_handle, offset, SEEK_SET);
        write(order_handle, (void *)&new_wkno, sizeof(new_wkno));
    }
    retval = ok;
    ___set_cache_need_sync();

restore_header:
    lseek(order_handle, 0L, SEEK_SET);
    read(order_handle, (void *)order_header, sizeof(order_header_t));
    
out:
    unlock_kernel();
    return retval;
}

/*------------------------------------------------------------------------------------
 * 函数:    ZapOrder()
 *
 * 描述:    清除所有订单
 *
 * 返回:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL ZapOrder(void)
{
    FamesAssert(order_header);

    if(!order_header)
        return fail;
    if(order_handle < 0)
        return fail;

    lock_kernel();
    MEMSET((INT08S *)order_header, 0, sizeof(order_header_t));
    order_header->MAGIC = ORDER_MAGIC;
    order_header->VERSION = ORDER_VERSION;
    order_header->ORDERS = 0;
    lseek(order_handle, 0L, SEEK_SET);
    write(order_handle, (void *)order_header, sizeof(order_header_t));
    ___set_cache_need_sync();
    unlock_kernel();

    return ok;
}

/*------------------------------------------------------------------------------------
 * 函数:    PackWorkNo()
 *
 * 描述:    重整订单工号
 *
 * 返回:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL PackWorkNo(int interval)
{
    int i, hardno;
    INT16U workno;
    long  offset;        
    
    FamesAssert(order_header);
    FamesAssert(interval > 0);

    if(!order_header)
        return fail;
    if(order_handle < 0)
        return fail;
    if(interval <= 0)
        return fail;
    lock_kernel();
    i = order_header->ORDERS;
    unlock_kernel();
    i *= interval;
    if(i<0 || i>9999)
        return fail;

    lock_kernel();
    workno = (INT16U)interval;
    for(i=0; i<order_header->ORDERS; i++){
        order_header->WORKNO[i] = workno;
        hardno = order_header->HARDNO[i];
        offset =  (long)sizeof(order_header_t);
        offset += (long)sizeof(order_struct) * hardno;
        lseek(order_handle, offset, SEEK_SET);
        write(order_handle, (void *)&workno, sizeof(workno));
        workno += (INT16U)interval;
    }
    lseek(order_handle, 0L, SEEK_SET);
    write(order_handle, (void *)order_header, sizeof(order_header_t));
    ___set_cache_need_sync();
    unlock_kernel();

    return ok;
}

/*------------------------------------------------------------------------------------
 * 函数:    get_first_workno()
 *
 * 描述:    读取第一笔订单的工号
**----------------------------------------------------------------------------------*/
INT16U get_first_workno(void)
{
    INT16U wkno;
    int    nr;
    FamesAssert(order_header);

    if(!order_header)
        return 0;

    lock_kernel();
    nr = order_header->ORDERS;
    if(nr > 0)
        wkno = order_header->WORKNO[0];
    else
        wkno = 0;
    unlock_kernel();

    return wkno;
}

/*------------------------------------------------------------------------------------
 * 函数:    get_last_workno()
 *
 * 描述:    读取最后一笔订单的工号
**----------------------------------------------------------------------------------*/
INT16U get_last_workno(void)
{
    INT16U wkno;
    int    nr;
    FamesAssert(order_header);

    if(!order_header)
        return 0;

    lock_kernel();
    nr = order_header->ORDERS;
    if(nr > 0)
        wkno = order_header->WORKNO[nr-1];
    unlock_kernel();

    if(nr == 0)
        return 0;

    return wkno;
}

/*------------------------------------------------------------------------------------
 * 函数:    get_order_nr()
 *
 * 描述:    读取订单总数
**----------------------------------------------------------------------------------*/
int get_order_nr(void)
{
    int    nr;
    FamesAssert(order_header);

    if(!order_header)
        return 0;

    lock_kernel();
    nr = order_header->ORDERS;
    unlock_kernel();

    FamesAssert(nr >= 0);

    return nr;
}

/*------------------------------------------------------------------------------------
 * 函数:    make_order_info()
 *
 * 描述:    生成订单信息文本
**----------------------------------------------------------------------------------*/
void make_order_info(char * order_info_buf, order_struct * order)
{
    FamesAssert(order_info_buf);
    FamesAssert(order);

    if(!order_info_buf || !order)
        return;

    sprintf(order_info_buf,
            pick_string(
                "=== 未实现 ===",
                "=== Not Implemented ==="
            )
           );        
}

/*------------------------------------------------------------------------------------
 * 函数:    similar_order()
 *
 * 描述:    两笔订单是否相似
**----------------------------------------------------------------------------------*/
BOOL similar_order(order_struct * order1, order_struct * order2)
{
    FamesAssert(order1);
    FamesAssert(order2);
    if(!order1 || !order2)
        return fail;

    if(order1->CUT_LENGTH != order2->CUT_LENGTH)
        return fail;

    return ok;
}


/*====================================================================================
 * 
 * 本文件结束: order.c
 * 
**==================================================================================*/


