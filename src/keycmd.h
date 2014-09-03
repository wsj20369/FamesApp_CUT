/*************************************************************************************
 * 文件:    keycmd.h
 *
 * 描述:    按键命令处理
 *
 * 作者:    Jun
 *
 * 时间:    2011-3-3
*************************************************************************************/
#ifndef FAMES_KEY_CMD_H
#define FAMES_KEY_CMD_H

/*------------------------------------------------------------------------------------
 * 
 *      按键命令结构 & 按钮命令结构
 * 
**----------------------------------------------------------------------------------*/
#define KEYCMD_FLAG_EXIT    0x0001    /* 执行完后, 退出KEYCMD循环 */

typedef void (* keycmd_func_t)(void * data);

struct key_cmd_s {
    KEYCODE  key;
    INT16U   flag;
    keycmd_func_t func;
    void   * data;
};

typedef struct key_cmd_s key_cmd_t;

struct button_cmd_s {
    KEYCODE  key;
    INT16U   flag;
    keycmd_func_t func;
    void  * data;
    INT08S * comment;
};

typedef struct button_cmd_s button_cmd_t;

typedef KEYCODE (*buttoncmds_key_hook_fn)(KEYCODE);


/*------------------------------------------------------------------------------------
 * 
 *      函数声明
 * 
**----------------------------------------------------------------------------------*/
void do_keycmds(key_cmd_t * keycmds);
int  do_buttoncmds(button_cmd_t * buttoncmds, gui_widget * buttons[], 
                   int buttons_nr, int default_one,
                   INT16U focus_style, INT16U unfocus_style, buttoncmds_key_hook_fn);
void buttoncmd_set_comment(button_cmd_t * buttoncmds, KEYCODE key, char * comment);


#endif /* #ifndef FAMES_KEY_CMD_H */

/*====================================================================================
 * 
 * 本文件结束: keycmd.h
 * 
**==================================================================================*/

