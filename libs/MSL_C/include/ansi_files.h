typedef unsigned char console_buff[256];

static console_buff stdin_buff;
static console_buff stdout_buff;
static console_buff stderr_buff;

void __close_all(void);