/*************************************************************************************
 * �ļ�:    order.c
 *
 * ˵��:    ��������
 *
 * ����:    Jun
 *
 * ʱ��:    2011-3-26
 *          2012-10-28 Ϊ���л��޸�
 *
 * �汾:    V0.1.3(3)
 *
 * ����:    ����ļ�ʵ���˶�������(�ļ�)�Ĺ���, ����ҪĿ��Ϊ:
 *          1) ����Ҫ�Թ�������,�������ܿ��ٴ���
 *          2) �����ļ��е�ʵ��˳�����߼�˳��ͬ, ������ʵ�����ߵ���ϵ
 *          3) ��λͼ�ķ���ʵ�ֶ����ļ��п�λ�Ĳ���,�ӿ��˲嵥�ȶ������ٶ�
 *          4) ���Ų����ظ�!
 *
 **** ��ʷ *******
 *
 * 1.       2012-10-28, �޸��˶����ļ����ļ����������ṹ(Ϊ���л���λ������)
 *
 *
*************************************************************************************/
#define  FAMES_ORDER_C
#include <FamesOS.h>
#include "common.h"

/*------------------------------------------------------------------------------------
 * 
 *      �ļ�����ָ��
 * 
**----------------------------------------------------------------------------------*/
#define  ORDER_MAGIC      0xFF004F46L       /* ���������ļ���ʶ��"FO"              */
#define  ORDER_VERSION    1                 /* �����ļ��汾��                      */

char   * order_fname =  "order.cut";        /* �����ļ���                          */

/*------------------------------------------------------------------------------------
 * 
 *      �������ٻ���ṹ
 * 
**----------------------------------------------------------------------------------*/
struct order_cache_s {
    int  capacity;
    int  nr;
    int  first_index;
    int  need_sync;
    order_struct * orders;
};

static struct order_cache_s cache_for_view    = { /* ������������ʾ(�����) */
        32, 0, 0, 1,
};
static struct order_cache_s cache_for_ord_chg = { /* ���ڻ���               */
        8, 0, 0, 1,
};

#define ___set_cache_need_sync() cache_for_view.need_sync = 1; \
                                 cache_for_ord_chg.need_sync = 1;


/*------------------------------------------------------------------------------------
 * 
 *      ȫ�ֻ���(�����ļ�ͷ)
 * 
**----------------------------------------------------------------------------------*/
static order_header_t * order_header;        /* �����ļ�ͷ����         */
static int              order_handle = -1;


/*------------------------------------------------------------------------------------
 * ����:    InitOrderEnv()
 *
 * ����:    ��ʼ��������������
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
        /* ע��OnExit����ʧ��, �⽫���³����˳�ʱ, �����ļ�û�йرյ����� */
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
 * ����:    OpenOrderFile()
 *
 * ����:    �򿪶����ļ�
 *
 * ˵��:    �˺�������ض����ļ�ͷ
**----------------------------------------------------------------------------------*/
void OpenOrderFile(void)
{
    order_struct tor = INIT_order_struct;
    int  i;
    int  fd = -1;

    lock_kernel();
    fd=open(order_fname, O_RDWR|O_BINARY);
    if(fd<0){ /* �����ʧ��,��Ҫ���Դ����ļ�     */
        fd=open(order_fname, O_WRONLY|O_BINARY|O_CREAT, S_IREAD|S_IWRITE);
        if(fd>=0){ /* д�����ļ� */
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
 * ����:    CloseOrderFile()
 *
 * ˵��:    �رն����ļ�
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
 * ����:    FindOrder()
 *
 * ����:    �����Ų���һ�ʶ���(���ַ�����)
 *
 * ����:    workno ��ʼ����
 *
 * ����:    �����߼�������(-1=����ʧ��)
**----------------------------------------------------------------------------------*/
#if 1 /* ��һ���㷨, ����Linux */
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
#else /* ��ǰ���㷨, �õ��˵ݹ� */
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
 * ����:    FindOrderSlot()
 *
 * ����:    �����Ų���һ����λ(���ַ�����)
 *
 * ����:    workno ��ʼ����
 *
 * ����:    �ù���Ӧ��ռ�е��߼�λ��(-1��ʾ�ù����Ѵ���)
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
 * ����:    LoadOrder()
 *
 * ����:    ����һ�������Ķ���
 *
 * ����:    dest   ��Ŷ����黺�����ָ��
 *          workno ��ʼ����
 *          num    �����Ķ�����
 *
 * ����:    ʵ�ʶ����Ķ�����
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
 * ����:    GetOrderForView()
 *
 * ˵��:    Ϊ����������ʾ����ȡ����
 *
 * ����:    dest   �������Ͻ���ŵ�λ��
 *          index  ���������(0~INDEX_BUF.HEADER.ORDERS-1)
 *
 * ����:    ok/fail
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
 * ����:    GetOrderForOrdChg()
 *
 * ˵��:    �뻻����صĶ�����ȡ
 *
 * ����:    dest   �������Ͻ���ŵ�λ��
 *          index  ���������(0~INDEX_BUF.HEADER.ORDERS-1)
 *
 * ����:    ok/fail
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
 * ����:    DeleteOrder()
 *
 * ����:    ɾ��һ�ʶ���(���߼����)
 *
 * ����:    index �������߼����, 0=��һ��
 *
 * ����:    ok/fail
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
        nr--; /* �����Ѿ���1��, �����for��䲻���ټ�1, ����: for(i=index; i<(nr-1); i++)�Ǵ���� */
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
 * ����:    InsertOrder()
 *
 * ����:    ����һ�ʶ���(������)
 *
 * ����:    order �µĶ���
 *
 * ����:    ok/fail
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
    if(index >=0){ /* �˹����Ѵ���: �޸Ķ��� */
        hardno = order_header->HARDNO[index];
    } else { /* �˹��Ų�����, ��һ����λ: ���붩�� */
        if(nr >= NUM_OF_ORDERS)
            goto out;
        index = FindOrderSlot(order->WORKNO);
        if(index < 0)
            goto out;
        /* �ڶ���������Ųһ����λ����, index=Ŀ���λ����� */
        for(i=nr; i>index; i--){
            order_header->HARDNO[i] = order_header->HARDNO[i-1];
            order_header->WORKNO[i] = order_header->WORKNO[i-1];
        }
        /* �ڶ�����������һ����λ */
        for(y=0; y<sizeof(order_header->USEDMAP); y++){
            if(order_header->USEDMAP[y] != 0xFFFF)
                break;
        }
        FamesAssert(y<sizeof(order_header->USEDMAP));
        ___t = order_header->USEDMAP[y];
        ___t = ~___t; /* ��λȡ�� */
        for(x=0; x<16; x++){
            if(___t & BITOFF16[x])
                break;
        }
        order_header->USEDMAP[y] |= BITOFF16[x]; /* λͼ��λ */
        /* �ҵ���... */
        hardno = (y*16)+x;
        if(i < 0)
            i = 0;
        order_header->HARDNO[i] = hardno;
        order_header->WORKNO[i] = order->WORKNO;
        order_header->ORDERS++;
    }
    if(hardno >= 0){
        /* �Ѿ�ȷ������������λ��, ���ھͰѶ������浽����ȥ... */
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
 * ����:    MoveOrder()
 *
 * ����:    ����(���Ĺ���)
 *
 * ����:    order    Ŀ�궩��
 *          new_wkno �µĹ���
 *
 * ����:    ok/fail
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
    if(FindOrder(new_wkno) >= 0) /* Ŀ�깤���Ѵ��� */
        goto out;

    index = FindOrder(old_wkno);
    if(index < 0) /* û��������� */
        goto out;

    nr = order_header->ORDERS;
    /* ��ԭ����λ��ȡ���� */
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
    /* �ҵ�Ӧ�ò����λ��... */
    order_header->ORDERS--;
    index = FindOrderSlot(new_wkno);
    order_header->ORDERS++;
    if(index < 0) /* ...û�ҵ� */
        goto restore_header;
    /* �ڶ���������Ųһ����λ����, index=Ŀ���λ����� */
    for(i=nr; i>index; i--){
        order_header->HARDNO[i] = order_header->HARDNO[i-1];
        order_header->WORKNO[i] = order_header->WORKNO[i-1];
    }
    order_header->HARDNO[i] = hardno;
    order_header->WORKNO[i] = new_wkno;
    lseek(order_handle, 0L, SEEK_SET);
    write(order_handle, (void *)order_header, sizeof(order_header_t));
    if(1){ /* ���¹���д�������� */
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
 * ����:    ZapOrder()
 *
 * ����:    ������ж���
 *
 * ����:    ok/fail
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
 * ����:    PackWorkNo()
 *
 * ����:    ������������
 *
 * ����:    ok/fail
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
 * ����:    get_first_workno()
 *
 * ����:    ��ȡ��һ�ʶ����Ĺ���
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
 * ����:    get_last_workno()
 *
 * ����:    ��ȡ���һ�ʶ����Ĺ���
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
 * ����:    get_order_nr()
 *
 * ����:    ��ȡ��������
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
 * ����:    make_order_info()
 *
 * ����:    ���ɶ�����Ϣ�ı�
**----------------------------------------------------------------------------------*/
void make_order_info(char * order_info_buf, order_struct * order)
{
    FamesAssert(order_info_buf);
    FamesAssert(order);

    if(!order_info_buf || !order)
        return;

    sprintf(order_info_buf,
            pick_string(
                "=== δʵ�� ===",
                "=== Not Implemented ==="
            )
           );        
}

/*------------------------------------------------------------------------------------
 * ����:    similar_order()
 *
 * ����:    ���ʶ����Ƿ�����
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
 * ���ļ�����: order.c
 * 
**==================================================================================*/


