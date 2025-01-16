# StackPatch -STM32F401RE


## Usage

In this folder, we provide two trigger mechanisms of StackPatch on the stm32f401re development board.

#### The structure of this folder is as follows:
```

flashpatch
|
└─── dwtflashpatch       <--- DWT hardware breakpoints trigger mechanisms
└─── fpbflashpatch       <--- FPB hardware breakpoints trigger mechanisms

srampatch     <--- The trigger mechanism of software breakpoints requires the program to run in SRAM, so we build bootloader (stored in FLASH) and apps (stored in SRAM). 
|
└─── bootloader          <--- For this trigger mechanism, you need to burn the bootloader into flash and send the compiled bin file of the app program through UART.
└─── freertos_cve_test   <--- Example program of using stackpatch to patch FreeRTOS vulnerabilities.
└─── sram_cve            <--- Example program of using stackpatch to patch other RTOSes or libraries vulnerabilities.

```

