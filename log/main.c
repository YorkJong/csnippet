#include <stdio.h>
#include <stdint.h>

#define LOG_LEVEL   LL_DEBUG
#include "log.h"

int main()
{
    char buf[] = "YYYY/MM/DD";
    int y, m, d;

    INF(("sizeof(int)=%d", sizeof(int)));
    INF(("%02X", 10));
    INF(("%04d/%02d/%02d", 2010, 3, 10));

    sprintf(buf, "%04d/%02d/%02d", 2010, 3, 10);
    DBG((buf));
    sprintf(buf, "%d/%d", 2010, 3);
    DBG((buf));

    sscanf("2010/3", "%d/%d", &y, &m);
    DBG(("%04d/%02d", y, m));
    sscanf("2010/4", "%d%*[^0-9]%d", &y, &m);
    DBG(("%04d/%02d", y, m));

    DebugCode (
        DBG(("DebugCode Block is Tested OK!\n"));
    )

    return 0;
}
