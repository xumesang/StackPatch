# StackPatch Implementation on ESP32S3

StackPatch on the ESP32S3 (Xtensa‑LX7) platform hooks into the FreeRTOS exception mechanism and instruments target functions to enable live hot‑patching without reboot.

---

## 1. Modified Exception Handler (`panic_handler_asm.S`)

**Location:**  
`<esp-idf-root>/components/freertos/port/xtensa/xtensa_vectors.S` (or your project’s equivalent)

**Changes:**  
- Replaces the default `xt_panic` handler with a custom entry point.  
- Saves CPU registers and stack context, then transfers control to the StackPatch dispatcher.  
- After patch application, restores context and returns execution to the fixed code path.

**Integration Steps:**  
1. Copy the provided `panic_handler_asm.S` into your ESP‑IDF component directory, overwriting the stock handler.  
2. Rebuild your ESP‑IDF project as usual:  
   ```bash
   idf.py fullclean
   idf.py build
   idf.py flash