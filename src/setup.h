/******************************************************************************************
 * �ļ�:    setup.h
 *
 * ����:    ��������
 *
 * ����:    Jun
 *
 * ʱ��:    2013-1-31
 *
 * ˵��:    ��ͷ�ļ����������Ͳ�������ͷ�ļ�, ����
 *          ��ͨ����(�������),
 *          ���ٱ�,
 *          ����������,
 *          �߼�����(�����û�)
******************************************************************************************/
#ifndef APP_SETUP_H
#define APP_SETUP_H

/*-----------------------------------------------------------------------------------------
 * 
 *      �����������ṹ
 * 
**---------------------------------------------------------------------------------------*/

typedef struct __driver_param {
    int id;
    char * name;
    char * comment;
} driver_param_t;

typedef struct __driver_setup_desc {
    char * driver_name;
    char * description;
    int    params_nr;
    driver_param_t * params_table;
    BOOL (*get_value)(int index, char * buf, int buf_len);
    BOOL (*set_value)(int index, char * buf, int buf_len, BOOL active_now);
    BOOL (*is_writable)(int index);
} driver_setup_desc_t;


/*-----------------------------------------------------------------------------------------
 * 
 *      ��������������
 * 
**---------------------------------------------------------------------------------------*/
gui_widget * init_setup_screen(void);
void enter_setup(void);

char * get_driver_types_name(int type);


/* ��ͨ���� */
void init_generic_param_dlg(void);
BOOL do_setup_generic_param(void);

/* �߼����� */
void init_super_param_dlg(void);
BOOL do_setup_super_param(void);

/* ���ٱ� */
void init_spdlmt_setup_dlg(void);
void do_setup_spdlmt_tbl(void);

/* ���������� */
void init_driver_setup_dlg(void);
void do_setup_driver_params(void);

/* �������� */
INT16S get_speed_limit(INT16S cutlen);



#endif /* #ifndef APP_SETUP_H */

/*=========================================================================================
 * 
 * ���ļ�����: setup.h
 * 
**=======================================================================================*/


