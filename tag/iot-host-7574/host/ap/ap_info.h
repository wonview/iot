

#ifndef AP_INFO_H
#define AP_INFO_H

#include "common/ieee80211.h"
#include "common/bitops.h"
#include <pbuf.h>
#include "common/ether.h"
#include "ap_config.h"
#include "common/defs.h"
#include "hw_config.h"
#include "neighbor_ap_list.h"
#include "ap_def.h"
#include <os_wrapper.h>
#include "ap_tx.h"
#include "ap.h"
#include <cmd_def.h>
#include "ap_config.h"







#define AID_WORDS ((AP_MAX_STA + 31) / 32)
#define bcn_cnt 2


//#define AID_WORDS (sizeof(unsigned long) * BITS_TO_LONGS(IEEE80211_MAX_AID + 1))



enum beacon_tim_types {

	tim_type_none,			//None
	tim_type_2bytes,		//AID 	0~15
	tim_type_top_half,		//AID 	0~7
	tim_type_bottom_half,	//AID	8~15
};



//IEEE80211_CHAN_FLAGS_NONE			= 0,
enum ieee80211_channel_flags {	
	IEEE80211_CHAN_FLAGS_DISABLED		= BIT(0),
	IEEE80211_CHAN_FLAGS_PASSIVE_SCAN	= BIT(1),
	IEEE80211_CHAN_FLAGS_NO_IBSS		= BIT(2),
	IEEE80211_CHAN_FLAGS_RADAR			= BIT(3),
	IEEE80211_CHAN_FLAGS_NO_HT40PLUS	= BIT(4),
	IEEE80211_CHAN_FLAGS_NO_HT40MINUS	= BIT(5),
};


#define IEEE80211_CHAN_NO_HT40 \
	(IEEE80211_CHAN_NO_HT40PLUS | IEEE80211_CHAN_NO_HT40MINUS)



typedef enum ieee80211_security_policy {
	SECURITY_PLAINTEXT = 0,
	SECURITY_STATIC_WEP = 1,
	SECURITY_IEEE_802_1X = 2,
	SECURITY_WPA_PSK = 3,			//PSK
	SECURITY_WPA = 4				//Enterprise  WPA-RADIUS/EAP
} secpolicy;



STRUCT_PACKED struct ieee80211_channel_data {
	u8 chan;
	short freq;
	u8 max_tx_power;
	int flag;	
};


//struct os_timer;
typedef enum{
	AP_STATE_INIT			,
	AP_STATE_IDLE			,	//Off
	AP_STATE_READY			,	//Init
	//AP_STATE_READY_SET_AP_MODE	,
	AP_STATE_RUNNING		,	//Running
	AP_STATE_PAUSE			,	
}ap_state;



typedef enum{
	AP_READY_SET_AP_MODE	,
	AP_READY_GET_HW_INFO	,
	AP_READY_SET_INFO		,	
}ap_ready_state;


struct ssv6xxx_beacon_info {
	u32 pubf_addr;
	u16 len;
	u8 tim_offset;
	u8 tim_cnt;	
};


typedef struct ApInfo{
	//--------------

	//ap_state state;
	//APCmdCb cb;


	u8* pOSMgmtframe;		//pMgmtPkt




	u8* pMgmtPkt;

	
	u8 eCurrentApMode;
	bool b80211n;
	u8  nCurrentChannel;
	//struct ieee80211_channel_data stChannelData[AP_MAX_CHANNEL];


	u8 own_addr[ETH_ALEN];	
	
	

	/*
	 * Bitfield for indicating which AIDs are allocated. Only AID values
	 * 1-2007 are used and as such, the bit at index 0 corresponds to AID
	 * 1.
	 */
	
	u32 sta_aid[AID_WORDS];


//--------------------------------------------------------------------------------------

	//u8 sta_aid[sizeof(unsigned long) * BITS_TO_LONGS(IEEE80211_MAX_AID + 1)];
	

	/* yes, this looks ugly, but guarantees that we can later use
	 * bitmap_empty :)
	 * NB: don't touch this bitmap, use sta_info_{set,clear}_tim_bit */

	u8 tim[sizeof(unsigned long) * BITS_TO_LONGS(IEEE80211_MAX_AID + 1)];
	
	
	//

	int dtim_count;
	bool dtim_bc_mc;
//----------------------------------
//station info

	u8 num_sta; /* number of entries in sta_list */

	u8 num_sta_ps;//atomic_t num_sta_ps; /* number of stations in PS mode */	
	u8 num_sta_authorized;/* number of authorized stations */

	/* Number of associated Non-ERP stations (i.e., stations using 802.11b
	* in 802.11g BSS) */
	u8 num_sta_non_erp;
	
	/* Number of associated stations that do not support Short Preamble */
	u8 num_sta_no_short_preamble;

	/* Number of associated stations that do not support Short Slot Time */
	u8 num_sta_no_short_slot_time;

	//secpolicy policy ;


	/* Number of HT associated stations that do not support greenfield */
	int num_sta_ht_no_gf;

	/* Number of associated non-HT stations */
	int num_sta_no_ht;

	/* Number of HT associated stations 20 MHz */
	int num_sta_ht_20mhz;

	
	//----------------------------
	//Beacon
	//-----------------------------

	struct cfg_bcn_info bcn_info;
    
    u8  bcn[AP_MGMT_BEACON_LEN];    

	u8* pBeaconHead;
	u16 nBHeadLen;

	u8 *pBeaconTail;	
	u16 nBTailLen;
	
	u8 *pBeaconTim;



    struct ssv6xxx_beacon_info hw_bcn_info[bcn_cnt]; // record hw bcn info

	//u16 nBLen;
	

	//----------------------------
	//QoS
	//-----------------------------

	u8 parameter_set_count;


	//----------------------------
	//Detect other AP activities
	//----------------------------
	int olbc; /* Overlapping Legacy BSS Condition */
	int olbc_ht;/* Overlapping BSS information */

	u8 num_neighbor_ap;
	struct neighbor_ap_info neighbor_ap_list[AP_NEIGHBOR_AP_TABLE_MAX_SIZE];



	
	//------------------------------
	//PS related
	u16 total_ps_buffered;
	struct ap_tx_desp_head ps_bc_buf;
	
	//Used to store tx descriptor
	struct ap_tx_desp *pApTxData;







	//-----------------------------
	//security
	//-----------------------------

	int auth_algs; /* bitfield of allowed IEEE 802.11 authentication
			* algorithms, WPA_AUTH_ALG_{OPEN,SHARED,LEAP} */
	int tkip_countermeasures;


	ssv6xxx_sec_type sec_type;
	u8 password[MAX_PASSWD_LEN+1];






	//-----------------------------
	//Timer
	//-----------------------------	
	//struct os_timer timer[AP_TIMER_DESP_NUM];


	//-----------------------------
	//Configuration
	//----------------------------
	struct ssv6xxx_host_ap_config config;

	

}ApInfo_st;




#endif /* AP_INFO_H */
