#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "ti.h"
#include "comm.h"

int rs485Set(int dev, u8 mode, u32 baud, u8 stopB, u8 parity, u8 add)
{
	ModbusSetingsType settings;
	u8 buff[5];

	if (baud > 920600 || baud < 1200)
	{
		if (mode == 0)
		{
			baud = 9600;
		}
		else
		{
			printf("Invalid RS485 Baudrate [1200, 920600]!\n");
			return ERROR;
		}
	}
	if (mode > 1)
	{
		printf("Invalid RS485 mode : 0 = disable, 1= Modbus RTU (Slave)!\n");
		return ERROR;
	}
	if (stopB < 1 || stopB > 2)
	{
		if (mode == 0)
		{
			stopB = 1;
		}
		else
		{
			printf("Invalid RS485 stop bits [1, 2]!\n");
			return ERROR;
		}
	}
	if (parity > 2)
	{
		printf(
			"Invalid RS485 parity 0 = none; 1 = even; 2 = odd! Set to none.\n");
		parity = 0;
	}
	if (add < 1)
	{
		printf("Invalid MODBUS device address: [1, 255]! Set to 1.\n");
		add = 1;
	}
	settings.mbBaud = baud;
	settings.mbType = mode;
	settings.mbParity = parity;
	settings.mbStopB = stopB;
	settings.add = add;

	memcpy(buff, &settings, sizeof(ModbusSetingsType));
	if (OK != i2cMem8Write(dev, I2C_MODBUS_SETINGS_ADD, buff, 5))
	{
		printf("Fail to write RS485 settings!\n");
		return ERROR;
	}
	return OK;
}

int rs485Get(int dev)
{
	ModbusSetingsType settings;
	u8 buff[5];

	if (OK != i2cMem8Read(dev, I2C_MODBUS_SETINGS_ADD, buff, 5))
	{
		printf("Fail to read RS485 settings!\n");
		return ERROR;
	}
	memcpy(&settings, buff, sizeof(ModbusSetingsType));
	printf("<mode> <baudrate> <stopbits> <parity> <add> %d %d %d %d %d\n",
		(int)settings.mbType, (int)settings.mbBaud, (int)settings.mbStopB,
		(int)settings.mbParity, (int)settings.add);
	return OK;
}

void doRs485Read(int argc, char *argv[]);
const CliCmdType CMD_RS485_READ =
{
	"cfg485rd",
	1,
	&doRs485Read,
	"\tcfg485rd:	Read the RS485 communication settings\n",
	"\tUsage:		ti cfg485rd\n",
	"",
	"\tExample:		ti cfg485rd; Read the RS485 settings on Board #0\n"};

void doRs485Read(int argc, char *argv[])
{
	int dev = 0;
	UNUSED(argv);
	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 2)
	{
		if (OK != rs485Get(dev))
		{
			printf("Fail to read RS485 settings!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_RS485_READ.usage1);
		exit(1);
	}

}

void doRs485Write(int argc, char *argv[]);
const CliCmdType CMD_RS485_WRITE =
	{
		"cfg485wr",
		1,
		&doRs485Write,
		"\tcfg485wr:	Write the RS485 communication settings\n",
		"\tUsage:		ti cfg485wr <mode> <baudrate> <stopBits> <parity> <slaveAddr>\n",
		"",
		"\tExample:		ti cfg485wr 1 9600 1 0 1; Write the RS485 settings on Board #0 \n\t\t\t(mode = Modbus RTU; baudrate = 9600 bps; stop bits one; parity none; modbus slave address = 1)\n"};

void doRs485Write(int argc, char *argv[])
{
	int dev = 0;
	u8 mode = 0;
	u32 baud = 1200;
	u8 stopB = 1;
	u8 parity = 0;
	u8 add = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 7)
	{
		mode = 0xff & atoi(argv[3]);
		baud = atoi(argv[4]);
		stopB = 0xff & atoi(argv[5]);
		parity = 0xff & atoi(argv[6]);
		add = 0xff & atoi(argv[7]);
		if (OK != rs485Set(dev, mode, baud, stopB, parity, add))
		{
			printf("Fail to write RS485 settings!\n");
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_RS485_WRITE.usage1);
		exit(1);
	}
}