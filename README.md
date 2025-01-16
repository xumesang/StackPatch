### StackPatch

## Introduction
This repository contains demo projects from our work, which you can compile and run to validate the functionalities of StackPatch. 


To evaluate *StackPatch* and obtain results similar to those presented in the paper, access to one of the following **four** boards is required: 
**nRF52840, STM32-F401RE, GD32VF103 or ESP32S3**


#### The structure of the artifact is as follows:

```
-------------\
| - Evaluation: The evluation of StackPatch was conducted using STM32F401RE (Coetex-M4), GD32VF103 (RSIC-V32), and ESP32S3 (Xtensa-LX7) development boards.
| - VulDevice: This folder contains the implementation of StackPatch on the STM32F401RE (Coetex-M4), GD32VF103 (RSIC-V32), and ESP32S3 (Xtensa-LX7) development boards. 
```


## Usage
we recommend using the STM32F401RE board to compile the StackPatch runtime with keil IDE. In addition, you need to use a debugger (such as J-LINK) to connect the PC to the development board.