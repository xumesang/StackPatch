# StackPatch Implementation on GD32VF103  

StackPatch on the GD32VF103 (RISC-V32) platform installs a custom exception handler to intercept faults and apply hot-patches at runtime without reboot.

---

## Usage

### 1. Modified Exception Handler (`entry.S`)

**Location:**  
`VulDevice/GD32VF103/entry.S`

**Purpose:**  
Replace the default `trap_entry` routine to capture CPU context on exceptions, invoke the StackPatch dispatcher, then return execution to the patched code path.

**Changes:**  
- Saved all general-purpose registers and interrupt state on entry.  
- Branched to the StackPatch dispatcher with the saved context pointer.  
- After the patch is applied, restored registers and returned from exception.  

**Integration Steps:**  
1. Copy our provided `entry.S` into your GD32VF103 project’s startup directory, overwriting the stock handler.  
2. Rebuild your firmware using your Eclipse-based RISC-V toolchain.  
3. Flash the updated binary to the board via GD-LINK.  
4. Monitor the serial console to verify StackPatch logs and confirm that the vulnerable routine has been patched.

---

With this handler in place, any exception triggered by a known vulnerability will automatically divert to StackPatch for live correction, then resume normal execution seamlessly.  

---

#  Development Environment Setup & Demo project 
1. Download the required toolchain from https://www.nucleisys.com/download.php 
2. Import the GD32VF103_Demo_Suites1.1.1 project using Nuclei Studio IDE (201909), and select NucleiStudio_workspace3 as the workspace. 
3. Connect the GD32VF103 development board to the update host using both GD-LINK and UART simultaneously.
4. Compile the project and flash the firmware to the development board.
5. Observe the output information from the UART port.

For instructions on how to build the project, you can refer to https://www.rvmcu.com/site/gd32vf103c_start_ide_quickstart/