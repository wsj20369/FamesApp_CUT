/**************************************************************************************
 * 文件:    preload.c
 *
 * 说明:    资源文件的加载
 *
 * 作者:    Jun
 *
 * 时间:    2011-3-3, 2012-10-29
**************************************************************************************/
#define  APP_PRELOAD_C
#include <includes.h>
#include "common.h"

int font16, font48, font24;
int font_big1, font_big2;

static FONTINFO font_asc_16 = {NO, FONT_TYPE_ASCII,   0,  8, 16, 1,  0,  "ASC16",     NULL};
static FONTINFO font_hz_16  = {NO, FONT_TYPE_GB2312,  0, 16, 16, 2,  0,  "HZK16",     NULL};
static FONTINFO font_asc_48 = {NO, FONT_TYPE_ASCII,   0, 24, 48, 3, 32,  "ASC48",     NULL};
static FONTINFO font_asc_24 = {NO, FONT_TYPE_ASCII,   0, 12, 24, 2,  0,  "ASC24",     NULL};
static FONTINFO font_hz_24  = {NO, FONT_TYPE_GB2312,  0, 24, 24, 3, 15*94, "HZK24",   NULL};

static FONTINFO font_dec_40x28  = {NO, FONT_TYPE_ASCII, 0, 28, 40,  4, 0x30, "dec40x28.me", NULL};
static FONTINFO font_dec_76x64  = {NO, FONT_TYPE_ASCII, 0, 64, 76,  8, 0x30, "dec76x64.me", NULL};
static FONTINFO font_dec_89x77  = {NO, FONT_TYPE_ASCII, 0, 77, 89, 10, 0x30, "dec89x77.me", NULL};
static FONTINFO font_dec_60x51  = {NO, FONT_TYPE_ASCII, 0, 51, 60,  7, 0x30, "dec60x51.me", NULL};
static FONTINFO font_dec_87x64  = {NO, FONT_TYPE_ASCII, 0, 64, 87,  8, 0x30, "dec87x64.me", NULL};
static FONTINFO font_dec_87x64b = {NO, FONT_TYPE_ASCII, 0, 64, 87,  8, 0x30, "dec8764b.me", NULL};
static FONTINFO font_dec_60x45  = {NO, FONT_TYPE_ASCII, 0, 45, 60,  6, 0x30, "dec60x45.me", NULL};
static FONTINFO font_dec_60x45b = {NO, FONT_TYPE_ASCII, 0, 45, 60,  6, 0x30, "dec6045b.me", NULL};

static int font40x28, font76x64, font89x77, font60x51;
static int font87x64, font87x64b, font60x45, font60x45b;

void load_fonts(void) 
{
    load_font(&font_asc_16);
    load_font(&font_hz_16);
    load_font(&font_asc_48);
    load_font(&font_hz_24);
    load_font(&font_asc_24);

    font16     = register_font(&font_hz_16, &font_asc_16);
    font48     = register_font(NULL, &font_asc_48);
    font24     = register_font(&font_hz_24, &font_asc_24);

    load_font(&font_dec_40x28);
    load_font(&font_dec_76x64);     /* CourierNew */
    load_font(&font_dec_89x77);     /* CourierNew */
    load_font(&font_dec_60x51);     /* CourierNew */
    load_font(&font_dec_60x45);     /* Batang,+56 */
    load_font(&font_dec_60x45b);    /* Batang,+56,bold */
    load_font(&font_dec_87x64);     /* Batang,+80 */
    load_font(&font_dec_87x64b);    /* Batang,+80,bold */

    font40x28  = register_font(NULL, &font_dec_40x28);
    font76x64  = register_font(NULL, &font_dec_76x64);
    font89x77  = register_font(NULL, &font_dec_89x77);
    font60x51  = register_font(NULL, &font_dec_60x51);
    font60x45  = register_font(NULL, &font_dec_60x45);
    font60x45b = register_font(NULL, &font_dec_60x45b);
    font87x64  = register_font(NULL, &font_dec_87x64);
    font87x64b = register_font(NULL, &font_dec_87x64b);

    font_big1  = font87x64b;
    font_big2  = font60x45b;
}

BMPINFO welcome_picture, icon, company_bmp;
static char welcome_filename[64]  = "welcome.bmp";
static char icon_filename[64]     = "icon.bmp";
static char company_bmp_fname[64] = "company.bmp";

void load_bmps(void)
{
    load_string(welcome_filename,  sizeof(welcome_filename),  "welcome_picture");
    load_string(icon_filename,     sizeof(icon_filename),     "icon_picture");
    load_string(company_bmp_fname, sizeof(company_bmp_fname), "company_picture");

    InitBMPINFO(&icon);
    LoadBmp(&icon, icon_filename);
    
    InitBMPINFO(&company_bmp);
    LoadBmp(&company_bmp, company_bmp_fname);

    InitBMPINFO(&welcome_picture);
    LoadBmp(&welcome_picture, welcome_filename);
}

void early_loads(void)
{
    load_fonts();
    load_bmps();
}

int get_font_big1(int id)
{
    int big1;

    switch (id) {
        default:
        case 0:
            big1 = font87x64;
            break;
        case 1:
            big1 = font87x64b;
            break;
        case 2:
            big1 = font89x77;
            break;
        case 3:
            big1 = font76x64;
            break;
        case 4:
            big1 = font60x51;
            break;
    }

    return big1;
}

int get_font_big2(int id)
{
    int big2;

    switch (id) {
        default:
        case 0:
            big2 = font60x45;
            break;
        case 1:
            big2 = font60x45b;
            break;
        case 2:
            big2 = font60x51;
            break;
        case 3:
            big2 = font40x28;
            break;
        case 4:
            big2 = font48;
            break;
    }

    return big2;
}


/*=====================================================================================
 * 
 * 本文件结束: preload.c
 * 
**===================================================================================*/

