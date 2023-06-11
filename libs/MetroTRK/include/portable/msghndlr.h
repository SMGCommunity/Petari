#ifndef MSGHNDLR_H
#define MSGHNDLR_H

static int* IsTRKConnected;

int TRKDoConnect(void);
void SetTRKConnected(int);
int GetTRKConnected(void);

#endif // MSGHNDLR_H