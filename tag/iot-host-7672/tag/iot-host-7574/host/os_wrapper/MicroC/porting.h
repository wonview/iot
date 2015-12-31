
#ifndef _OS_PORTING_H_
#define _OS_PORTING_H_

#include "types.h"
#include "os.h"
#include "application.h"

/*============OS parameter setting===================*/
typedef struct OsMsgQ_st
{
    void*       qpool;
    OS_EVENT*   msssageQ;
}OsMessgQ;
typedef void (*OsTask)(void *);
typedef void *              OsTaskHandle;
typedef void *              OsTimer;
typedef void (*OsTimerHandler)( OsTimer xTimer );

typedef OS_EVENT*           OsMutex;
typedef OS_EVENT*           OsSemaphore;
typedef OsMessgQ*           OsMsgQ;

#define OS_TASK_ENTER_CRITICAL()        OS_CPU_SR cpu_sr; \
                                        OS_ENTER_CRITICAL()

#define OS_TASK_EXIT_CRITICAL()         OS_EXIT_CRITICAL()


#define TMR_TASK_PRIORITY  19
#define CMD_ENG_PRIORITY   20     
#define WIFI_TX_PRIORITY   21     
#define WIFI_RX_PRIORITY   22     
#define TCPIP_PRIORITY     23    
#define DHCPD_PRIORITY     24
#define NETAPP_PRIORITY    25     
#define NETAPP_PRIORITY_1  NETAPP_PRIORITY+1       
#define NETAPP_PRIORITY_2  NETAPP_PRIORITY+2
#define MLME_TASK_PRIORITY NETAPP_PRIORITY_2+1
#define TASK_END_PRIO      NETAPP_PRIORITY_2+2       

#if 0
//iComm wifii
#define TMR_TASK_PRIORITY        18
#define NETAPP_PRIORITY         19
#define NETAPP_PRIORITY_1         20
#define NETAPP_PRIORITY_2         21
#define DHCPD_PRIORITY         22
#define TCPIP_PRIORITY           23
#define CMD_ENG_PRIORITY         24
#define WIFI_TX_PRIORITY         25
#define WIFI_RX_PRIORITY         26
#define TASK_END_PRIO            27
#endif
#define TICK_RATE_MS (1000/OS_TICKS_PER_SEC)
#define TASK_IDLE_STK_SIZE OS_TASK_IDLE_STK_SIZE


/*============Console setting===================*/
#define hal_print print_string
//#define PRINTF print_string
//#define stdout NULL
#define FFLUSH(x)
#define hal_putchar(ch) uart0_data_send(ch,1)
extern void uart0_data_send(u8 data, u8 wait);
extern INT32S uart0_data_get(u8 *data, u8 wait);
//cli getchar
extern INT32S uart0_data_get(u8 *data, u8 wait);
extern u8 hal_getchar(void);


/*============Compiler setting===================*/
#define ARM_ADS
#define PACK( __Declaration__ ) __packed __Declaration__;
#undef STRUCT_PACKED
#define STRUCT_PACKED __packed
#define UNION_PACKED __packed
//LWIP PACK Definition
#define PACK_STRUCT_BEGIN __packed
#define PACK_STRUCT_FIELD(x) __packed x
#define inline __inline


/*============SSV-DRV setting===================*/
#define INTERFACE "spi"
#define	CONFIG_RX_POLL      0
#define SDRV_INCLUDE_SPI    1

#endif
