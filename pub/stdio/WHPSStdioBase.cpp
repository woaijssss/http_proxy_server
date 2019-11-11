#include "WHPSStdioBase.h"

void vsprintfGet(char* buffer, const char* fmt, va_list& ap)
{
        if (buffer)
        {
                vsprintf(buffer, fmt, ap);
        }
}
