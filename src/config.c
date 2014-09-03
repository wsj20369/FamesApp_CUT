/*************************************************************************************
 * 文件:    cut/config.c
 *
 * 描述:    cut配置文件
 *
 * 作者:    Jun
 *
 * 时间:    2012-10-25
*************************************************************************************/
#define  CUT_CONFIG_C
#include <includes.h>
#include "common.h"

/*------------------------------------------------------------------------------------
 * 
 *          文件名
 * 
**----------------------------------------------------------------------------------*/
static char * config_fname = "config.cut";   /* 配置文件名                    */

/* config定义在文件config.h中
struct cut_config_s config = { ... }
*/

/*------------------------------------------------------------------------------------
 * 函数:    read_config()
 *
 * 描述:    读取配置文件
 *
 * 返回:    ok/fail
 *
 * 注意:    在读取成功后,激活了其中的设置
**----------------------------------------------------------------------------------*/
BOOL read_config(void)
{
    BOOL retval;
    int  fd = -1;

    retval = fail;
    
    lock_kernel();
    fd = open(config_fname, O_RDONLY|O_BINARY);
    if (fd < 0) {                             /* 如果打开失败,则要尝试创建文件     */
        /* 初始化config... */
        if (save_config()) {
            fd=open(config_fname, O_RDONLY|O_BINARY);
        } else {
            retval=fail;                      /* 创建失败,可能是磁盘空间不足,退出! */
            goto out;
        }        
    }
    if (fd >= 0) {
        if( read(fd, (void *)&config, sizeof(config)) > 0) {
            if (config.magic==config_magic && config.size==sizeof(config)) {
                active_config();              /* 读取成功,激活其中的设置            */
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
 * 函数:    save_config()
 *
 * 描述:    保存(或创建)配置文件
 *
 * 返回:    ok/fail
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
 * 函数:    active_config()
 *
 * 描述:    激活设置
 *
 * 返回:    ok/fail
**----------------------------------------------------------------------------------*/
BOOL active_config(void)
{
    #define ____reset(x) MEMSET((INT08S*)&(x), 0, sizeof(x))
    
    /* 激活设置... */

    return ok;

    #undef ____reset
}

/*------------------------------------------------------------------------------------
 * 函数:    check_config()
 *
 * 描述:    检测配置并更正错误的设置
 *
 * 返回:    ok/fail
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
 * 函数:    copy_to_config()
 *
 * 描述:    将cfg拷贝到全局配置config中
 *
 * 返回:    ok/fail
 *
 * 说明:    这个函数主要是为了保证config中的一些动态数据不会被破坏
**----------------------------------------------------------------------------------*/
BOOL copy_to_config(struct cut_config_s * cfg)
{
    FamesAssert(cfg);
    if(!cfg)
        return fail;
    
    lock_kernel();

    /* 动态数据... */

    /* 拷贝 */
    config = (*cfg);
    unlock_kernel();
    
    return ok;
}


/*====================================================================================
 * 
 * 本文件结束: cut/config.c
 * 
**==================================================================================*/

