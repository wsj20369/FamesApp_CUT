/*************************************************************************************
 * �ļ�:    language.c
 *
 * ����:    �����л�
 *
 * ����:    Jun
 *
 * ʱ��:    2011-4-21
*************************************************************************************/
#define  FAMES_LANGUAGE_C
#include <includes.h>

/*------------------------------------------------------------------------------------
 * 
 *          �ļ���
 * 
**----------------------------------------------------------------------------------*/
static char * zh_filename = "zh.txt";   /* ��������   */
static char * en_filename = "en.txt";   /* Ӣ��       */

static int fd = -1;

static int ___language = 0;             /* ��ǰ���õ����� */

/*------------------------------------------------------------------------------------
 * ����:    load_string()
 *
 * ����:    ����һ���ַ���
**----------------------------------------------------------------------------------*/
void load_string(char * buf, int buf_size, char * tag)
{
    char ___buf[512], ch, * str;
    int i, size;

    FamesAssert(buf);
    FamesAssert(buf_size > 2); /* ����Ӧ����һ�����ְ� */
    FamesAssert(tag);

    if(!buf || !tag || buf_size <= 2)
        return;

    if(fd < 0)
        return;

    lock_kernel();

    lseek(fd, 0L, SEEK_SET); /* �Ƶ��ļ�ͷ */

    for(;;){ /* ɨ�������ļ� */
        for(;;){    /* ���������ܵĻس����з� */
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
        } else { /* ��û���ҵ����з�, �п���������̫����, ��Ҫ�ҵ�������β! */
            ___buf[i-1] = 0;
            for(;;){
                if(read(fd, &ch, 1) != 1)
                    break;
                if(ch == '\n' || ch == '\r')
                    break;
            }
        }
        /* ������, ___buf���Ѿ�������һ������, ������Ҫ�ҵ��Ǹ��Ⱥ�(=) */
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
        /* strָ���˵Ⱥ�֮������ݻ�NULL(δ�ҵ�'=') */
        if(!STRICMP(___buf, tag)){ /* �ҵ���tag */
            if(str){
                size = STRLEN(str)+1;
                if(size > buf_size)
                    size = buf_size;
                str[size-1] = 0;
                MEMCPY(buf, str, size);
            }
            goto out;
        }
        /* ����Ҫ�ҵĲ�����һ��, ��ô����ɨ����ȥ... */
    }

out:
    unlock_kernel();
}

/*------------------------------------------------------------------------------------
 * ����:    pick_string()
 *
 * ����:    ��ѡһ���ַ���
 *
 * ����:    zh_str  �����ַ���
 *          en_str  Ӣ���ַ���
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
 * ����:    language_initialize()
 *
 * ����:    �����л���ʼ��
 *
 * ˵��:    language_index = 0 �����������
 *          language_index = 1 ����Ӣ��
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
 * ���ļ�����: slc/tmp_cfg.c
 * 
**==================================================================================*/


