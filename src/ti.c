/*
 * ti.c:
 *	Command-line interface to the Raspberry
 *	Pi's Thermal Integration board.
 *	Copyright (c) 2016-2022 Sequent Microsystem
 *	<http://www.sequentmicrosystem.com>
 ***********************************************************************
 *	Author: Alexandru Burcea
 ***********************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "ti.h"
#include "comm.h"
#include "thread.h"

#define VERSION_BASE	(int)1
#define VERSION_MAJOR	(int)0
#define VERSION_MINOR	(int)5


char *warranty =
	"	       Copyright (c) 2016-2022 Sequent Microsystems\n"
		"                                                             \n"
		"		This program is free software; you can redistribute it and/or modify\n"
		"		it under the terms of the GNU Leser General Public License as published\n"
		"		by the Free Software Foundation, either version 3 of the License, or\n"
		"		(at your option) any later version.\n"
		"                                    \n"
		"		This program is distributed in the hope that it will be useful,\n"
		"		but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		"		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
		"		GNU Lesser General Public License for more details.\n"
		"			\n"
		"		You should have received a copy of the GNU Lesser General Public License\n"
		"		along with this program. If not, see <http://www.gnu.org/licenses/>.";

void usage(void)
{
	int i = 0;
	while (gCmdArray[i] != NULL)
	{
		if (gCmdArray[i]->name != NULL)
		{
			if (strlen(gCmdArray[i]->usage1) > 2)
			{
				printf("%s", gCmdArray[i]->usage1);
			}
			if (strlen(gCmdArray[i]->usage2) > 2)
			{
				printf("%s", gCmdArray[i]->usage2);
			}
		}
		i++;
	}
	printf("Type ti -h <command> for more help\n");
}

int doBoardInit(void)
{
	int dev = 0;
	int add = 0;
	uint8_t buff[8];

	add = SLAVE_OWN_ADDRESS_BASE;
	dev = i2cSetup(add);
	if (dev == -1)
	{
		return ERROR;
	}
	if (ERROR == i2cMem8Read(dev, I2C_REVISION_MAJOR_MEM_ADD, buff, 1))
	{
		printf("Thermal Integration card did not detected!\n");
		return ERROR;
	}
	return dev;
}

int boardCheck(void)
{
	int dev = 0;
	int add = 0;
	uint8_t buff[8];

	add = SLAVE_OWN_ADDRESS_BASE;
	dev = i2cSetup(add);
	if (dev == -1)
	{
		return ERROR;
	}
	if (ERROR == i2cMem8Read(dev, I2C_REVISION_MAJOR_MEM_ADD, buff, 1))
	{

		return ERROR;
	}
	return OK;
}
static void doHelp(int argc, char *argv[]);
const CliCmdType CMD_HELP =
{
	"-h",
	1,
	&doHelp,
	"\t-h		Display the list of command options or one command option details\n",
	"\tUsage:		ti -h    Display command options list\n",
	"\tUsage:		ti -h <param>   Display help for <param> command option\n",
	"\tExample:		ti -h rread    Display help for \"rread\" command option\n"};

static void doHelp(int argc, char *argv[])
{
	int i = 0;
	if (argc == 3)
	{
		while (NULL != gCmdArray[i])
		{
			if (gCmdArray[i]->name != NULL)
			{
				if (strcasecmp(argv[2], gCmdArray[i]->name) == 0)
				{
					printf("%s%s%s%s", gCmdArray[i]->help, gCmdArray[i]->usage1,
						gCmdArray[i]->usage2, gCmdArray[i]->example);
					break;
				}
			}
			i++;
		}
		if (NULL == gCmdArray[i])
		{
			printf("Option \"%s\" not found\n", argv[2]);
			i = 0;
			while (NULL != gCmdArray[i])
			{
				if (gCmdArray[i]->name != NULL)
				{
					printf("%s", gCmdArray[i]->help);
					break;
				}
				i++;
			}
		}
	}
	else
	{
		i = 0;
		while (NULL != gCmdArray[i])
		{
			if (gCmdArray[i]->name != NULL)
			{
				printf("%s", gCmdArray[i]->help);
			}
			i++;
		}
	}
}

static void doVersion(int argc, char *argv[]);
const CliCmdType CMD_VERSION =
{
	"-v",
	1,
	&doVersion,
	"\t-v		Display the ti command version number\n",
	"\tUsage:		ti -v\n",
	"",
	"\tExample:		ti -v  Display the version number\n"};

static void doVersion(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);
	printf("ti v%d.%d.%d Copyright (c) 2016 - 2022 Sequent Microsystems\n",
	VERSION_BASE, VERSION_MAJOR, VERSION_MINOR);
	printf("\nThis is free software with ABSOLUTELY NO WARRANTY.\n");
	printf("For details type: ti -warranty\n");

}

static void doWarranty(int argc, char* argv[]);
const CliCmdType CMD_WAR =
{
	"-warranty",
	1,
	&doWarranty,
	"\t-warranty	Display the warranty\n",
	"\tUsage:		ti -warranty\n",
	"",
	"\tExample:		ti -warranty  Display the warranty text\n"};

static void doWarranty(int argc UNU, char* argv[] UNU)
{
	printf("%s\n", warranty);
}

static void doList(int argc, char *argv[]);
const CliCmdType CMD_LIST =
	{
		"-list",
		1,
		&doList,
		"\t-list:		List all ti boards connected\n\t\t\treturn the # of boards and stack level for every board\n",
		"\tUsage:		ti -list\n",
		"",
		"\tExample:		ti -list display: 1,0 \n"};

static void doList(int argc, char *argv[])
{

	UNUSED(argc);
	UNUSED(argv);

	if (boardCheck() == OK)
	{
		printf("Thermal Integration board detected\n");
	}
	else
	{
		printf("NO Thermal Integration board detected!\n");
	}

}

static void doBoard(int argc, char *argv[]);
const CliCmdType CMD_BOARD =
	{
		"board",
		1,
		&doBoard,
		"\tboard		Display the board status and firmware version number\n",
		"\tUsage:		ti board\n",
		"",
		"\tExample:		ti board  Display vcc, temperature, firmware version, battery voltage \n"};

static void doBoard(int argc, char *argv[])
{
	int dev = -1;
	u8 buff[5];
	int resp = 0;
	int temperature = 25;
	float vIn = 24;
	float vBatt = 5;

	UNUSED(argv);
	if (argc != 2)
	{
		printf("Invalid arguments number type \"ti -h\" for details\n");
		exit(1);
	}
	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}
	resp = i2cMem8Read(dev, I2C_DIAG_TEMPERATURE_MEM_ADD, buff, 5);
	if (FAIL == resp)
	{
		printf("Fail to read board info!\n");
		exit(1);
	}
	temperature = buff[0];
	memcpy(&resp, &buff[1], 2);
	vIn = (float)resp / 1000; //read in milivolts

	memcpy(&resp, &buff[3], 2);
	vBatt = (float)resp / 1000; //read in milivolts	

	resp = i2cMem8Read(dev, I2C_REVISION_MAJOR_MEM_ADD, buff, 2);
	if (FAIL == resp)
	{
		printf("Fail to read board info!\n");
		exit(1);
	}
	printf(
		"Firmware ver %02d.%02d, CPU temperature %d C, Power source %0.2f V, Battery %0.2f V\n",
		(int)buff[0], (int)buff[1], temperature, vIn, vBatt);
}

//*************************************************************
static void doRTCBatt(int argc, char *argv[]);
const CliCmdType CMD_RTC_BATT_READ =
{
	"rtcbrd",
	1,
	&doRTCBatt,
	"\trtcbrd		Display the RTC backup battery voltage\n",
	"\tUsage:		ti rtcbrd\n",
	"",
	"\tExample:		ti rtcbrd -> Display the RTC backup battery voltage \n"};

static void doRTCBatt(int argc, char *argv[])
{
	int dev = -1;
	u8 buff[5];
	int resp = 0;

	float vBatt = 5;

	UNUSED(argv);
	if (argc != 2)
	{
		printf("Invalid arguments number type \"ti -h\" for details\n");
		exit(1);
	}
	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}
	resp = i2cMem8Read(dev, I2C_MEM_RTC_VBAT, buff, 2);
	if (FAIL == resp)
	{
		printf("Fail to read board info!\n");
		exit(1);
	}

	memcpy(&resp, &buff[0], 2);
	vBatt = (float)resp / 1000; //read in milivolts	

	printf("%0.2f V\n", vBatt);
}

//**************************RELAYS**********************************
int relayChSet(int dev, u8 channel, OutStateEnumType state)
{
	int resp = 0;
	u8 buff[2];

	if ( (channel < CHANNEL_NR_MIN) || (channel > RELAY_CH_NR_MAX))
	{
		printf("Invalid relay nr!\n");
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_RELAY_VAL_ADD, buff, 1))
	{
		return FAIL;
	}

	switch (state)
	{
	case OFF:
		buff[0] &= ~ (1 << (channel - 1));
		resp = i2cMem8Write(dev, I2C_RELAY_VAL_ADD, buff, 1);
		break;
	case ON:
		buff[0] |= 1 << (channel - 1);
		resp = i2cMem8Write(dev, I2C_RELAY_VAL_ADD, buff, 1);
		break;
	default:
		printf("Invalid relay state!\n");
		return ERROR;
		break;
	}
	return resp;
}

int relayChGet(int dev, u8 channel, OutStateEnumType* state)
{
	u8 buff[2];

	if (NULL == state)
	{
		return ERROR;
	}

	if ( (channel < CHANNEL_NR_MIN) || (channel > RELAY_CH_NR_MAX))
	{
		printf("Invalid relay nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_RELAY_VAL_ADD, buff, 1))
	{
		return ERROR;
	}

	if (buff[0] & (1 << (channel - 1)))
	{
		*state = ON;
	}
	else
	{
		*state = OFF;
	}
	return OK;
}

int relaySet(int dev, int val)
{
	u8 buff[2];

	buff[0] = 0x0f & val;

	return i2cMem8Write(dev, I2C_RELAY_VAL_ADD, buff, 1);
}

int relayGet(int dev, int* val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_RELAY_VAL_ADD, buff, 1))
	{
		return ERROR;
	}
	*val = buff[0];
	return OK;
}

static void doRelayWrite(int argc, char *argv[]);
const CliCmdType CMD_TRIAC_WRITE =
{
	"relwr",
	1,
	&doRelayWrite,
	"\trelwr:		Set relays (AC switch) On/Off\n",
	"\tUsage:		ti relwr <channel> <0/1>\n",
	"\tUsage:		ti relwr <value>\n",
	"\tExample:		ti relwr 2 1; Turn relay #2 On\n"};

static void doRelayWrite(int argc, char *argv[])
{
	int pin = 0;
	OutStateEnumType state = STATE_COUNT;
	int val = 0;
	int dev = 0;
	OutStateEnumType stateR = STATE_COUNT;
	int valR = 0;
	int retry = 0;

	if ( (argc != 3) && (argc != 4))
	{
		printf("%s", CMD_TRIAC_WRITE.usage1);
		printf("%s", CMD_TRIAC_WRITE.usage2);
		exit(1);
	}

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}
	if (argc == 4)
	{
		pin = atoi(argv[2]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > RELAY_CH_NR_MAX))
		{
			printf("Relay channel number value out of range\n");
			exit(1);
		}

		/**/if ( (strcasecmp(argv[3], "up") == 0)
			|| (strcasecmp(argv[3], "on") == 0))
			state = ON;
		else if ( (strcasecmp(argv[3], "down") == 0)
			|| (strcasecmp(argv[3], "off") == 0))
			state = OFF;
		else
		{
			if ( (atoi(argv[3]) >= STATE_COUNT) || (atoi(argv[3]) < 0))
			{
				printf("Invalid relay state!\n");
				exit(1);
			}
			state = (OutStateEnumType)atoi(argv[3]);
		}

		retry = RETRY_TIMES;

		while ( (retry > 0) && (stateR != state))
		{
			if (OK != relayChSet(dev, pin, state))
			{
				printf("Fail to write relay\n");
				exit(1);
			}
			if (OK != relayChGet(dev, pin, &stateR))
			{
				printf("Fail to read relay\n");
				exit(1);
			}
			retry--;
		}
#ifdef DEBUG_I
		if(retry < RETRY_TIMES)
		{
			printf("retry %d times\n", 3-retry);
		}
#endif
		if (retry == 0)
		{
			printf("Fail to write relay\n");
			exit(1);
		}
	}
	else
	{
		val = atoi(argv[2]);
		if (val < 0 || val > 0x03)
		{
			printf("Invalid relay value\n");
			exit(1);
		}

		retry = RETRY_TIMES;
		valR = -1;
		while ( (retry > 0) && (valR != val))
		{

			if (OK != relaySet(dev, val))
			{
				printf("Fail to write relay!\n");
				exit(1);
			}
			if (OK != relayGet(dev, &valR))
			{
				printf("Fail to read relay!\n");
				exit(1);
			}
		}
		if (retry == 0)
		{
			printf("Fail to write relay!\n");
			exit(1);
		}
	}
}

static void doRelayRead(int argc, char *argv[]);
const CliCmdType CMD_TRIAC_READ =
{
	"relrd",
	1,
	&doRelayRead,
	"\trelrd:		Read relays  status\n",
	"\tUsage:		ti relrd <channel>\n",
	"\tUsage:		ti relrd\n",
	"\tExample:		ti trrd 2; Read Status of Relay #2 \n"};

static void doRelayRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;
	OutStateEnumType state = STATE_COUNT;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		pin = atoi(argv[2]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > RELAY_CH_NR_MAX))
		{
			printf("Relay channel number value out of range!\n");
			exit(1);
		}

		if (OK != relayChGet(dev, pin, &state))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		if (state != 0)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
	}
	else if (argc == 2)
	{
		if (OK != relayGet(dev, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", val);
	}
	else
	{
		printf("%s", CMD_TRIAC_READ.usage1);
		printf("%s", CMD_TRIAC_READ.usage2);
		exit(1);
	}
}

static void doRelayTest(int argc, char* argv[]);
const CliCmdType CMD_TEST =
{
	"reltest",
	1,
	&doRelayTest,
	"\treltest:		Turn ON and OFF the relays until press a key\n",
	"\tUsage:		ti reltest\n",
	"",
	"\tExample:		ti reltest\n"};

static void doRelayTest(int argc, char* argv[])
{
	int dev = 0;
	int i = 0;
	int retry = 0;
	int trVal;
	int valR;
	int relayResult = 0;
	FILE* file = NULL;
	const u8 relayOrder[2] =
	{
		1,
		2, };

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}
	if (argc == 3)
	{
		file = fopen(argv[2], "w");
		if (!file)
		{
			printf("Fail to open result file\n");
			//return -1;
		}
	}
//relay test****************************
	if (strcasecmp(argv[1], "reltest") == 0)
	{
		trVal = 0;
		printf(
			"Are all relays and LEDs turning on and off in sequence?\nPress y for Yes or any key for No....");
		startThread();
		while (relayResult == 0)
		{
			for (i = 0; i < 2; i++)
			{
				relayResult = checkThreadResult();
				if (relayResult != 0)
				{
					break;
				}
				valR = 0;
				trVal = (u8)1 << (relayOrder[i] - 1);

				retry = RETRY_TIMES;
				while ( (retry > 0) && ( (valR & trVal) == 0))
				{
					if (OK != relayChSet(dev, relayOrder[i], ON))
					{
						retry = 0;
						break;
					}

					if (OK != relayGet(dev, &valR))
					{
						retry = 0;
					}
				}
				if (retry == 0)
				{
					printf("Fail to write relay\n");
					if (file)
						fclose(file);
					exit(1);
				}
				busyWait(150);
			}

			for (i = 0; i < 2; i++)
			{
				relayResult = checkThreadResult();
				if (relayResult != 0)
				{
					break;
				}
				valR = 0xff;
				trVal = (u8)1 << (relayOrder[i] - 1);
				retry = RETRY_TIMES;
				while ( (retry > 0) && ( (valR & trVal) != 0))
				{
					if (OK != relayChSet(dev, relayOrder[i], OFF))
					{
						retry = 0;
					}
					if (OK != relayGet(dev, &valR))
					{
						retry = 0;
					}
				}
				if (retry == 0)
				{
					printf("Fail to write relay!\n");
					if (file)
						fclose(file);
					exit(1);
				}
				busyWait(150);
			}
		}
	}
	else
	{
		usage();
		exit(1);
	}
	if (relayResult == YES)
	{
		if (file)
		{
			fprintf(file, "Relay Test ............................ PASS\n");
		}
		else
		{
			printf("Relay Test ............................ PASS\n");
		}
	}
	else
	{
		if (file)
		{
			fprintf(file, "Relay Test ............................ FAIL!\n");
		}
		else
		{
			printf("Relay Test ............................ FAIL!\n");
		}
	}
	if (file)
	{
		fclose(file);
	}
	relaySet(dev, 0);
}

/*****************************************************************
***************************LED's**********************************
 * ***************************************************************
 */
int ledChSet(int dev, u8 channel, OutStateEnumType state)
{
	int resp = 0;
	u8 buff[2];

	if ( (channel < CHANNEL_NR_MIN) || (channel > LED_CH_NR_MAX))
	{
		printf("Invalid led nr!\n");
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_LED_VAL_ADD, buff, 1))
	{
		return FAIL;
	}

	switch (state)
	{
	case OFF:
		buff[0] &= ~ (1 << (channel - 1));
		resp = i2cMem8Write(dev, I2C_LED_VAL_ADD, buff, 1);
		break;
	case ON:
		buff[0] |= 1 << (channel - 1);
		resp = i2cMem8Write(dev, I2C_LED_VAL_ADD, buff, 1);
		break;
	default:
		printf("Invalid led state!\n");
		return ERROR;
		break;
	}
	return resp;
}

int ledChGet(int dev, u8 channel, OutStateEnumType* state)
{
	u8 buff[2];

	if (NULL == state)
	{
		return ERROR;
	}

	if ( (channel < CHANNEL_NR_MIN) || (channel > LED_CH_NR_MAX))
	{
		printf("Invalid led nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_LED_VAL_ADD, buff, 1))
	{
		return ERROR;
	}

	if (buff[0] & (1 << (channel - 1)))
	{
		*state = ON;
	}
	else
	{
		*state = OFF;
	}
	return OK;
}

int ledSet(int dev, int val)
{
	u8 buff[2];

	buff[0] = 0x0f & val;

	return i2cMem8Write(dev, I2C_LED_VAL_ADD, buff, 1);
}

int ledGet(int dev, int* val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_LED_VAL_ADD, buff, 1))
	{
		return ERROR;
	}
	*val = buff[0];
	return OK;
}

static void doLedWrite(int argc, char *argv[]);
const CliCmdType CMD_LED_WRITE =
{
	"ledwr",
	1,
	&doLedWrite,
	"\tledwr:		Set leds On/Off\n",
	"\tUsage:		ti ledwr <channel> <0/1>\n",
	"\tUsage:		ti ledwr <value>\n",
	"\tExample:		ti ledwr 2 1; Turn led #2 On\n"};

static void doLedWrite(int argc, char *argv[])
{
	int pin = 0;
	OutStateEnumType state = STATE_COUNT;
	int val = 0;
	int dev = 0;
	OutStateEnumType stateR = STATE_COUNT;
	int valR = 0;
	int retry = 0;

	if ( (argc != 3) && (argc != 4))
	{
		printf("%s", CMD_LED_WRITE.usage1);
		printf("%s", CMD_LED_WRITE.usage2);
		exit(1);
	}

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}
	if (argc == 4)
	{
		pin = atoi(argv[2]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > LED_CH_NR_MAX))
		{
			printf("Led channel number value out of range\n");
			exit(1);
		}

		/**/if ( (strcasecmp(argv[3], "up") == 0)
			|| (strcasecmp(argv[3], "on") == 0))
			state = ON;
		else if ( (strcasecmp(argv[3], "down") == 0)
			|| (strcasecmp(argv[3], "off") == 0))
			state = OFF;
		else
		{
			if ( (atoi(argv[3]) >= STATE_COUNT) || (atoi(argv[3]) < 0))
			{
				printf("Invalid led state!\n");
				exit(1);
			}
			state = (OutStateEnumType)atoi(argv[3]);
		}

		retry = RETRY_TIMES;

		while ( (retry > 0) && (stateR != state))
		{
			if (OK != ledChSet(dev, pin, state))
			{
				printf("Fail to write led\n");
				exit(1);
			}
			if (OK != ledChGet(dev, pin, &stateR))
			{
				printf("Fail to read led\n");
				exit(1);
			}
			retry--;
		}
#ifdef DEBUG_I
		if(retry < RETRY_TIMES)
		{
			printf("retry %d times\n", 3-retry);
		}
#endif
		if (retry == 0)
		{
			printf("Fail to write led\n");
			exit(1);
		}
	}
	else
	{
		val = atoi(argv[2]);
		if (val < 0 || val > 0x0f)
		{
			printf("Invalid led value\n");
			exit(1);
		}

		retry = RETRY_TIMES;
		valR = -1;
		while ( (retry > 0) && (valR != val))
		{

			if (OK != ledSet(dev, val))
			{
				printf("Fail to write led!\n");
				exit(1);
			}
			if (OK != ledGet(dev, &valR))
			{
				printf("Fail to read led!\n");
				exit(1);
			}
		}
		if (retry == 0)
		{
			printf("Fail to write led!\n");
			exit(1);
		}
	}
}

static void doLedRead(int argc, char *argv[]);
const CliCmdType CMD_LED_READ =
{
	"ledrd",
	1,
	&doLedRead,
	"\tledrd:		Read leds  status\n",
	"\tUsage:		ti ledrd <channel>\n",
	"\tUsage:		ti ledrd\n",
	"\tExample:		ti ledrd 2; Read Status of Led #2 \n"};

static void doLedRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;
	OutStateEnumType state = STATE_COUNT;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		pin = atoi(argv[2]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > LED_CH_NR_MAX))
		{
			printf("Led channel number value out of range!\n");
			exit(1);
		}

		if (OK != ledChGet(dev, pin, &state))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		if (state != 0)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
	}
	else if (argc == 2)
	{
		if (OK != ledGet(dev, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", val);
	}
	else
	{
		printf("%s", CMD_LED_READ.usage1);
		printf("%s", CMD_LED_READ.usage2);
		exit(1);
	}
}

static void doLedTest(int argc, char* argv[]);
const CliCmdType CMD_LED_TEST =
{
	"ledtest",
	1,
	&doLedTest,
	"\tledtest:		Turn ON and OFF the leds until press a key\n",
	"\tUsage:		ti ledtest\n",
	"",
	"\tExample:		ti ledtest\n"};

static void doLedTest(int argc, char* argv[])
{
	int dev = 0;
	int i = 0;
	int retry = 0;
	int trVal;
	int valR;
	int ledResult = 0;
	FILE* file = NULL;
	const u8 ledOrder[LED_CH_NR_MAX] =
	{
		1,
		2,
		3,
		4,};

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}
	if (argc == 3)
	{
		file = fopen(argv[2], "w");
		if (!file)
		{
			printf("Fail to open result file\n");
			//return -1;
		}
	}
//led test****************************
	if (strcasecmp(argv[1], "ledtest") == 0)
	{
		trVal = 0;
		printf(
			"Are all leds and LEDs turning on and off in sequence?\nPress y for Yes or any key for No....");
		startThread();
		while (ledResult == 0)
		{
			for (i = 0; i < LED_CH_NR_MAX; i++)
			{
				ledResult = checkThreadResult();
				if (ledResult != 0)
				{
					break;
				}
				valR = 0;
				trVal = (u8)1 << (ledOrder[i] - 1);

				retry = RETRY_TIMES;
				while ( (retry > 0) && ( (valR & trVal) == 0))
				{
					if (OK != ledChSet(dev, ledOrder[i], ON))
					{
						retry = 0;
						break;
					}

					if (OK != ledGet(dev, &valR))
					{
						retry = 0;
					}
				}
				if (retry == 0)
				{
					printf("Fail to write led\n");
					if (file)
						fclose(file);
					exit(1);
				}
				busyWait(150);
			}

			for (i = 0; i < LED_CH_NR_MAX; i++)
			{
				ledResult = checkThreadResult();
				if (ledResult != 0)
				{
					break;
				}
				valR = 0xff;
				trVal = (u8)1 << (ledOrder[i] - 1);
				retry = RETRY_TIMES;
				while ( (retry > 0) && ( (valR & trVal) != 0))
				{
					if (OK != ledChSet(dev, ledOrder[i], OFF))
					{
						retry = 0;
					}
					if (OK != ledGet(dev, &valR))
					{
						retry = 0;
					}
				}
				if (retry == 0)
				{
					printf("Fail to write led!\n");
					if (file)
						fclose(file);
					exit(1);
				}
				busyWait(150);
			}
		}
	}
	else
	{
		usage();
		exit(1);
	}
	if (ledResult == YES)
	{
		if (file)
		{
			fprintf(file, "Led Test ............................ PASS\n");
		}
		else
		{
			printf("Led Test ............................ PASS\n");
		}
	}
	else
	{
		if (file)
		{
			fprintf(file, "Led Test ............................ FAIL!\n");
		}
		else
		{
			printf("Led Test ............................ FAIL!\n");
		}
	}
	if (file)
	{
		fclose(file);
	}
	ledSet(dev, 0);
}


//***********************CONTACTS ***********************************

int contactChGet(int dev, u8 channel, OutStateEnumType* state)
{
	u8 buff[2];

	if (NULL == state)
	{
		return ERROR;
	}

	if ( (channel < CHANNEL_NR_MIN) || (channel > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_DRY_CONTACT_VAL_ADD, buff, 1))
	{
		return ERROR;
	}

	if (buff[0] & (1 << (channel - 1)))
	{
		*state = ON;
	}
	else
	{
		*state = OFF;
	}
	return OK;
}

int contactGet(int dev, int* val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_DRY_CONTACT_VAL_ADD, buff, 1))
	{
		return ERROR;
	}
	*val = buff[0];
	return OK;
}

int contactCountGet(int dev, u8 ch, u32* val)
{
	u8 buff[4];

	if (NULL == val)
	{
		return ERROR;
	}
	if ( (ch < CHANNEL_NR_MIN) || (ch > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}
	if (FAIL
		== i2cMem8Read(dev, I2C_MEM_DRY_CONTACT_CONTORS + 4 * (ch - 1), buff, 4))
	{
		return ERROR;
	}
	memcpy(val, buff, 4);
	return OK;
}

int contactPpsGet(int dev, u8 ch, u16* val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if ( (ch < CHANNEL_NR_MIN) || (ch > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}
	if (FAIL
		== i2cMem8Read(dev, I2C_MEM_DRY_CONTACT_PPS1 + 2 * (ch - 1), buff, 2))
	{
		return ERROR;
	}
	memcpy(val, buff, 2);
	return OK;
}

int contactCountReset(int dev, u8 channel)
{
	u8 buff[4];

	if ( (channel < CHANNEL_NR_MIN) || (channel > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}
	buff[0] = channel;
	if (FAIL == i2cMem8Write(dev, I2C_MEM_DRY_CONTACT_CH_CONT_RESET, buff, 1))
	{
		printf("Fail to reset contor! \n");
		return ERROR;
	}
	return OK;
}

int contactCountRisingSet(int dev, u8 channel, u8 state)
{
	u8 buff[4];

	if ( (channel < CHANNEL_NR_MIN) || (channel > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_DRY_CONTACT_RISING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	if (state != 0)
	{
		buff[0] |= 1 << (channel - 1);
	}
	else
	{
		buff[0] &= ~ (1 << (channel - 1));
	}
	if (FAIL == i2cMem8Write(dev, I2C_MEM_DRY_CONTACT_RISING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	return OK;
}

int contactCountFallingSet(int dev, u8 channel, u8 state)
{
	u8 buff[4];

	if ( (channel < CHANNEL_NR_MIN) || (channel > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_DRY_CONTACT_FALLING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	if (state != 0)
	{
		buff[0] |= 1 << (channel - 1);
	}
	else
	{
		buff[0] &= ~ (1 << (channel - 1));
	}
	if (FAIL == i2cMem8Write(dev, I2C_MEM_DRY_CONTACT_FALLING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	return OK;
}

int contactCountRisingGet(int dev, u8 channel, u8* state)
{
	u8 buff[4];

	if (NULL == state)
	{
		return ERROR;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_DRY_CONTACT_RISING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	if ( (buff[0] & (1 << (channel - 1))) != 0)
	{
		*state = 1;
	}
	else
	{
		*state = 0;
	}
	return OK;
}

int contactCountFallingGet(int dev, u8 channel, u8* state)
{
	u8 buff[4];

	if (NULL == state)
	{
		return ERROR;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_DRY_CONTACT_FALLING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	if ( (buff[0] & (1 << (channel - 1))) != 0)
	{
		*state = 1;
	}
	else
	{
		*state = 0;
	}
	return OK;
}

static void doContactRead(int argc, char *argv[]);
const CliCmdType CMD_CONTACT_READ =
	{
		"contactrd",
		1,
		&doContactRead,
		"\tcontactrd:	Read dry contact status\n\t\t\tWarning: For this measurements to be valid place the jumper in position \"1K\" \n",
		"\tUsage:		ti contactrd <channel>\n",
		"\tUsage:		ti contactrd\n",
		"\tExample:		ti contactrd 2; Read Status of Dry contact pin #2\n"};

static void doContactRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;
	OutStateEnumType state = STATE_COUNT;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		pin = atoi(argv[2]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > CONTACT_CH_NR_MAX))
		{
			printf("Dry contact number value out of range!\n");
			exit(1);
		}

		if (OK != contactChGet(dev, pin, &state))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		if (state != 0)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
	}
	else if (argc == 2)
	{
		if (OK != contactGet(dev, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", val);
	}
	else
	{
		printf("%s", CMD_CONTACT_READ.usage1);
		printf("%s", CMD_CONTACT_READ.usage2);
		exit(1);
	}
}

static void doCountRead(int argc, char *argv[]);
const CliCmdType CMD_COUNTER_READ =
	{
		"countrd",
		1,
		&doCountRead,
		"\tcountrd:	Read dry contact transitions count\n\t\t\tWarning: For this measurements to be valid place the jumper in position \"1K\" \n",
		"\tUsage:		ti countrd <channel>\n",
		"",
		"\tExample:		ti countrd 2; Read transitions count of dry contact pin #2\n"};

static void doCountRead(int argc, char *argv[])
{
	u8 pin = 0;
	u32 val = 0;
	int dev = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		pin = (u8)atoi(argv[2]);

		if (OK != contactCountGet(dev, pin, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%u\n", (unsigned int)val);
	}
	else
	{
		printf("%s", CMD_COUNTER_READ.usage1);
		exit(1);
	}
}

static void doPpsRead(int argc, char *argv[]);
const CliCmdType CMD_PPS_READ =
	{
		"ppsrd",
		1,
		&doPpsRead,
		"\tppsrd:	Read dry contact transitions per second\n\t\t\tWarning: For this measurements to be valid place the jumper in position \"1K\" \n",
		"\tUsage:		ti ppsrd <channel>\n",
		"",
		"\tExample:		ti ppsrd 2; Read transitions counts per second of dry contact pin #2\n"};

static void doPpsRead(int argc, char *argv[])
{
	u8 pin = 0;
	u16 val = 0;
	int dev = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		pin = (u8)atoi(argv[2]);

		if (OK != contactPpsGet(dev, pin, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%u\n", (unsigned int)val);
	}
	else
	{
		printf("%s", CMD_PPS_READ.usage1);
		exit(1);
	}
}

static void doCountReset(int argc, char *argv[]);
const CliCmdType CMD_COUNTER_RST =
{
	"countrst",
	1,
	&doCountReset,
	"\tcountrst:	Reset dry contact transitions count\n",
	"\tUsage:		ti countrst <channel>\n",
	"",
	"\tExample:		ti countrst 2; Reset transitions count of dry contact pin #2\n"};

static void doCountReset(int argc, char *argv[])
{
	u8 pin = 0;
	int dev = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		pin = (u8)atoi(argv[2]);

		if (OK != contactCountReset(dev, pin))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("%s", CMD_COUNTER_RST.usage1);
		exit(1);
	}
}

static void doEdgeRead(int argc, char *argv[]);
const CliCmdType CMD_EDGE_READ =
	{
		"edgerd",
		1,
		&doEdgeRead,
		"\tedgerd:		Read dry contact transitions type, ret 0 - disable, 1 - rising, 2 - falling, 3 - both\n",
		"\tUsage:		ti edgerd <channel> \n",
		"",
		"\tExample:		ti edgerd 2; Read transitions type of dry contact pin #2\n"};

static void doEdgeRead(int argc, char *argv[])
{
	u8 pin = 0;
	u8 rising = 0;
	u8 falling = 0;
	int dev = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		pin = (u8)atoi(argv[2]);

		if (OK != contactCountRisingGet(dev, pin, &rising))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		if (OK != contactCountFallingGet(dev, pin, &falling))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", (int) (rising + falling * 2));
	}
	else
	{
		printf("%s", CMD_EDGE_READ.usage1);
		exit(1);
	}
}

static void doEdgeWrite(int argc, char *argv[]);
const CliCmdType CMD_EDGE_WRITE =
	{
		"edgewr",
		1,
		&doEdgeWrite,
		"\tedgewr:		Write dry contact transitions type: 0 - disable, 1 - rising, 2 - falling, 3 - both\n",
		"\tUsage:		ti edgewr <channel> <val>\n",
		"",
		"\tExample:		ti edgewr 2 1; Set transitions type of dry contact pin #2 to rising\n"};

static void doEdgeWrite(int argc, char *argv[])
{
	u8 pin = 0;
	u8 rising = 0;
	u8 falling = 0;
	int dev = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		pin = (u8)atoi(argv[2]);

		if ( (1 & atoi(argv[3])) != 0)
		{
			rising = 1;
		}
		if ( (2 & atoi(argv[3])) != 0)
		{
			falling = 1;
		}
		if (OK != contactCountRisingSet(dev, pin, rising))
		{
			printf("Fail to write!\n");
			exit(1);
		}
		if (OK != contactCountFallingSet(dev, pin, falling))
		{
			printf("Fail to write!\n");
			exit(1);
		}

	}
	else
	{
		printf("%s", CMD_EDGE_WRITE.usage1);
		exit(1);
	}
}

int dacGet(int dev, int ch, float* val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > DAC_CH_NR_MAX))
	{
		printf("DAC channel out of range!\n");
		return ERROR;
	}
	if (OK != i2cMem8Read(dev, I2C_U0_10_OUT_VAL1_ADD + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to read!\n");
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 1000;
	return OK;
}

int dacSet(int dev, int ch, float val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > DAC_CH_NR_MAX))
	{
		printf("DAC channel out of range!\n");
		return ERROR;
	}
	if (val < 0)
	{
		val = 0;
	}
	if (val > 10)
	{
		val = 10;
	}
	raw = (u16)ceil(val * 1000); //transform to milivolts
	memcpy(buff, &raw, 2);
	if (OK != i2cMem8Write(dev, I2C_U0_10_OUT_VAL1_ADD + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to write!\n");
		return ERROR;
	}
	return OK;
}

static void doDacRead(int argc, char *argv[]);
const CliCmdType CMD_DAC_READ =
{
	"dacrd",
	1,
	&doDacRead,
	"\tdacrd:		Read DAC voltage value (0 - 10V)\n",
	"\tUsage:		ti dacrd <channel>\n",
	"",
	"\tExample:		ti dacrd 2; Read the voltage on 0-10V out channel #2\n"};

static void doDacRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		ch = atoi(argv[2]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > DAC_CH_NR_MAX))
		{
			printf("DAC channel out of range!\n");
			exit(1);
		}

		if (OK != dacGet(dev, ch, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}

		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_DAC_READ.usage1);
		exit(1);
	}
}

static void doDacWrite(int argc, char *argv[]);
const CliCmdType CMD_DAC_WRITE =
{
	"dacwr",
	1,
	&doDacWrite,
	"\tdacwr:		Write DAC output voltage value (0..10V)\n",
	"\tUsage:		ti dacwr <channel> <value>\n",
	"",
	"\tExample:		ti dacwr 2 2.5; Write 2.5V to 0-10V out channel #2\n"};

static void doDacWrite(int argc, char *argv[])
{
	int ch = 0;
	int dev = 0;
	float volt = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[2]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > DAC_CH_NR_MAX))
		{
			printf("DAC channel out of range!\n");
			exit(1);
		}
		volt = atof(argv[3]);
		if (volt < 0 || volt > 10)
		{
			printf("Invalid DAC voltage value, must be 0..10 \n");
			exit(1);
		}

		if (OK != dacSet(dev, ch, volt))
		{
			printf("Fail to write!\n");
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_DAC_WRITE.usage1);
		exit(1);
	}
}

int odGet(int dev, int ch, float* val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > OD_CH_NR_MAX))
	{
		printf("Open drain output channel out of range!\n");
		return ERROR;
	}
	if (OK != i2cMem8Read(dev, I2C_MEM_OD_VAL1 + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to read!\n");
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 100;
	return OK;
}

int odSet(int dev, int ch, float val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > OD_CH_NR_MAX))
	{
		printf("Open drain output channel out of range!\n");
		return ERROR;
	}
	if (val < 0)
	{
		val = 0;
	}
	if (val > 100)
	{
		val = 100;
	}
	raw = (u16)ceil(val * 100);
	memcpy(buff, &raw, 2);
	if (OK != i2cMem8Write(dev, I2C_MEM_OD_VAL1 + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to write!\n");
		return ERROR;
	}
	return OK;
}

static void doOdRead(int argc, char *argv[]);
const CliCmdType CMD_OD_READ =
{
	"odrd",
	1,
	&doOdRead,
	"\todrd:		Read open drain pwm value (0 - 100%)\n",
	"\tUsage:		ti odrd <channel>\n",
	"",
	"\tExample:		ti odrd 2; Read the pwm on open drain out channel #2\n"};

static void doOdRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		ch = atoi(argv[2]);

		if (OK != odGet(dev, ch, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}

		printf("%0.2f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_OD_READ.usage1);
		exit(1);
	}
}

static void doOdWrite(int argc, char *argv[]);
const CliCmdType CMD_OD_WRITE =
	{
		"odwr",
		1,
		&doOdWrite,
		"\todwr:		Write the open drain output pwm value (0..100%)\n",
		"\tUsage:		ti odwr <channel> <value>\n",
		"",
		"\tExample:		ti odwr 2 2.5; Write 2.5% PWM to open drain output channel #2\n"};

static void doOdWrite(int argc, char *argv[])
{
	int ch = 0;
	int dev = 0;
	float pwm = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[2]);

		pwm = atof(argv[3]);
		if (pwm < 0 || pwm > 100)
		{
			printf("Invalid pwm value, must be 0..100 \n");
			exit(1);
		}

		if (OK != odSet(dev, ch, pwm))
		{
			printf("Fail to write!\n");
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_OD_WRITE.usage1);
		exit(1);
	}
}

int adcGet(int dev, int ch, float* val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
	{
		printf("ADC channel out of range!\n");
		return ERROR;
	}
	if (OK != i2cMem8Read(dev, I2C_U0_10_IN_VAL1_ADD + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to read!\n");
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 1000;
	return OK;
}

static void doAdcRead(int argc, char *argv[]);
const CliCmdType CMD_ADC_READ =
	{
		"adcrd",
		1,
		&doAdcRead,
		"\tadcrd:		Read ADC input voltage value (0 - 10V)\n\t\t\tWarning: For this measurements to be valid place the jumper in position \"0-10V\" \n",
		"\tUsage:		ti adcrd <channel>\n",
		"",
		"\tExample:		ti adcrd 2; Read the voltage input on 0-10V in channel #2\n"};

static void doAdcRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		ch = atoi(argv[2]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
		{
			printf("ADC channel out of range!\n");
			exit(1);
		}

		if (OK != adcGet(dev, ch, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}

		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_ADC_READ.usage1);
		exit(1);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
int r1kGet(int dev, int ch, float* val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
	{
		printf("ADC channel out of range!\n");
		return ERROR;
	}
	if (OK != i2cMem8Read(dev, I2C_R_1K_CH1 + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to read!\n");
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 1000;
	return OK;
}

static void doR1kRead(int argc, char *argv[]);
const CliCmdType CMD_R1K_READ =
	{
		"r1krd",
		1,
		&doR1kRead,
		"\tr1krd:		Read resistor input  value (1k) in kiloOhms, Warning: You must palce the jumper in position \"1K\".\n\t\t\tReturn 30kOhm for resistor out of range\n",
		"\tUsage:		ti r1krd <channel>\n",
		"",
		"\tExample:	ti r1krd 2; Read the resistance input on channel #2\n"};

static void doR1kRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		ch = atoi(argv[2]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
		{
			printf("Resistor read channel out of range!\n");
			exit(1);
		}

		if (OK != r1kGet(dev, ch, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}

		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_R1K_READ.usage1);
		exit(1);
	}
}

int r10kGet(int dev, int ch, float* val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
	{
		printf("ADC channel out of range!\n");
		return ERROR;
	}
	if (OK != i2cMem8Read(dev, I2C_R_10K_CH1 + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to read!\n");
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 1000;
	return OK;
}

static void doR10kRead(int argc, char *argv[]);
const CliCmdType CMD_R10K_READ =
	{
		"r10krd",
		1,
		&doR10kRead,
		"\tr10krd:		Read resistor input  value (10k) in kiloOhms, Warning: You must palce the jumper in position \"10K\".\n\t\t\tReturn 30kOhm for resistor out of range\n",
		"\tUsage:		ti r10krd <channel>\n",
		"",
		"\tExample:	ti r10krd 2; Read the resistance input on channel #2\n"};

static void doR10kRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		ch = atoi(argv[2]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
		{
			printf("Resistor read channel out of range!\n");
			exit(1);
		}

		if (OK != r10kGet(dev, ch, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}

		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_R10K_READ.usage1);
		exit(1);
	}
}

//#####################################################################
int rThermGet(int dev, int ch, float* val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > THERM_CH_NO))
	{
		printf("Thermistors channel out of range!\n");
		return ERROR;
	}
	if (OK != i2cMem8Read(dev, I2C_THERM_CH1 + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to read!\n");
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 1000;
	return OK;
}

static void doRThRead(int argc, char *argv[]);
const CliCmdType CMD_RTH_READ =
	{
		"rthrd",
		1,
		&doRThRead,
		"\trthrd:		Read thermistor input  resistance value in kiloOhms, Return 60kOhm for resistor out of range\n",
		"\tUsage:		ti rthrd <channel>\n",
		"",
		"\tExample:	ti rthrd 2; Read the thermistor resistance on channel #2\n"};

static void doRThRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		ch = atoi(argv[2]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > THERM_CH_NO))
		{
			printf("Thermistor read channel out of range!\n");
			exit(1);
		}

		if (OK != rThermGet(dev, ch, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}

		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_RTH_READ.usage1);
		exit(1);
	}
}

/**
 * Read the pwm filling factor
 */

int rPwmGet(int dev, int ch, float* val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > PWM_IN_CH_NO))
	{
		printf("PWM in channel out of range!\n");
		return ERROR;
	}
	if (OK != i2cMem8Read(dev, I2C_MEM_PWM_IN1 + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to read!\n");
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 10; //express in precent
	return OK;
}

static void doPWMRead(int argc, char *argv[]);
const CliCmdType CMD_PWM_READ =
{
	"pwmrd",
	1,
	&doPWMRead,
	"\tpwmrd:		Read PWM input\n",
	"\tUsage:		ti pwmrd <channel>\n",
	"",
	"\tExample:	ti pwmrd 2; Read the pwm input filling factor on channel #2\n"};

static void doPWMRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		ch = atoi(argv[2]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > PWM_IN_CH_NO))
		{
			printf("PWM in channel out of range!\n");
			exit(1);
		}

		if (OK != rPwmGet(dev, ch, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}

		printf("%0.1f %%\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_PWM_READ.usage1);
		exit(1);
	}
}

/**
 * Calibration functions
 */
void getCalStat(int dev)
{
	u8 buff[2];

	busyWait(100);
	if (OK != i2cMem8Read(dev, I2C_MEM_CALIB_STATUS, buff, 1))
	{
		printf("Fail to read calibration status!\n");
		exit(1);
	}
	switch (buff[0])
	{
	case 0:
		printf("Calibration in progress\n");
		break;
	case 1:
		printf("Calibration done\n");
		break;
	case 2:
		printf("Calibration error!\n");
		break;
	default:
		printf("Unknown calibration status\n");
		break;
	}
}

static void doAdcCal(int argc, char *argv[]);
const CliCmdType CMD_ADC_CAL =
	{
		"adccal",
		1,
		&doAdcCal,
		"\tadccal:		Calibrate one ADC channel, the calibration must be done in 2 points at min 5V apart\n",
		"\tUsage:		ti adccal <channel> <value>\n",
		"",
		"\tExample:		ti adccal 2 0.5; Calibrate the voltage input on ADC channel #2   at 0.5V\n"};

static void doAdcCal(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;
	u8 buff[4] =
	{
		0,
		0};
	u16 raw = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[2]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
		{
			printf("ADC channel out of range!\n");
			exit(1);
		}

		val = atof(argv[3]);
		if ( (val < 0) || (val > 10))
		{
			printf("ADC calibration value out of range!\n");
			exit(1);
		}
		raw = (u16)ceil(val * VOLT_TO_MILIVOLT);
		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_0_10V_IN_START_ID - 1;
		buff[3] = CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_ADC_CAL.usage1);
		exit(1);
	}
}

static void doAdcCalRst(int argc, char *argv[]);
const CliCmdType CMD_ADC_CAL_RST =
	{
		"adccalrst",
		1,
		&doAdcCalRst,
		"\tadccalrst:	Reset the calibration for one ADC channel\n",
		"\tUsage:		ti adccalrst <channel>\n",
		"",
		"\tExample:		ti adccalrst 2 ; Reset the calibration on ADC channel #2   at factory default\n"};

static void doAdcCalRst(int argc, char *argv[])
{
	int ch = 0;

	int dev = 0;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		ch = atoi(argv[2]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
		{
			printf("ADC channel out of range!\n");
			exit(1);
		}

		buff[2] = ch + CAL_0_10V_IN_START_ID - 1;
		;
		buff[3] = RESET_CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_ADC_CAL_RST.usage1);
		exit(1);
	}
}

static void doDacCal(int argc, char *argv[]);
const CliCmdType CMD_DAC_CAL =
	{
		"daccal",
		1,
		&doDacCal,
		"\tdaccal:		Calibrate one DAC channel, the calibration must be done in 2 points at min 5V apart\n",
		"\tUsage:		ti daccal <channel> <value>\n",
		"",
		"\tExample:		ti daccal 2 0.5; Calibrate the voltage outputs on DAC channel #2   at 0.5V\n"};

static void doDacCal(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;
	u8 buff[4] =
	{
		0,
		0};
	u16 raw = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[2]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > DAC_CH_NR_MAX))
		{
			printf("DAC channel out of range!\n");
			exit(1);
		}

		val = atof(argv[3]);
		if ( (val < 0) || (val > 10))
		{
			printf("DAC calibration value out of range!\n");
			exit(1);
		}
		raw = (u16)ceil(val * VOLT_TO_MILIVOLT);
		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_0_10V_OUT_START_ID - 1;
		buff[3] = CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_DAC_CAL.usage1);
		exit(1);
	}
}

static void doDacCalRst(int argc, char *argv[]);
const CliCmdType CMD_DAC_CAL_RST =
	{
		"daccalrst",
		1,
		&doDacCalRst,
		"\tdaccalrst:	Reset calibration for one DAC channel\n",
		"\tUsage:		ti daccalrst <channel>\n",
		"",
		"\tExample:		ti daccalrst 2; Reset calibration data on DAC channel #2   at factory default\n"};

static void doDacCalRst(int argc, char *argv[])
{
	int ch = 0;

	int dev = 0;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};
	u16 raw = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		ch = atoi(argv[2]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > DAC_CH_NR_MAX))
		{
			printf("DAC channel out of range!\n");
			exit(1);
		}

		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_0_10V_OUT_START_ID - 1;
		buff[3] = RESET_CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}
		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_DAC_CAL_RST.usage1);
		exit(1);
	}
}

static void doWdtReload(int argc, char *argv[]);
const CliCmdType CMD_WDT_RELOAD =
	{
		"wdtr",
		1,
		&doWdtReload,
		"\twdtr:		Reload the watchdog timer and enable the watchdog if is disabled\n",
		"\tUsage:		ti wdtr\n",
		"",
		"\tExample:		ti wdtr; Reload the watchdog timer   with the period \n"};

static void doWdtReload(int argc, char *argv[])
{
	int dev = 0;
	u8 buff[2] =
	{
		0,
		0};

	UNUSED(argv);
	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 2)
	{
		buff[0] = WDT_RESET_SIGNATURE;
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_RESET_ADD, buff, 1))
		{
			printf("Fail to write watchdog reset key!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_RELOAD.usage1);
		exit(1);
	}
}

static void doWdtSetPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_PERIOD =
	{
		"wdtpwr",
		1,
		&doWdtSetPeriod,
		"\twdtpwr:		Set the watchdog period in seconds, \n\t\t\treload command must be issue in this interval to prevent Raspberry Pi power off\n",
		"\tUsage:		ti wdtpwr <val> \n",
		"",
		"\tExample:		ti wdtpwr 10; Set the watchdog timer period   at 10 seconds \n"};

static void doWdtSetPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		period = (u16)atoi(argv[2]);
		if (0 == period)
		{
			printf("Invalid period!\n");
			exit(1);
		}
		memcpy(buff, &period, 2);
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_INTERVAL_SET_ADD, buff, 2))
		{
			printf("Fail to write watchdog period!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtGetPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_PERIOD =
	{
		"wdtprd",
		1,
		&doWdtGetPeriod,
		"\twdtprd:		Get the watchdog period in seconds, \n\t\t\treload command must be issue in this interval to prevent Raspberry Pi power off\n",
		"\tUsage:		ti wdtprd \n",
		"",
		"\tExample:		ti wdtprd; Get the watchdog timer period  \n"};

static void doWdtGetPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] =
	{
		0,
		0};

	UNUSED(argv);
	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 2)
	{
		if (OK != i2cMem8Read(dev, I2C_MEM_WDT_INTERVAL_GET_ADD, buff, 2))
		{
			printf("Fail to read watchdog period!\n");
			exit(1);
		}
		memcpy(&period, buff, 2);
		printf("%d\n", (int)period);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtSetInitPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_INIT_PERIOD =
	{
		"wdtipwr",
		1,
		&doWdtSetInitPeriod,
		"\twdtipwr:	Set the watchdog initial period in seconds, \n\t\t\tThis period is loaded after power cycle, giving Raspberry time to boot\n",
		"\tUsage:		ti wdtipwr <val> \n",
		"",
		"\tExample:		ti wdtipwr 10; Set the watchdog timer initial period   at 10 seconds \n"};

static void doWdtSetInitPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		period = (u16)atoi(argv[2]);
		if (0 == period)
		{
			printf("Invalid period!\n");
			exit(1);
		}
		memcpy(buff, &period, 2);
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_INIT_INTERVAL_SET_ADD, buff, 2))
		{
			printf("Fail to write watchdog period!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_INIT_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtGetInitPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_INIT_PERIOD =
	{
		"wdtiprd",
		1,
		&doWdtGetInitPeriod,
		"\twdtiprd:	Get the watchdog initial period in seconds. \n\t\t\tThis period is loaded after power cycle, giving Raspberry time to boot\n",
		"\tUsage:		ti wdtiprd \n",
		"",
		"\tExample:		ti wdtiprd; Get the watchdog timer initial period  \n"};

static void doWdtGetInitPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] =
	{
		0,
		0};

	UNUSED(argv);
	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 2)
	{
		if (OK != i2cMem8Read(dev, I2C_MEM_WDT_INIT_INTERVAL_GET_ADD, buff, 2))
		{
			printf("Fail to read watchdog period!\n");
			exit(1);
		}
		memcpy(&period, buff, 2);
		printf("%d\n", (int)period);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_INIT_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtSetOffPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_OFF_PERIOD =
	{
		"wdtopwr",
		1,
		&doWdtSetOffPeriod,
		"\twdtopwr:	Set the watchdog off period in seconds (max 48 days). \n\t\t\tThis is the time that watchdog mantain Raspberry turned off \n",
		"\tUsage:		ti wdtopwr <val> \n",
		"",
		"\tExample:		ti wdtopwr 10; Set the watchdog off interval   at 10 seconds \n"};

static void doWdtSetOffPeriod(int argc, char *argv[])
{
	int dev = 0;
	u32 period;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		period = (u32)atoi(argv[2]);
		if ( (0 == period) || (period > WDT_MAX_OFF_INTERVAL_S))
		{
			printf("Invalid period!\n");
			exit(1);
		}
		memcpy(buff, &period, 4);
		if (OK
			!= i2cMem8Write(dev, I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD, buff, 4))
		{
			printf("Fail to write watchdog period!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_OFF_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtGetOffPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_OFF_PERIOD =
	{
		"wdtoprd",
		1,
		&doWdtGetOffPeriod,
		"\twdtoprd:	Get the watchdog off period in seconds (max 48 days) \n\t\t\tThis is the time that watchdog mantain Raspberry turned off \n",
		"\tUsage:		ti wdtoprd \n",
		"",
		"\tExample:		ti wdtoprd; Get the watchdog off period  \n"};

static void doWdtGetOffPeriod(int argc, char *argv[])
{
	int dev = 0;
	u32 period;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};

	UNUSED(argv);
	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 2)
	{
		if (OK
			!= i2cMem8Read(dev, I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD, buff, 4))
		{
			printf("Fail to read watchdog period!\n");
			exit(1);
		}
		memcpy(&period, buff, 4);
		printf("%d\n", (int)period);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_OFF_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtSetStayOnBattery(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_STAY_ON_BATTERY =
	{
		"sobwr",
		1,
		&doWdtSetStayOnBattery,
		"\tsobwr:	Set the \"Stay on Battery\" parameter. \n\t\t\tIf this is 0 when watchdog expire and main power is not present, the card shut down until the main power is back \n",
		"\tUsage:		ti sobwr <val> (0/1)\n",
		"",
		"\tExample:		ti sobwr 1; Turn on the \"Stay on Battery\" parameter.\n"};

static void doWdtSetStayOnBattery(int argc, char *argv[])
{
	int dev = 0;
	u8 state = 0;


	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{

		if(state <= atoi(argv[2]))
		{
			state = 0;
		}
		if (atoi(argv[2]) >= 1)
		{
			state = 1;
		}

		if (OK
			!= i2cMem8Write(dev, I2C_WDT_KEEP_POWER_ON_BATTERY, &state, 1))
		{
			printf("Fail to write Stay on Battery parameter!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_STAY_ON_BATTERY.usage1);
		exit(1);
	}
}

static void doWdtGetStayOnBattery(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_STAY_ON_BATTERY =
	{
		"sobrd",
		1,
		&doWdtGetStayOnBattery,
		"\tsobrd:	Returns the \"Stay on Battery\" parameter. \n\t\t\tIf this is 0 when watchdog expire and main power is not present, the card shut down until the main power is back \n",
		"\tUsage:		ti sobrd\n",
		"",
		"\tExample:		ti sobrd; Returns 0 or 1  \n"};

static void doWdtGetStayOnBattery(int argc, char *argv[])
{
	int dev = 0;
	u8 value = 0;

	UNUSED(argv);
	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 2)
	{
		if (OK != i2cMem8Read(dev, I2C_WDT_KEEP_POWER_ON_BATTERY, &value, 1))
		{
			printf("Fail to read Stay on battery parameter!\n");
			exit(1);
		}

		printf("%d\n", (int)value);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_PERIOD.usage1);
		exit(1);
	}
}



int rtcBatteryVoltGet(int dev, float* val)
{
	u8 buff[2];
	u16 raw = 0;

	if (NULL == val)
	{
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_RTC_VBAT, buff, 2))
	{
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 1000;
	return OK;
}
static void doRTCGet(int argc, char *argv[]);
const CliCmdType CMD_RTC_GET =
{
	"rtcrd",
	1,
	&doRTCGet,
	"\trtcrd:		Get the internal RTC  date and time(mm/dd/yy hh:mm:ss)\n",
	"\tUsage:		ti rtcrd \n",
	"",
	"\tExample:	ti rtcrd; Get the nternal RTC time and date  \n"};

static void doRTCGet(int argc, char *argv[])
{
	int dev = 0;
	u8 buff[6];

	UNUSED(argv);
	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 2)
	{
		if (OK != i2cMem8Read(dev, I2C_RTC_YEAR_ADD, buff, 6))
		{
			printf("Fail to read RTC!\n");
			exit(1);
		}

		printf("%02u/%02u/%02u %02u:%02u:%02u\n", buff[1], buff[2], buff[0],
			buff[3], buff[4], buff[5]);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_RTC_GET.usage1);
		exit(1);
	}
}

static void doRTCBattGet(int argc, char *argv[]);
const CliCmdType CMD_RTC_BATT_GET =
{
	"rtcbrd",
	1,
	&doRTCBattGet,
	"\rtcbrd:		Get the internal RTC backup battery voltage\n",
	"\tUsage:		ti rtcbrd \n",
	"",
	"\tExample:	ti rtcbrd; Get the internal RTC backup battery voltage \n"};

static void doRTCBattGet(int argc, char *argv[])
{
	int dev = 0;
	float volt = 0;

	UNUSED(argv);
	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 2)
	{
		if (OK != rtcBatteryVoltGet(dev, &volt))
		{
			printf("Fail to read RTC Battery!\n");
			exit(1);
		}

		printf("%.2f V\n", volt);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_RTC_BATT_GET.usage1);
		exit(1);
	}
}


static void doRTCSet(int argc, char *argv[]);
const CliCmdType CMD_RTC_SET =
	{
		"rtcwr",
		1,
		&doRTCSet,
		"\trtcwr:		Set the internal RTC  date and time(mm/dd/yy hh:mm:ss)\n",
		"\tUsage:		ti rtcwr <mm> <dd> <yy> <hh> <mm> <ss> \n",
		"",
		"\tExample:	ti rtcwr 9 15 20 21 43 15; Set the internal RTC time and date   at Sept/15/2020  21:43:15\n"};

static void doRTCSet(int argc, char *argv[])
{
	int dev = 0;
	u8 buff[7];
	int i = 0;

	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 8)
	{
		i = atoi(argv[2]); //month
		if (i < 1 || i > 12)
		{
			printf("Invalid month!\n");
			exit(1);
		}
		buff[1] = i;
		i = atoi(argv[3]);
		if (i < 1 || i > 31)
		{
			printf("Invalid date!\n");
			exit(1);
		}
		buff[2] = i;
		i = atoi(argv[4]);
		if (i < 0 || i > 99)
		{
			printf("Invalid year!\n");
			exit(1);
		}
		buff[0] = i;
		i = atoi(argv[5]);
		if (i < 0 || i > 23)
		{
			printf("Invalid hour!\n");
			exit(1);
		}
		buff[3] = i;
		i = atoi(argv[6]);
		if (i < 0 || i > 59)
		{
			printf("Invalid minute!\n");
			exit(1);
		}
		buff[4] = i;
		i = atoi(argv[7]);
		if (i < 0 || i > 59)
		{
			printf("Invalid second!\n");
			exit(1);
		}
		buff[5] = i;
		buff[6] = CALIBRATION_KEY;
		if (OK != i2cMem8Write(dev, I2C_RTC_SET_YEAR_ADD, buff, 7))
		{
			printf("Fail to read RTC!\n");
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_RTC_SET.usage1);
		exit(1);
	}
}

int batteryVoltGet(int dev, float* val)
{
	u8 buff[2];
	u16 raw = 0;

	if (NULL == val)
	{
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_DIAG_V_BATT_MEM_ADD, buff, 2))
	{
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 1000;
	return OK;
}
const char stateNames[STATUS_COUNT][20] =
{
	"CHARGING",
	"CHARGE_END",
	"CHARGE_FAULT",
	"ON_BATTERY",
};

int batteryStatusGet(int dev, char* state)
{
	u8 buff = 0;
	if (NULL == state)
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_DIAG_POWER_STATE_MEM_ADD, &buff, 1))
	{
		return ERROR;
	}
	if(buff >= STATUS_COUNT)
	{
		return ERROR;
	}
	sprintf(state,"%s",stateNames[buff]);
	return OK;
}

static void doBackupBattGet(int argc, char *argv[]);
const CliCmdType CMD_BCK_BATT_GET =
{
	"bckbrd",
	1,
	&doBackupBattGet,
	"\tbckbrd:		Get the backup battery voltage\n",
	"\tUsage:		ti bckbrd \n",
	"",
	"\tExample:	ti bckbrd; Get the backup battery voltage \n"};

static void doBackupBattGet(int argc, char *argv[])
{
	int dev = 0;
	float volt = 0;

	UNUSED(argv);
	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 2)
	{
		if (OK != batteryVoltGet(dev, &volt))
		{
			printf("Fail to read Backup Battery voltage!\n");
			exit(1);
		}

		printf("%.2f V\n", volt);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_BCK_BATT_GET.usage1);
		exit(1);
	}
}

static void doPowerStatusGet(int argc, char *argv[]);
const CliCmdType CMD_PWR_STAT_GET =
{
	"pwrsrd",
	1,
	&doPowerStatusGet,
	"\tpwrsrd:		Get the power supply status\n",
	"\tUsage:		ti pwrsrd \n",
	"",
	"\tExample:	ti pwrsrd; Get the power supply status \n"};

static void doPowerStatusGet(int argc, char *argv[])
{
	int dev = 0;
	char state[20];

	UNUSED(argv);
	dev = doBoardInit();
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 2)
	{
		if (OK != batteryStatusGet(dev, state))
		{
			printf("Fail to read Backup Battery voltage!\n");
			exit(1);
		}

		printf("%s\n", state);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_PWR_STAT_GET.usage1);
		exit(1);
	}
}

const CliCmdType* gCmdArray[] =
{
	&CMD_VERSION,
	&CMD_HELP,
	&CMD_WAR,
	&CMD_LIST,
	&CMD_BOARD,
	&CMD_RTC_BATT_READ,
	&CMD_TRIAC_WRITE,
	&CMD_TRIAC_READ,
	&CMD_TEST,
	&CMD_LED_WRITE,
	&CMD_LED_READ,
	&CMD_LED_TEST,
	&CMD_CONTACT_READ,
	&CMD_COUNTER_READ,
	&CMD_PPS_READ,
	&CMD_COUNTER_RST,
	&CMD_EDGE_READ,
	&CMD_EDGE_WRITE,
	&CMD_DAC_READ,
	&CMD_DAC_WRITE,
	&CMD_OD_READ,
	&CMD_OD_WRITE,
	&CMD_ADC_READ,
	&CMD_R1K_READ,
	&CMD_R10K_READ,
	&CMD_RTH_READ,
	&CMD_PWM_READ,
	&CMD_ADC_CAL,
	&CMD_ADC_CAL_RST,
	&CMD_DAC_CAL,
	&CMD_DAC_CAL_RST,
	&CMD_WDT_RELOAD,
	&CMD_WDT_SET_PERIOD,
	&CMD_WDT_GET_PERIOD,
	&CMD_WDT_SET_INIT_PERIOD,
	&CMD_WDT_GET_INIT_PERIOD,
	&CMD_WDT_SET_OFF_PERIOD,
	&CMD_WDT_GET_OFF_PERIOD,
	&CMD_RTC_GET,
	&CMD_RTC_BATT_GET,
	&CMD_RTC_SET,
	&CMD_BCK_BATT_GET,
	&CMD_PWR_STAT_GET,
	&CMD_WDT_SET_STAY_ON_BATTERY,
	&CMD_WDT_GET_STAY_ON_BATTERY,
	&CMD_RS485_READ,
	&CMD_RS485_WRITE,
	NULL}; //null terminated array of cli structure pointers

int main(int argc, char *argv[])
{
	int i = 0;

	if (argc == 1)
	{
		usage();
		return -1;
	}
	while (NULL != gCmdArray[i])
	{
		if ( (gCmdArray[i]->name != NULL) && (gCmdArray[i]->namePos < argc))
		{
			if (strcasecmp(argv[gCmdArray[i]->namePos], gCmdArray[i]->name) == 0)
			{
				gCmdArray[i]->pFunc(argc, argv);
				return 0;
			}
		}
		i++;
	}
	printf("Invalid command option\n");
	usage();

	return -1;
}
