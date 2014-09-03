/*************************************************************************************
 * 文件:    cfg_defs.h
 *
 * 作者:    Jun
 *
 * 时间:    2012-10-25
 *
 * 描述:    系统定义文件
*************************************************************************************/
#ifndef FAMES__CFG_DEFS_H
#define FAMES__CFG_DEFS_H

/*
 * 控制相关的设定
*/
#define CONFIG_SPDLMT_SIZE              32      /* 限速表项目, 不应小于16 */
#define CONFIG_SPDLMT_MAXIMUM           999     /* 限速最大值 */

/*
 * 画面相关的设定
*/
#define MAIN_ORDER_ITEM_HEIGHT          42      /* 主画面中, 一笔订单显示的高度 */


/*
 * 注册相关
*/

#define CONFIG_REG_PROMPT_TIME1         10UL    /* 10秒钟之后, 第一组提醒(仅一次) */
#define CONFIG_REG_PROMPT_TIME2         21606UL /* 自启动6小时之后 [+6秒] */
#define CONFIG_REG_PROMPT_TIMEx         600UL   /* 下次提醒间隔时间: 10分钟 */


/*
 * 计算相关
*/



/*
 * 字体, 此定义要与函数load_fonts()一致
*/
#define CONFIG_FONT_SYSTEM              0
#define CONFIG_FONT_16                  1
#define CONFIG_FONT_48                  2
#define CONFIG_FONT_24                  3



#endif /* #ifndef FAMES__CFG_DEFS_H */

/*====================================================================================
 * 
 * 本文件结束: cfg_defs.h
 * 
**==================================================================================*/

