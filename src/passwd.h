/******************************************************************************************
 * �ļ�:    cut/passwd.h
 *
 * ����:    ��������(���)
 *
 * ����:    Jun
 *
 * ����:    2011-3-31
******************************************************************************************/
#ifndef CUT_PASSWORD_H
#define CUT_PASSWORD_H

/*-----------------------------------------------------------------------------------------
 * 
 *      �򵥼��������(������)
 * 
**---------------------------------------------------------------------------------------*/
#define  __encrypt(x) (((INT32U)x^0xC31A76E9L)+0xFF5AA5FFL)
#define  __decrypt(x) (((INT32U)x-0xFF5AA5FFL)^0xC31A76E9L)

/*-----------------------------------------------------------------------------------------
 * 
 *      SLC��ʹ�õ�����
 * 
**---------------------------------------------------------------------------------------*/

#define  PASSWORD_PARAM   __encrypt(1937)  /* ��ͨ�������� */
#define  PASSWORD_SUPER   __encrypt(5588)  /* ��������     */
#define  PASSWORD_DRIVER  __encrypt(1949)  /* �������������� */


/*-----------------------------------------------------------------------------------------
 * 
 *      ���������ָ��
 * 
**---------------------------------------------------------------------------------------*/
#ifndef CUT_PASSWORD_C

extern gui_widget * const password_dialog;

#endif

/*-----------------------------------------------------------------------------------------
 * 
 *      ��������
 * 
**---------------------------------------------------------------------------------------*/
BOOL check_password_dialog(int x, int y, INT32U password);
void init_password_dialog(void);


#endif /* #ifndef CUT_PASSWORD_H */

/*=========================================================================================
 * 
 * ���ļ�����: cut/passwd.h
 * 
**=======================================================================================*/

