/*************************************************************************************
 * 文件:    cut/mach_id.c
 *
 * 描述:    读取机器ID号(用于注册)
 *
 * 作者:    Jun
 *
 * 时间:    2012-9-2
 *
 * 说明:    目前只能读取硬盘ID(只能检测第1个IDE设备(IDE0))
*************************************************************************************/
#define  MACHINE_ID_C
#include <includes.h>
#include "common.h"

/*
IDE控制器的端口(helppc 2.10):

    170-17F  Fixed disk 1 (AT)
    170 disk 1 data
    171 disk 1 error
    172 disk 1 sector count
    173 disk 1 sector number
    174 disk 1 cylinder low
    175 disk 1 cylinder high
    176 disk 1 drive/head
    177 disk 1 status

    1F0-1FF  Fixed disk 0 (AT)
    1F0 disk 0 data
    1F1 disk 0 error
    1F2 disk 0 sector count
    1F3 disk 0 sector number
    1F4 disk 0 cylinder low
    1F5 disk 0 cylinder high
    1F6 disk 0 drive/head
    1F7 disk 0 status
*/

#define IDE_SN_START_WORD 10
#define IDE_SN_WORD_COUNT 10

static int __do_read_id(unsigned char __BUF * buf, int count)
{
    unsigned int dd[256]; /* DiskData */
    unsigned int dd_off; /* DiskData offset */
    unsigned char ide_sn_string[64];
    int  length, i, j, ret = ok;
    long wait;

    FamesAssert(buf != NULL);

    /* 读硬盘序列号 */
    lock_kernel();

    wait = 200000L;
    while (inportb(0x1F7) != 0x50) { /* Wait for controller not busy */
        wait--;
        if (wait <= 0L)
            break;
    }
    if (wait <= 0L) { /* still busy? or error occured? */
        ret = fail;
        goto unlock_out;
    }

    outportb(0x1F6, 0xA0); /* Get first/second drive */
    outportb(0x1F7, 0xEC); /* Get drive info data */
    wait = 200000L;
    while (inportb(0x1F7) != 0x58) { /* Wait for data ready */
        wait--;
        if (wait <= 0L)
            break;
    }
    if (wait <= 0L) { /* still busy? or error occured? */
        ret = fail;
        goto unlock_out;
    }

    for (dd_off = 0; dd_off != 256; dd_off++) /* Read "sector" */
        dd[dd_off] = inport(0x1F0);

unlock_out:
    unlock_kernel();

    if (ret == fail) /* Read fail, goto out */
        goto out;

    /* 将序列号分离出来, 生成一个字符串 */
    for (i = 0, j = 0; i < IDE_SN_WORD_COUNT; i++) {
        int tmp;
        tmp = dd[IDE_SN_START_WORD+i];
        ide_sn_string[j++] = (unsigned char)(tmp >> 8); /* Get High byte */
        ide_sn_string[j++] = (unsigned char)(tmp & 0xff); /* Get Low byte */
    }
    length = STRLEN((INT08S *)ide_sn_string);

    /* 将这个字符串写到输出缓冲去 */
    MEMSET((INT08S *)buf, 0, count);
    for (i = 0, j = 0; i < length; i++) {
        buf[j] = (buf[j] << 1) + ide_sn_string[i];
        j++;
        j %= count;
    }
    ret = ok;

out:
    return ret;
}

int machine_id_get(unsigned char __BUF * buf, int buf_len)
{
    static int readed = 0;
    static int do_read_count = 0;
    static unsigned char ____machine_hard_id[12];

    if (!readed) {
        if (do_read_count >= 3) /* 最多重试3次 */
            return fail;
        do_read_count++;
        if (!__do_read_id(____machine_hard_id, 8)) /* 硬件ID实际上只有8位 */
            return fail;
    }
    readed = 1; /* 读出来了, Oh Yeah! */

    MEMSET((INT08S *)buf, 0, buf_len);
    if (buf_len > sizeof(____machine_hard_id))
        buf_len = sizeof(____machine_hard_id);
    MEMCPY((INT08S *)buf, (INT08S *)____machine_hard_id, buf_len);

    return ok;
}


/*====================================================================================
 * 
 * 本文件结束: cut/mach_id.c
 * 
**==================================================================================*/

