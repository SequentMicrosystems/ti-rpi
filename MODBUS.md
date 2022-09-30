[![ti-rpi](res/sequent.jpg)](https://sequentmicrosystems.com)

# Modbus

The Heatweb Controller can be accessed trough Modbus RTU protocol over RS-485 port.
You can set-up the RS-485 port with **ti** command.

Example:
```bash
~$ ti cfg485wr 1 9600 1 0 1
```
Set Modbus RTU , Baudrate: 9600bps, 1 Stop Bit,  parity: None, slave address offset: 1
```bash
~$ ti -h cfg485wr
```
display the full set of options

## Slave Address
The slave address is setable with ```cfg485wr``` option

## Modbus object types
All modbus RTU object type with standard addresses are implemented : Coils, Discrete Inputs, Input registers, Holding registers.

### Coils

Acces level Read/Write, Size 1 bit

| Device function | Register Address | Modbus Address |
| --- | --- | --- |
| COIL_RELAY1 | 0001 | 0x00 |
| COIL_RELAY2 | 0002 | 0x01 |
|	COIL_LED1 | 0003 | 0x04 |
|	COIL_LED2 | 0004 | 0x05 |
|	COIL_LED3 | 0005 | 0x06 |
|	COIL_LED4 | 0006 | 0x07 |
|	COIL_DRY_C_RISING1 | 0007 | 0x08 |
|	COIL_DRY_C_RISING2 | 0008 | 0x09 |
|	COIL_DRY_C_RISING3  0009 | 0x0a |
|	COIL_DRY_C_RISING4 | 0010 | 0x0b |
|	COIL_DRY_C_RISING5 | 0011 | 0x0c |
|	COIL_DRY_C_RISING6 | 0012 | 0x0d |
|	COIL_DRY_C_RISING7 | 0013 | 0x0e |
|	COIL_DRY_C_RISING8 | 0014 | 0x0f |
|	COIL_DRY_C_FALLING1 | 0015 | 0x10 |
|	COIL_DRY_C_FALLING2 | 0016 | 0x11 |
|	COIL_DRY_C_FALLING3 | 0017 | 0x12 |
|	COIL_DRY_C_FALLING4 | 0018 | 0x13 |
|	COIL_DRY_C_FALLING5 | 0019 | 0x14 |
|	COIL_DRY_C_FALLING6 | 0020 | 0x15 |
|	COIL_DRY_C_FALLING7 | 0021 | 0x16 |
|	COIL_DRY_C_FALLING8 | 0022 | 0x17 |
|	COIL_KEEP_POWER_ON_BATTERY | 0023 | 0x18 |
|	COIL_RST_CNT_DRY_C1 | 0024 | 0x19 |
|	COIL_RST_CNT_DRY_C2 | 0025 | 0x1a |
|	COIL_RST_CNT_DRY_C3 | 0026 | 0x1b |
|	COIL_RST_CNT_DRY_C4 | 0027 | 0x1c |
|	COIL_RST_CNT_DRY_C5 | 0028 | 0x1d |
|	COIL_RST_CNT_DRY_C6 | 0029 | 0x1e |
|	COIL_RST_CNT_DRY_C7 | 0030 | 0x1f |
|	COIL_RST_CNT_DRY_C8 | 0031 | 0x20 |


OPTO_RISING_COUNT coil enable/disable counting on rising edge of coresponding opto channel 
OPTO_FALLING_COUNT coil enable/disable counting on falling edge of coresponding opto channel 

### Discrete Inputs

Access level Read Only, Size 1 bit

| Device function | Register Address | Modbus Address |
| --- | --- | --- |
| OPTO_1 | 10001 | 0x00 |
| OPTO_2 | 10002 | 0x01 |
| OPTO_3 | 10003 | 0x02 |
| OPTO_4 | 10004 | 0x03 |


### Input registers

Access level Read Only, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit |
| --- | --- | --- | --- |
| 0-10V_IN_1 | 30001 | 0x00 | mV |
| 0-10V_IN_2 | 30002 | 0x01 | mV |
| 0-10V_IN_3 | 30003 | 0x02 | mV |
| 0-10V_IN_4 | 30004 | 0x03 | mV |
| +/-10V_IN_1 | 30005 | 0x04 | mV |
| +/-10V_IN_2 | 30006 | 0x05 | mV |
| +/-10V_IN_3 | 30007 | 0x06 | mV |
| +/-10V_IN_4 | 30008 | 0x07 | mV |
| 4-20mA_IN_1 | 30009 | 0x08 | uA |
| 4-20mA_IN_2 | 30010 | 0x09 | uA |
| 4-20mA_IN_3 | 30011 | 0x0a | uA |
| 4-20mA_IN_4 | 30012 | 0x0b | uA |
| OPTO_COUNT_1 | 30013 | 0x0c |  |
| OPTO_COUNT_2 | 30014 | 0x0d |  |
| OPTO_COUNT_3 | 30015 | 0x0e |  |
| OPTO_COUNT_4 | 30016 | 0x0f |  |


### Holding registers

Access level Read/Write, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit | Range |
| --- | --- | --- | --- | --- |
| 0-10V_OUT_1 | 40001 | 0x00 | mV | 0..10000 |
| 0-10V_OUT_2 | 40002 | 0x01 | mV | 0..10000 |
| 0-10V_OUT_3 | 40003 | 0x02 | mV | 0..10000 |
| 0-10V_OUT_4 | 40004 | 0x03 | mV | 0..10000 |
| 4-20mA_OUT_1 | 40005 | 0x04 | uA | 4000..20000 |
| 4-20mA_OUT_2 | 40006 | 0x05 | uA | 4000..20000 |
| 4-20mA_OUT_3 | 40007 | 0x06 | uA | 4000..20000 |
| 4-20mA_OUT_4 | 40008 | 0x07 | uA | 4000..20000 |
| OPEN_DRAIN_PWM_1 | 40009 | 0x08 |  | 0..10000 |
| OPEN_DRAIN_PWM_2 | 40010 | 0x09 |  | 0..10000 |
| OPEN_DRAIN_PWM_3 | 40011 | 0x0a |  | 0..10000 |
| OPEN_DRAIN_PWM_4 | 40012 | 0x0b |  | 0..10000 |

## Function codes implemented

* Read Coils (0x01)
* Read Discrete Inputs (0x02)
* Read Holding Registers (0x03)
* Read Input Registers (0x04)
* Write Single Coil (0x05)
* Write Single Register (0x06)
* Write Multiple Coils (0x0f)
* Write Multiple registers (0x10)
