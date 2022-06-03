# update

This is the HEATWEB SQ3 board firmware update tool.

## Usage

```bash
~$ git clone https://github.com/SequentMicrosystems/ti-rpi.git
~$ cd ti-rpi/update/
~/ti-rpi/update$ ./update
```

If you clone the repository already, skip the first step. 
The command will download the newest firmware version from our server and write it  to the board.
During firmware update we strongly recommend to disconnect all outputs from the board since they can change state unpredictably.
Please make sure no program or script tries to access the I2C port during update. 