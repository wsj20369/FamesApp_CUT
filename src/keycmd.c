/*************************************************************************************
 * �ļ�:    keycmd.c
 *
 * ����:    ���������
 *
 * ����:    Jun
 *
 * ʱ��:    2011-3-3
*************************************************************************************/
#define  FAMES_KEY_CMD_C
#include <includes.h>
#include "common.h"

#define  BUTTON_PRESSED_STYLE  0 /* BUTTON_STYLE_FOCUS_BORDER */

/*------------------------------------------------------------------------------------
 * ����:    do_keycmds()
 *
 * ����:    ִ��һ�鰴������
 *
 * ����:    keycmds  ���������б�
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
        TaskSleep(10L); /* 1����������100������(��ͬʱҲ��Ϊ�˸�����������·) */
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
 * ����:    do_buttoncmds()
 *
 * ����:    ִ��һ�鰴ť����
 *
 * ����:    buttoncmds    ���������б�
 *          buttons       ��ť����
 *          buttons_nr    ��ť����
 *          default_one   Ĭ�ϰ�ť(1=��1��)
 *          focus_style   ��ѡ�а�ť����ʾ���
 *          unfocus_style δѡ�а�ť����ʾ���
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

    default_one--; /* Ĭ�ϰ�ť����Ŵ�1��ʼ���� */
    
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
        TaskSleep(10L); /* 1����������100������(��ͬʱҲ��Ϊ�˸�����������·) */
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
                if(focus_old == -2){ /* ��һ���ƶ����� */
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
        if(t->key==NONEKEY){ /* ����δ�ҵ� */
            switch(key){
                case ENTER:
                    t = buttoncmds + focus;
                    pressed = focus;
                    message(buttoncmds[pressed].comment);
                    if(focus_old == -2){ /* ��һ���ƶ����� */
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
                default:    /* ���ΰ������ô��� */
                    break;
            }
        }
        if(focus_old == -2){ /* ��һ���ƶ����� */
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
    return (focus+1); /* ����Ǵ�1��ʼ�� */
}

/*------------------------------------------------------------------------------------
 * ����:    buttoncmd_set_comment()
 *
 * ����:    ����ĳ�����ע��
 *
 * ����:    buttoncmds  ���������б�
 *          key         Ҫ���ҵİ���
 *          comment     �µ�ע��
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
 * ���ļ�����: keycmd.c
 * 
**==================================================================================*/


