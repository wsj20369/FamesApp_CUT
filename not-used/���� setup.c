/******************************************************************************************
 * 文件:    setup.c
 *
 * 描述:    参数设置
 *
 * 作者:    Jun
 *
 * 时间:    2012-12-3
******************************************************************************************/
#define  APP_SETUP_C
#include <FamesOS.h>
#include "common.h"


/*

说明:  参数分成3类，普通参数Setup，高级参数Setup2, 驱动器参数

       Setup包括了常规参数及限速表，其中限速表也用TEXT控件实现
       Setup2包括了高级参数，比如中英文切换，串口设置等
       驱动器参数由各驱动器文件提供

       所有3个设置画面均由主画面单独进入，并进到另一个root_widget

       Setup与Setup2就由本文件实现


*/


/*-----------------------------------------------------------------------------------------
 *          
 *      参数画面中的控件及其它定义
 * 
**---------------------------------------------------------------------------------------*/
static gui_widget * __setup_screen      = NULL; /* 参数画面的主控件 */
static gui_window_t * setup_window      = NULL;

static gui_widget * params_view         = NULL;
static gui_widget * params_title        = NULL;

static gui_widget * params_status_bar   = NULL;

#define PARAMS_BUTTONS_NR  4                    /* 根据实际需要调整 */
#define PARAMS_BUTTONS_W   128
#define PARAMS_BUTTONS_H   48
#define PARAMS_BUTTONS_X   48
#define PARAMS_BUTTONS_y   480
static gui_widget * params_buttons[4]   = { NULL, };

extern struct cut_config_s config;
extern BMPINFO  icon;                           /* 图标             */

static BOOL params_view_init_view_private(void);
static char * setup_get_param_class_name(enum param_class_enum p_class);
static int setup_get_param_number(enum param_class_enum p_class);
static BOOL setup_get_param_name(enum param_class_enum p_class, int index, char * buf, int buf_len);
static BOOL setup_get_param_value(enum param_class_enum p_class, int index, char * buf, int buf_len);
static BOOL setup_set_param_value(enum param_class_enum p_class, int index, char * buf, int buf_len, BOOL active_now);
static BOOL setup_get_param_comment(enum param_class_enum p_class, int index, char * buf, int buf_len);
static BOOL setup_get_param_password(enum param_class_enum p_class, char * buf, int buf_len);

/*-----------------------------------------------------------------------------------------
 *          
 *      当前的参数类型
 * 
**---------------------------------------------------------------------------------------*/
static enum param_class_enum param_class = PARAM_CLASS_NONE;


/*-----------------------------------------------------------------------------------------
 *          
 *      config的临时缓冲, 会在p_xxx.c文件中引用
 * 
**---------------------------------------------------------------------------------------*/
struct cut_config_s config_for_setting;


/*-----------------------------------------------------------------------------------------
 *          
 *      参数画面的定义(或初始化)
 * 
**---------------------------------------------------------------------------------------*/
gui_widget * init_setup_screen(void)
{
    /* 主界面       */
    __setup_screen = gui_create_widget(GUI_WIDGET_FORM, 1, 1, 1021, 765, 0, 0, font16, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE);
    if(!__setup_screen)
        goto some_error;
    setup_window = gui_create_window(__setup_screen);
    gui_widget_link(NULL, __setup_screen);      /* 设置主界面背景 */
    gui_form_init_private(__setup_screen, 128);
    gui_form_set_icon(__setup_screen, &icon);
    gui_form_set_caption(__setup_screen, pick_string("参数设置", "Setup"));
    
    /* 工具条       */
    params_status_bar = gui_create_widget(GUI_WIDGET_LABEL, 7, 725, 872, 30, 0, 0, 1, LABEL_STYLE_CLIENT_BDR);
    if(!params_status_bar)
        goto some_error;
    gui_widget_link(__setup_screen, params_status_bar);
    gui_label_init_private(params_status_bar, 128);
    gui_label_set_text(params_status_bar, pick_string(" >>>F10 确认                  >>>ESC 返回",
                                                      " >>>F10 Save                  >>>ESC Return"));

    /* 机器名       */
    params_title = gui_create_widget(GUI_WIDGET_LABEL, 10, 36, 998, 32, COLOR_YELLOW, 17, font24, LABEL_ALIGN_CENTER|LABEL_STYLE_SUBSIDE);
    if(!params_title)
        goto some_error;
    gui_widget_link(__setup_screen, params_title);
    gui_label_init_private(params_title, 128);
    gui_label_set_text(params_title, "");

    /* 刀线参数     */
    params_view = gui_create_widget(GUI_WIDGET_VIEW, 10, 72, 998, 633, 0, 0, 1, 0x70|VIEW_STYLE_NONE_FIRST);
    if(!params_view)
        goto some_error;
    gui_widget_link(params_title, params_view);
    if(!params_view_init_view_private())
        goto some_error;

    /* 4个按钮, 参数类型 */
    
    return __setup_screen;

some_error:
    sys_print("init_setup_screen(): failed to create widgets!\n");
    ExitApplication();
    return NULL;
}

/*-----------------------------------------------------------------------------------------
 *          
 *      参数视图控件的相关定义
 * 
**---------------------------------------------------------------------------------------*/
enum params_view_fields_id {
    __id_param_sn,
    __id_param_name,
    __id_param_value,
    __id_param_comment,
};

static view_fields_t params_view_fields[] =
{
#define ____style        0
#define ____draw_style   DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER
    { "序号",      __id_param_sn,       4,  8,  ____style,  ____draw_style,  "", },
    { "参数名称",  __id_param_name,     12, 16, ____style,  ____draw_style,  "", },
    { "参数值",    __id_param_value,    8,  12, ____style,  ____draw_style,  "", },
    { "参数说明",  __id_param_comment,  64, 96, ____style,  ____draw_style,  "", },
    { NULL, }
#undef ____style
#undef ____draw_style
};

static void ____set_comment_en(void)
{
    char * s;

    #define ____do_set(index, name_en, comment_en) \
                    do { \
                        s = pick_string(NULL, name_en); \
                        if(s) \
                            params_view_fields[index].caption = s; \
                        s = pick_string(NULL, comment_en); \
                        if(s) \
                            params_view_fields[index].comment = comment_en; \
                    } while(0)

    ____do_set(0, "S/N",      "Param S/N");
    ____do_set(1, "Name",     "Param Name");
    ____do_set(2, "Value",    "Param Value");
    ____do_set(3, "Comment",  "Param Comment");

    #undef ____do_set
}

static int params_view_get_max(void)
{
    return setup_get_param_number(param_class) - 1;
}

static void params_view_show_statistics(int index,
                           int x, int y, int width, int height, 
                           int color, int bkcolor, int font,
                           INT08S *field_comment,
                           INT08S *old, INT16U option)
{
    char ___s[96];
    
    FamesAssert(old);
    if(!old)
        return;

    option = option;

    sprintf(___s, pick_string("共有%d个参数, 当前参数是第%d个",
                              "Totally %d Params, Current is %d"),
                  params_view_get_max()+1, index+1);

    draw_font_for_widget(x, y, width, height, ___s, old, color, bkcolor, font, DRAW_OPT_FIL_BG);
}

static BOOL params_view_is_writable(int index, int field_id, INT16U option)
{
    BOOL retval;

    option = option;

    retval = ok;

    switch(field_id){
        case __id_param_value:  /* 只有参数值可设定 */
            retval = ok; 
            break;
        default:
            retval = fail;
            break;
    }
    
    return  retval;
}

static BOOL params_view_get_item(int index, int field_id, char * buf, int buf_len, INT16U option)
{
    BOOL retval;

    FamesAssert(buf);
    if(!buf)
        return fail;

    option  = option;

    if(index > params_view_get_max())
        return fail;

    retval = ok;

    switch(field_id){
        case __id_param_sn:
            sprintf(buf, "%d", index+1);
            break;
        case __id_param_name:
            retval = setup_get_param_name(param_class, index, buf, buf_len);
            break;
        case __id_param_value:
            retval = setup_get_param_value(param_class, index, buf, buf_len);
            break;
        case __id_param_comment:
            retval = setup_get_param_comment(param_class, index, buf, buf_len);
            break;
        default:
            retval = fail;
            break;
    }
    
    return retval;
}

static BOOL params_view_set_item(int index, int field_id, char * buf, int buf_len, KEYCODE key, INT16U option)
{
    BOOL retval, active_now = NO;

    FamesAssert(buf);
    if(!buf)
        return fail;

    option  = option;
    if (key == F10)
        active_now = YES;

    if(index > params_view_get_max())
        return fail;

    switch(field_id){
        case __id_param_value:
            retval = setup_set_param_value(param_class, index, buf, buf_len, active_now);
        default:
            retval = fail;
            break;
    }

    return retval;
}

static KEYCODE setup_params(void)
{
    KEYCODE key;

    key = gui_view_editing(params_view, 0);

    if(key == NONEKEY)
        key = waitkey(0L);

    return key;
}

static BOOL params_view_init_view_private(void)
{
    BOOL retval;

    ____set_comment_en();
    
    retval = gui_view_init_private( params_view, 
                                    params_view_fields, 
                                    params_view_get_max,
                                    params_view_get_item,
                                    params_view_set_item, 
                                    params_view_is_writable,
                                    NULL,
                                    pick_string("=说=明=", "Comment"),
                                    params_view_show_statistics, 
                                    NULL,
                                    COLOR_YELLOW, /* 123, */
                                    17, /* WIDGET_BKCOLOR+65, */
                                    30, /*height_per_row*/
                                    1
                                  );

    return retval;
}

/*-----------------------------------------------------------------------------------------
 *          
 *      进入参数设置画面
 * 
**---------------------------------------------------------------------------------------*/
void enter_setup(void)
{
    KEYCODE key;
    char ___s[64];

    if(!__setup_screen)
        return;

    lock_kernel();
    config_for_setting = config;
    unlock_kernel();

    sprintf(___s, setup_get_param_class_name(param_class));
    gui_label_set_text(params_title, ___s);
    gui_show_window(setup_window);

    param_class = PARAM_CLASS_DEFAULT;

    for(;;){
        key = setup_params();
        switch(key){
            case ESC:
                return;
            case UP:
                gui_view_move_up(params_view);
                break;
            case DOWN:
                gui_view_move_down(params_view);
                break;
            case PGUP:
                gui_view_page_up(params_view);
                break;
            case PGDN:
                gui_view_page_down(params_view);
                break;
            case CTRL_HOME:
                gui_view_goto_top(params_view);
                break;
            case CTRL_END:
                gui_view_goto_bottom(params_view);
                break;
            case F10:
                lock_kernel();
                copy_to_config(&config_for_setting);
                save_config();
                unlock_kernel();
                break;
            default:
                break;
        }
    }
}

/*-----------------------------------------------------------------------------------------
 *          
 *      参数定义及操作例程
 * 
**---------------------------------------------------------------------------------------*/
extern param_class_type param_class_normal;
extern param_class_type param_class_spdlimit;
extern param_class_type param_class_driver;
extern param_class_type param_class_super;

/*
//需创建4个文件: p_normal.c, p_spdlmt.c, p_driver.c, p_super.c
//分别用于管理各自的参数
*/


/* 参数类型初始化
*/
void init_param_classes(void)
{
    return;
    #define __pclass_init(x) if (x.init) x.init()

    __pclass_init(param_class_normal);
    __pclass_init(param_class_spdlimit);
    __pclass_init(param_class_driver);
    __pclass_init(param_class_super);

    #undef __pclass_init
}

static param_class_type * find_param_class(enum param_class_enum p_class)
{
    switch (p_class) {
        case PARAM_CLASS_NORMAL:
            return &param_class_normal;
        case PARAM_CLASS_SPDLIMIT:
            return &param_class_spdlimit;
        case PARAM_CLASS_DRIVER:
            return &param_class_driver;
        case PARAM_CLASS_SUPER:
            return &param_class_super;
        default:
            FamesAssert(!"Find_param_class(): invalid p_class!");
            return NULL;
    }
}

static param_struct * find_param(enum param_class_enum p_class, int index)
{
    param_class_type * t;
    int i;

    t = find_param_class(p_class);
    if (!t)
        return NULL;

    for (i = 0; i < t->param_nr; i++) {
        if (index == 0)
            break;
        index--;
    }

    if (i < t->param_nr)
        return (t->param_table + i);
    else
        return NULL;
}

static char * setup_get_param_class_name(enum param_class_enum p_class)
{
    param_class_type * t;

    t = find_param_class(p_class);

    return t->class_name;
}

static BOOL setup_get_param_password(enum param_class_enum p_class, char * buf, int buf_len)
{
    param_class_type * t;
    BOOL retval = fail;

    t = find_param_class(p_class);

    if (t && t->get_password);
        retval = t->get_password(buf, buf_len);

    return retval;
}

static int setup_get_param_number(enum param_class_enum p_class)
{
    param_class_type * t;

    t = find_param_class(p_class);

    return t->param_nr;
}

static BOOL setup_get_param_name(enum param_class_enum p_class, int index, char * buf, int buf_len)
{
    param_struct * t;

    t = find_param(p_class, index);

    if (t)
        slprintf(buf, buf_len, "%s", t->name);

    if (t)
        return ok;
    else
        return fail;
}

static BOOL setup_get_param_value(enum param_class_enum p_class, int index, char * buf, int buf_len)
{
    param_class_type * t;
    BOOL retval = fail;

    t = find_param_class(p_class);

    if (t && t->get_value);
        retval = t->get_value(index, buf, buf_len);

    return retval;
}

static BOOL setup_get_param_comment(enum param_class_enum p_class, int index, char * buf, int buf_len)
{
    param_struct * t;

    t = find_param(p_class, index);

    if (t)
        snprintf(buf, buf_len, "%s", t->comment);

    if (t)
        return ok;
    else
        return fail;
}

static BOOL setup_set_param_value(enum param_class_enum p_class, int index, char * buf, int buf_len, BOOL active_now)
{
    param_class_type * t;
    BOOL retval = fail;

    t = find_param_class(p_class);

    if (t && t->set_value);
        retval = t->set_value(index, buf, buf_len, active_now);

    return retval;
}


/*=========================================================================================
 * 
 * 本文件结束: setup.c
 * 
**=======================================================================================*/



