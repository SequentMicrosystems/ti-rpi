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
|	COIL_DRY_C_RISING3 | 0009 | 0x0a |
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


COIL_DRY_C_RISING[x] coil enable/disable counting on rising edge of coresponding dry contact channel 

COIL_DRY_C_FALLING[x] coil enable/disable counting on falling edge of coresponding dry contact channel 

COIL_RST_CNT_DRY_C[x] coil resets the counter for the coresponding dry contact channel 

### Discrete Inputs

Access level Read Only, Size 1 bit

| Device function | Register Address | Modbus Address |
| --- | --- | --- |
|	DI_DRY_C1 | 10001 | 0x00 |
|	DI_DRY_C2 | 10001 | 0x01 |
|	DI_DRY_C3 | 10001 | 0x02 |
|	DI_DRY_C4 | 10001 | 0x03 |
|	DI_DRY_C5 | 10001 | 0x04 |
|	DI_DRY_C6 | 10001 | 0x05 |
|	DI_DRY_C7 | 10001 | 0x06 |
|	DI_DRY_C8 | 10001 | 0x0 |
|	DI_BUTTON | 10001 | 0x00 |

DI_DRY_C[x] - Dry contact input state



### Input registers

Access level Read Only, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit | Description |
| --- | --- | --- | --- | --- |
|	IR_0_10IN1| 30001 | 0x00 | mV | 0 - 10V input channel 1 |
|	IR_0_10IN2| 30002 | 0x01 | mV | 0 - 10V input channel 2 |
|	IR_0_10IN3| 30003 | 0x02 | mV | 0 - 10V input channel 3 |
|	IR_0_10IN4| 30004 | 0x03 | mV | 0 - 10V input channel 4 |
|	IR_THERM1| 30005 | 0x04 | ohm | Thermistor input channel 1 |
|	IR_THERM2| 30006 | 0x05 | ohm | Thermistor input channel 2 |
|	IR_THERM3| 30007 | 0x06 | ohm | Thermistor input channel 3 |
|	IR_THERM4| 30008 | 0x07 | ohm | Thermistor input channel 4 |
|	IR_THERM5| 30009 | 0x08 | ohm | Thermistor input channel 5 |
|	IR_THERM6| 30010 | 0x09 | ohm | Thermistor input channel 6 |
|	IR_THERM7| 30011 | 0x0a | ohm | Thermistor input channel 7 |
|	IR_THERM8| 30012 | 0x0b | ohm | Thermistor input channel 8 |
|	IR_R_1K1| 30013 | 0x0c | ohm | Universal inputs 1k thermistor channel 1 |
|	IR_R_1K2| 30014 | 0x0d | ohm | Universal inputs 1k thermistor channel 2 |
|	IR_R_1K3| 30015 | 0x0e | ohm | Universal inputs 1k thermistor channel 3 |
|	IR_R_1K4| 30016 | 0x0f | ohm | Universal inputs 1k thermistor channel 4 |
|	IR_R_10K1| 30017 | 0x10 | ohm | Universal inputs 10k thermistor channel 1 |
|	IR_R_10K2| 30018 | 0x11 | ohm | Universal inputs 10k thermistor channel 2 |
|	IR_R_10K3| 30019 | 0x12 | ohm | Universal inputs 10k thermistor channel 3 |
|	IR_R_10K4| 30020 | 0x13 | ohm | Universal inputs 10k thermistor channel 4 |
|	IR_DRY_C_CNT1L| 30021 | 0x14 | - | Dry contact transitions contor low channel 1 |
|	IR_DRY_C_CNT1H| 30022 | 0x15 | - | Dry contact transitions contor high channel 1 |
|	IR_DRY_C_CNT2L| 30023 | 0x16 | - | Dry contact transitions contor low channel 2 |
|	IR_DRY_C_CNT2H| 30024 | 0x17 | - | Dry contact transitions contor high channel 2 |
|	IR_DRY_C_CNT3L| 30025 | 0x18 | - | Dry contact transitions contor low channel 3 |
|	IR_DRY_C_CNT3H| 30025 | 0x18 | - | Dry contact transitions contor high channel 3 |
|	IR_DRY_C_CNT4L| 30027 | 0x1a | - | Dry contact transitions contor low channel 4 |
|	IR_DRY_C_CNT4H| 30028 | 0x1b | - | Dry contact transitions contor high channel 4 |
|	IR_DRY_C_CNT5L| 30029 | 0x1c | - | Dry contact transitions contor low channel 5 |
|	IR_DRY_C_CNT5H| 30030 | 0x1d | - | Dry contact transitions contor high channel 5 |
|	IR_DRY_C_CNT6L| 30031 | 0x1e | - | Dry contact transitions contor low channel 6 |
|	IR_DRY_C_CNT6H| 30032 | 0x1f | - | Dry contact transitions contor high channel 6 |
|	IR_DRY_C_CNT7L| 30033 | 0x20 | - | Dry contact transitions contor low channel 7 |
|	IR_DRY_C_CNT7H| 30034 | 0x21 | - | Dry contact transitions contor high channel 7 |
|	IR_DRY_C_CNT8L| 30035 | 0x22 | - | Dry contact transitions contor low channel 8 |
|	IR_DRY_C_CNT8H| 30036 | 0x23 | - | Dry contact transitions contor high channel 8 |
|	IR_DRY_C_PPS1| 30037 | 0x24 | Hz | Dry contact transitions per second on channel 1 |
|	IR_DRY_C_PPS2| 30038 | 0x25 | Hz | Dry contact transitions per second on channel 2 |
|	IR_DRY_C_PPS3| 30039 | 0x26 | Hz | Dry contact transitions per second on channel 3 |
|	IR_DRY_C_PPS4| 30040 | 0x27 | Hz | Dry contact transitions per second on channel 4 |
|	IR_DRY_C_PPS5| 30041 | 0x28 | Hz | Dry contact transitions per second on channel 5 |
|	IR_DRY_C_PPS6| 30042 | 0x29 | Hz | Dry contact transitions per second on channel 6 |
|	IR_DRY_C_PPS7| 30043 | 0x2a | Hz | Dry contact transitions per second on channel 7 |
|	IR_DRY_C_PPS8| 30044 | 0x2b | Hz | Dry contact transitions per second on channel 8 |
|	IR_PWM1| 30045 | 0x2c | ‰ | Filling factor for PWM input channel 1 |
|	IR_PWM2| 30046 | 0x2d | ‰ | Filling factor for PWM input channel 2 |
|	IR_PWM3| 30047 | 0x2e | ‰ | Filling factor for PWM input channel 3 |
|	IR_PWM4| 30048 | 0x2f | ‰ | Filling factor for PWM input channel 4 |
|	IR_VIN| 30049 | 0x30 | mV | Power supply voltage |
|	IR_VB| 30050 | 0x31 | mV | Battery backup voltage |


### Holding registers

Access level Read/Write, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit | Range |
| --- | --- | --- | --- | --- |
| 0-10V_OUT_1 | 40001 | 0x00 | mV | 0..10000 |
| 0-10V_OUT_2 | 40002 | 0x01 | mV | 0..10000 |
| 0-10V_OUT_3 | 40003 | 0x02 | mV | 0..10000 |
| 0-10V_OUT_4 | 40004 | 0x03 | mV | 0..10000 |
| OPEN_DRAIN_PWM_1 | 40005 | 0x04 |  | 0..10000 |
| OPEN_DRAIN_PWM_2 | 40006 | 0x05 |  | 0..10000 |
| OPEN_DRAIN_PWM_3 | 40007 | 0x06 |  | 0..10000 |
| OPEN_DRAIN_PWM_4 | 40008 | 0x07 |  | 0..10000 |

## Function codes implemented

* Read Coils (0x01)
* Read Discrete Inputs (0x02)
* Read Holding Registers (0x03)
* Read Input Registers (0x04)
* Write Single Coil (0x05)
* Write Single Register (0x06)
* Write Multiple Coils (0x0f)
* Write Multiple registers (0x10)
