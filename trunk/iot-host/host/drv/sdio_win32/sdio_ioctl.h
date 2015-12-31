#ifndef _SDIO_IOCTL_H_
#define _SDIO_IOCTL_H_

#define FILE_DEVICE_CONTROLLER 	0x00000004

#define FILE_ANY_ACCESS			0
#ifndef FILE_READ_ACCESS
#define	FILE_READ_ACCESS		0x0001
#endif
#ifndef FILE_WRITE_ACCESS
#define FILE_WRITE_ACCESS		0x0002
#endif
#ifndef METHOD_BUFFERED
#define METHOD_BUFFERED 		0
#endif
#ifndef METHOD_IN_DIRECT
#define METHOD_IN_DIRECT 		1
#endif
#ifndef METHOD_OUT_DIRECT
#define METHOD_OUT_DIRECT 		2
#endif
#ifndef METHOD_NEITHER
#define METHOD_NEITHER 			3
#endif

#define sdio_ioctl(access, code, method)	(((FILE_DEVICE_CONTROLLER) << 16) | ((access) << 14) | ((code) << 2) | (method))

#define IOCTL_SSV6XXX_SDIO_GET_DRIVER_VERSION 			sdio_ioctl(FILE_ANY_ACCESS, 0x780, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_GET_FUNCTION_NUMBER 			sdio_ioctl(FILE_ANY_ACCESS, 0x781, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_GET_FUNCTION_FOCUS 			sdio_ioctl(FILE_ANY_ACCESS, 0x782, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_SET_FUNCTION_FOCUS 			sdio_ioctl(FILE_ANY_ACCESS, 0x783, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_GET_BUS_WIDTH 				sdio_ioctl(FILE_ANY_ACCESS, 0x784, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_SET_BUS_WIDTH 				sdio_ioctl(FILE_ANY_ACCESS, 0x785, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_GET_BUS_CLOCK 				sdio_ioctl(FILE_ANY_ACCESS, 0x786, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_SET_BUS_CLOCK 				sdio_ioctl(FILE_ANY_ACCESS, 0x787, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_GET_BLOCK_MODE 				sdio_ioctl(FILE_ANY_ACCESS, 0x788, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_SET_BLOCK_MODE 				sdio_ioctl(FILE_ANY_ACCESS, 0x789, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_GET_BLOCKLEN 					sdio_ioctl(FILE_ANY_ACCESS, 0x78a, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_SET_BLOCKLEN 					sdio_ioctl(FILE_ANY_ACCESS, 0x78b, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_GET_FN0_BLOCKLEN 				sdio_ioctl(FILE_ANY_ACCESS, 0x78c, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_SET_FN0_BLOCKLEN 				sdio_ioctl(FILE_ANY_ACCESS, 0x78d, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_GET_BUS_INTERFACE_CONTROL 	sdio_ioctl(FILE_ANY_ACCESS, 0x78e, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_SET_BUS_INTERFACE_CONTROL 	sdio_ioctl(FILE_ANY_ACCESS, 0x78f, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_GET_INT_ENABLE 				sdio_ioctl(FILE_ANY_ACCESS, 0x790, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_SET_INT_ENABLE 				sdio_ioctl(FILE_ANY_ACCESS, 0x791, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_GET_AUTO_ACK_INT          	sdio_ioctl(FILE_ANY_ACCESS, 0x7a0, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_SET_AUTO_ACK_INT              sdio_ioctl(FILE_ANY_ACCESS, 0x7a1, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_ACK_INT                       sdio_ioctl(FILE_ANY_ACCESS, 0x7a2, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_READ_BYTE 					sdio_ioctl(FILE_ANY_ACCESS, 0x7b0, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_WRITE_BYTE 					sdio_ioctl(FILE_ANY_ACCESS, 0x7b1, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_GET_MULTI_BYTE_IO_PORT 		sdio_ioctl(FILE_ANY_ACCESS, 0x7b2, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_SET_MULTI_BYTE_IO_PORT 		sdio_ioctl(FILE_ANY_ACCESS, 0x7b3, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_READ_MULTI_BYTE 				sdio_ioctl(FILE_ANY_ACCESS, 0x7b4, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_WRITE_MULTI_BYTE 				sdio_ioctl(FILE_ANY_ACCESS, 0x7b5, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_GET_MULTI_BYTE_REG_IO_PORT 	sdio_ioctl(FILE_ANY_ACCESS, 0x7b6, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_SET_MULTI_BYTE_REG_IO_PORT 	sdio_ioctl(FILE_ANY_ACCESS, 0x7b7, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_READ_REG 						sdio_ioctl(FILE_ANY_ACCESS, 0x7b8, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_WRITE_REG 					sdio_ioctl(FILE_ANY_ACCESS, 0x7b9, METHOD_BUFFERED)
#define IOCTL_SSV6XXX_SDIO_READ_DATA 					sdio_ioctl(FILE_ANY_ACCESS, 0x7ba, METHOD_BUFFERED)


#endif	/* _SDIO_IOCTL_H_ */
