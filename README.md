# ISM330DHCX_Driver
Simple ISM330DHCX Gyro/Accelerometer Driver

Files of Interest:
1. ISM330DHCX.h: Device specific header file - Shouldn't need modification
2. ISM330DHCX.c: Device specific source file - Shouldn't need modification
3. ISM330DHCX_Hardware.h: Hardware specific header file - Should not need modification beyond the exported low level driver
4. ISM330DHCX_Hardware.c: Hardware specific source file - User must implement this file for their board/project needs

To Use:
0. Include ISM330DHCX.h
1. Create an ISM330DHCX_Init_Struct_t with desired user settings.
2. Create an ISM330DHCX_IO_Drv_t with necessary low level IO functions (I2C/SPI, GPIO Communication functions).
3. Create a ISM330DHCX_Handle_t
4. Pass the init struct, IO Driver, and device handle to ISM330DHCX_Init()
5. Functions listed in ISM330DHCX.h can now be used by passing the initialized device handle as a function arguement
