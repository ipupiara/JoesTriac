/*
 * sdDisk.h
 *
 *  Created on: Jan 6, 2025
 *      Author: Brigitte
 */

#include <mainJt.h>
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h" /* defines SD_Driver as external */

#ifndef APPLICATION_USER_TOOLS_HW_SDDISK_H_
#define APPLICATION_USER_TOOLS_HW_SDDISK_H_


uint32_t initMmc();
uint32_t mmcStoreDefines(uint32_t * persistentDefines);
uint32_t writeToLogfile(char* transmitBuffer);


#endif /* APPLICATION_USER_TOOLS_HW_SDDISK_H_ */
