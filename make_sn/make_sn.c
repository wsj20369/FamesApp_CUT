/******************************************************************************************
 * �ļ�:    cut/make_sn.c
 *
 * ����:    CUTע���(SN������)
 *
 * ����:    Jun
 *
 * ����:    2012-10-28
******************************************************************************************/
#define THIS_IS_UNREGISTER
#include "..\src\reg_core.c"

void __task start(void * data)
{
    char user_sn[32];
    char mach_id[32];
    char user_id[32], saved_id[32];
    int i;

    data = data;
    OpenConsole();

    lock_kernel();

    printf("\nCutter System - shujun, 2012.11.27\n");
    printf("\nInput User ID: ");
    scanf("%s", user_id);
    memcpy(saved_id, user_id, sizeof(saved_id));

    printf("\n---------------------------------------\n\n");
    if (decrypt_machine_id(mach_id, user_id) == 0) {
        printf("ID is invalid: %s\n", saved_id);
    } else {
        generate_register_sn(user_sn, mach_id);
        printf("User ID: %s\n", saved_id);
        printf("Mach ID: %02X", 0xff & mach_id[0]);
        for (i = 1; i < MACH_ID_LEN; i++)
            printf("-%02X", 0xff & mach_id[i]);
        printf("\n");
        printf("User SN: %s\n", user_sn);
    }
    printf("\n---------------------------------------\n\n");

    unlock_kernel();

    waitkey(10000);

    ExitApplication();
}

void apical TaskSwitchHook(void)
{
}

void quit(void)
{
    ExitApplication();
}

long get_free_mem(void)
{
    long mem;

    lock_kernel();
    mem = (long)coreleft();
    unlock_kernel();

    return mem;
}

/* ���õ��ô˺���, ��ֻ��������ȥ���뾯���, ��û���� */
int machine_id_get(unsigned char __BUF * buf, int buf_len)
{
    printf("Should not call this function, Please check it\n!");
    buf = buf;
    buf_len = buf_len;

    return fail;
}

/*=========================================================================================
 * 
 * ���ļ�����: cut/make_sn.c
 * 
**=======================================================================================*/


