#include <console.h>
#include <string.h>
#include <stdarg.h>

void prin(const char* str)
{
    size_t len = strlen(str);

    for (size_t i = 0; i < len; ++i)
    {
        put_c(str[i]);
    }
}

void printk(const char* fmt, ...)
{
    va_list argumentos;
    size_t len = strlen(fmt);
    va_start(argumentos, fmt);

    for (size_t i = 0; i < len; ++i)
    {
        if (fmt[i] == '%')
        {
            i++;
            if (fmt[i] == 'i')
            {
                char buf[16];
                int argumento = va_arg(argumentos, int);
                itoa(argumento, buf, 10);
                prin(buf);
            
            } else if (fmt[i] == 'c')
            {
                char argumento = va_arg(argumentos, int);
                put_c(argumento);
            }

        } else
        {
            put_c(fmt[i]);
        }
    }
}