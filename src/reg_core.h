/******************************************************************************************
 * 文件:    reg_core.h
 *
 * 描述:    Register Service Core
 *
 * 作者:    Jun
 *
 * 时间:    2012-8-24
******************************************************************************************/
#ifndef FAEMS_REG_CORE_H
#define FAEMS_REG_CORE_H

/*-----------------------------------------------------------------------------------------
 * 
 *      Register Service Definations
 * 
**---------------------------------------------------------------------------------------*/
enum register_service_request_cmds
{
    RS_REQUEST_NONE,
    RS_REQUEST_REGISTER_CALLBACK,
    RS_REQUEST_DEREGISTER_CALLBACK,
    RS_REQUEST_GET_ID,
    RS_REQUEST_GET_SN,
    RS_REQUEST_SN,
    RS_REQUEST_CHECK,
    RS_REQUEST_EVENT, /* RegisterService发出的事件 */
    RS_REQUEST_GET_OPTIONS,
    RS_REQUEST_SET_OPTIONS,
    RS_REQUEST_EXTENDED_CMD,
    NUMBER_OF_RS_REQUEST_CMDS,
};

#define ID_LEN        20    /* ID 的实际长度 */
#define SN_LEN        23    /* SN 的实际长度 */

#define ID_MAX_LEN    32
#define SN_MAX_LEN    32

/* Register Service回调函数 */
typedef int (*register_callback_fn)(int cmd, void * data);

/*-----------------------------------------------------------------------------------------
 * 
 *      函数声明
 * 
**---------------------------------------------------------------------------------------*/
int register_service_init(void);

/* define this as macro to insert the code directly to caller */
#define register_service_request(cmd, data) \
        do { \
            void register_service_wakeup(void); \
            extern int    ____rs__requests_cmd[]; \
            extern void * ____rs__requests_data[]; \
            if (cmd > RS_REQUEST_NONE && cmd < NUMBER_OF_RS_REQUEST_CMDS) { \
                lock_kernel(); \
                ____rs__requests_cmd[cmd] = 1; \
                ____rs__requests_data[cmd] = data; \
                unlock_kernel(); \
                /* register_service_wakeup(); */ \
            } \
        } while(0)


#endif /* #ifndef FAEMS_REG_CORE_H */

/*=========================================================================================
 * 
 * 本文件结束: reg_core.h
 * 
**=======================================================================================*/


