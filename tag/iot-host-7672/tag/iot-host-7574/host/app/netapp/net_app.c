#include <config.h>
#include <msgevt.h>
#include <log.h>
#include "net_app.h"
#include <os_wrapper.h>


#define NET_APP_MAX_ARGS            10
#define NET_APP_ARGS_SIZE           (sizeof(s8 *)*NET_APP_MAX_ARGS)
#define NET_APP_BUF_SIZE            128


extern void net_app_ping(s32, s8 **);
extern void net_app_dhcp(s32, s8 **);
//#ifndef __SSV_UNIX_SIM__
extern void net_app_ttcp(s32, s8 **);
extern void net_app_iperf3(s32, s8 **);

//#endif


static struct task_info_st sg_net_app_task[] = 
{
    { "net_task0",  (OsMsgQ)0, 1, OS_NETAPP_TASK0_PRIO, 64, NULL, net_app_task    }, 
    { "net_task1",  (OsMsgQ)0, 1, OS_NETAPP_TASK1_PRIO, 64, NULL, net_app_task    },   
    { "net_task2",  (OsMsgQ)0, 1, OS_NETAPP_TASK2_PRIO, 64, NULL, net_app_task    },
    //{ "net_task3",  (OsMsgQ)0, 1, OS_TASK_PRIO2, 64, NULL, net_app_task    },
};








extern u32 g_wsimp_flags;


static struct net_app_info_st sg_net_app_info[] =
{
    { "ping",   net_app_ping },
//    { "dhcp",   net_app_dhcp },
//#ifndef __SSV_UNIX_SIM__
    { "ttcp",   net_app_ttcp },
//#endif
    { "iperf3",  net_app_iperf3 },

};





void net_app_task(void *args)
{
    s32 argc, res, pid=(s32)args;
    MsgEvent *msg_evt;
    OsMsgQ mbox;
    s8 **argv;
    struct net_app_args_st *app_args;
    net_app_func app_func_t;

    /* Get app_info of this task */
    app_args = (net_app_args *)sg_net_app_task[pid].args;
    
    while(1)
    {
        mbox = sg_net_app_task[pid].qevt;
        res = msg_evt_fetch(mbox, &msg_evt);
        ASSERT(res==OS_SUCCESS);

        if (msg_evt->MsgType != MEVT_NET_APP_REQ) {
            msg_evt_free(msg_evt);
            argv[0] = (void *)0;
            continue;
        }
        app_func_t = (net_app_func)msg_evt->MsgData;
        argc = (s32)msg_evt->MsgData1;
        argv = (s8 **)msg_evt->MsgData2;
        msg_evt_free(msg_evt);
        if (app_func_t == NULL)
            continue;
        app_func_t(argc, argv);

        /**
                * argv[0]  means the task is in the idle state. So once
                * the job has completed, we shall set it to 0 to indicate
                * idle state. net_app_run() checks this value to decide
                * if the task is available or not.
                */
        app_args->app_info = NULL;
        argv[0] = (void *)0;
    }

}
    


s32 net_app_run(s32 argc, s8 *argv[])
{
    struct net_app_args_st *app_args;
    MsgEvent *msg_evt;
    s8 **sargv, *arg_pos;
    s32 i, j, k, len;

    /* Check to see if the net app has been registered */
    len = sizeof(sg_net_app_info)/sizeof(net_app_info);
    for(j=0; j<len; j++) {
        if (strcmp(argv[0], 
            sg_net_app_info[j].app_name)==0) 
            break;
    }
    if (j >= len)
        return -1;

    /* Allocate an idle net_app task */
    len = sizeof(sg_net_app_task)/sizeof(task_info);
    for(i=0; i<len; i++) {
        app_args = (net_app_args *)sg_net_app_task[i].args;
        sargv = (s8 **)app_args->cmd_buf;
        arg_pos = (s8 *)app_args->cmd_buf;
        //if (sargv[0] == NULL)
        if (app_args->app_info == NULL)
            break;
    }
    if (i >= len)
        return -1;

    for(k=0, arg_pos+=NET_APP_ARGS_SIZE; k<argc; k++) {
        len = strlen(argv[k]);
        memcpy(arg_pos, argv[k], len);
        sargv[k] = arg_pos;
        arg_pos += len;
        *arg_pos++ = 0x00;
    }
    app_args->app_info = &sg_net_app_info[j];
    msg_evt = msg_evt_alloc();
    ASSERT(msg_evt);  
    msg_evt->MsgType = MEVT_NET_APP_REQ;
    msg_evt->MsgData  = (u32)sg_net_app_info[j].app_func;
    msg_evt->MsgData1 = (u32)argc;
    msg_evt->MsgData2 = (u32)sargv;
    msg_evt_post(sg_net_app_task[i].qevt, msg_evt);
    return 0;
}



s32 net_app_show(void)
{
    struct net_app_args_st *app_args;
    u32 i;

    LOG_PRINTF("\r\n");
    LOG_PRINTF("  NetAppTask      status      AppName\r\n");
    LOG_PRINTF("  ----------      ------      ---------\r\n");
    for(i=0; i<sizeof(sg_net_app_task)/sizeof(task_info); i++) {
        app_args = (net_app_args *)sg_net_app_task[i].args;
        LOG_PRINTF("  %-10s      %-6s      %-16s\r\n",
            sg_net_app_task[i].task_name,
            ((app_args->app_info==NULL)? "idle": "run"),
            ((app_args->app_info==NULL)? "NULL": app_args->app_info->app_name)
        );
    }
    return 0;
}


s32 net_app_init(void)
{
    struct net_app_args_st *app_args;
    OsMsgQ *msgq;
    u32 i, qsize, size;

    for(i=0; i<sizeof(sg_net_app_task)/sizeof(task_info); i++) {
        /* Allocate memory for net_app_args */
        sg_net_app_task[i].args = (void *)MALLOC(sizeof(net_app_args));
        ASSERT(sg_net_app_task[i].args);

        /* Allocate cmdline buffer */
        app_args = (struct net_app_args_st *)sg_net_app_task[i].args;
        size = NET_APP_BUF_SIZE + NET_APP_ARGS_SIZE;
        app_args->cmd_buf = (s8 *)MALLOC(size);
        ASSERT(app_args->cmd_buf != NULL);
        ssv6xxx_memset((void *)app_args->cmd_buf, 0, size);

        /* Init app_info */
        app_args->app_info = NULL;
        
        msgq = &sg_net_app_task[i].qevt;
        qsize = (s32)sg_net_app_task[i].qlength;
        if (OS_MsgQCreate(msgq, qsize) != OS_SUCCESS)
            return OS_FAILED;

        OS_TaskCreate(net_app_task, 
    		sg_net_app_task[i].task_name,
    		sg_net_app_task[i].stack_size<<4, 
    		(void *)i, 
    		sg_net_app_task[i].prio, 
    		NULL);
    }
    return OS_SUCCESS;    

}


