#include <config.h>

typedef struct ssv_cabrio_reg_st {
    u32 address;
    u32 data;
} ssv_cabrio_reg;


static const ssv_cabrio_reg phy_setting[]={
    // {0xce000000, 0x80000016},   // CabrioA/E difference
    {0xce000008, 0x0000006a},
    {0xce00000c, 0x00000064},
    {0xce000010, 0x00007FFF},
    {0xce000014, 0x00000003},
    {0xce000018, 0x0055003C},
    {0xce00001c, 0x00000064},
    {0xce000020, 0x20000000},
    {0xce00002c, 0x00000000},
    {0xce000030, 0x80046072},
    {0xce000034, 0x1f300f6f},
    {0xce000038, 0x660F36D0},
    {0xce00003c, 0x106C0004},
    {0xce000040, 0x01600500},
    {0xce000044, 0x00600008},
    {0xce000048, 0xff000160},
    {0xce00004c, 0x00000840},
    {0xce000060, 0x01000405},
    {0xce000064, 0x06090813},
    {0xce000068, 0x12070000},
    {0xce00006c, 0x01000405},
    {0xce000070, 0x06090813},
    {0xce000074, 0x12010000},
    {0xce000078, 0x00000000},
    {0xce00007c, 0x10110003},
    {0xce000080, 0x0110000F},
    {0xce000084, 0x00000000},
    {0xce000088, 0x00000000},
    {0xce000094, 0x01012425},
    {0xce000098, 0x01010101},
    {0xce00009c, 0x00000011},
    {0xce0000a0, 0x1fff0000},
    {0xce0000a4, 0x1fff0000},
    {0xce0000a8, 0x1fff0000},
    {0xce0000ac, 0x1fff0000},
    {0xce0000b8, 0x0000fe3e},
    {0xce0000fc, 0xffffffff},
    {0xce000108, 0x0ead04f5},
    {0xce00010c, 0x0fd60080},
    {0xce000110, 0x00000009},
    {0xce0010a4, 0x0000002c},
    {0xce0010b4, 0x00003001},
    {0xce0010d4, 0x00000001},
    {0xce002000, 0x00000044},
    {0xce002004, 0x00040000},
    {0xce002008, 0x20400050},
    {0xce00200c, 0x00003467},
    {0xce002010, 0x00540000},
    {0xce002014, 0x20304015},
    {0xce002018, 0x00390002},
    {0xce00201c, 0x02333567},
    {0xce002020, 0x00350046},
    {0xce002024, 0x00570057},
    {0xce002028, 0x00236700},
    {0xce00202c, 0x000d1746},
    {0xce002030, 0x04061787},
    {0xce002034, 0x07800000},
    {0xce00209c, 0x00C0000A},
    {0xce0020a0, 0x00000000},
    {0xce0023f8, 0x00000000},
    {0xce0023fc, 0x00000001},
    {0xce0030a4, 0x00001901},
    {0xce0030b8, 0x5d08908e},
    {0xce004000, 0x00000044},
    {0xce004004, 0x00750075},
    {0xce004008, 0x00000075},
    {0xce00400c, 0x10000075},
    {0xce004010, 0x3F384905},
    {0xce004014, 0x40182000},
    {0xce004018, 0x28680000},
    {0xce00401c, 0x0C010120},
    {0xce004020, 0x50505050},
    {0xce004024, 0x50000000},
    {0xce004028, 0x50505050},
    {0xce00402c, 0x506070A0},
    {0xce004030, 0xF0000000},
    {0xce004034, 0x00002424},
    {0xce004038, 0x00001420},
    {0xce00409c, 0x0000300A},
    {0xce0040c0, 0x40000280},
    {0xce0040c4, 0x30023002},
    {0xce0040c8, 0x0000003a},
    {0xce004130, 0x40000000},
    {0xce004164, 0x009C007E},
    {0xce004180, 0x00044400},
    {0xce004188, 0x82000000},
    {0xce004190, 0x00001820},
    //{0xce004194, 0x092a092a},
    {0xce004194, 0xFFFFFFFF},
    {0xce004380, 0x00700010},
    {0xce004384, 0x00007575},
    {0xce004388, 0x0001fe3e},
    {0xce00438c, 0x0000fe3e},
    {0xce0043f8, 0x00000001},
    // {0xce0043fc, 0x000104e1},    // CabrioA/E difference
    // {0xce007044, 0x00028080},    // should be set in iq-k registers
    // {0xce0071bc, 0x80808080},    // should be set in iq-k registers
    // {0xce000004, 0x0000017f},    // mode & enable, shoud not be included in default config
    {0xce007000, 0x00000000},
    {0xce007004, 0x00008000},
    {0xce007008, 0x00000000},
    {0xce00700c, 0x00000000},
    {0xce007010, 0x00000000},
    {0xce007014, 0x00000000},
    {0xce007018, 0x00000000},
    {0xce00701c, 0x00000000},
    {0xce007020, 0x00000000},
    {0xce007024, 0x00000000},
    {0xce007028, 0x00000000},
    {0xce00702c, 0x00000000},
    {0xce007030, 0x00000000},
    {0xce007034, 0x00000000},
    {0xce007038, 0x00000000},
    {0xce00703c, 0x00000000},
    {0xce007040, 0x02000200},
    // {0xce007044, 0x00028080},   // CabrioA/E difference
    {0xce007048, 0x00000000},
    {0xce00704c, 0x00000000},
    {0xce007050, 0x00000000},
    {0xce007054, 0x00000000},
    {0xce007058, 0x000028ff},
    {0xce00705c, 0x00000000},
    {0xce007060, 0x00000000},
    {0xce007064, 0x00000000},
    {0xce007068, 0x00000000},
    {0xce00706c, 0x00000202},
    {0xce007070, 0x80ffc200},
    {0xce007074, 0x00000000},
    {0xce007078, 0x00000000},
    {0xce00707c, 0x00000000},
    {0xce007080, 0x00000000},
    {0xce007084, 0x00000000},
    {0xce007088, 0x00000000},
    {0xce00708c, 0x00000000},
    {0xce007090, 0x00000000},
    {0xce007094, 0x00000000},
    {0xce007098, 0x00000000},
    {0xce00709c, 0x00000000},
    {0xce0070a0, 0x00000000},
    {0xce0070a4, 0x00000000},
    {0xce0070a8, 0x00000000},
    {0xce0070ac, 0x00000000},
    {0xce0070b0, 0x00000000},
    {0xce0070b4, 0x00000000},
    {0xce0070b8, 0x00000000},
    {0xce0070bc, 0x00000000},
    {0xce0070c0, 0x00000000},
    {0xce0070c4, 0x00000000},
    {0xce0070c8, 0x00000000},
    {0xce0070cc, 0x00000000},
    {0xce0070d0, 0x00000000},
    {0xce0070d4, 0x00000000},
    {0xce0070d8, 0x00000000},
    {0xce0070dc, 0x00000000},
    {0xce0070e0, 0x00000000},
    {0xce0070e4, 0x00000000},
    {0xce0070e8, 0x00000000},
    {0xce0070ec, 0x00000000},
    {0xce0070f0, 0x00000000},
    {0xce0070f4, 0x00000000},
    {0xce0070f8, 0x00000000},
    {0xce0070fc, 0x00000000},
    {0xce007100, 0x00000000},
    {0xce007104, 0x00000000},
    {0xce007108, 0x00000000},
    {0xce00710c, 0x00000000},
    {0xce007110, 0x00000000},
    {0xce007114, 0x00000000},
    {0xce007118, 0x00000000},
    {0xce00711c, 0x00000000},
    {0xce007120, 0x02000200},
    {0xce007124, 0x02000200},
    {0xce007128, 0x02000200},
    {0xce00712c, 0x02000200},
    {0xce007130, 0x02000200},
    {0xce007134, 0x02000200},
    {0xce007138, 0x02000200},
    {0xce00713c, 0x02000200},
    {0xce007140, 0x02000200},
    {0xce007144, 0x02000200},
    {0xce007148, 0x02000200},
    {0xce00714c, 0x02000200},
    {0xce007150, 0x02000200},
    {0xce007154, 0x02000200},
    {0xce007158, 0x00000000},
    {0xce00715c, 0x00000000},
    {0xce007160, 0x00000000},
    {0xce007164, 0x00000000},
    {0xce007168, 0x00000000},
    {0xce00716c, 0x00000000},
    {0xce007170, 0x00000000},
    {0xce007174, 0x00000000},
    {0xce007178, 0x00000000},
    {0xce00717c, 0x00000000},
    {0xce007180, 0x00000000},
    {0xce007184, 0x00000000},
    {0xce007188, 0x00000000},
    {0xce00718c, 0x00000000},
    {0xce007190, 0x00000000},
    {0xce007194, 0x00000000},
    {0xce007198, 0x00000000},
    {0xce00719c, 0x00000000},
    {0xce0071a0, 0x00000000},
    {0xce0071a4, 0x00000000},
    {0xce0071a8, 0x00000000},
    {0xce0071ac, 0x00000000},
    {0xce0071b0, 0x00000000},
    {0xce0071b4, 0x00000100},
    {0xce0071b8, 0x00000000},
#if (CONFIG_CHIP_ID == SSV6051Z)
    {0xce0071bc, 0x5B606C72},//TX gain
#elif (CONFIG_CHIP_ID == SSV6060P)
    {0xce0071bc, 0x79806C72},//TX gain
#else
    {0xce0071bc, 0x79807980},//TX gain
#endif
    {0xce0071c0, 0x00000000},
    {0xce0071c4, 0x00000000},
    {0xce0071c8, 0x00000000},
    {0xce0071cc, 0x00000000},
    {0xce0071d0, 0x00000000},
    {0xce0071d4, 0x00000000},
    {0xce0071d8, 0x00000000},
    {0xce0071dc, 0x00000000},
    {0xce0071e0, 0x00000000},
    {0xce0071e4, 0x00000000},
    {0xce0071e8, 0x00000000},
    {0xce0071ec, 0x00000000},
    {0xce0071f0, 0x00000000},
    {0xce0071f4, 0x00000000},
    {0xce0071f8, 0x00000000},
    {0xce0071fc, 0x00000000},
#ifdef CONFIG_SSV_CABRIO_E
    /*
    RIFS  on, STBC off     0xce0043fc  0x000104E3
    RIFS  off, STBC on     0xce0043fc  0x000104E5
    RIFS  on, STBC on     0xce0043fc  0x000104E7
    */
    {0xce0043fc, 0x000104e7}, //0x000004e1},
    {0xce007044, 0x00028080}, //0x00038080},
    {0xce000000, 0x80000016}, //0x0000001e},
#endif
#ifdef CONFIG_SSV_CABRIO_A
    {0xce0043fc, 0x000004e1},
    {0xce007044, 0x00038080},
    {0xce000000, 0x0000001e},
#endif
};

static const u32 wifi_tx_gain[]={
    0x79807980,
    0x72797279,
    0x6C726C72,
    0x666C666C,
    0x60666066,
    0x5B605B60,
    0x565B565B,
    0x51565156,
    0x4C514C51,
    0x484C484C,
    0x44484448,
    0x40444044,
    0x3C403C40,
    0x3A3D3A3D,
    0x36393639,
};

static ssv_cabrio_reg asic_rf_setting[]={
//#if SSV_XTAL!=24
    {0xCE010038, 0x0003E07C},   // xtal dependent           this register for SDIO clock
    {0xCE010060, 0x00406000},   // xtal dependent           this register for SDIO clock
    {0xCE01009C, 0x00000024},   // xtal dependent          this register for SDIO clock
    {0xCE0100A0, 0x00EC4CC5},   // xtal dependent          this register for SDIO clock
//#endif

    {0xCE010000, 0x40001000},
    {0xCE010004, 0x00020FC0},

#if SSV_IPD

    //{0xCE010008, 0x002DF69A},
#if (SSV_INTERNAL_LDO == 1)
    {0xCE010008, 0x008B7C1C},
#else
# error "Please redefine SSV_INTERNAL_LDO for IPD!!"
#endif

    {0xCE01000C, 0x151558C5},
    {0xCE010014, 0x3D7E84FE},
    {0xCE01001C, 0x000103EB},
    {0xCE010020, 0x000103EA},
#else

    //{0xCE010008, 0x002DF69A},
    //Internal IDO setting
#if (SSV_INTERNAL_LDO == 0)
    {0xCE010008, 0x000DF69B},
#elif (SSV_INTERNAL_LDO == 1)
    {0xCE010008, 0x008B7C1C},
#else
# error "Please redefine SSV_INTERNAL_LDO!!"
#endif

    {0xCE01000C, 0x151558C5},
    {0xCE010014, 0x3D3E84FE},
    {0xCE01001C, 0x000103A7},
    {0xCE010020, 0x000103A6},
#endif
#if (CONFIG_CHIP_ID == SSV6051Z)
    {0xCE01002C, 0x00062CA8},
#else
    {0xCE01002C, 0x00032CA8}, // 6051Q & 6060P
#endif
    {0xCE010030, 0x20EA0224},
    {0xCE010010, 0x01011A88},
    {0xCE010018, 0x01457D79},
    {0xCE010024, 0x00012001},
    {0xCE010028, 0x00036000},
    {0xCE010034, 0x44000755},
    {0xCE01003C, 0x55D89D8A},
    {0xCE010040, 0x005508BB},
    {0xCE010044, 0x07C08BFF},
    {0xCE010048, 0xFCCCCC27},
    {0xCE01004C, 0x07700830},
    {0xCE010050, 0x0047C000},
    {0xCE010054, 0x00007FF4},
    {0xCE010058, 0x0000000E},
    {0xCE01005C, 0x00088018},
    {0xCE010064, 0x08820820},
    {0xCE010068, 0x00820820},
    {0xCE01006C, 0x00820820},
    {0xCE010070, 0x00820820},
    {0xCE010074, 0x00820820},
    {0xCE010078, 0x00820820},
    {0xCE01007C, 0x00820820},
    {0xCE010080, 0x00820820},
    {0xCE010084, 0x00004080},
    {0xCE010088, 0x200800FE},
    {0xCE01008C, 0xAAAAAAAA},
    {0xCE010090, 0xAAAAAAAA},
    {0xCE010094, 0x0000A487},
    {0xCE010098, 0x0000070E},
    {0xCE0100A4, 0x00000F43},
    {0xCE0100A8, 0x00098900},
    {0xCE0100AC, 0x00000000},
    //
    {0xC00003AC, 0x00000000},   // GPIO registers
    {0xC00003B0, 0x00000000},
    {0xC00003B4, 0x00000000},
    {0xC00003BC, 0x00000000},
    {0xC0001D00, 0x5E000040},   // PMU registers //CIT-34
    {0xC0001D04, 0x015D015D},

#if (SSV_VOLT_REGULATOR == VOLT_DCDC_CONVERT)
    {0xC0001D08, 0x00000001},   //DCDC
#elif (SSV_VOLT_REGULATOR == VOLT_LDO_REGULATOR)
    {0xC0001D08, 0x00000000},   //LDO
#endif
    {0xC0001D0C, 0x55550000},
    {0xC0001D20, 0x7FFF0000},
    {0xC0001D24, 0x00000003},
    {0xC0001D28, 0x00000000},
    {0xC0001D2C, 0x00000000},
};



static const ssv_cabrio_reg fpga_rf_setting[]=
{
	{0xcb110000,0x5F00EFCE},
	{0xcb110004,0x00001FC0},
	{0xcb110008,0x1C96CA3A},
	{0xcb11000c,0x15155A74},
	{0xcb110010,0x01011A88},
	{0xcb110014,0x3CBF703C},
	{0xcb110018,0x00057579},
	{0xcb11001c,0x000103A7},
	{0xcb110020,0x000103A6},
	{0xcb110024,0x00012001},
	{0xcb110028,0x00036000},
	{0xcb11002c,0x00000CA8},
	{0xcb110030,0x002A0224},
	{0xcb110034,0x00001E55},
	{0xcb110038,0x00006C7C},
	{0xcb11003c,0x55666666},
	{0xcb110040,0x005508F8},
	{0xcb110044,0x07C08BFF},
	{0xcb110048,0xF1111A27},
	{0xcb11004c,0x2773F53C},
	{0xcb110050,0x00000A7C},
	{0xcb110054,0x00087FF8},
	{0xcb110058,0x00103014},
	{0xcb11005c,0x0000848A},
	{0xcb110060,0x00406030},
	{0xcb110064,0x00820820},
	{0xcb110068,0x00820820},
	{0xcb11006c,0x00820820},
	{0xcb110070,0x00820820},
	{0xcb110074,0x00820820},
	{0xcb110078,0x00820820},
	{0xcb11007c,0x00820820},
	{0xcb110080,0x00820820},
	{0xcb110084,0x00004080},
	{0xcb110088,0x00003EAA},
	{0xcb11008c,0x5E00FFEB},
	{0xcb110090,0xAAAAAAAA},
	{0xcb110094,0x0000243F},
	{0xcb110098,0x00018B10},

	{0xcb120080,0x00000000},
	{0xcb120084,0x00000000},
	{0xcb120088,0x00000000},
	{0xcb120090,0x00000813},
	{0xcb120094,0x00000000},
	{0xcb1203f8,0xFF000000},
};
