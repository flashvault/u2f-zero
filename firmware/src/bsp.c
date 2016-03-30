/*
 * bsp.c
 *
 */
#include <SI_EFM8UB1_Register_Enums.h>
#include <efm8_usb.h>
#include <stdarg.h>
#include <stdio.h>
#include "app.h"
#include "bsp.h"


void u2f_delay(uint32_t ms) {
	uint32_t ms_now = get_ms();
	while((get_ms() - ms_now) < ms)
	{}
}

void usb_write(uint8_t* buf, uint8_t len)
{
	uint8_t errors = 0;
	while (USB_STATUS_OK != (USBD_Write(EP1IN, buf, len, false)))
	{
		u2f_delay(2);
		if (errors++ > 30)
		{
			set_app_error(ERROR_USB_WRITE);
			break;
		}
	}
}


#ifdef U2F_PRINT

void putf(char c)
{
	uint8_t i;
	SBUF0 = c;
	for (i=0; i<200; i++){}
	for (i=0; i<200; i++){}
	for (i=0; i<190; i++){}
	watchdog();
}


void dump_hex(uint8_t* hex, uint8_t len)
{
	uint8_t i;
	for (i=0 ; i < len ; i++)
	{
		if (hex[i]<0x10)
		{
			putf('0');
		}
		u2f_putb(hex[i]);
	}
	u2f_prints("\r\n");
}


void u2f_prints(char* d)
{
	static SI_SEG_DATA uint16_t i;
	while(*d)
	{
		// UART0 output queue
		putf(*d++);
	}
}

static void int2str_reduce_n(char ** snum, uint32_t copy, uint8_t n)
{
    do
    {
        *snum++;
        copy /= n;
    }while(copy);
}


static const char * __digits = "0123456789abcdef";
static char __int2str_buf[9];

static void int2str_map_n(char ** snum, uint32_t i, uint8_t n)
{
    do
    {
        *--*snum = __digits[i % n];
        i /= n;
    }while(i);
}

#define dint2str(i)     __int2strn(i,10)
#define xint2str(i)     __int2strn(i,16)

char * __int2strn(int32_t i, uint8_t n)
{
    char * snum = __int2str_buf;
    if (i<0) *snum++ = '-';
    int2str_reduce_n(&snum, i, n);
    *snum = '\0';
    int2str_map_n(&snum, i, n);
    return snum;
}

void u2f_putd(int32_t i)
{
	u2f_prints(dint2str((int32_t)i));
}

void u2f_putx(int32_t i)
{
	u2f_prints(xint2str((int32_t)i));
}

static void put_space()
{
	u2f_prints(" ");
}
static void put_line()
{
	u2f_prints("\r\n");
}

void u2f_printd(const char * tag, uint8_t c, ...)
{
	va_list args;
	u2f_prints(tag);
    va_start(args,c);
    while(c--)
    {
        u2f_putd(va_arg(args, int16_t));

    }
    put_line();
    va_end(args);
}

void u2f_printl(const char * tag, uint8_t c, ...)
{
    va_list args;
    u2f_prints(tag);
    va_start(args,c);
    while(c--)
    {
        u2f_putl(va_arg(args, int32_t));
        u2f_prints(" ");
    }
    put_line();
    va_end(args);
}

void u2f_printx(const char * tag, uint8_t c, ...)
{
    va_list args;
    u2f_prints(tag);
    va_start(args,c);
    while(c--)
    {
        u2f_putx(va_arg(args, uint16_t));
        u2f_prints(" ");
    }
    put_line();
    va_end(args);
}

void u2f_printb(const char * tag, uint8_t c, ...)
{
	va_list args;
    u2f_prints(tag);
    va_start(args,c);
    while(c--)
    {
        u2f_putb(va_arg(args, uint8_t));
        put_space();
    }
    put_line();
    va_end(args);
}

void u2f_printlx(const char * tag, uint8_t c, ...)
{
    va_list args;
    u2f_prints(tag);
    va_start(args,c);
    while(c--)
    {
        u2f_putlx(va_arg(args, int32_t));
        put_space();
    }
    put_line();
    va_end(args);
}

#else




#endif









