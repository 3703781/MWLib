<img src="./res/mwlogo.png" alt="MWLib" title="MWLib" align="right" height="40" />

# MWLib

![C](https://img.shields.io/badge/C-ANSI/89/99-blue.svg)
![Platform](https://img.shields.io/badge/platform-stm32-lightgray.svg)
![License](https://img.shields.io/badge/license-GPLv3-green.svg)
![Contributions welcome](https://img.shields.io/badge/contributions-welcome-orange.svg)

MWLib  is written in C/C++ for STM32F4 series. It contains some of the drivers for a variety of devices.

## Getting Started

### Overview
This library is based on *STM32F4 Standard Peripheral Libraries* , thus it follows the code style. The file *[mwlib.uvprojx](./mwlib.uvprojx)* is MDK project file. Source code under the folder *[user](.user/)* is in paired form of *xxx.h* and *xxx.c*, where *xxx* refers to model name of the device. There are code from *STM32F4 Standard Peripheral Libraries*  under the folder *[fwlib](./fwlib)* and some tool functions in *[utils.c](./user/utils.c)*.
The library covers several devices as listed:
- [DRV8825](./res/drv8826.jpg) - Stepper motor driver carrier
- [E18-D80NK](./res/e18d80nk.jpg) - Infrared photoelectric sensor
- [HC-SR04](./res/hcsr04.jpg) - Ultrasonic distance sensor 
- [TB6612FNG](./res/tb6612fng.png) - Motor drive module
- [GP2Y0E03](./res/gp2y0e03.png) - Distance sensor module
- [OLED](./res/oled.jpg) - I2C 12864 SSD1306 oled module
- [MPU6050](./res/mpu6050.jpg) - 3 axis accelerometer gyroscope module
- [MPU9250](./res/mpu9250.jpg) - 9 axis accelerometer gyroscope magnetometer sensor

### Prerequisites

The following are required.
- Windows 7 / 8 / 8.1 / 10
- Keil MDK uVersion5 or later
- STM32F4xx_DFP Pack v 2.13 or later

### How to use?
Of most devices in this library, you can simply use them as follows:
```c
#include "xxx.h"

void fun()
{
	//Initialize the device.
	xxx_Init();
	
	//Your code.
	...
	...
	...
}
```

## Want to contribute?

If you want to help the MWLib project, there are some guidlines:
- Respect my coding style.
- Avoid to commit several features in one commit.
- Make your modification compact - don't reformat source code in your request.

## License

Source code in the following files is released under GPLv3 license for non-commercial projects only. See *[license](./LICENSE)* for details.
```
./user/drv8825.c
./user/drv8825.h
./user/e18d80nk.c
./user/e18d80nk.h
./user/gp2y0e03.c
./user/gp2y0e03.h
./user/hallencoder.c
./user/hallencoder.h
./user/hcsr04.c
./user/hcsr04.h
./user/key.c
./user/key.h
./user/led.c
./user/led.h
./user/tb6612fng.c
./user/tb6612fng.h
./user/oled/oled.c
./user/oled/oled.h
./user/mpu9250/mpu9250.c
./user/mpu9250/mpu9250.h
./user/mpu6050/mpu6050.c
./user/mpu6050/mpu6050.h
```
## Acknowledgments

- [Embdefire](https://github.com/Embdefire)
- Alientek
- Anyone who make contributions