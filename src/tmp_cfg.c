/*************************************************************************************
 * 文件:    cut/tmp_cfg.c
 *
 * 描述:    CUT临时设置文件
 *
 * 作者:    Jun
 *
 * 时间:    2011-4-2, 2012-10-29
*************************************************************************************/
#define  CUT_TMP_CONFIG_C
#include <includes.h>
#include "common.h"

/*------------------------------------------------------------------------------------
 * 
 *          文件名
 * 
**----------------------------------------------------------------------------------*/
static char * tmp_config_fname = "tmp.cut";   /* 配置文件名                    */

struct cut_tmp_config_s tmp_config = {
    0,
};

/*------------------------------------------------------------------------------------
 * 函数:    read_tmp_config()
 *
 * 描述:    读取临时配置文件
 *
 * 返回:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL read_tmp_config(void)
{
    BOOL retval;
    int  fd = -1;

    retval = fail;
    
    lock_kernel();
    fd=open(tmp_config_fname, O_RDONLY|O_BINARY);
    if(fd<0){                                 /* 如果打开失败,则要尝试创建文件     */
        if(save_tmp_config()){
            fd=open(tmp_config_fname, O_RDONLY|O_BINARY);
        } else {
            retval=fail;                      /* 创建失败,可能是磁盘空间不足,退出! */
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
 * 函数:    save_tmp_config()
 *
 * 描述:    保存(或创建)临时配置文件
 *
 * 返回:    ok/fail
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
 * 本文件结束: cut/tmp_cfg.c
 * 
**==================================================================================*/

