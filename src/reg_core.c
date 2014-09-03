/******************************************************************************************
 * 文件:    reg_core.c
 *
 * 描述:    Register Service Core
 *
 * 作者:    Jun
 *
 * 说明:    注册SN保存在当前目录下的cut.sn中, 此文件由程序自动生成.
 *
 * 创建:    2011-3-31(for SLC), 2012-11-27(for Cut)
******************************************************************************************/
#define  REGISTER_SERVICE_CORE
#include <FamesOS.h>
#include "reg_core.h"

/*-----------------------------------------------------------------------------------------
 * slc.sn file structure
**---------------------------------------------------------------------------------------*/
struct sn_struct {
    int magic; /* Magic Number: 0x4e53, 'SN' */
    unsigned char sn_seed[2];
    unsigned char lrc[2];
    unsigned char sn_encrypted1[SN_LEN];
    unsigned char sn_encrypted2[SN_LEN];
};

#define SN_STRUCT_MAGIC 0x4e53

/*-----------------------------------------------------------------------------------------
 * machine id structure
**---------------------------------------------------------------------------------------*/
#define MACH_ID_LEN    8    /* 硬件ID号只有8位 */

struct mach_id_struct {
    int magic; /* Magic Number: 0x4449, 'ID' */
    unsigned char lrc[2];
    unsigned char id_encrypted1[MACH_ID_LEN];
    unsigned char id_encrypted2[MACH_ID_LEN];
};

#define ID_STRUCT_MAGIC 0x4449

#if sizeof(struct mach_id_struct) != ID_LEN
#error sizeof(struct mach_id_struct) != ID_LEN, please check it!
#endif

#define __ID_ENCRYPT(c, seed) (((((c) - 0x2a) ^ (seed + 17)) + 0x64 ) ^ 0xa1)
#define __ID_DECRYPT(c, seed) (((((c) ^ 0xa1) - 0x64) ^ (seed + 17)) + 0x2a)


/*-----------------------------------------------------------------------------------------
 * Definations for Encode & Decode
**---------------------------------------------------------------------------------------*/
static char en_hex[] = "A60EB85293F17C4D"; /* Encrypted Hex letters */
static char de_hex[] = "2B79E61C5804DF3A"; /* Decrypted Hex letters */
/* static char seeds[]  = "8D3AF2E49061C75B"; /* En-Seeds Hex letters */
static unsigned char encode_seed = 0x79; /* 修改这里可以改变注册码的运算结果 */

/* 0x40是特别选的, 这样的话, slc.sn文件中就只有可显示字符了 */
#define ____HIGH4(x)  (en_hex[0xf & ((x) >> 4)])
#define ____LOW4(x)   (en_hex[0xf & ((x))])
#define ____GLUE(h,l) ((CHARtoHEX(de_hex[0xf & CHARtoHEX(h)]) << 4) | \
                       (CHARtoHEX(de_hex[0xf & CHARtoHEX(l)]) & 0xf))

#define SEED_INCREMENT 0x21


/*-----------------------------------------------------------------------------------------
 * Definations & Prototypes
**---------------------------------------------------------------------------------------*/
#define SERVICE_STACK_SIZE  2048

static HANDLE service_handle = InvalidHandle;
static register_callback_fn register_callback = NULL;

static INT32U __sleep_on_start = 800UL;  /* 0.8 s */
static INT32U __sleep_on_nap   = 800UL;  /* 0.8 s */

/* Register Service 请求表(按优先级排列) 
 * requests_cmd的下标代表请求命令, requests_data保存了请求的参数
*/
int    ____rs__requests_cmd[NUMBER_OF_RS_REQUEST_CMDS];
void * ____rs__requests_data[NUMBER_OF_RS_REQUEST_CMDS];
static void __do_request(int cmd, void * data);

static int __read_sn(char __BUF * machine_sn);
static int __save_sn(char __IN * user_sn);
static int __do_check_sn(char __BUF * machine_sn);
static void __encrypt_machine_id(char __BUF * machine_id);

int machine_id_get(unsigned char __BUF * buf, int buf_len);

static int default_register_callback(int cmd, void * data)
{
    cmd = cmd;
    data = data;
    /* nothing to do */
    return ok;
}

/*-----------------------------------------------------------------------------------------
 * Register Service Daemon
**---------------------------------------------------------------------------------------*/
static void __daemon register_service_thread(void * data)
{
    int i;

    data = data;
    
    TaskSleep(__sleep_on_start);

    while (1) {
        for (i = 0; i < NUMBER_OF_RS_REQUEST_CMDS; i++) {
            if (____rs__requests_cmd[i]) {
                lock_kernel();
                __do_request(i, ____rs__requests_data[i]);
                ____rs__requests_cmd[i] = 0;
                ____rs__requests_data[i] = NULL;
                unlock_kernel();
                break;
            }
        }
        __do_request(RS_REQUEST_EVENT, NULL); /* 定时发送注册信号 */
        TaskSleep(__sleep_on_nap);
    }

    while (1) { /* 没事了, 睡觉去吧 */
        TaskSleep(1000UL * 60UL * 60UL * 48UL);
    }
}

void register_service_wakeup(void)
{
    FamesAssert(service_handle >= 0);
    TaskAwake(service_handle);
}

int register_service_init(void)
{
    static char task_name[20];
    int i;

    /* read harddisk sn */

    for (i = 0; i < NUMBER_OF_RS_REQUEST_CMDS; i++) {
        ____rs__requests_cmd[i]  = 0;
        ____rs__requests_data[i] = NULL;
    }

    register_callback = default_register_callback;

    /* "Register-Service", 不能让破解人直接搜到这里... */
    task_name[5]  = 't';
    task_name[8]  = '-';
    task_name[11] = task_name[5] - 2; /* 'r' */
    task_name[6]  = 'e';
    task_name[9]  = 'S';
    task_name[12] = 'v';
    task_name[2]  = task_name[6] + 2; /* 'g' */
    task_name[13] = 'i';
    task_name[16] = '\0';
    task_name[0]  = 'R';
    task_name[4]  = task_name[9] + 32; /* 's' */
    task_name[17] = '\0';
    task_name[3]  = 'i';
    task_name[7]  = 'r';
    task_name[10] = task_name[3] - 4; /* 'e' */
    task_name[14] = 'c';
    task_name[1]  = 'e';
    task_name[15] = task_name[10]; /* 'e' */

    service_handle = TaskCreate(register_service_thread, NULL, task_name, NULL, SERVICE_STACK_SIZE, PRIO_SHARE, TASK_CREATE_OPT_NONE);

    if (service_handle >= 0)
        return ok;

    return fail;
}

/*-----------------------------------------------------------------------------------------
 * Register Service Request
**---------------------------------------------------------------------------------------*/
static void __do_request(int cmd, void * data)
{
    char machine_id[ID_MAX_LEN];
    char machine_sn[SN_MAX_LEN];
    int  ret;

    FamesAssert(register_callback != NULL);

    switch (cmd) {
        case RS_REQUEST_REGISTER_CALLBACK:
            if (data) { /* 这里的赋值没有用lock_kernel/unlock_kernel保护是因为
                         * 这个函数只会由__register_service()顺序调用,
                         * 因此并没有竞争条件. [下同]
                        */
                register_callback = (register_callback_fn)data;
            }
            break;
        case RS_REQUEST_DEREGISTER_CALLBACK:
            register_callback = default_register_callback;
            break;
        case RS_REQUEST_EVENT:
            __read_sn(machine_sn);
            (*register_callback)(RS_REQUEST_EVENT, (void *)__do_check_sn(machine_sn));
            break;
        case RS_REQUEST_CHECK:
            __read_sn(machine_sn);
            (*register_callback)(RS_REQUEST_CHECK, (void *)__do_check_sn(machine_sn));
            break;
        case RS_REQUEST_GET_ID:
            __encrypt_machine_id(machine_id);
            (*register_callback)(RS_REQUEST_GET_ID, (void *)machine_id);
            break;
        case RS_REQUEST_GET_SN:
            __read_sn(machine_sn);
            (*register_callback)(RS_REQUEST_GET_SN, (void *)machine_sn);
            break;
        case RS_REQUEST_SN:
            /* 保存SN */
            MEMCPY(machine_sn, (INT08S *)data, SN_LEN);
            machine_sn[SN_LEN] = 0;
            ret = __do_check_sn(machine_sn);
            if (ret == ok)
                __save_sn(machine_sn);
            (*register_callback)(RS_REQUEST_SN, (void *)ret);
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------------------------------
 *
 * SN File & SN Encode Algorithm
 *
**---------------------------------------------------------------------------------------*/
static int __sn_has_been_readed = 0;
static char __machine_sn[SN_MAX_LEN];

static int ____make_lrc(struct sn_struct * buf, int do_check)
{
    unsigned char lrc1, lrc2, lrc;
    unsigned char * c, new_lrc;
    int i;

    FamesAssert(buf);

    lrc1 = buf->lrc[0];
    lrc2 = buf->lrc[1];
    lrc  = ____GLUE(lrc1, lrc2);

    c = (unsigned char *)buf;
    new_lrc = 0;
    buf->lrc[0] = 0;
    buf->lrc[1] = 0;
    for (i = 0; i < sizeof(*buf); i++)
        new_lrc += c[i];
    new_lrc = (~new_lrc) + 1; /* 取补 */

    if (do_check) { /* 检查LRC */
        if (new_lrc == lrc) {
            buf->lrc[0] = lrc1;
            buf->lrc[1] = lrc2;
            return ok;
        }
        return fail;
    }

    buf->lrc[0] = ____HIGH4(new_lrc);
    buf->lrc[1] = ____LOW4(new_lrc);

    return ok;
}

static void ____encrypt_sn(struct sn_struct * buf, char __IN * user_sn)
{
    INT32U seconds;
    unsigned char seed, saved_seed;
    int i;

    FamesAssert(buf);
    FamesAssert(user_sn);

    /* 这里不用原子操作是因为我们不需要这个数据的正确性
     * 我们需要的只是一个随机数, 用于生成seed! [下同]
    */
    lock_kernel();
    seconds = SecondsFromStart;
    unlock_kernel();

    seed = (unsigned char)(seconds + (seconds >> 8));
    saved_seed = seed;
    for (i = 0; i < SN_LEN; i++) {
        unsigned char c;
        seed += SEED_INCREMENT;
        c = (user_sn[i] ^ seed); /* 异或加密 */
        buf->sn_encrypted1[i] = ____HIGH4(c);
        buf->sn_encrypted2[i] = ____LOW4(c);
    }

    buf->sn_seed[0] = ____HIGH4(saved_seed);
    buf->sn_seed[1] = ____LOW4(saved_seed);
}

static void ____decrypt_sn(char __OUT * read_sn, struct sn_struct * buf)
{
    unsigned char seed, c1, c2, c;
    int i;

    FamesAssert(buf);
    FamesAssert(read_sn);

    c1 = buf->sn_seed[0];
    c2 = buf->sn_seed[1];
    seed = ____GLUE(c1, c2);
    for (i = 0; i < SN_LEN; i++) {
        c1 = buf->sn_encrypted1[i];
        c2 = buf->sn_encrypted2[i];
        c = ____GLUE(c1, c2);
        seed += SEED_INCREMENT;
        read_sn[i] = (c ^ seed); /* 异或恢复 */
    }
}

static int __read_sn(char __BUF * machine_sn)
{
    char filename[8];
    int  fd = -1;
    struct sn_struct buf;
    int  ret = ok;

    FamesAssert(machine_sn != NULL);

    machine_sn[0] = '<';
    machine_sn[1] = ' ';
    machine_sn[2] = 'E';
    machine_sn[3] = 'r';
    machine_sn[4] = 'r';
    machine_sn[5] = ' ';
    machine_sn[6] = '>';
    machine_sn[7] = '\0';

    if (!__sn_has_been_readed) {
        filename[0] = 'c';
        filename[1] = 'u';
        filename[2] = 't';
        filename[3] = '.';
        filename[4] = 's';
        filename[5] = 'n';
        filename[6] = '\0';
        lock_kernel();
        fd = open(filename, O_RDONLY|O_BINARY);
        if (fd >= 0) { /* 打开成功 */
            if (read(fd, (void *)&buf, sizeof(buf)) <= 0)
                ret = fail;
            close(fd);
        } else {
            ret = fail;
        }
        unlock_kernel();
        if (ret == ok) { /* 已经读出来了, 将SN解析出来 */
            if (buf.magic == SN_STRUCT_MAGIC && ____make_lrc(&buf, 1)) {
                ____decrypt_sn(__machine_sn, &buf);
                __sn_has_been_readed = 1;
            }
        }
    }

    if (__sn_has_been_readed) {
        MEMCPY(machine_sn, __machine_sn, SN_LEN);
        machine_sn[SN_LEN] = 0;
    }

    return ret;
}

static int __save_sn(char __IN * user_sn)
{
    char filename[8];
    int  fd = -1;
    struct sn_struct buf;
    int  ret = ok;

    FamesAssert(user_sn != NULL);

    buf.magic = SN_STRUCT_MAGIC;
    ____encrypt_sn(&buf, user_sn);
    ____make_lrc(&buf, 0);

    filename[0] = 'c';
    filename[1] = 'u';
    filename[2] = 't';
    filename[3] = '.';
    filename[4] = 's';
    filename[5] = 'n';
    filename[6] = '\0';
    lock_kernel();
    fd = open(filename, O_WRONLY|O_BINARY|O_CREAT, S_IREAD|S_IWRITE);
    if (fd >= 0) { /* 打开成功 */
        lseek(fd, 0UL, SEEK_SET);
        write(fd, (void *)&buf, sizeof(buf));
        close(fd);
        __sn_has_been_readed = 0;
    } else {
        ret = fail;
    }
    unlock_kernel();

    return ret;
}

/*-----------------------------------------------------------------------------------------
 *
 *  Read ID & ID Encode Algorithm
 *
**---------------------------------------------------------------------------------------*/
static void __encrypt_machine_id(char __BUF * machine_id)
{
    struct mach_id_struct mach_id;
    unsigned char buf[MACH_ID_LEN];
    unsigned char seed;
    unsigned char lrc;
    unsigned char *c;
    int i;

    FamesAssert(machine_id != NULL);

    machine_id[0] = '<';
    machine_id[1] = ' ';
    machine_id[2] = 'E';
    machine_id[3] = 'r';
    machine_id[4] = 'r';
    machine_id[5] = ' ';
    machine_id[6] = '>';
    machine_id[7] = '\0';

    if (!machine_id_get(buf, sizeof(buf)))
        return;

    /* 先加密 */
    seed = encode_seed;
    for (i = 0; i < sizeof(buf); i++) {
        unsigned char ch;
        seed += SEED_INCREMENT;
        ch = (buf[i] ^ seed); /* 异或加密 */
        mach_id.id_encrypted1[i] = ____HIGH4(ch);
        mach_id.id_encrypted2[i] = ____LOW4(ch);
    }

    /* 再计算校验码 */
    mach_id.magic = ID_STRUCT_MAGIC;
    mach_id.lrc[0] = 0;
    mach_id.lrc[1] = 0;

    c = (unsigned char *)&mach_id;
    lrc = 0;
    for (i = 0; i < sizeof(struct mach_id_struct); i++)
        lrc += c[i];
    lrc = (~lrc) + 1; /* 取补 */

    mach_id.lrc[0] = ____HIGH4(lrc);
    mach_id.lrc[1] = ____LOW4(lrc);

    MEMCPY((INT08S *)machine_id, (INT08S *)c, sizeof(struct mach_id_struct));
    machine_id[sizeof(struct mach_id_struct)] = 0;
}

#ifdef THIS_IS_UNREGISTER
/* 解密用户发回来的machine_id */
static int __decrypt_machine_id(char __BUF * machine_id, struct mach_id_struct __IN * user_id)
{
    unsigned char id_buf[8]; /* 硬件ID实际上只有8位 */
    unsigned char seed;
    unsigned char lrc, lrc_check;
    unsigned char *c;
    int i;

    FamesAssert(user_id != NULL);
    FamesAssert(machine_id != NULL);

    /* 先检查MagicNumber */
    if (user_id->magic != ID_STRUCT_MAGIC)
        return 0;

    /* 再检查校验码 */
    lrc = ____GLUE(user_id->lrc[0], user_id->lrc[1]);
    user_id->lrc[0] = 0;
    user_id->lrc[1] = 0;
    c = (unsigned char *)user_id;
    lrc_check = 0;
    for (i = 0; i < sizeof(struct mach_id_struct); i++)
        lrc_check += c[i];
    lrc_check = (~lrc_check) + 1; /* 取补 */
    if (lrc != lrc_check)
        return 0; /* 校验码不对 */

    /* 看来这个ID是对的, 那么就解密吧 */
    seed = encode_seed;
    for (i = 0; i < sizeof(id_buf); i++) {
        unsigned char ch;
        seed += SEED_INCREMENT;
        ch = ____GLUE(user_id->id_encrypted1[i], user_id->id_encrypted2[i]);
        id_buf[i] = (ch ^ seed); /* 异或解密 */
    }

    if (i > 0) {
        MEMCPY((INT08S *)machine_id, (INT08S *)id_buf, i);
        machine_id[i] = 0; /* 实际上这不是个字符串(而是个HEX数组), 但还是给它加个结束符吧 */
    }

    return i;
}

/*
 * 返回值为解码的字节数, 若出错则返回0
*/
int decrypt_machine_id(char __BUF * machine_id, char __IN * user_id)
{
    return __decrypt_machine_id(machine_id, (struct mach_id_struct *)user_id);
}
#endif

/*-----------------------------------------------------------------------------------------
 *
 *  Register Algorithm
 *
**---------------------------------------------------------------------------------------*/
static int __do_check_sn(char __BUF * machine_sn)
{
    unsigned char real_sn[20];
    unsigned char id_buf[MACH_ID_LEN]; /* 硬件ID实际上只有8位 */
    unsigned char seed, lrc, lrc2;
    int i, j, error_count = 0;

    FamesAssert(machine_sn != NULL);

    if (!machine_id_get(id_buf, sizeof(id_buf)))
        return fail;

    /* 去掉格式化串xxxxx-xxxxx-xxxxx-xxxxx中的'-' */
    for (i = 0, j = 0; i < SN_LEN; i++) {
        if (i == 5 || i == 11 || i == 17)
            continue; /* 跳过对应的位置 '-' */
        real_sn[j++] = machine_sn[i];
    }

    /* 检查校验码LRC */
    lrc = ____GLUE(real_sn[18], real_sn[19]);
    real_sn[18] = 0;
    real_sn[19] = 0;
    lrc2 = 0;
    for (i = 0; i < sizeof(real_sn); i++)
        lrc2 += real_sn[i];
    lrc2 = (~lrc2) + 1; /* 取补 */
    if (lrc != lrc2) {
        error_count++;
        goto out; /* 故意这样写的: 用goto out;而不是直接返回fail */
    }

    /* 解码比较 */
    seed = encode_seed;
    for (i = 0; i < sizeof(id_buf); i++) {
        unsigned char ch, id;
        ch = ____GLUE(real_sn[i], real_sn[i+sizeof(id_buf)]);
        id = __ID_ENCRYPT(id_buf[i], seed);
        if (ch != id)
            error_count++;
        seed += SEED_INCREMENT;
    }

out:
    return (!error_count);
}

#ifdef THIS_IS_UNREGISTER

/* 注册序列号SN的生成算法, 在修改这个函数时须特别小心那些数组的大小 */
void generate_register_sn(char __BUF * user_sn, char __IN * machine_id)
{
    unsigned char real_sn[20];
    unsigned char id_buf[MACH_ID_LEN]; /* 硬件ID实际上只有8位 */
    unsigned char seed, lrc, sum = 0;
    int i, j;

    FamesAssert(user_sn != NULL);
    FamesAssert(machine_id != NULL);

    MEMCPY((INT08S *)id_buf, (INT08S *)machine_id, sizeof(id_buf));

    seed = encode_seed;
    for (i = 0; i < sizeof(id_buf); i++) {
        unsigned char ch = __ID_ENCRYPT(id_buf[i], seed);
        real_sn[i]  = ____HIGH4(ch);
        real_sn[i+sizeof(id_buf)] = ____LOW4(ch);
        sum += id_buf[i];
        seed += SEED_INCREMENT;
    }
    real_sn[16] = ____HIGH4(sum);
    real_sn[17] = ____LOW4(sum);
    real_sn[18] = 0;
    real_sn[19] = 0;

    /* 计算校验码LRC */
    lrc = 0;
    for (i = 0; i < sizeof(real_sn); i++)
        lrc += real_sn[i];
    lrc = (~lrc) + 1; /* 取补 */
    real_sn[18] = ____HIGH4(lrc);
    real_sn[19] = ____LOW4(lrc);

    /* 格式化为: xxxxx-xxxxx-xxxxx-xxxxx */
    for (i = 0, j = 0; i < SN_LEN; i++, j++) {
        if (i == 5 || i == 11 || i == 17) {
            user_sn[i++] = '-'; /* 加入符号 '-' */
        }
        user_sn[i] = real_sn[j];
    }
    user_sn[i] = 0;
}

#endif


/*=========================================================================================
 * 
 * 本文件结束: reg_core.c
 * 
**=======================================================================================*/


