/******************************************************************************************
 * 文件:    show_plt.c
 *
 * 说明:    显示调色板
 *
 * 作者:    Jun
 *
 * 时间:    2011-4-6
******************************************************************************************/
#define  SLC_SHOW_PALETTE_C
#include <includes.h>
#include "common.h"

gui_widget * palette_screen = NULL;
gui_window_t * palette_window = NULL;

void show_palette_init(void)
{
    int i, x, y, w, h, __x;
    char ___s[6];
    gui_widget * palettes[256];

    palette_screen = gui_create_widget(GUI_WIDGET_FORM, 1, 1, 1021, 765, 0, 0, 0, FORM_STYLE_XP_BORDER);
    if(!palette_screen)
        goto some_error;
    gui_widget_link(NULL, palette_screen);      /* 设置主界面背景 */
    gui_form_init_private(palette_screen, 8);

    __x = 12;
    x = __x;
    y = -32;
    w = 60;
    h = 44;

    for(i=0; i<256; i++){
        if(!(i%16)){
            y += (h+2);
            x  = __x;
        }
        palettes[i] = gui_create_widget(GUI_WIDGET_BUTTON, x, y, w, h, 0, 0, 0, 0);
        if(!palettes[i])
            goto some_error;
        gui_widget_link(palette_screen, palettes[i]);
        gui_set_widget_color(palettes[i], 255-i);
        gui_set_widget_bkcolor(palettes[i], i);
        gui_button_init_private(palettes[i], 8);
        sprintf(___s, "%d", i);
        gui_button_set_caption(palettes[i], ___s);
        x += (w+2);
    }

    palette_window = gui_create_window(palette_screen);

    return;

some_error:
    sys_print("show_palette_init(): failed to create widgets for palette screen!\n");
    ExitApplication();
}


/*=========================================================================================
 * 
 * 本文件结束: show_plt.c
 * 
**=======================================================================================*/


