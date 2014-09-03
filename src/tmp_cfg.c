/*************************************************************************************
 * �ļ�:    cut/tmp_cfg.c
 *
 * ����:    CUT��ʱ�����ļ�
 *
 * ����:    Jun
 *
 * ʱ��:    2011-4-2, 2012-10-29
*************************************************************************************/
#define  CUT_TMP_CONFIG_C
#include <includes.h>
#include "common.h"

/*------------------------------------------------------------------------------------
 * 
 *          �ļ���
 * 
**----------------------------------------------------------------------------------*/
static char * tmp_config_fname = "tmp.cut";   /* �����ļ���                    */

struct cut_tmp_config_s tmp_config = {
    0,
};

/*------------------------------------------------------------------------------------
 * ����:    read_tmp_config()
 *
 * ����:    ��ȡ��ʱ�����ļ�
 *
 * ����:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL read_tmp_config(void)
{
    BOOL retval;
    int  fd = -1;

    retval = fail;
    
    lock_kernel();
    fd=open(tmp_config_fname, O_RDONLY|O_BINARY);
    if(fd<0){                                 /* �����ʧ��,��Ҫ���Դ����ļ�     */
        if(save_tmp_config()){
            fd=open(tmp_config_fname, O_RDONLY|O_BINARY);
        } else {
            retval=fail;                      /* ����ʧ��,�����Ǵ��̿ռ䲻��,�˳�! */
            goto out;
        }        
    }
    if(fd>=0){
        if(read(fd, (void *)&tmp_config, sizeof(tmp_config)) > 0)
            retval = ok;
        close(fd);
    }

out:
    unlock_kernel();
    return retval;
}

/*------------------------------------------------------------------------------------
 * ����:    save_tmp_config()
 *
 * ����:    ����(�򴴽�)��ʱ�����ļ�
 *
 * ����:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL save_tmp_config(void)
{
    BOOL retval;
    int  fd = -1;

    retval = fail;
    
    lock_kernel();
    fd=open(tmp_config_fname, O_WRONLY|O_BINARY|O_CREAT, S_IREAD|S_IWRITE);
    if(fd>=0){
        if(write(fd, (void *)&tmp_config, sizeof(tmp_config)) > 0){
            retval=ok;
        }
        close(fd);
    }
    unlock_kernel();
    
    return retval;
}


/*====================================================================================
 * 
 * ���ļ�����: cut/tmp_cfg.c
 * 
**==================================================================================*/

