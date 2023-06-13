#ifndef UART_H
#define UART_H

int TRKPollUART(void);
int TRKReadUARTN(void *, unsigned long);
int TRKWriteUARTN(const void *, unsigned long);

#endif // UART_H