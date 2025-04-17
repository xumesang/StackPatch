### StackPatch

## Introduction
This repository contains demo projects from our work, which you can compile and run to validate the functionalities of StackPatch. 


### Description 
## The structure of the artifact is as follows:

```
-------------\
| - Evaluation: The evluation of StackPatch was conducted using STM32F401RE (Coetex-M4), GD32VF103 (RSIC-V32), and ESP32S3 (Xtensa-LX7) development boards.
└─── Compare: The comparison with other hot patching approaches for embedded systems.
└─── DispatchDelayBinarySearch: Added performance evaluation of the patch dispatcher for binary Search.
└─── HeartRateSensor: StackPatch fixes a vulnerability in the heart rate sensor.
└─── NetworkServiceLatency: StackPatch fixes the vulnerabilities Network Services.

| - VulDevice: This folder contains the implementation of StackPatch on the STM32F401RE (Coetex-M4), GD32VF103 (RSIC-V32), and ESP32S3 (Xtensa-LX7) development boards. 
└─── ESP32S3: Implementation of StackPatch on the STM32F401RE board.
└─── GD32VF103: Implementation of StackPatch on the STM32F401RE board.
└─── STM32F401RE: Implementation of StackPatch on the STM32F401RE board.
```



### Requirements
To evaluate *StackPatch* and obtain results similar to those presented in the paper, access to one of the following **four** boards is required: 
**nRF52840, STM32-F401RE, GD32VF103 or ESP32S3**
We recommend using the STM32F401RE board to compile the StackPatch runtime with keil IDE. 


# STM32-F401RE (recommend) or nRF52840   
Required:
 - Windows 10.
 - Keil uVision5 IDE.
 - Serial port tool (such as pyserial).
 - a debugger (J-LINK or ST-LINK).


Usage:
   1. Use a debugger (such as ST-LINK) to connect the PC to the development board.
   2. Open the KEIL project in the VulDevice/STM32F401RE/ folder (such as VulDevice\STM32F401RE\flashpatch\fpbflashpatch\Projects\MDK-ARM\f401re.uvprojx)
   3. Compiler and download the firmware to the board. 
   4. Using the serial port tool to communicate with the board, you can see the repair log of StackPatch.


# GD32VF103  
Required:
 - Windows 10.
 - eclipse IDE.
 - Serial port tool (such as pyserial).
 - a debugger (GD-LINK).

Usage: 
 - entry.S: We have modified the entry.S in the component, specifically the trap_entry exception handling function. You can use it to replace the corresponding file.


# ESP32S3
Required:
 - Windows 10.
 - ESP-IDF (including Python, Git, cross-compilers, CMake, Ninja compilation tools, etc.).
 - Serial port tool (such as pyserial).
 - a debugger (J-LINK).

Usage:
 - panc_handler_asm.S: We have modified the panc_handler_asm.S in the esp-idf component (which integrates FreeRTOS), specifically the xt_panic exception handling function. You can use it to replace the corresponding file in ESP-idf.
 - instrument.py: You can use it to perform instrumentation for functions in esp-idf.