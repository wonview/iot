#ifndef _SPI_H_
#define _SPI_H_

extern bool spi_host_init(void (*spi_isr)(void));
extern bool spi_host_write(u8 *buf, u32 sizeToTransfer, u32 *sizeToTransfered, u32 options);
extern bool spi_host_read(u8 *buf, u32 sizeToTransfer, u32 *sizeToTransfered, u32 options);
extern void spi_irq_enable(bool enable);
#endif