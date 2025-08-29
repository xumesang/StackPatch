# StackPatch Implementation on STM32F401RE (hardware‑breakpoint triggers)

There are two independent hardware units in STM32F401: FPB and DWT, both of which can implement hardware breakpoint functionality. The difference between the *dwtflashpatch* project and the *fpbflashpatch* project lies in the hardware unit they use. 
----------------

## Usage

- 1. Connect the STM32F401RE to the update host.
- 2. Open the Keil project (double-click *flashpatch\xxxflashpatch\Projects\MDK-ARM\f401re.uvprojx*).
- 3. Compile the Keil project and flash it into the development board.
- 4. Open the the serial terminal tool *UartAssist* (located in *VulDevice\SerialTerminalTool*) and observe the output information.

ps: This project is configured by default to remediate the vulnerability CVE-2018-16601. To remediate other vulnerabilities, it is necessary to replace the update point, binary patch, and its address (located in \VulDevice\STM32F401RE\flashpatingch\fpbflashpatch\User\freertos_demo.c).