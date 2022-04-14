# ti-rpi
Drivers for the HEATWEB SQ3 card

## Setup

Enable I2C communication first:
```bash
~$ sudo raspi-config
```
A good article about I2C on Raspberry can be found [here.](https://www.raspberrypi-spy.co.uk/2014/11/enabling-the-i2c-interface-on-the-raspberry-pi/) 

If you use Ubuntu you need to install ```raspi-config``` first:

```bash
~$ sudo apt update
~$ sudo apt install raspi-config
```

Make sure you have all tools you need:
```bash
~$ sudo apt update
~$ sudo apt-get install git
~$ sudo apt-get install build-essential
```
## Usage

Install the command:
```bash
~$ git clone https://github.com/SequentMicrosystems/ti-rpi.git
~$ cd ti-rpi/
~/ti-rpi$ sudo make install
```

Now you can access all the functions of the card through the command "ti". Use -h option for help:
```bash
~$ ti -h
```

If you clone the repository any update can be made with the following commands:

```bash
~$ cd ti-rpi/  
~/ti-rpi$ git pull
~/ti-rpi$ sudo make install
```  
