# STM32F769i_DISCO TBS Version 3.0.10.

Please note this TouchGFX Board Setup is only supported from TouchGFX 4.20.0. Configuration is done using CubeMX 6.5.0 based on STM32Cube FW_F7 V1.16.2 drivers. The default IDE is set to STM32CubeIDE, to change IDE open the STM32F769I_DISCO.ioc with CubeMX and select from the supported IDEs (EWARM, MDK-ARM, and STM32CubeIDE). Supports flashing of the STM32F769I_DISCO board directly from TouchGFX Designer using GCC and STM32CubeProgrammer.Flashing the board requires STM32CubeProgrammer which can be downloaded from the ST webpage. 

This TBS is configured for 480 x 800 pixels 16bpp screen resolution.  

Performance testing can be done using the GPIO pins designated with the following signals: VSYNC_FREQ  - Pin PC6(D1), RENDER_TIME - Pin PC7(D0), FRAME_RATE  - Pin PJ1(D2), MCU_ACTIVE  - Pin PF6(D3). 

 