# StackPatch Implementation on STM32F401RE 

This project is used to measure the total patching latency of StackPatch, with measurements conducted separately for the four triggering methods.
----------------


## Folder Structure

```plaintext
hardware/           # Flash-based triggers
├── dwtflashpatch/    # DWT hardware-breakpoint mode
└── fpbflashpatch/    # FPB hardware-breakpoint mode

software/            # SRAM-based triggers
├── bootloader/       # Bootloader in Flash; loads SRAM apps via serial
├── APPbreakpoint/   # Software-breakpoint mode
└── Hook/         # Instrument mode

## Usage

- 1. Connect the STM32F401RE to the update host.
- 2. Open the Keil project.
- 3. Compile the Keil project and flash it into the development board.
- 4. Open the the serial terminal tool *UartAssist* (located in *VulDevice\SerialTerminalTool*) and observe the output information.

