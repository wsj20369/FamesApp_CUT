/*************************************************************************************
 * 文件:    language.c
 *
 * 描述:    语言切换
 *
 * 作者:    Jun
 *
 * 时间:    2011-4-21
*************************************************************************************/
#define  FAMES_LANGUAGE_C
#include <includes.h>

/*------------------------------------------------------------------------------------
 * 
 *          文件名
 * 
**----------------------------------------------------------------------------------*/
static char * zh_filename = "zh.txt";   /* 简体中文   */
static char * en_filename = "en.txt";   /* 英文       */

static int fd = -1;

static int ___language = 0;             /* 当前所用的语言 */

/*------------------------------------------------------------------------------------
 * 函数:    load_string()
 *
 * 描述:    加载一个字符串
**----------------------------------------------------------------------------------*/
void load_string(char * buf, int buf_size, char * tag)
{
    char ___buf[512], ch, * str;
    int i, size;

    FamesAssert(buf);
    FamesAssert(buf_size > 2); /* 最少应该有一个汉字吧 */
    FamesAssert(tag);

    if(!buf || !tag || buf_size <= 2)
        return;

    if(fd < 0)
        return;

    lock_kernel();

    lseek(fd, 0L, SEEK_SET); /* 移到文件头 */

    for(;;){ /* 扫描整个文件 */
        for(;;){    /* 先跳过可能的回车换行符 */
            if(read(fd, &ch, 1) != 1)
                goto out;
            if(ch != '\n' && ch != '\r')
                break;
        }
        ___buf[0] = ch;
        for(i = 1; i < sizeof(___buf); i++){
            if(read(fd, &ch, 1) != 1)
                break;
            ___buf[i] = ch;
            if(ch == '\n' || ch == '\r')
                break;
        }
        if(i < sizeof(___buf)){
            ___buf[i] = 0;
        } else { /* 还没有找到换行符, 有可能是这行太长了, 需要找到它的行尾! */
            ___buf[i-1] = 0;
            for(;;){
                if(read(fd, &ch, 1) != 1)
                    break;
                if(ch == '\n' || ch == '\r')
                    break;
            }
        }
        /* 到这里, ___buf中已经包含了一行数据, 现在需要找到那个等号(=) */
        str = NULL;
        for(i = 0; i < sizeof(___buf); i++){
            if(___buf[i] == '='){
                ___buf[i] = 0;
                str = &___buf[i+1];
                break;
            }
            if(___buf[i] == '\0')
                break;
        }
        /* str指向了等号之后的内容或NULL(未找到'=') */
        if(!STRICMP(___buf, tag)){ /* 找到了tag */
            if(str){
                size = STRLEN(str)+1;
                if(size > buf_size)
                    size = buf_size;
                str[size-1] = 0;
                MEMCPY(buf, str, size);
            }
            goto out;
        }
        /* 我们要找的不是这一行, 那么继续扫描下去... */
    }

out:
    unlock_kernel();
}

/*------------------------------------------------------------------------------------
 * 函数:    pick_string()
 *
 * 描述:    挑选一个字符串
 *
 * 参数:    zh_str  中文字符串
 *          en_str  英文字符串
**----------------------------------------------------------------------------------*/
void * pick_string(void * zh_str, void * en_str)
{
    switch(___language){
        case 0:
            return zh_str;
        case 1:
            return en_str;
        default:
            return en_str;
    }
}

/*------------------------------------------------------------------------------------
 * 函数:    language_initialize()
 *
 * 描述:    语言切换初始化
 *
 * 说明:    language_index = 0 代表简体中文
 *          language_index = 1 代表英文
**----------------------------------------------------------------------------------*/
void language_onexit(void)
{
    if(fd >= 0)
        close(fd);
    fd = -1;
}

void language_initialize(int language_index)
{
    char * filename;

    ___language = language_index;
    
    switch(language_index){
        case 0:
            filename = zh_filename;
            break;
        case 1:
            filename = en_filename;
            break;
        default:
            filename = en_filename;
            break;
    }
    
    lock_kernel();
    fd = open(filename, O_RDONLY|O_BINARY);
    if(fd >= 0){
        RegisterOnExit(language_onexit);
    }
    unlock_kernel();
}


/*====================================================================================
 * 
 * 本文件结束: slc/tmp_cfg.c
 * 
**==================================================================================*/


