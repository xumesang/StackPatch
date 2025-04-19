# StackPatch implementation on the STM32F401RE board


## Usage

Users can use two trigger mechanisms to repair vulnerable programs running on the stm32f401re development board.

#### The structure of this folder is as follows:
```

flashpatch     <--- When the vulnerable program running in Flash memory, users can use the DWT or the FPB hardware breakpoints to repair it. 
|
└─── dwtflashpatch       <--- The DWT hardware breakpoints trigger mechanism
└─── fpbflashpatch       <--- The FPB hardware breakpoints trigger mechanism

srampatch     <--- When the vulnerable program running in SRAM, users are recommended to use software berakpoints to repair it. We build bootloader (stored in FLASH) and apps (stored in SRAM). 
|
└─── bootloader          <--- Users need to flash the bootloader into flash and send the compiled bin file of the app program through a serial interface (e.g., UART). 
└─── freertos_cve_test   <--- These examples show how to repair FreeRTOS vulnerabilities. 
└─── sram_cve            <--- These examples show how to repair vulnerabilities of other RTOSes and embedded libraries.

```

