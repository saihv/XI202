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
#include <emma/modems/modem.h>
#include <emma/modems/iridium.h>
#include <emma/watchdog.h>
#include <emma/rtc_clk.h>
#include <emma/ports.h>

#define RSTACK 64
#define CSTACK 512

__flash SENS_INIT Sensors_required[] =
{
  
  0L
};

__flash word16 user_rstack_size = RSTACK;
__flash word16 user_cstack_size = CSTACK;

word16 user_rstack[RSTACK];
word16 user_cstack[CSTACK];

const u8 ticks_per_sec = 20;
__flash u16 watchdog_ticks = 100;
u8 const rtc_holdoff= FALSE;
u8 mdm_mbox, gps_mbox;
IO_CONF spi_conf, uart_conf;
IO_HANDLE io_modem, io_uart;
MODEM_HANDLE mdm_handle;
u8 usertask_priority = 1;

void FlashFunctions(int option, u32 address, u8 data, u8 numBytes)
{
	u8 id;
	if(FlashPowerIsOn() == false)
	{
		FlashPowerOn();    
	}
	FlashInit();
	switch(option)
	{
		case 1:
		    FlashReadID(id);
			return id;
			break;
		case 2:
			FlashWriteBytes(address, data, numBytes);
			break;

		default:
			break;
	}
	FlashPowerOff();
 }

IO_HANDLE usart_module()
{
	usart_config();
	io_uart = IO_open(IO_UART, &uart_conf);
	IO_subscribe(io_uart, uart_mbox);
	IO_start_transaction(io_uart);
	return uh;
}

void usart_config()
{
	uart_conf.uart_conf.baud = BAUD_57600;
  	uart_conf.uart_conf.bits = EIGHT_BITS;
  	uart_conf.uart_conf.parity = PARITY_NONE;
  	uart_conf.uart_conf.channel = eUART_F0;
  	uart_conf.uart_conf.blocking = FALSE;
}

__task void user_task(void)
{
  	while(1)
	{
		init_routine();
	}
}

void init_routine()
{
	gps_module();
	modem_functions();
	usart_config();
	//POLL DATA FROM ADC's

	FlashFunctions(2, xx,xx,xx);
}

void modem_functions()
{
	BOOL mdm_status;
	int imeibuffer[32];
	MODEM_PARAMS mdm_params;
	mdm_params.pm = ON;
	modem_init();
	io_modem = IO_open(IO_SPI, &spi_conf);
	mdm_handle = Modem_open(mdmtype, io_modem);
	Modem_SetParams(mdm_handle, mdm_params);
	mdm_status = Modem_get_IMEI((u8 *)imeibuffer);
}

void gps_module()
{
	gps_subscribe(gps_mbox);

	gps_unsubscribe(gps_mbox);
}

void sensor_module(SENSOR_TYPE sensor_type, u16 interval)
{
	sensor_mbox = //ALLOCATE!!
	H_SENSOR sensorhandle;
	sensorhandle = sensor_subscribe(sensor_type, MBOX mbox, u16 interval, void *conf);
	void sensor_unsubscribe(H_SENSOR, MBOX);
	
	sensorhandle = sensor_register(SENSOR_TYPE type, SENSOR_FEED feed, MBOX *mbox, u8 task, SENS_CONF_CB config );
	//-----------TODO---------------
	void sensor_report_polled(sensor, u8 *data);

}

