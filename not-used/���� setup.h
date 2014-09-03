/******************************************************************************************
 * �ļ�:    setup.h
 *
 * ����:    ��������
 *
 * ����:    Jun
 *
 * ʱ��:    2012-12-3
******************************************************************************************/
#ifndef APP_SETUP_H
#define APP_SETUP_H

/*-----------------------------------------------------------------------------------------
 * 
 *      �����ṹ
 * 
**---------------------------------------------------------------------------------------*/
enum param_class_enum {
    PARAM_CLASS_NONE,
    PARAM_CLASS_NORMAL,     /* ��ͨ���� */
    PARAM_CLASS_SPDLIMIT,   /* ���ٱ� */
    PARAM_CLASS_DRIVER,     /* ���������� */
    PARAM_CLASS_SUPER,      /* �߼����� */
    
    PARAM_CLASS_NR,
};

#define PARAM_CLASS_DEFAULT PARAM_CLASS_NORMAL /* Ĭ�Ͻ�����ͨ���� */

typedef struct __param_struct {
    int id;
    char * name;
    char * comment;
} param_struct;

typedef struct __param_class_type {
    char * class_name;
    char * description;
    int    param_nr;
    param_struct * param_table;
    BOOL (*get_password)(char * buf, int buf_len);
    BOOL (*get_value)(int index, char * buf, int buf_len);
    BOOL (*set_value)(int index, char * buf, int buf_len, BOOL active_now);
    void (*init)(void);
} param_class_type;


/*-----------------------------------------------------------------------------------------
 * 
 *      ��������������
 * 
**---------------------------------------------------------------------------------------*/
gui_widget * init_setup_screen(void);
void enter_setup(void);


#endif /* #ifndef APP_SETUP_H */

/*=========================================================================================
 * 
 * ���ļ�����: setup.h
 * 
**=======================================================================================*/


