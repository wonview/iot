#ifndef _MLME_TASK_H
#define _MLME_TASK_H

#include <host_apis.h>

#if(MLME_TASK==1)
#define MBOX_MLME_TASK       g_mlme_task_info[0].qevt

extern s32 mlme_host_event_handler_(struct cfg_host_event *pPktInfo);

void mlme_task( void *args );
s32 mlme_init(void);
void mlme_sta_mode_init(void);

#endif

#define NUM_AP_INFO     (30)
extern void sta_mode_ap_list_handler (void *data,struct ssv6xxx_ieee80211_bss *ap_list);
void mlme_sta_mode_deinit(void);

#endif //_MLME_TASK_H
