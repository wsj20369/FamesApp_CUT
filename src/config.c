/*************************************************************************************
 * �ļ�:    cut/config.c
 *
 * ����:    cut�����ļ�
 *
 * ����:    Jun
 *
 * ʱ��:    2012-10-25
*************************************************************************************/
#define  CUT_CONFIG_C
#include <includes.h>
#include "common.h"

/*------------------------------------------------------------------------------------
 * 
 *          �ļ���
 * 
**----------------------------------------------------------------------------------*/
static char * config_fname = "config.cut";   /* �����ļ���                    */

/* config�������ļ�config.h��
struct cut_config_s config = { ... }
*/

/*------------------------------------------------------------------------------------
 * ����:    read_config()
 *
 * ����:    ��ȡ�����ļ�
 *
 * ����:    ok/fail
 *
 * ע��:    �ڶ�ȡ�ɹ���,���������е�����
**----------------------------------------------------------------------------------*/
BOOL read_config(void)
{
    BOOL retval;
    int  fd = -1;

    retval = fail;
    
    lock_kernel();
    fd = open(config_fname, O_RDONLY|O_BINARY);
    if (fd < 0) {                             /* �����ʧ��,��Ҫ���Դ����ļ�     */
        /* ��ʼ��config... */
        if (save_config()) {
            fd=open(config_fname, O_RDONLY|O_BINARY);
        } else {
            retval=fail;                      /* ����ʧ��,�����Ǵ��̿ռ䲻��,�˳�! */
            goto out;
        }        
    }
    if (fd >= 0) {
        if( read(fd, (void *)&config, sizeof(config)) > 0) {
            if (config.magic==config_magic && config.size==sizeof(config)) {
                active_config();              /* ��ȡ�ɹ�,�������е�����            */
                retval = ok;
            } else {
                sys_print("Error: config.cut has been corruptted!!!\n\n"
                          "  remove it manually, there will be a new one!\n\n"
                          "Any key to exit\n");
                getch();
                ExitApplication();
            }
        }
        close(fd);
    }

out:
    unlock_kernel();
    return retval;
}

/*------------------------------------------------------------------------------------
 * ����:    save_config()
 *
 * ����:    ����(�򴴽�)�����ļ�
 *
 * ����:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL save_config(void)
{
    BOOL retval;
    int  fd = -1;

    retval = fail;
    
    lock_kernel();
    fd = open(config_fname, O_WRONLY|O_BINARY|O_CREAT, S_IREAD|S_IWRITE);
    if (fd >= 0) {
        check_config();
        if (write(fd, (void *)&config, sizeof(config)) > 0) {
            retval=ok;
        }
        close(fd);
    }
    unlock_kernel();
    
    return retval;
}

/*------------------------------------------------------------------------------------
 * ����:    active_config()
 *
 * ����:    ��������
 *
 * ����:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL active_config(void)
{
    #define ____reset(x) MEMSET((INT08S*)&(x), 0, sizeof(x))
    
    /* ��������... */

    return ok;

    #undef ____reset
}

/*------------------------------------------------------------------------------------
 * ����:    check_config()
 *
 * ����:    ������ò��������������
 *
 * ����:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL check_config(void)
{
    lock_kernel();
    if (config.magic != config_magic) {
        config.magic = config_magic;
    }
    if (config.size != sizeof(config)) {
        config.size = sizeof(config);
    }
    unlock_kernel();
    return ok;
}

/*------------------------------------------------------------------------------------
 * ����:    copy_to_config()
 *
 * ����:    ��cfg������ȫ������config��
 *
 * ����:    ok/fail
 *
 * ˵��:    ���������Ҫ��Ϊ�˱�֤config�е�һЩ��̬���ݲ��ᱻ�ƻ�
**----------------------------------------------------------------------------------*/
BOOL copy_to_config(struct cut_config_s * cfg)
{
    FamesAssert(cfg);
    if(!cfg)
        return fail;
    
    lock_kernel();

    /* ��̬����... */

    /* ���� */
    config = (*cfg);
    unlock_kernel();
    
    return ok;
}


/*====================================================================================
 * 
 * ���ļ�����: cut/config.c
 * 
**==================================================================================*/

