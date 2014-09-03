/******************************************************************************************
 * 文件:    setup.h
 *
 * 描述:    参数设置
 *
 * 作者:    Jun
 *
 * 时间:    2012-12-3
******************************************************************************************/
#ifndef APP_SETUP_H
#define APP_SETUP_H

/*-----------------------------------------------------------------------------------------
 * 
 *      参数结构
 * 
**---------------------------------------------------------------------------------------*/
enum param_class_enum {
    PARAM_CLASS_NONE,
    PARAM_CLASS_NORMAL,     /* 普通参数 */
    PARAM_CLASS_SPDLIMIT,   /* 限速表 */
    PARAM_CLASS_DRIVER,     /* 驱动器参数 */
    PARAM_CLASS_SUPER,      /* 高级参数 */
    
    PARAM_CLASS_NR,
};

#define PARAM_CLASS_DEFAULT PARAM_CLASS_NORMAL /* 默认进入普通参数 */

typedef struct __param_struct {
    int id;
    char * name;
    char * comment;
} param_struct;

typedef struct __param_class_type {
    char * class_name;
    char * description;
    int    param_nr;
    param_struct * param_table;
    BOOL (*get_password)(char * buf, int buf_len);
    BOOL (*get_value)(int index, char * buf, int buf_len);
    BOOL (*set_value)(int index, char * buf, int buf_len, BOOL active_now);
    void (*init)(void);
} param_class_type;


/*-----------------------------------------------------------------------------------------
 * 
 *      函数及变量声明
 * 
**---------------------------------------------------------------------------------------*/
gui_widget * init_setup_screen(void);
void enter_setup(void);


#endif /* #ifndef APP_SETUP_H */

/*=========================================================================================
 * 
 * 本文件结束: setup.h
 * 
**=======================================================================================*/


