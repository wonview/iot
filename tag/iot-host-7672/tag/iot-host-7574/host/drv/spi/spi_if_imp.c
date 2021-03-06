#include "config.h"
#include "msgevt.h"


#include "ssv_drv_if.h"
#include "ssv_drv.h"
#include "ssv6200_reg.h"
#include "ssv_drv_config.h"
#include "spi.h"
#include "spi_def.h"
#include "rtos.h"

static void _spi_host_isr(void);
static bool _spi_read(u8 *buf, u32 sizeToTransfer, u32 *sizeToTransfered, u32 options);
static bool __spi_write(u8 *buf, u32 sizeToTransfer, u32 *sizeToTransfered, u32 options);
static u32 _ssvcabrio_spi_read_status(void);
static bool _ssvcabrio_spi_read_reg (u32 addr, u32 *data);
static bool _ssvcabrio_spi_write_reg (u32 addr, u32 data);
static bool _ssvcabrio_spi_init (void);
static u32 _ssvcabrio_write_fw_to_sram(u8* fw_bin, u32 fw_bin_len);
static bool _ssvcabrio_do_firmware_checksum(u32 origin);
static bool _check_rx_rdy(u32 *rx_len);

extern struct task_info_st g_host_task_info[];
static void _spi_host_isr(void)
{
    MsgEvent *pMsgEv;
    SDRV_DEBUG("%s()\r\n",__FUNCTION__);
    pMsgEv=msg_evt_alloc();
    pMsgEv->MsgType=MEVT_PKT_BUF;
    pMsgEv->MsgData=0;
    pMsgEv->MsgData1=0;
    pMsgEv->MsgData2=0;
    pMsgEv->MsgData3=0;
    msg_evt_post(MBOX_SIM_RX_DRIVER, pMsgEv);
}

static bool _spi_read(u8 *buf, u32 sizeToTransfer, u32 *sizeToTransfered, u32 options)
{
    return spi_host_read(buf,sizeToTransfer, sizeToTransfered,options);
}

static bool _spi_write(u8 *buf, u32 sizeToTransfer, u32 *sizeToTransfered, u32 options)
{
    return spi_host_write(buf,sizeToTransfer, sizeToTransfered,options);
}

static u32 _ssvcabrio_spi_read_status(void)
{
    u32 sizeToTransfer;
    u32 sizeTransfered;
    bool status=TRUE;
    u8     cmd[5];
    u32    data32[2];
    u8    *data = (u8 *)&data32[0];

    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);
    // Send read register address
    sizeToTransfer = 1;
    data += 4;
    cmd[0] = SPI_READ_STS_CMD;
    status = _spi_write(cmd, sizeToTransfer,&sizeTransfered,
                        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                        | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE
                        | SPI_TRANSFER_OPTIONS_CPU_POLLING_MODE
                        );
    if(FALSE==status) goto ERROR;

    MEMSET((u8 *)data32,0,sizeof(data32));
    sizeToTransfer = 4;
    status = _spi_read(data, sizeToTransfer, &sizeTransfered,
                    SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                    | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE
                    | SPI_TRANSFER_OPTIONS_CPU_POLLING_MODE
                    );

    if(FALSE==status) goto ERROR;
    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
    return data32[1];
ERROR:
    SDRV_ERROR("%s(): FAIL\r\n",__FUNCTION__);
    return 0xFFFFFFFF;
}

static bool _ssvcabrio_spi_read_reg (u32 addr, u32 *data)
{
    u32 sizeToTransfer = 0;
    u32 sizeTransfered=0;
    u32 retry=0;
    bool status=TRUE;
    u8  cmd[9]; // 1B CMD + 4B ADDR + 4B DATA
    u8  temp[4]={0};

    // Send read register address
    sizeToTransfer = 5;
    cmd[0] = SPI_READ_REG_CMD;
    cmd[1]= (u8)(addr >> 24);
    cmd[2]= (u8)(addr >> 16);
    cmd[3]= (u8)(addr >> 8);
    cmd[4]= (u8)(addr);

    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);
    status = _spi_write(cmd, sizeToTransfer, &sizeTransfered,
                        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                      | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE
                      | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE
                      | SPI_TRANSFER_OPTIONS_CPU_POLLING_MODE
                      );
    if(FALSE==status) goto ERROR;

    status = _spi_read((u8 *)temp, 4, &sizeTransfered,
                        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                        | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE
                        | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE
                        | SPI_TRANSFER_OPTIONS_CPU_POLLING_MODE
                      );
    if(FALSE==status) goto ERROR;

    *data = (  ((u32)temp[0]) | ((u32)temp[1] << 8)
            | ((u32)temp[2] << 16) | ((u32)temp[3] << 24));

    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
    return TRUE;
ERROR:
    SDRV_ERROR("%s(): FAIL\r\n",__FUNCTION__);
    return FALSE;
}

static bool _ssvcabrio_spi_write_reg (u32 addr, u32 data)
{
    u32 sizeToTransfer = 0;
    u32 sizeTransfered=0;
    u32 retry=0;
    bool status=TRUE;
    u8  cmd[9]; // 1B CMD + 4B ADDR + 4B DATA

    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);
    //BEGIN_SPI_TRANS();

    cmd[0] = SPI_WRITE_REG_CMD;
    cmd[1]= (u8)(addr >> 24);
    cmd[2]= (u8)(addr >> 16);
    cmd[3]= (u8)(addr >> 8);
    cmd[4]= (u8)(addr);
    cmd[5]= (u8)(data);
    cmd[6]= (u8)(data >> 8);
    cmd[7]= (u8)(data >> 16);
    cmd[8]= (u8)(data >> 24); // MSB last
    sizeToTransfer = 5;
    sizeTransfered = 0;
    status = _spi_write(cmd, sizeToTransfer, &sizeTransfered,
                        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                    | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE
                    | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE
                    | SPI_TRANSFER_OPTIONS_CPU_POLLING_MODE
                    );
    if(FALSE==status) goto ERROR;

    sizeToTransfer = 4;
    sizeTransfered = 0;
    status = _spi_write(&cmd[5], sizeToTransfer, &sizeTransfered,
                        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                    | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE
                    | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE
                    | SPI_TRANSFER_OPTIONS_CPU_POLLING_MODE
                    );
    if(FALSE==status) goto ERROR;

    //Ian ToDo: check status
    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);

    return TRUE;
ERROR:
    SDRV_ERROR("%s(): FAIL\r\n",__FUNCTION__);
    return FALSE;
}


static bool _ssvcabrio_spi_init (void)
{
    u32 data;
    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);
    //OS_MutexInit(&s_SPIMutex);
    // Read status register to ensure CS is up for 1st meaningful command.
    _ssvcabrio_spi_read_status();
    // Switch HCI to SPI
    _ssvcabrio_spi_write_reg(0xc0000a0c, 0);
    // bit 8 to enable TX. Set block size.
    _ssvcabrio_spi_write_reg(0xc0000a24, 0x100 | (SPI_TX_BLOCK_SHIFT));

    //read CHIP_ID_0, 0x31333131
    _ssvcabrio_spi_read_reg(0xc0000008,&data);
    //read CHIP_ID_1, 0x322d3230
    _ssvcabrio_spi_read_reg(0xc000000C,&data);
    //read CHIP_ID_2, 0x32303041 ==> get 0x32303241 ?
    _ssvcabrio_spi_read_reg(0xc0000010,&data);
    //read CHIP_ID_3, 0x53535636 ==> get 0x53535336 ?
    _ssvcabrio_spi_read_reg(0xc0000014,&data);
    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
    return TRUE;
}

static bool _check_rx_rdy(u32 *rx_len)
{
#define CHECK_RDY_RETRY_COUNT 1
    u32 status32 = 0;
    u8  *ptr=NULL;
    u8 retry=0;

    do {
        status32 = _ssvcabrio_spi_read_status();
        ptr= (u8 *)&status32;
        if(status32!=0xFFFFFFFF){
            if (ptr[READ_STATUS_BYTE0] & READ_STATUS_BYTE0_RX_RDY) // rx ready?
            {
                *rx_len= (((u32)(ptr[READ_STATUS_RX_LEN_L]) & (u32)0x000000ffUL)) |
                (((u32)(ptr[READ_STATUS_RX_LEN_H]) & (u32)0x000000ffUL) << 8);
                break;
            }
        }
        retry++;
    } while (retry < CHECK_RDY_RETRY_COUNT);

    if(retry==CHECK_RDY_RETRY_COUNT)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}
static u32 _ssvcabrio_write_fw_to_sram(u8* fw_bin, u32 fw_bin_len)
{
    u8   *fw_buffer = NULL;
    u8   *fw_ptr = fw_bin;
    u32  sram_addr = 0x00000000;
    u32  bin_len=fw_bin_len;
    u32  i=0,len=0, checksum=FW_CHECKSUM_INIT, temp=0;
    u32  j=0;

    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);
    fw_buffer = (u8 *)OS_MemAlloc(M_SPI_FW_BLOCK_SIZE);
    if (fw_buffer == NULL) {
        SDRV_FAIL("%s(): Failed to allocate buffer for firmware.\r\n",__FUNCTION__);
        return 0;
    }

    while(bin_len > 0){
        len=(bin_len >= M_SPI_FW_BLOCK_SIZE)?M_SPI_FW_BLOCK_SIZE:bin_len;
        bin_len -= len;
        if(len!=FW_BLOCK_SIZE){
            MEMSET((void*)fw_buffer, 0xA5, M_SPI_FW_BLOCK_SIZE);
        }
        MEMCPY((void*)fw_buffer,(void*)fw_ptr,len);
        fw_ptr += len;

        SDRV_DEBUG("%s(): read len=0x%x,sram_addr=0x%x\r\n",__FUNCTION__,len,sram_addr);
        for (i = 0; i < (M_SPI_FW_BLOCK_SIZE)/4; i++) /* force 1024 bytes a set. */
        {
            temp = *((u32 *)(&fw_buffer[i*4]));
            if(FALSE==_ssvcabrio_spi_write_reg(sram_addr, temp)) goto ERROR;
            sram_addr += 4;
            checksum += temp;
        }
        j++;
        SDRV_INFO("* ",__FUNCTION__);
        if(j%16==0)
            SDRV_INFO("\r\n",__FUNCTION__);
    }// while(bin_len > 0){
    SDRV_INFO("\r\n",__FUNCTION__);
    SDRV_DEBUG("%s(): checksum = 0x%x\r\n",__FUNCTION__, checksum);
    OS_MemFree(fw_buffer);
    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
    return checksum;
ERROR:
    SDRV_INFO("\r\n",__FUNCTION__);
    OS_MemFree(fw_buffer);
    SDRV_ERROR("%s(): FAIL\r\n",__FUNCTION__);
    return 0;
}

static bool _ssvcabrio_do_firmware_checksum(u32 origin)
{
    #define RETRY_COUNT 30
    u32 retry=0;
    u32 fw_checksum=0;
	u32 fw_status = 0;

	origin = (((origin >> 24) + (origin >> 16) + (origin >> 8) + origin) & FW_CHK_SUM_SIZE);
    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);
    retry=0;
    do{

		do{
            _ssvcabrio_spi_read_reg(FW_STATUS_REG,&fw_status);
            if(fw_status & FW_STATUS_FW_CHKSUM_BIT)
                break;
        }while(1);


        fw_checksum = (fw_status & FW_CHK_SUM_MASK)>>16;



        SDRV_INFO("%s(): fw check sum = 0x%x, check sum = 0x%x\r\n",__FUNCTION__, fw_checksum, origin);
        if (fw_checksum == origin)
        {
            SDRV_INFO("%s(): [ssv] check sum is the same.\r\n",__FUNCTION__);
            _ssvcabrio_spi_write_reg(FW_STATUS_REG, (~origin & FW_STATUS_MASK));
            break;
        }
        else
        {
            SDRV_FAIL("%s(): [ssv] check sum is fail.\r\n",__FUNCTION__);
        }

        retry++;

        retry++;
        OS_MsDelay(50);
    }while(retry!=RETRY_COUNT);

    if(retry==RETRY_COUNT)
    {
        SDRV_FAIL("%s(): [ssv] check sum is fail.sum = 0x%x, org = 0x%x\r\n",__FUNCTION__,fw_checksum, origin);
        return FALSE;
    }
    else
    {
        SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
        return TRUE;
    }
}


static bool ssvcabrio_init(void)
{
    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);
    spi_host_init(_spi_host_isr);
    _ssvcabrio_spi_init();
    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
    return TRUE;
}

static u32 ssvcabrio_read_reg(u32 addr)
{
    u32 data;
    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);

    if(FALSE==_ssvcabrio_spi_read_reg(addr, &data))
    {
        data=0;
    }

    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
    return data;
}

static bool ssvcabrio_write_reg(u32 addr, u32 data)
{
    bool ret=FALSE;
    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);

    ret = _ssvcabrio_spi_write_reg(addr, data);

    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
    return ret;
}

static bool ssvcabrio_download_firmware(u8* fw_bin, u32 fw_bin_len)
{
    bool ret = TRUE;

#ifdef ENABLE_FW_SELF_CHECK
    u32   checksum = FW_CHECKSUM_INIT;
    u32   fw_checksum = 0x0;
    u32   retry_count = 1;
#else
    int   writesize=0;
    u32   retry_count = 1;
#endif
    u32 clk_en;
    int block_count = 0;

    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);
    if((fw_bin==NULL)||(fw_bin_len==0))
    {
        SDRV_TRACE("%s():wrong input parameters\r\n",__FUNCTION__);
        return FALSE;
    }

    do { //retry loop

        SDRV_INFO("%s(): bin size=0x%x\r\n",__FUNCTION__,fw_bin_len);
        ret=TRUE;
        if(ret==TRUE) ret = _ssvcabrio_spi_write_reg(ADR_BRG_SW_RST, 0x0);
        if(ret==TRUE) ret = _ssvcabrio_spi_write_reg(ADR_BOOT, 0x01);
        if(ret==TRUE) ret = _ssvcabrio_spi_read_reg(ADR_PLATFORM_CLOCK_ENABLE, &clk_en);
        if(ret==TRUE) ret = _ssvcabrio_spi_write_reg(ADR_PLATFORM_CLOCK_ENABLE, clk_en | (1 << 2));

        SDRV_INFO("%s(): Writing firmware to SSV6XXX...\r\n",__FUNCTION__);

        checksum = _ssvcabrio_write_fw_to_sram(fw_bin,fw_bin_len);
        //checksum = ((checksum >> 24) + (checksum >> 16) + (checksum >> 8) + checksum) & 0x0FF;
        //checksum <<= 16;

        SDRV_INFO("%s(): checksum = 0x%x\r\n",__FUNCTION__, checksum);

        block_count = fw_bin_len / M_SPI_FW_BLOCK_SIZE;
        block_count = ((fw_bin_len % M_SPI_FW_BLOCK_SIZE)> 0)?block_count+1:block_count;
        SDRV_INFO("%s(): block_count = 0x%x\r\n",__FUNCTION__, block_count);

        if(ret==TRUE) ret= _ssvcabrio_spi_write_reg(FW_STATUS_REG, (block_count << 16));
        //ssv_spi_read_reg(spi, FW_STATUS_REG, &tmp);

        if(ret==TRUE) ret = _ssvcabrio_spi_write_reg(ADR_BRG_SW_RST, 0x01);
        //OS_MsDelay(10);
        //printk(KERN_INFO "test output z to uart.\n");

        //ssv_spi_write_reg(spi, 0xc0000c00, 'z');

        SDRV_INFO("%s(): Firmware \" loaded, checksum = %x\r\n",__FUNCTION__, checksum);
        // Wait FW to calculate checksum.
        if(TRUE==_ssvcabrio_do_firmware_checksum(checksum))
        {
            ret=TRUE;
            break;
        }

    } while (--retry_count); //do { //retry loop
    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
    return ret;
}
static bool ssvcabrio_open(void)
{
    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);
    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
    return TRUE;
}

static bool ssvcabrio_close(void)
{
    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);
    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
    return TRUE;
}


static s32 ssvcabrio_recv(u8 *data, size_t len)
{
    u32 sizeToTransfer = 0;
    u32 sizeTransfered=0;
    bool status=TRUE;
    u8  cmd[4]={0};
    u8  dummy[4]={0};
    u8 align_4_len;
    u32 rlen=0;

    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);
    if(data==NULL)
    {
        SDRV_ERROR("%s(): data is a null pointer\r\n",__FUNCTION__);
        return -1;
    }

    if(FALSE==_check_rx_rdy(&rlen))
    {
        goto NOT_READY;
    }

    SDRV_DEBUG("%s() :rlen=%d(0x%x) \r\n",__FUNCTION__,rlen,rlen);
    rlen=(rlen<len)?rlen:len;
    //align_4_len
    align_4_len=(rlen%4)?(4-(rlen%4)):0;
    SDRV_DEBUG("%s() :align_4_len=%d(0x%x) \r\n",__FUNCTION__,align_4_len,align_4_len);

    // Send read register address
    sizeToTransfer = 4;
    cmd[0] = SPI_RX_DATA_CMD;

    status = _spi_write(cmd, sizeToTransfer, &sizeTransfered,
                        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                      | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE
                      | SPI_TRANSFER_OPTIONS_CPU_POLLING_MODE
                      );
    if(FALSE==status) goto ERROR;

    //read data
    if(align_4_len!=0)
    {
        status = _spi_read((u8 *)data, rlen, &sizeTransfered,
                        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES);

        if(FALSE==status) goto ERROR;

        status = _spi_read((u8 *)&dummy[0], align_4_len, &sizeTransfered,
                            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                            | SPI_TRANSFER_OPTIONS_CPU_POLLING_MODE
                            | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
        if(FALSE==status) goto ERROR;
    }
    else
    {
        status = _spi_read((u8 *)data, rlen, &sizeTransfered,
                        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                        | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE
                      );
        if(FALSE==status) goto ERROR;

    }
    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
    return rlen;
ERROR:
    SDRV_ERROR("%s(): fail\r\n",__FUNCTION__);
    return -1;
NOT_READY:
    SDRV_DEBUG("%s(): NOT_READY\r\n",__FUNCTION__);
    return -1;
}



static s32 ssvcabrio_send(void *data, size_t len)
{
    u32 sizeToTransfer = 0;
    u32 sizeTransfered=0;
    bool status=TRUE;
    u8  cmd[4]={0};
    u8  dummy[4]={0};
    u8  align_4_len;

    SDRV_TRACE("%s() => :%d\r\n",__FUNCTION__,__LINE__);
    //alignment 4 bytes
    align_4_len=(len%4)?(4-(len%4)):0;
    SDRV_DEBUG("%s() :align_4_len=%d(0x%x) \r\n",__FUNCTION__,align_4_len,align_4_len);

    // Send read register address
    sizeToTransfer = 4;
    cmd[0] = SPI_TX_DATA_CMD;

    status = _spi_write(cmd, sizeToTransfer, &sizeTransfered,
                        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                      | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE
                      | SPI_TRANSFER_OPTIONS_CPU_POLLING_MODE
                      );

    if(FALSE==status) goto ERROR;

    //write data
    if(align_4_len!=0)
    {
        status = _spi_write((u8 *)data, len, &sizeTransfered,
                        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                        | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE
                      );

        if(FALSE==status) goto ERROR;

        status = _spi_write((u8 *)&dummy[0], align_4_len, &sizeTransfered,
                        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                        | SPI_TRANSFER_OPTIONS_CPU_POLLING_MODE
                        | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE
                      );
        if(FALSE==status) goto ERROR;
    }
    else
    {
        status = _spi_write((u8 *)data, len, &sizeTransfered,
                        SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES
                        | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE
                      );
        if(FALSE==status) goto ERROR;


    }
    SDRV_TRACE("%s() <= :%d\r\n",__FUNCTION__,__LINE__);
    return len;
ERROR:
    SDRV_ERROR("%s():FAIL\r\n",__FUNCTION__);
    return -1;
}

static void ssvcabrio_irq_enable(void)
{
    spi_host_irq_enable(TRUE);
}

static void ssvcabrio_irq_disable(void)
{
    spi_host_irq_enable(FALSE);
}
struct ssv6xxx_drv_ops  g_drv_spi=
{
    DRV_NAME_SPI, //name
    ssvcabrio_open, //open
    ssvcabrio_close, //close
    ssvcabrio_init, //init
    ssvcabrio_recv, //recv
    ssvcabrio_send, //send
    NULL, //get name
    NULL, //ioctl
    NULL, //handle
    NULL, //ack init
    NULL, //write sram
    NULL, //read sram
    ssvcabrio_write_reg, //write reg
    ssvcabrio_read_reg, // read reg
    NULL, //write bytes
    NULL, //read bytes
    ssvcabrio_download_firmware, //load fw
    NULL, //start
    NULL, //stop
    ssvcabrio_irq_enable, //irq enable
    ssvcabrio_irq_disable, //irq disable
};


