/******************************************************************************************
 * 文件:    setup_dr.c
 *
 * 描述:    驱动器参数设置
 *
 * 作者:    Jun
 *
 * 时间:    2013-2-8
******************************************************************************************/
#define  APP_SETUP_DRIVER_C
#include <FamesOS.h>
#include "common.h"


/*-----------------------------------------------------------------------------------------
 *          
 *      驱动器参数的定义
 * 
**---------------------------------------------------------------------------------------*/
/*
static driver_setup_desc_t * current_driver_setup = &pseudo_driver_setup;
*/
static driver_setup_desc_t * driver_setup_list[] =
{
    NULL,

};

/*-----------------------------------------------------------------------------------------
 *          
 *      驱动器参数画面的控件及其它定义
 * 
**---------------------------------------------------------------------------------------*/
static gui_widget   * driver_params_dlg = NULL;     /* 驱动器参数 */
static gui_widget   * driver_params_view = NULL;
static gui_window_t * driver_params_window = NULL;

extern BMPINFO  icon;                               /* 图标 */

extern struct cut_config_s config;
extern struct cut_config_s __config_for_setup;


static BOOL driver_params_init_view_private(void);


/*-----------------------------------------------------------------------------------------
 *          
 *      驱动器参数画面的定义
 * 
**---------------------------------------------------------------------------------------*/
void init_driver_setup_dlg(void)
{
    gui_widget * status_bar;
    int height_add = 90;
    char tmp_buf[128];

    /* 限速表设定窗口 */
    driver_params_dlg = gui_create_widget(GUI_WIDGET_FORM, 140, 106, 740, (421 + height_add), 0, 0, font16, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE);
    if(!driver_params_dlg)
        goto some_error;
    gui_widget_link(NULL, driver_params_dlg);      /* 设置主界面背景 */
    gui_form_init_private(driver_params_dlg, 32);
    gui_form_set_icon(driver_params_dlg, &icon);
    sprintf(tmp_buf, pick_string("驱动器参数(%s)", "Driver Params (%s)"), get_driver_types_name(config.driver_type));
    gui_form_set_caption(driver_params_dlg, tmp_buf);

    /* 工具条 */
    status_bar = gui_create_widget(GUI_WIDGET_LABEL, 5, (380 + height_add), 730, 36, 0, 0, font16, LABEL_STYLE_CLIENT_BDR);
    if(!status_bar)
        goto some_error;
    gui_widget_link(driver_params_dlg, status_bar);
    gui_label_init_private(status_bar, 128);
    gui_label_set_text(status_bar, pick_string(" [F10] 保存       [ESC] 返回",
                                               " [F10] Save       [ESC] Return"));
    /* 限速表视图 */
    driver_params_view = gui_create_widget(GUI_WIDGET_VIEW, 12, 38, 716, (334 + height_add), 0, 0, 1, 0x70|VIEW_STYLE_NONE_FIRST);
    if(!driver_params_view)
        goto some_error;
    gui_widget_link(driver_params_dlg, driver_params_view);
    if(!driver_params_init_view_private())
        goto some_error;

    driver_params_window = gui_create_window(driver_params_dlg);
    if (!driver_params_window)
        goto some_error;

    return;

some_error:
    sys_print("init_driver_setup_dlg(): failed to create widgets or window!\n");
    ExitApplication();
}

/*-----------------------------------------------------------------------------------------
 *          
 *      参数视图控件的相关定义
 * 
**---------------------------------------------------------------------------------------*/
enum params_view_fields_id {
    __id_sn,
    __id_name,
    __id_value,
    __id_comment,
};

static view_fields_t params_view_fields[] =
{
#define ____style       0
#define ____draw_style  DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER
    { "序号",           __id_sn,       4,  8,  ____style,  ____draw_style,  "", },
    { "名称",           __id_name,     12, 16, ____style,  ____draw_style,  "", },
    { "参数值",         __id_value,    8,  12, ____style,  ____draw_style,  "", },
    { "== 说明 ==",     __id_comment,  64, 96, ____style,  DRAW_OPT_FIL_BG, "", },
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
    return 23;
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

    sprintf(___s, pick_string("共有 %d 个参数, 当前参数是第 %d 个",
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
        case __id_value:  /* 只有参数值可设定 */
            retval = ok;
            break;
        default:
            retval = fail;
            break;
    }

    if (index % 8 == 0)
        retval = fail;
    
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
        case __id_sn:
            sprintf(buf, "%d", index+1);
            break;
        case __id_name:
            sprintf(buf, "参数%d", index+1);
            break;
        case __id_value:
            sprintf(buf, "%d", rand());
            break;
        case __id_comment:
            sprintf(buf, "没有什么可说的");
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
        case __id_value:
            retval = ok;
        default:
            retval = fail;
            break;
    }

    return retval;
}

static BOOL driver_params_init_view_private(void)
{
    BOOL retval;

    ____set_comment_en();
    
    retval = gui_view_init_private( driver_params_view, 
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
                                    COLOR_BLACK, /* WIDGET_BKCOLOR+65, */
                                    30, /*height_per_row*/
                                    1
                                  );

    return retval;
}

/*-----------------------------------------------------------------------------------------
 *          
 *      进入驱动器参数设置画面
 * 
**---------------------------------------------------------------------------------------*/
static KEYCODE __setup_driver_param_item(void)
{
    KEYCODE key;

    key = gui_view_editing(driver_params_view, 0);

    if (key == NONEKEY)
        key = waitkey(0L);

    return key;
}

void do_setup_driver_params(void)
{
    KEYCODE key;

    if(!driver_params_window)
        return;

    lock_kernel();
    __config_for_setup = config;
    unlock_kernel();

    gui_show_window(driver_params_window);

    for (;;) {
        key = __setup_driver_param_item();
        switch(key){
            case ESC:
                goto out;
            case UP:
                gui_view_move_up(driver_params_view);
                break;
            case DOWN:
                gui_view_move_down(driver_params_view);
                break;
            case PGUP:
                gui_view_page_up(driver_params_view);
                break;
            case PGDN:
                gui_view_page_down(driver_params_view);
                break;
            case CTRL_HOME:
                gui_view_goto_top(driver_params_view);
                break;
            case CTRL_END:
                gui_view_goto_bottom(driver_params_view);
                break;
            case F10:
                lock_kernel();
                copy_to_config(&__config_for_setup);
                save_config();
                unlock_kernel();
                break;
            default:
                break;
        }
    }

out:
    gui_hide_window(driver_params_window);

    return;
}


/*-----------------------------------------------------------------------------------------
 *          
 *      参数定义及操作例程
 * 
**---------------------------------------------------------------------------------------*/
/*
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

static BOOL setup_set_param_value(enum param_class_enum p_class, int index, char * buf, int buf_len, BOOL active_now)
{
    param_class_type * t;
    BOOL retval = fail;

    t = find_param_class(p_class);

    if (t && t->set_value);
        retval = t->set_value(index, buf, buf_len, active_now);

    return retval;
}
*/


/*=========================================================================================
 * 
 * 本文件结束: setup.c
 * 
**=======================================================================================*/



