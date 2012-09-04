#include <cxfuncs.h>		/* get cmx include header file */
#include <ioxm256a3.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <emma/io.h>
#include <emma/resources.h>
#include <emma/mailboxes.h>
#include <emma/sensors.h>
#include <emma/flash.h>
#include <emma/flash_lo.h>
#include <emma/accelmgr.h>
#include <emma/motion.h>
#include <emma/modems/modem.h>
#include <emma/modems/iridium.h>
#include <emma/adc.h>
#include <emma/gps.h>
#include <emma/watchdog.h>
#include <emma/rtc_clk.h>
#include <emma/ports.h>
__flash SENS_INIT Sensors_required[] =
{
  
  0L
};

#define RSTACK 64
#define CSTACK 512

__flash word16 user_rstack_size = RSTACK;
__flash word16 user_cstack_size = CSTACK;

word16 user_rstack[RSTACK];
word16 user_cstack[CSTACK];

const u8 ticks_per_sec = 20;
__flash u16 watchdog_ticks = 100;
u8 const rtc_holdoff= FALSE;
u8 mdm_mbox;
IO_CONF spi_conf;
IO_HANDLE uh,mdm;
MODEM_TYPE modemtype = MDM_IRIDIUM;
MODEM_HANDLE mdm_handle;
 u8 usertask_priority = 1;
//char buffer[];
__task void user_task(void)
{
  BOOL status;
  //IO_start_transaction(mdm);
  //IO_subscribe(mdm, mdm_mbox); 
  //uh = IO_open(IO_UART, &uart_conf);
  //mdm = IO_open(IO_UART, &uart_conf);
  modem_init();
  mdm_handle = Modem_open(modemtype,mdm);  
  iridium_init();
  mdm = (IO_HANDLE)ird_open();
  ird_config(mdm_handle, &spi_conf);
  //ird_write(
  status = Iridium_get_time(mdm_handle);
  rtc_init();
  rtc_set_seconds(1000);
  FlashPowerOn();
  FlashInit();
  FlashInit();
  /*uart_conf.uart_conf.baud = BAUD_57600;
  uart_conf.uart_conf.bits = EIGHT_BITS;
  uart_conf.uart_conf.parity = PARITY_NONE;
  uart_conf.uart_conf.channel = eUART_F0;
  uart_conf.uart_conf.blocking = FALSE;
  sprintf(buffer,"hello world");
  uh = IO_open(IO_UART, &uart_conf);
  IO_subscribe(uh, uart_mbox);
  IO_start_transaction(uh);
  IO_write(uh, (u8 *)buffer, strlen(buffer));*/ 
  while(1);
}