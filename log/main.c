#include <stdio.h>
#include <stdint.h>

#define LOG_LEVEL   LL_DEBUG
#include "log.h"

int main()
{
    char buf[] = "YYYY/MM/DD";
    int y, m, d;

    LInfo(("sizeof(int)=%d", sizeof(int)));
    LInfo(("%02X", 10));
    LInfo(("%04d/%02d/%02d", 2010, 3, 10));

    sprintf(buf, "%04d/%02d/%02d", 2010, 3, 10);
    LDbg((buf));
    sprintf(buf, "%d/%d", 2010, 3);
    LDbg((buf));

    sscanf("2010/3", "%d/%d", &y, &m);
    LDbg(("%04d/%02d", y, m));
    sscanf("2010/4", "%d%*[^0-9]%d", &y, &m);
    LDbg(("%04d/%02d", y, m));

    DebugCode (
        LDbg(("DebugCode Block is Tested OK!\n"));
    )

    return 0;
}
