/******************************************************************************************
 * 文件:    setup_su.c
 *
 * 描述:    高级参数设置(超级用户)
 *
 * 作者:    Jun
 *
 * 时间:    2013-1-29
******************************************************************************************/
#define  APP_SETUP_SUPER_C
#include <FamesOS.h>
#include "common.h"


static gui_widget   * super_param_dlg = NULL;
static gui_window_t * super_param_window = NULL;

extern BMPINFO  icon;       /* 图标  */


/*-----------------------------------------------------------------------------------------
 *          
 *      高级参数的相关定义
 * 
**---------------------------------------------------------------------------------------*/
enum super_param_id {
    __driver,
    __driver_type,

    __control,

    __1_port,
    __1_port_base,
    __1_port_rate,
    __1_port_data,
    __1_port_stop,
    __1_port_parity,
    __1_port_irq,

    __2_port,
    __2_port_base,
    __2_port_rate,
    __2_port_data,
    __2_port_stop,
    __2_port_parity,
    __2_port_irq,

    __misc,
    __language,
};

char * get_driver_types_name(int type)
{
    char * name = NULL;

    switch (type) {
        case 0:
            name = pick_string("正频", "ZhengPing");
            break;
        case 1:
            name = pick_string("包米勒", "BaoMiLe");
            break;
        default:
            name = "N/A";
            break;
    }

    return name;
}

static char __note_driver_type_zh[] = "驱动器类型: 0=正频, 1=包米勒.";

static char __note_driver_type_en[] = "Driver Type: 0=ZhengPing, 1=BaoMiLe.";

static char __note_port_base_zh[]   = "I/O地址, 比如3F8, 2F8等";
static char __note_port_rate_zh[]   = "波特率, 9600 ~ 115200";
static char __note_port_data_zh[]   = "数据位, 5 ~ 8";
static char __note_port_stop_zh[]   = "停止位, 1 ~ 2";
static char __note_port_irq_zh[]    = "中断号(IRQ), 3 ~ 15";
static char __note_port_parity_zh[] = "校验位, N=无校验, E=偶校验, O=奇校验";

static char __note_port_base_en[]   = "I/O Port Address, For example: 3F8, 2F8";
static char __note_port_rate_en[]   = "Baudrate: 9600 ~ 115200";
static char __note_port_data_en[]   = "Data Bits: 5 ~ 8";
static char __note_port_stop_en[]   = "Stop Bits: 1 ~ 2";
static char __note_port_irq_en[]    = "IRQ: 3 ~ 15";
static char __note_port_parity_en[] = "Parity: N=None, E=Even, O=Odd";

static char * __err_port_base_zh    = "I/O地址好象不太对, 请重新输入一次吧";
static char * __err_port_rate_zh    = "波特率不应该是这个值, 其正确的范围应该是: 2400 ~ 115200";
static char * __err_port_data_zh    = "数据位无效, 其正确的范围应该是: 5 ~ 8";
static char * __err_port_stop_zh    = "停止位无效, 其正确的范围应该是: 1 ~ 2";
static char * __err_port_irq_zh     = "中断号(IRQ)无效, 其正确的范围应该是: 3 ~ 15";
static char * __err_port_parity_zh  = "校验位应该是以下之一: N, E, O";

static char * __err_port_base_en    = "I/O Port Address Invalid, Try again!";
static char * __err_port_rate_en    = "Baudrate Invalid! [2400 ~ 115200]";
static char * __err_port_data_en    = "Data Bits Invalid! [5 ~ 8]";
static char * __err_port_stop_en    = "Stop Bits Invalid! [1 ~ 2]";
static char * __err_port_irq_en     = "IRQ Invalid! [3 ~ 15]";
static char * __err_port_parity_en  = "Parity Invalid! [N, E, O]";

#define  __err_port_base    (pick_string(__err_port_base_zh,   __err_port_base_en))
#define  __err_port_rate    (pick_string(__err_port_rate_zh,   __err_port_rate_en))
#define  __err_port_data    (pick_string(__err_port_data_zh,   __err_port_data_en))
#define  __err_port_stop    (pick_string(__err_port_stop_zh,   __err_port_stop_en))
#define  __err_port_irq     (pick_string(__err_port_irq_zh,    __err_port_irq_en))
#define  __err_port_parity  (pick_string(__err_port_parity_zh, __err_port_parity_en))

#define ___STYLE  0
#define ___FONT   1  /* font16 */
DECLARE_INPUT_DIALOG_START(define_super_param_dialog)
/* 驱动器选项 */
#undef  ___X
#undef  ___Y
#define ___X  28
#define ___Y  45
INPUT_DIALOG_GROUP(" 驱动器 ",   __driver,      ___X,  ___Y, 448, 95, ___FONT, GROUPBOX_STYLE_CAPTION)
INPUT_DIALOG_ITEM("类  型: ",    __driver_type,    1,  ___X+10,  ___Y+24,  180, 28, ___FONT, ___STYLE,  __note_driver_type_zh)
/* 串口1设置 */
#undef  ___X
#undef  ___Y
#define ___X  28
#define ___Y  149
INPUT_DIALOG_GROUP(" 串口1(连驱动器) ", __1_port, ___X, ___Y, 216, 223, ___FONT, GROUPBOX_STYLE_CAPTION)
INPUT_DIALOG_ITEM("基地址: ",    __1_port_base,     4,  ___X+10,  ___Y+24,  180, 28, ___FONT, ___STYLE, __note_port_base_zh)
INPUT_DIALOG_ITEM("波特率: ",    __1_port_rate,     6,  ___X+10,  ___Y+56,  180, 28, ___FONT, ___STYLE, __note_port_rate_zh)
INPUT_DIALOG_ITEM("数据位: ",    __1_port_data,     1,  ___X+10,  ___Y+88,  180, 28, ___FONT, ___STYLE, __note_port_data_zh)
INPUT_DIALOG_ITEM("停止位: ",    __1_port_stop,     1,  ___X+10,  ___Y+120, 180, 28, ___FONT, ___STYLE, __note_port_stop_zh)
INPUT_DIALOG_ITEM("校验位: ",    __1_port_parity,   1,  ___X+10,  ___Y+152, 180, 28, ___FONT, ___STYLE, __note_port_parity_zh)
INPUT_DIALOG_ITEM("中断号: ",    __1_port_irq,      2,  ___X+10,  ___Y+184, 180, 28, ___FONT, ___STYLE, __note_port_irq_zh)
/* 串口2设置 */
#undef  ___X
#undef  ___Y
#define ___X  260
#define ___Y  149
INPUT_DIALOG_GROUP(" 串口2 ", __2_port, ___X, ___Y, 216, 223, ___FONT, GROUPBOX_STYLE_CAPTION)
INPUT_DIALOG_ITEM("基地址: ",    __2_port_base,     4,  ___X+10,  ___Y+24,  180, 28, ___FONT, ___STYLE, __note_port_base_zh)
INPUT_DIALOG_ITEM("波特率: ",    __2_port_rate,     6,  ___X+10,  ___Y+56,  180, 28, ___FONT, ___STYLE, __note_port_rate_zh)
INPUT_DIALOG_ITEM("数据位: ",    __2_port_data,     1,  ___X+10,  ___Y+88,  180, 28, ___FONT, ___STYLE, __note_port_data_zh)
INPUT_DIALOG_ITEM("停止位: ",    __2_port_stop,     1,  ___X+10,  ___Y+120, 180, 28, ___FONT, ___STYLE, __note_port_stop_zh)
INPUT_DIALOG_ITEM("校验位: ",    __2_port_parity,   1,  ___X+10,  ___Y+152, 180, 28, ___FONT, ___STYLE, __note_port_parity_zh)
INPUT_DIALOG_ITEM("中断号: ",    __2_port_irq,      2,  ___X+10,  ___Y+184, 180, 28, ___FONT, ___STYLE, __note_port_irq_zh)
/* 控制选项 */
#undef  ___X
#undef  ___Y
#define ___X  492
#define ___Y  45
INPUT_DIALOG_GROUP(" 控制选项 ", __control,  ___X, ___Y, 220, 95, ___FONT, GROUPBOX_STYLE_CAPTION)
/* 其它选项 */
#undef  ___X
#undef  ___Y
#define ___X  492
#define ___Y  149
INPUT_DIALOG_GROUP(" 其它选项 ", __misc,     ___X, ___Y, 220, 223, ___FONT, GROUPBOX_STYLE_CAPTION)
INPUT_DIALOG_ITEM("语言选择:",   __language,    1, ___X+10,  ___Y+56,  180, 28, ___FONT, ___STYLE,  "语言: 0=简体中文(GB2312), 1=ENGLISH")
/* 对话框属性 */
INPUT_DIALOG_SET(define_super_param_dialog, "高级参数", &icon, 138, 128, 740, 476, 70, WIDGET_BKCOLOR, CONFIG_FONT_16, FORM_STYLE_XP_BORDER|FORM_STYLE_TITLE)
DECLARE_INPUT_DIALOG_ENDED(define_super_param_dialog);

void init_super_param_dlg(void)
{
    char * s;

    input_dialog_preset_title(&define_super_param_dialog, pick_string(NULL, "CUT Super Parameters"));

    #define ____do_set(id, name_en, comment_en) \
                  do { \
                    s = pick_string(NULL, name_en); \
                    if(s) \
                        input_dialog_preset_item_name(&define_super_param_dialog, id, s); \
                    s = pick_string(NULL, comment_en); \
                    if(s) \
                        input_dialog_preset_item_comment(&define_super_param_dialog, id, s); \
                  } while(0)

    ____do_set(__driver,            " Driver ",       NULL);
    ____do_set(__driver_type,       "Type: ",         __note_driver_type_en);

    ____do_set(__1_port,            " Port1 ",        NULL);
    ____do_set(__1_port_base,       "PortAddr: ",     __note_port_base_en);
    ____do_set(__1_port_rate,       "Baudrate: ",     __note_port_rate_en);
    ____do_set(__1_port_data,       "DataBits: ",     __note_port_data_en);
    ____do_set(__1_port_stop,       "StopBits: ",     __note_port_stop_en);
    ____do_set(__1_port_parity,     "Parity:   ",     __note_port_parity_en);
    ____do_set(__1_port_irq,        "IRQ:      ",     __note_port_irq_en);

    ____do_set(__2_port,            " Port2 ",        NULL);
    ____do_set(__2_port_base,       "PortAddr: ",     __note_port_base_en);
    ____do_set(__2_port_rate,       "Baudrate: ",     __note_port_rate_en);
    ____do_set(__2_port_data,       "DataBits: ",     __note_port_data_en);
    ____do_set(__2_port_stop,       "StopBits: ",     __note_port_stop_en);
    ____do_set(__2_port_parity,     "Parity:   ",     __note_port_parity_en);
    ____do_set(__2_port_irq,        "IRQ:      ",     __note_port_irq_en);

    ____do_set(__control,           " Control ",      NULL);

    ____do_set(__misc,              " MISC ",         NULL);
    ____do_set(__language,          "Language:  ",   "Language: 0=简体中文(GB2312), 1=ENGLISH");

    #undef ____do_set

    super_param_dlg = input_dialog_initialize(&define_super_param_dialog);
    if (!super_param_dlg)
        goto some_error;
    input_dialog_set_buttons_caption(&define_super_param_dialog, 
                                     pick_string("确认[F10]", "OK.[F10]"), 
                                     pick_string("取消[ESC]", "No.[ESC]"));
    input_dialog_set_ok_comment(&define_super_param_dialog, 
                                     pick_string("确认, 请按F10或回车键", "Press F10/Enter to Confirm"));
    input_dialog_set_cancel_comment(&define_super_param_dialog, 
                                     pick_string("取消, 请按ESC或回车键", "Press ESC/Enter to Cancel"));
    gui_widget_link(NULL, super_param_dlg);

    super_param_window = gui_create_window(super_param_dlg);
    if (!super_param_window)
        goto some_error;

    return;

some_error:
    sys_print("init_super_param_dlg(): failed to create widgets or window!\n");
    ExitApplication();
}

static void ____super_param_alert(char * s)
{
    input_dialog_alert(&define_super_param_dialog, s, COLOR_WARNING);
}

static char * __parity_str[] = { "None", "Even", "Odd" };

static void super_param_prepare(int id, char *buf, void * data, INT16U opt)
{
    struct cut_config_s * __config;

    FamesAssert(buf);
    FamesAssert(data);
    if(!buf || !data)
        return;

    __config = (struct cut_config_s *)data;

    switch(id){
        case __driver_type:
            if(opt & INPUT_DIALOG_PREPARE_OPT_EDITING){
                sprintf(buf, "%d", __config->driver_type);
            } else {
                sprintf(buf, "%s", get_driver_types_name(__config->driver_type));
            }
            break;
        case __1_port_base:
            sprintf(buf, "%X", __config->port1.base);
            break;
        case __1_port_rate:
            sprintf(buf, "%ld", __config->port1.baudrate);
            break;
        case __1_port_data:
            sprintf(buf, "%d", __config->port1.data);
            break;
        case __1_port_stop:
            sprintf(buf, "%d", __config->port1.stop);
            break;
        case __1_port_parity:
            sprintf(buf, "%s", __parity_str[__config->port1.parity]);
            break;
        case __1_port_irq:
            sprintf(buf, "%d", __config->port1.int_no);
            break;
        case __2_port_base:
            sprintf(buf, "%X", __config->port2.base);
            break;
        case __2_port_rate:
            sprintf(buf, "%ld", __config->port2.baudrate);
            break;
        case __2_port_data:
            sprintf(buf, "%d", __config->port2.data);
            break;
        case __2_port_stop:
            sprintf(buf, "%d", __config->port2.stop);
            break;
        case __2_port_parity:
            sprintf(buf, "%s", __parity_str[__config->port2.parity]);
            break;
        case __2_port_irq:
            sprintf(buf, "%d", __config->port2.int_no);
            break;
        case __language:
            if(opt & INPUT_DIALOG_PREPARE_OPT_EDITING){            
                sprintf(buf, "%d", __config->language);
            } else {
                switch(__config->language){
                    case 0:
                        STRCPY(buf, "简体中文");
                        break;
                    case 1:
                    default:
                        STRCPY(buf, "ENGLISH");
                        break;
                }
            }
            break;
        default:
            sprintf(buf, "?");
            break;
    } /* switch(id) */

    return;
}

/***************************************************************************************
** 返回值: 0=内容无效, 1=内容有效, -1=所有内容须刷新
*/
static int super_param_finish(int id, char *buf, void * data, KEYCODE key)
{
    INT16U  chg_flag;
    struct cut_config_s * __config;
    int  temp;
    INT32S t32;

    FamesAssert(buf);
    FamesAssert(data);
    if(!buf || !data)
        return 1; /* 如果缓冲无效, 那么就认为其内容总是有效的 */

    key = key;

    __config = (struct cut_config_s *)data;

    chg_flag = CHG_OPT_DEC;

    temp = STRtoINT16(buf, chg_flag);
    
    switch (id) {
        case __driver_type:
            if(temp >= 0) /* FIXME: 这里需要判断驱动器类型的有效性 */
                __config->driver_type = temp;
            break;
        case __1_port_base:
            temp = STRtoINT16(buf, 0);
            if(temp < 0x100){
                ____super_param_alert(__err_port_base);
                return 0;
            }
            __config->port1.base = temp;
            break;
        case __1_port_rate:
            t32 = STRtoINT32(buf, CHG_OPT_DEC);
            if(t32 < 2400L || t32 > 115200L){
                ____super_param_alert(__err_port_rate);
                return 0;
            }
            __config->port1.baudrate = t32;
            break;
        case __1_port_data:
            if(temp < 5 || temp > 8){
                ____super_param_alert(__err_port_data);
                return 0;
            }
            __config->port1.data = temp;
            break;
        case __1_port_stop:
            if(temp < 1 || temp > 2){
                ____super_param_alert(__err_port_stop);
                return 0;
            }
            __config->port1.stop = temp;
            break;
        case __1_port_parity:
            switch(buf[0]){
                case 'n':
                case 'N':
                    __config->port1.parity = COM_PARITY_NONE;
                    break;
                case 'e':
                case 'E':
                    __config->port1.parity = COM_PARITY_EVEN;
                    break;
                case 'o':
                case 'O':
                    __config->port1.parity = COM_PARITY_ODD;
                    break;
                default:
                    ____super_param_alert(__err_port_parity);
                    return 0;
            }
            break;
        case __1_port_irq:
            if(temp < 3 || temp > 15){
                ____super_param_alert(__err_port_irq);
                return 0;
            }
            __config->port1.int_no = temp;
            break;
        case __2_port_base:
            temp = STRtoINT16(buf, 0);
            if(temp < 0x100){
                ____super_param_alert(__err_port_base);
                return 0;
            }
            __config->port2.base = temp;
            break;
        case __2_port_rate:
            t32 = STRtoINT32(buf, CHG_OPT_DEC);
            if(t32 < 2400L || t32 > 115200L){
                ____super_param_alert(__err_port_rate);
                return 0;
            }
            __config->port2.baudrate = t32;
            break;
        case __2_port_data:
            if(temp < 5 || temp > 8){
                ____super_param_alert(__err_port_data);
                return 0;
            }
            __config->port2.data = temp;
            break;
        case __2_port_stop:
            if(temp < 1 || temp > 2){
                ____super_param_alert(__err_port_stop);
                return 0;
            }
            __config->port2.stop = temp;
            break;
        case __2_port_parity:
            switch(buf[0]){
                case 'n':
                case 'N':
                    __config->port2.parity = COM_PARITY_NONE;
                    break;
                case 'e':
                case 'E':
                    __config->port2.parity = COM_PARITY_EVEN;
                    break;
                case 'o':
                case 'O':
                    __config->port2.parity = COM_PARITY_ODD;
                    break;
                default:
                    ____super_param_alert(__err_port_parity);
                    return 0;
            }
            break;
        case __2_port_irq:
            if(temp < 3 || temp > 15){
                ____super_param_alert(__err_port_irq);
                return 0;
            }
            __config->port2.int_no = temp;
            break;
        case __language:
            if(temp)
                __config->language = 1;
            else 
                __config->language = 0;
            break;            
        default:
            break;
    } /* switch(id) */
    
    return 1;
}

BOOL do_setup_super_param(void)
{
    BOOL retval;
    struct cut_config_s t;
     
    FamesAssert(super_param_window);
    if(!super_param_window)
        return fail;

    t = config;    
    gui_show_window(super_param_window);
    retval = input_dialog_method(&define_super_param_dialog, super_param_prepare, super_param_finish, (void *)&t, 0);
    if(retval){
        lock_kernel();
        copy_to_config(&t);
        save_config();
        unlock_kernel();
        /* messagebox(); */
        ____super_param_alert(pick_string("高级参数在重启之后才能生效!", 
                                          "Super Params is applied only after reboot!"));
    }
    gui_hide_window(super_param_window);
    
    return retval;
}


/*=========================================================================================
 * 
 * 本文件结束: setup_su.c
 * 
**=======================================================================================*/



