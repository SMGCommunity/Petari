#ifndef EUART_H
#define EUART_H

#include <export/UART.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	EUART_ERROR_NONE = 0,
	EUART_ERROR_UNINITIALIZED,
	EUART_ERROR_CANNOT_USE,
	EUART_ERROR_CHANNEL_FULL,
	EUART_ERROR_NOT_FIND,
	EUART_ERROR_INTERNAL,
	EUART_ERROR_INVALID_PARAMETER,
	EUART_ERROR_INVALID_HANDLE,
	EUART_ERROR_COM_OPEN,
	EUART_ERROR_COMM
} EUARTError;


UARTError InitializeUART(UARTBaudRate);
UARTError WriteUARTN(const void *, unsigned long);

#ifdef __cplusplus
}
#endif

#endif // EUART_H