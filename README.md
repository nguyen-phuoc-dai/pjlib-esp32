# PJLIB on ESP32-IDF
PJLIB as Espressif's ESP-IDF component

## Overview

PJSIP [PJSIP](https://www.pjsip.org/) is a free and open source multimedia communication library. It is core component for a lot of VoIP communication applications. For now, there is no any port of PJSIP for ESP-IDF. As first attempt for porting whole PJSIP stack to ESP32 platform, its core component (PJLIB) will be ported first.

## Requirements

* ESP-IDF v4.x
* GCC 8

## Using this port in your project (compiling for ESP)
For now, ESP-IDF's pthread compoment have not completely compatible with POSIX standard, it still misses some core APIs. This port has also added these missing APIs. To compile code you need to make one change manually on FreeRTOS.h

```
#ifndef configUSE_APPLICATION_TASK_TAG
	#define configUSE_APPLICATION_TASK_TAG 1
#endif
```