static unsigned char bUseSerialIO;

unsigned char GetUseSerialIO(void) {
    return bUseSerialIO;
}

void SetUseSerialIO(unsigned char use) {
    bUseSerialIO = use;
}