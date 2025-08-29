# StackPatch Implementation on STM32F401RE (software‑breakpoint triggers)



----------------

## Usage
## Step one: 
- 1. Connect the STM32F401RE to the update host.
- 2. Open the Keil project (double-click *srampatch\bootloader\Projects\MDK-ARM\f401re.uvprojx*).
- 3. Compile the Keil project and flash it into the development board.

## Step two: (Make sure the development board is connected to the update host.)
- 1. Open the Keil project (*freertos_cve_test*).
- 2. Compile the Keil project to generate the binary firmware.
- 3. Open the serial terminal tool *UartAssist* (located in *VulDevice\SerialTerminalTool*),and then use the serial debugging assistant to send the binary firmware (located in VulDevice\STM32F401RE\srampatch\freertos_cve_test\Output\hhf401re.bin) to the development board.
- 4. Observe the output information in the serial terminal tool.


ps: This project is configured by default to remediate the vulnerability CVE-2018-16601. To remediate other vulnerabilities, it is necessary to replace the update point, binary patch, and its address (located in \VulDevice\STM32F401RE\srampatch\freertos_cve_test\User\freertos_demo.c).