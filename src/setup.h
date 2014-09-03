/******************************************************************************************
 * 文件:    setup.h
 *
 * 描述:    参数设置
 *
 * 作者:    Jun
 *
 * 时间:    2013-1-31
 *
 * 说明:    此头文件是所有类型参数的总头文件, 包括
 *          普通参数(常规参数),
 *          限速表,
 *          驱动器参数,
 *          高级参数(超级用户)
******************************************************************************************/
#ifndef APP_SETUP_H
#define APP_SETUP_H

/*-----------------------------------------------------------------------------------------
 * 
 *      驱动器参数结构
 * 
**---------------------------------------------------------------------------------------*/

typedef struct __driver_param {
    int id;
    char * name;
    char * comment;
} driver_param_t;

typedef struct __driver_setup_desc {
    char * driver_name;
    char * description;
    int    params_nr;
    driver_param_t * params_table;
    BOOL (*get_value)(int index, char * buf, int buf_len);
    BOOL (*set_value)(int index, char * buf, int buf_len, BOOL active_now);
    BOOL (*is_writable)(int index);
} driver_setup_desc_t;


/*-----------------------------------------------------------------------------------------
 * 
 *      函数及变量声明
 * 
**---------------------------------------------------------------------------------------*/
gui_widget * init_setup_screen(void);
void enter_setup(void);

char * get_driver_types_name(int type);


/* 普通参数 */
void init_generic_param_dlg(void);
BOOL do_setup_generic_param(void);

/* 高级参数 */
void init_super_param_dlg(void);
BOOL do_setup_super_param(void);

/* 限速表 */
void init_spdlmt_setup_dlg(void);
void do_setup_spdlmt_tbl(void);

/* 驱动器参数 */
void init_driver_setup_dlg(void);
void do_setup_driver_params(void);

/* 其它函数 */
INT16S get_speed_limit(INT16S cutlen);



#endif /* #ifndef APP_SETUP_H */

/*=========================================================================================
 * 
 * 本文件结束: setup.h
 * 
**=======================================================================================*/


