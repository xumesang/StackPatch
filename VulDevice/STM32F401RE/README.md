# StackPatch Implementation on STM32F401RE

StackPatch on the STM32F401RE (Cortex-M4) platform supports both hardware- and software-breakpoint triggers to apply hot patches at runtime without reboot.

---

## Usage

Choose the trigger mechanism based on where your vulnerable code executes:

1. **Flash-based Programs**  
   Use hardware breakpoints (DWT or FPB) when the firmware runs from Flash.

2. **SRAM-based Programs**  
   Use software breakpoints when the application runs from SRAM via a bootloader.

---

## Folder Structure

```plaintext
flashpatch/           # Flash-based triggers
├── dwtflashpatch/    # DWT hardware-breakpoint mode
└── fpbflashpatch/    # FPB hardware-breakpoint mode

srampatch/            # SRAM-based triggers
├── bootloader/       # Bootloader in Flash; loads SRAM apps via serial
├── freertos_cve_test/# Software-breakpoint patches for FreeRTOS
└── sram_cve/         # Software-breakpoint patches for other RTOSes/libs