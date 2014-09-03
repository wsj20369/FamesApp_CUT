/******************************************************************************************
 * 文件:    setup.c
 *
 * 描述:    常规参数设置(超级用户)
 *
 * 作者:    Jun
 *
 * 时间:    2013-1-31
******************************************************************************************/
#define  APP_SETUP_GENERIC_C
#include <FamesOS.h>
#include "common.h"


static gui_widget   * generic_param_dlg = NULL;    /* 常规参数 */
static gui_window_t * generic_param_window = NULL;

static gui_widget   * spdlmt_dlg = NULL;          /* 限速表参数 */
static gui_widget   * spdlmt_view = NULL;
static gui_window_t * spdlmt_window = NULL;


extern BMPINFO  icon;       /* 图标  */

extern struct cut_config_s config;

struct cut_config_s __config_for_setup; /* config的一份拷贝, 在设置参数时使用 */


/*-----------------------------------------------------------------------------------------
 *          
 *      常规参数的相关定义
 * 
**---------------------------------------------------------------------------------------*/
enum generic_param_id {
    __generic,
    __alarm_count,
    __alarm_overspd,
    __oc_speaker,
    __pushmark,
    __scrap_count,
    __scrap_cutlen,

    __system,
    __reset_prompt,
    __today_data, /* 本日生产数据 */
    __min_cutlen,
    __mm_inch,
    __speed_scale,
    __speed_plus,

    __misc,
    __font_big1,
    __font_big2,

    __set_time,
    __set_date,
};

#define ___STYLE  0
#define ___STYL2  LABEL_ALIGN_CENTER
#define ___FONT   1  /* font16 */
DECLARE_INPUT_DIALOG_START(define_generic_param_dialog)
/* 常规参数 */
#undef  ___X
#undef  ___Y
#define ___X  28
#define ___Y  45
INPUT_DIALOG_GROUP(" 常规参数 ",  __generic, ___X, ___Y, 448, 159, ___FONT, GROUPBOX_STYLE_CAPTION)
INPUT_DIALOG_ITEM("闪灯张数: ",   __alarm_count,   3,  ___X+12,  ___Y+24,   188, 28, ___FONT, ___STYLE, "剩余张数小于此值时开始闪灯")
INPUT_DIALOG_ITEM("超速闪灯: ",   __alarm_overspd, 1,  ___X+12,  ___Y+56,   188, 28, ___FONT, ___STYLE, "当车速超过限速时的闪灯, 1=启用, 0=关闭")
INPUT_DIALOG_ITEM("换单警报: ",   __oc_speaker,    1,  ___X+12,  ___Y+88,   188, 28, ___FONT, ___STYLE, "换单时的警报信号, 1=启用, 0=关闭")
INPUT_DIALOG_ITEM("喷墨开关: ",   __pushmark,      1,  ___X+12,  ___Y+120,  188, 28, ___FONT, ___STYLE, "当生产到一叠时, 发喷墨信号[现未用]")
INPUT_DIALOG_ITEM("切废张数: ",   __scrap_count,   2,  ___X+230, ___Y+24,   188, 28, ___FONT, ___STYLE, "切废功能设置, 0=关闭, >0时其值为切废张数")
INPUT_DIALOG_ITEM("切废切长: ",   __scrap_cutlen,  4,  ___X+230, ___Y+56,   188, 28, ___FONT, ___STYLE, "切废时所用的切长, 单位毫米")
/* 系统选项 */
#undef  ___X
#undef  ___Y
#define ___X  28
#define ___Y  220
INPUT_DIALOG_GROUP(" 系统选项 ",  __system,  ___X, ___Y, 448, 159, ___FONT, GROUPBOX_STYLE_CAPTION)
INPUT_DIALOG_ITEM("复位提示: ",   __reset_prompt,  1,  ___X+12,  ___Y+24,   188, 28, ___FONT, ___STYLE, "开机时, 是否提示计数复位, 1=提示, 0=不提示")
INPUT_DIALOG_ITEM("本日数据: ",   __today_data,    1,  ___X+12,  ___Y+56,   188, 28, ___FONT, ___STYLE, "本日生产数据的显示, 1=显示, 0=不显示")
INPUT_DIALOG_ITEM("公/英 制: ",   __mm_inch,       1,  ___X+12,  ___Y+88,   188, 28, ___FONT, ___STYLE, "公/英制设定, 0=公制(mm), 1=英制(inch)")
INPUT_DIALOG_ITEM("最小切长: ",   __min_cutlen,    4,  ___X+230, ___Y+24,   188, 28, ___FONT, ___STYLE, "系统可接受的最小设定长(切长), 单位: 毫米")
INPUT_DIALOG_ITEM("车速比例: ",   __speed_scale,   5,  ___X+230, ___Y+56,   188, 28, ___FONT, ___STYLE, "车速比例, 可输入小数, 例如: 1.2")
INPUT_DIALOG_ITEM("车速增益: ",   __speed_plus,    4,  ___X+230, ___Y+88,   188, 28, ___FONT, ___STYLE, "车速增益(加数)")
/* 其它选项 */
#undef  ___X
#undef  ___Y
#define ___X  492
#define ___Y  45
INPUT_DIALOG_GROUP(" 其它选项 ", __misc,    ___X, ___Y, 220, 334, ___FONT, GROUPBOX_STYLE_CAPTION)
INPUT_DIALOG_ITEM("大字体#1: ",   __font_big1,     2,  ___X+12,  ___Y+24,   188, 28, ___FONT, ___STYLE, "最大号字体的字体选择, 0 ~ 5")
INPUT_DIALOG_ITEM("大字体#2: ",   __font_big2,     2,  ___X+12,  ___Y+56,   188, 28, ___FONT, ___STYLE, "第二大字体的字体选择, 0 ~ 5")
INPUT_DIALOG_ITEM("当前时间: ",   __set_time,      8,  ___X+12,  ___Y+248,  188, 28, ___FONT, ___STYLE, "设定系统时间, 按F9确认时间")
INPUT_DIALOG_ITEM("当前日期: ",   __set_date,     10,  ___X+12,  ___Y+280,  188, 28, ___FONT, ___STYLE, "设定系统日期, 按F9确认日期")
/* 对话框属性 */
INPUT_DIALOG_SET(define_generic_param_dialog, "参数设置", &icon, 138, 128, 740, 476, 70, WIDGET_BKCOLOR, CONFIG_FONT_16, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE)
DECLARE_INPUT_DIALOG_ENDED(define_generic_param_dialog);

void init_generic_param_dlg(void)
{
    char * s;

    input_dialog_preset_title(&define_generic_param_dialog, pick_string(NULL, "CUT Generic Parameters"));

    #define ____do_set(id, name_en, comment_en) \
                  do { \
                    s = pick_string(NULL, name_en); \
                    if(s) \
                        input_dialog_preset_item_name(&define_generic_param_dialog, id, s); \
                    s = pick_string(NULL, comment_en); \
                    if(s) \
                        input_dialog_preset_item_comment(&define_generic_param_dialog, id, s); \
                  } while(0)

    ____do_set(__generic,         " Generic ",      NULL);
    ____do_set(__alarm_count,     "AlarmCnt: ",     "The remain count threshold to Alarm");
    ____do_set(__alarm_overspd,   "AlarmSpd: ",     "Over Speed Alarm, 1=enable, 0=disable");
    ____do_set(__oc_speaker,      "OC_Speak: ",     "Enable Speaker when Order changes");
    ____do_set(__pushmark,        "PushMark: ",     "Send Mark Signal");
    ____do_set(__scrap_count,     "ScrapCnt: ",     "Scrap Function, 0=disable, >0 means the scrap count");
    ____do_set(__scrap_cutlen,    "ScrapLen: ",     "The Scrap Cutlen if enabled");

    ____do_set(__system,          " System ",       NULL);
    ____do_set(__reset_prompt,    "Reset_Pr: ",     "Enable Reset Prompt on Startup, 1=Enable, 0=Disable");
    ____do_set(__today_data,      "TodayDat: ",     "Show the producing data for today, 1=Yes, 0=No");
    ____do_set(__min_cutlen,      "MiCutLen: ",     "The Minimum CutLen, used for check the input order. Unit: mm");
    ____do_set(__mm_inch,         "mm/inch:  ",     "The Unit of the CutLen, 1=mm, 0=inch");
    ____do_set(__speed_scale,     "SpdScale: ",     "The Speed scale, ex: 1.000");
    ____do_set(__speed_plus,      "Spd_Plus: ",     "Speed Plus");

    ____do_set(__misc,            " MISC ",         NULL);
    ____do_set(__font_big1,       "FontBig1: ",     "Font Big1 selection, 0 ~ 5");
    ____do_set(__font_big2,       "FontBig2: ",     "Font Big2 selection, 0 ~ 5");
    ____do_set(__set_time,        "Set_Time: ",     "Set System Time, F9 to confirm");
    ____do_set(__set_date,        "Set_Date: ",     "Set System Date, F9 to confirm");

    #undef ____do_set

    generic_param_dlg = input_dialog_initialize(&define_generic_param_dialog);
    if (!generic_param_dlg)
        goto some_error;
    input_dialog_set_buttons_caption(&define_generic_param_dialog, 
                                     pick_string("确认[F10]", "OK.[F10]"), 
                                     pick_string("取消[ESC]", "No.[ESC]"));
    input_dialog_set_ok_comment(&define_generic_param_dialog, 
                                     pick_string("确认, 请按F10或回车键", "Press F10/Enter to Confirm"));
    input_dialog_set_cancel_comment(&define_generic_param_dialog, 
                                     pick_string("取消, 请按ESC或回车键", "Press ESC/Enter to Cancel"));
    gui_widget_link(NULL, generic_param_dlg);

    generic_param_window = gui_create_window(generic_param_dlg);
    if (!generic_param_window)
        goto some_error;

    return;

some_error:
    sys_print("init_generic_param_dlg(): failed to create widgets or window!\n");
    ExitApplication();
}

static void ____generic_param_alert(char * s)
{
    input_dialog_alert(&define_generic_param_dialog, s, COLOR_WARNING);
}

static void generic_param_prepare(int id, char *buf, void * data, INT16U opt)
{
    struct cut_config_s * __config;

    FamesAssert(buf);
    FamesAssert(data);
    if(!buf || !data)
        return;

    __config = (struct cut_config_s *)data;

    #define bool_val(x) (((x) != 0) ? 1 : 0)

    switch (id) {
        case __reset_prompt:
            sprintf(buf, "%d", bool_val(__config->reset_onstartup));
            break;
        case __today_data:
            sprintf(buf, "%d", bool_val(__config->today_data_show));
            break;
        case __alarm_count:
            sprintf(buf, "%d", __config->alarm_count);
            break;
        case __alarm_overspd:
            sprintf(buf, "%d", bool_val(__config->overspd_alarm_en));
            break;
        case __oc_speaker:
            sprintf(buf, "%d", bool_val(__config->oc_speaker_en));
            break;
        case __pushmark:
            sprintf(buf, "%d", bool_val(__config->pushmark_en));
            break;
        case __scrap_count:
            sprintf(buf, "%d", __config->scrap_count);
            break;
        case __scrap_cutlen:
            sprintf(buf, "%d", __config->scrap_cutlen);
            break;
        case __min_cutlen:
            sprintf(buf, "%d", __config->min_cutlen);
            break;
        case __mm_inch:
            if (opt & INPUT_DIALOG_PREPARE_OPT_EDITING) {
                sprintf(buf, "%d", bool_val(__config->mm_inch));
            } else {
                sprintf(buf, "%s", (__config->mm_inch == 0)
                                    ? pick_string("公制", "mm")
                                    : pick_string("英制", "Inch"));
            }
            break;
        case __speed_scale:
            INT16toSTR(buf, __config->speed_scale,
                            CHG_OPT_END|CHG_OPT_DEC|CHG_OPT_FRC|0x53);
            break;
        case __speed_plus:
            sprintf(buf, "%d", __config->speed_plus);
            break;
        case __font_big1:
            sprintf(buf, "%d", __config->font_big1);
            break;
        case __font_big2:
            sprintf(buf, "%d", __config->font_big2);
            break;
        case __set_time:
            {
                struct time now;
                lock_kernel();
                gettime(&now);
                unlock_kernel();
                sprintf(buf, "%02d:%02d:%02d",
                        now.ti_hour,
                        now.ti_min,
                        now.ti_sec);
            }
            break;
        case __set_date:
            {
                struct date today;
                lock_kernel();
                getdate(&today);
                unlock_kernel();
                sprintf(buf, "%02d-%02d-%02d",
                        today.da_year,
                        today.da_mon,
                        today.da_day);
            }
            break;
        default:
            sprintf(buf, "?");
            break;
    } /* switch(id) */

    #undef bool_val

    return;
}

/***************************************************************************************
** 返回值: 0=内容无效, 1=内容有效, -1=所有内容须刷新
*/
static int generic_param_finish(int id, char *buf, void * data, KEYCODE key)
{
    INT16U  chg_flag;
    struct cut_config_s * __config;
    int  temp, bool_val;

    FamesAssert(buf);
    FamesAssert(data);
    if(!buf || !data)
        return 1; /* 如果缓冲无效, 那么就认为其内容总是有效的 */

    key = key;

    __config = (struct cut_config_s *)data;

    chg_flag = CHG_OPT_DEC;

    temp = STRtoINT16(buf, chg_flag);
    bool_val = (temp) ? 1 : 0;
    
    switch (id) {
        case __reset_prompt:
            __config->reset_onstartup = bool_val;
            break;
        case __today_data:
            if (__config->today_data_show != bool_val) {
                __config->today_data_show = bool_val;
                show_today_data_screen(bool_val);
            }
            break;
        case __alarm_count:
            if (temp <= 0 || temp >= 1000) {
                ____generic_param_alert(pick_string(
                      "闪灯张数不能小于1",
                      "Should not less than 1"));
                return 0;
            }
            __config->alarm_count = temp;
            break;
        case __alarm_overspd:
            __config->overspd_alarm_en = bool_val;
            break;
        case __oc_speaker:
            __config->oc_speaker_en = bool_val;
            break;
        case __pushmark:
            __config->pushmark_en = bool_val;
            break;
        case __scrap_count:
            if (temp < 0) {
                ____generic_param_alert(pick_string(
                      "不应该是个负数",
                      "Should not negative"));
                return 0;
            }
            __config->scrap_count = temp;
            break;
        case __scrap_cutlen:
            if (temp < __config->min_cutlen) {
                ____generic_param_alert(pick_string(
                      "不应该小于最小切长",
                      "Should greater than the min_cutlen"));
                return 0;
            }
            __config->scrap_cutlen = temp;
            break;
        case __min_cutlen:
            if (temp < 200) {
                ____generic_param_alert(pick_string(
                      "不应该小于200mm",
                      "Should not less than 200mm"));
                return 0;
            }
            __config->min_cutlen = temp;
            break;
        case __mm_inch:
            __config->mm_inch = bool_val;
            break;
        case __speed_scale:
            temp = STRtoINT16(buf, CHG_OPT_DEC|CHG_OPT_FRC|0x53);
            if (temp < 0) {
                ____generic_param_alert(pick_string(
                      "不应该是个负数或者输入的数字太大",
                      "Should not negative or too big"));
                return 0;
            }
            __config->speed_scale = temp;
            break;
        case __speed_plus:
            __config->speed_plus = temp;
            break;
        case __font_big1:
            if (__config->font_big1 != temp) {
                __config->font_big1 = temp;
                set_status_font_big1(temp);
            }
            break;
        case __font_big2:
            if (__config->font_big2 != temp) {
                __config->font_big2 = temp;
                set_status_font_big2(temp);
            }
            break;
        case __set_time:
            if (key == F9) {
                struct time now;
                int hour, min, sec;
                char s[64];
                sscanf(buf, "%d:%d:%d", &hour, &min, &sec);
                if (hour < 0 || hour > 23) {
                    ____generic_param_alert(pick_string(
                          "小时无效, 其范围是[0 ~ 23]",
                          "Hour is invalid, it should be 0 ~ 23"));
                    return 0;
                }
                if (min < 0 || min > 59) {
                    ____generic_param_alert(pick_string(
                          "分钟无效, 其范围是[0 ~ 59]",
                          "Minute is invalid, it should be 0 ~ 59"));
                    return 0;
                }
                if (sec < 0 || sec > 59) {
                    ____generic_param_alert(pick_string(
                          "秒钟无效, 其范围是[0 ~ 59]",
                          "Second is invalid, it should be 0 ~ 59"));
                    return 0;
                }
                now.ti_hour = hour;
                now.ti_min = min;
                now.ti_sec = sec;
                lock_kernel();
                settime(&now);
                unlock_kernel();
                sprintf(s, pick_string("系统时间被设定为: %02d:%02d:%02d", 
                                       "System Time Setted: %02d:%02d:%02d"),
                           now.ti_hour, now.ti_min, now.ti_sec);
                message(s);
            }
            break;
        case __set_date:
            if (key == F9) {
                struct date today;
                int year, mon, day;
                int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
                char s[64];
                sscanf(buf, "%d-%d-%d", &year, &mon, &day);
                if (year < 2000 || year > 2099) {
                    ____generic_param_alert(pick_string(
                          "年份无效, 其范围是[2000 ~ 2099]",
                          "Year is invalid, it should be 2000 ~ 2099"));
                    return 0;
                }
                if (mon < 1 || mon > 12) {
                    ____generic_param_alert(pick_string(
                          "月份无效, 其范围是[1 ~ 12]",
                          "Month is invalid, it should be 1 ~ 12"));
                    return 0;
                }
                if ((year % 400) == 0 || /* 判断是否是闰年 */
                    ((year % 4) == 0 && (year % 100) != 0))
                    days[1] = 29;
                if (day < 1 || day > days[mon-1]) {
                    sprintf(s, pick_string(
                          "日期无效, 其范围是[1 ~ %d]",
                          "The Day is invalid, it should be 1 ~ %d"), days[mon-1]);
                    ____generic_param_alert(s);
                    return 0;
                }
                today.da_year = year;
                today.da_mon = mon;
                today.da_day = day;
                lock_kernel();
                setdate(&today);
                unlock_kernel();
                sprintf(s, pick_string("系统日期被设定为: %02d-%02d-%02d", 
                                       "System Date Setted: %02d-%02d-%02d"),
                           today.da_year, today.da_mon, today.da_day);
                message(s);
            }
            break;
        default:
            break;
    } /* switch(id) */
    
    return 1;
}

BOOL do_setup_generic_param(void)
{
    BOOL retval;
    struct cut_config_s t;
     
    FamesAssert(generic_param_window);
    if(!generic_param_window)
        return fail;

    t = config;    
    gui_show_window(generic_param_window);
    retval = input_dialog_method(&define_generic_param_dialog, generic_param_prepare, generic_param_finish, (void *)&t, 0);
    if (retval) {
        lock_kernel();
        copy_to_config(&t);
        save_config();
        unlock_kernel();
    }
    gui_hide_window(generic_param_window);
    
    return retval;
}

/*-----------------------------------------------------------------------------------------
 *          
 *      限速表的相关定义
 * 
**---------------------------------------------------------------------------------------*/
enum spdlmt_view_fields_id {
    __spdlmt_sn,
    __spdlmt_setlen,
    __spdlmt_spdlmt,
};

static view_fields_t spdlmt_view_fields[] =
{
#define ____style        0
#define ____draw_style   DRAW_OPT_FIL_BG|DRAW_OPT_ALIGN_CENTER
    { "序号",    __spdlmt_sn,       6, 12,  ____style,  ____draw_style,  "", },
    { "切长",    __spdlmt_setlen,   5, 16,  ____style,  ____draw_style,  "请输入: 裁切长度(毫米)", },
    { "限速",    __spdlmt_spdlmt,   4, 16,  ____style,  ____draw_style,  "请输入: 车速可达到的最大值", },
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
                            spdlmt_view_fields[index].caption = s; \
                        s = pick_string(NULL, comment_en); \
                        if(s) \
                            spdlmt_view_fields[index].comment = comment_en; \
                    } while(0)

    ____do_set(0, "S/N",            "");
    ____do_set(1, "CutLen",         "Enter: Cut Length(mm)");
    ____do_set(2, "Speed Limit",    "Enter: The Maximum Speed Available");

    #undef ____do_set
}

static int spdlmt_view_get_max(void)
{
    return (CONFIG_SPDLMT_SIZE - 1);
}

static void spdlmt_view_show_statistics(int index,
                           int x, int y, int width, int height, 
                           int color, int bkcolor, int font,
                           INT08S *field_comment,
                           INT08S *old, INT16U option)
{
    char ___s[96];
    
    FamesAssert(old);
    if(!old)
        return;

    index = index;
    option = option;

    sprintf(___s, "%s", field_comment);

    draw_font_for_widget(x, y, width, height, ___s, old, color, bkcolor, font, DRAW_OPT_FIL_BG);
}

static BOOL spdlmt_view_is_writable(int index, int field_id, INT16U option)
{
    BOOL retval;

    index = index;
    option = option;

    retval = ok;

    switch(field_id){
        case __spdlmt_setlen:
            retval = ok; 
            break;
        case __spdlmt_spdlmt:
            retval = ok; 
            break;
        default:
            retval = fail;
            break;
    }
    
    return  retval;
}

static BOOL spdlmt_view_get_item(int index, int field_id, char * buf, int buf_len, INT16U option)
{
    BOOL retval;

    FamesAssert(buf);
    if(!buf)
        return fail;

    option = option;
    buf_len = buf_len;

    if(index < 0 || index > spdlmt_view_get_max())
        return fail;

    retval = ok;

    switch(field_id){
        case __spdlmt_sn:
            sprintf(buf, pick_string("限速%d", "#%d"), index+1);
            break;
        case __spdlmt_setlen:
            sprintf(buf, "%d", __config_for_setup.spdlmt_tbl[index].cutlen);
            break;
        case __spdlmt_spdlmt:
            sprintf(buf, "%d", __config_for_setup.spdlmt_tbl[index].spdlmt);
            break;
        default:
            retval = fail;
            break;
    }
    
    return retval;
}

static BOOL spdlmt_view_set_item(int index, int field_id, char * buf, int buf_len, KEYCODE key, INT16U option)
{
    BOOL retval;
    INT16S temp;

    FamesAssert(buf);
    if(!buf)
        return fail;

    key = key;
    option = option;
    buf_len = buf_len;

    if(index < 0 || index > spdlmt_view_get_max())
        return fail;

    temp = STRtoINT16(buf, CHG_OPT_DEC);

    switch(field_id){
        case __spdlmt_setlen:
            __config_for_setup.spdlmt_tbl[index].cutlen = temp;
            break;
        case __spdlmt_spdlmt:
            __config_for_setup.spdlmt_tbl[index].spdlmt = temp;
            break;
        default:
            retval = fail;
            break;
    }

    return retval;
}

static BOOL spdlmt_view_init_view_private(void)
{
    BOOL retval;

    ____set_comment_en();
    
    retval = gui_view_init_private( spdlmt_view, 
                                    spdlmt_view_fields, 
                                    spdlmt_view_get_max,
                                    spdlmt_view_get_item,
                                    spdlmt_view_set_item, 
                                    spdlmt_view_is_writable,
                                    NULL,
                                    pick_string("=说=明=", "Comment"),
                                    spdlmt_view_show_statistics, 
                                    NULL,
                                    COLOR_YELLOW, /* 123, */
                                    COLOR_BLACK, /* 17 */
                                    30, /*height_per_row*/
                                    1
                                  );

    return retval;
}

void init_spdlmt_setup_dlg(void)
{
    gui_widget * status_bar;
    int height_add = 30;

    /* 限速表设定窗口 */
    spdlmt_dlg = gui_create_widget(GUI_WIDGET_FORM, 238, 148, 540, (387 + height_add), 0, 0, font16, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE);
    if(!spdlmt_dlg)
        goto some_error;
    gui_widget_link(NULL, spdlmt_dlg);      /* 设置主界面背景 */
    gui_form_init_private(spdlmt_dlg, 32);
    gui_form_set_icon(spdlmt_dlg, &icon);
    gui_form_set_caption(spdlmt_dlg, pick_string("限速表设置", "Speed Limit Setup"));

    /* 工具条 */
    status_bar = gui_create_widget(GUI_WIDGET_LABEL, 5, (350 + height_add), 530, 32, 0, 0, font16, LABEL_STYLE_CLIENT_BDR);
    if(!status_bar)
        goto some_error;
    gui_widget_link(spdlmt_dlg, status_bar);
    gui_label_init_private(status_bar, 128);
    gui_label_set_text(status_bar, pick_string(" [F10] 保存       [ESC] 返回",
                                               " [F10] Save       [ESC] Return"));
    /* 限速表视图 */
    spdlmt_view = gui_create_widget(GUI_WIDGET_VIEW, 12, 38, 516, (304 + height_add), 0, 0, 1, 0x70|VIEW_STYLE_NONE_FIRST);
    if(!spdlmt_view)
        goto some_error;
    gui_widget_link(spdlmt_dlg, spdlmt_view);
    if(!spdlmt_view_init_view_private())
        goto some_error;

    spdlmt_window = gui_create_window(spdlmt_dlg);
    if (!spdlmt_window)
        goto some_error;

    return;

some_error:
    sys_print("init_spdlmt_setup_dlg(): failed to create widgets or window!\n");
    ExitApplication();
}

static KEYCODE __setup_spdlmt_item(void)
{
    KEYCODE key;

    key = gui_view_editing(spdlmt_view, 0);

    if (key == NONEKEY)
        key = waitkey(0L);

    return key;
}

void do_setup_spdlmt_tbl(void)
{
    KEYCODE key;

    if(!spdlmt_window)
        return;

    lock_kernel();
    __config_for_setup = config;
    unlock_kernel();

    gui_show_window(spdlmt_window);

    for (;;) {
        key = __setup_spdlmt_item();
        switch(key){
            case ESC:
                goto out;
            case UP:
                gui_view_move_up(spdlmt_view);
                break;
            case DOWN:
                gui_view_move_down(spdlmt_view);
                break;
            case PGUP:
                gui_view_page_up(spdlmt_view);
                break;
            case PGDN:
                gui_view_page_down(spdlmt_view);
                break;
            case CTRL_HOME:
                gui_view_goto_top(spdlmt_view);
                break;
            case CTRL_END:
                gui_view_goto_bottom(spdlmt_view);
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
    gui_hide_window(spdlmt_window);

    return;
}

INT16S get_speed_limit(INT16S cutlen)
{
    int i, found = -1;
    INT16S min, t;
    INT16S spdlmt = CONFIG_SPDLMT_MAXIMUM;

    lock_kernel();
    for (i = 0; i < CONFIG_SPDLMT_SIZE; i++) {
        t = config.spdlmt_tbl[i].cutlen;
        if (t == 0) /* 长度为0代表结尾 */
            break;
        t = t - cutlen;
        if (t < 0)
            t = -t;
        if (found == -1) {
            found = i;
            min = t;
        } else if (t < min) {
            found = i;
            min = t;
        }
    }
    if (found >= 0)
        spdlmt = config.spdlmt_tbl[found].spdlmt;
    unlock_kernel();

    if (spdlmt > CONFIG_SPDLMT_MAXIMUM)
        spdlmt = CONFIG_SPDLMT_MAXIMUM;

#if 0
    lock_kernel();
    printf("cutlen = %d, found = %d, min = %d, spdlmt = %d\n",
            cutlen, found, min, spdlmt);
    getch();
    unlock_kernel();
#endif

    return spdlmt;
}


/*=========================================================================================
 * 
 * 本文件结束: setup.c
 * 
**=======================================================================================*/



