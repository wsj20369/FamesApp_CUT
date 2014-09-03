/*************************************************************************************
 * 文件:    keycmd.c
 *
 * 描述:    按键命令处理
 *
 * 作者:    Jun
 *
 * 时间:    2011-3-3
*************************************************************************************/
#define  FAMES_KEY_CMD_C
#include <includes.h>
#include "common.h"

#define  BUTTON_PRESSED_STYLE  0 /* BUTTON_STYLE_FOCUS_BORDER */

/*------------------------------------------------------------------------------------
 * 函数:    do_keycmds()
 *
 * 描述:    执行一组按键命令
 *
 * 参数:    keycmds  按键命令列表
**----------------------------------------------------------------------------------*/
void do_keycmds(key_cmd_t * keycmds)
{
    keycmd_func_t f;
    KEYCODE  key;
    key_cmd_t * t;
    
    FamesAssert(keycmds);

    if(!keycmds)
        return;

    key = NONEKEY;

    for(;;) {
        TaskSleep(10L); /* 1秒种最多接受100个按键(这同时也是为了给其它任务让路) */
        key = waitkey(0L);
        t = keycmds;
        while(t->key != NONEKEY){
            if(t->key != key){
                t++;
                continue;
            }
            f = t->func;
            if(f)
                f(t->data);
            if(t->flag & KEYCMD_FLAG_EXIT)
                goto out;
            break;
        }
    }

out:
    return;
}

/*------------------------------------------------------------------------------------
 * 函数:    do_buttoncmds()
 *
 * 描述:    执行一组按钮命令
 *
 * 参数:    buttoncmds    按键命令列表
 *          buttons       按钮数组
 *          buttons_nr    按钮个数
 *          default_one   默认按钮(1=第1个)
 *          focus_style   被选中按钮的显示风格
 *          unfocus_style 未选中按钮的显示风格
**----------------------------------------------------------------------------------*/
int do_buttoncmds(button_cmd_t * buttoncmds, gui_widget * buttons[], 
                        int buttons_nr, int default_one, 
                        INT16U focus_style, INT16U unfocus_style, buttoncmds_key_hook_fn hook)
{
    keycmd_func_t f;
    KEYCODE  key;
    button_cmd_t * t;
    int focus, focus_old, i;
    int pressed, pressed_old;
    
    FamesAssert(buttoncmds);
    FamesAssert(buttons);
    FamesAssert(buttons_nr>0)

    if(!buttoncmds || !buttons || buttons_nr<=0)
        return -1;

    default_one--; /* 默认按钮的序号从1开始排序 */
    
    focus = default_one;
    if(focus < 0)
        focus = 0;
    if(focus >= buttons_nr)
        focus = buttons_nr-1;
    pressed = focus;
    focus_old = -2;
    pressed_old = -1;
    default_one = focus;

    gui_set_widget_style(buttons[focus], unfocus_style|BUTTON_PRESSED_STYLE);

    key = NONEKEY;

    for(;;) {
        TaskSleep(10L); /* 1秒种最多接受100个按键(这同时也是为了给其它任务让路) */
        do {
            key = waitkey(0L);
            if (hook)
                key = hook(key);
        } while (key == NONEKEY);
        t = buttoncmds;
        for(i=0; (t->key!=NONEKEY); i++, t++){
            if(t->key != key){
                continue;
            }
            #if 0
            printf("KeyPressed: 0x%04X\n", key);
            #endif
            pressed = i;
            message(buttoncmds[pressed].comment);
            if(i < buttons_nr){
                focus = i;
                if(focus_old == -2){ /* 第一次移动焦点 */
                    gui_set_widget_style(buttons[default_one], unfocus_style);
                }
                if(focus_old >= 0)
                    gui_set_widget_style(buttons[focus_old], unfocus_style);
                gui_set_widget_style(buttons[focus], focus_style);
                focus_old = focus;
            }
            f = t->func;
            if(f)
                f(t->data);
            if(t->flag & KEYCMD_FLAG_EXIT)
                goto out;
            break;
        }
        if(t->key==NONEKEY){ /* 按键未找到 */
            switch(key){
                case ENTER:
                    t = buttoncmds + focus;
                    pressed = focus;
                    message(buttoncmds[pressed].comment);
                    if(focus_old == -2){ /* 第一次移动焦点 */
                        gui_set_widget_style(buttons[default_one], unfocus_style);
                    }
                    if(focus_old >= 0)
                        gui_set_widget_style(buttons[focus_old], unfocus_style);
                    gui_set_widget_style(buttons[focus], focus_style);
                    focus_old = focus;
                    f = t->func;
                    if(f)
                        f(t->data);
                    if(t->flag & KEYCMD_FLAG_EXIT)
                        goto out;
                    break;
                case TAB:
                case RIGHT:
                case DOWN:
                    focus++;
                    if(focus >= buttons_nr)
                        focus = 0;
                    pressed = focus;
                    message(buttoncmds[pressed].comment);
                    break;
                case LEFT:
                case UP:
                    focus--;
                    if(focus < 0)
                        focus = buttons_nr-1;
                    pressed = focus;
                    message(buttoncmds[pressed].comment);
                    break;
                case NONE_KEY:
                case CTRL:
                case ALT:
                case SHIFT:
                case CTRL|ALT:
                case CTRL|SHIFT:
                case ALT|SHIFT:
                case CTRL|ALT|SHIFT:
                    continue;
                default:    /* 本次按键不用处理 */
                    break;
            }
        }
        if(focus_old == -2){ /* 第一次移动焦点 */
            gui_set_widget_style(buttons[default_one], unfocus_style);
            focus_old = -1;
        }
        if(focus != focus_old){
            if(focus_old >= 0)
                gui_set_widget_style(buttons[focus_old], unfocus_style);
            gui_set_widget_style(buttons[focus], focus_style);
            focus_old = focus;
        }
        if(1 || pressed != pressed_old){
            if(pressed >= buttons_nr && pressed_old < buttons_nr){
                gui_set_widget_style(buttons[focus], unfocus_style|BUTTON_PRESSED_STYLE);
            }
            pressed_old = pressed;;
        }
    }

out:
    if(focus >= 0)
        gui_set_widget_style(buttons[focus], unfocus_style);
    if(focus_old >= 0)
        gui_set_widget_style(buttons[focus_old], unfocus_style);
    message("");
    return (focus+1); /* 序号是从1开始的 */
}

/*------------------------------------------------------------------------------------
 * 函数:    buttoncmd_set_comment()
 *
 * 描述:    设置某命令的注解
 *
 * 参数:    buttoncmds  按键命令列表
 *          key         要查找的按键
 *          comment     新的注解
**----------------------------------------------------------------------------------*/
void buttoncmd_set_comment(button_cmd_t * buttoncmds, KEYCODE key, char * comment)
{
    button_cmd_t * t;
    int i;
    
    FamesAssert(buttoncmds);

    if(!buttoncmds || !comment)
        return;

    t = buttoncmds;
    for(i=0; (t->key!=NONEKEY); i++, t++){
        if(t->key == key){
            lock_kernel();
            t->comment = comment;
            unlock_kernel();
            break;
        }
    }
}

/*====================================================================================
 * 
 * 本文件结束: keycmd.c
 * 
**==================================================================================*/


