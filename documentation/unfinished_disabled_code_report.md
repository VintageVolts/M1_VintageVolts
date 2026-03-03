# Unfinished & Disabled Code Report

*Generated: March 3, 2026*

---

## 1. Disabled Code Blocks (`#if 0` ... `#endif`)

In C, wrapping code in `#if 0` / `#endif` is a common way to "turn off" code without deleting it. The compiler completely ignores everything between those lines. Here are the significant ones in the project's own code:

### m1_bq27421.c — Battery Fuel Gauge (6 disabled blocks)

- **Line 573:** Old way to read state-of-charge percentage — replaced by `bq27421_soc(FILTERED)` on line 581
- **Line 583:** Alternate logic for reading SOC/SOH based on health state — superseded by simpler direct calls
- **Line 878:** `bq27421_readStateofCharge_percent()` — entire function disabled, replaced by `bq27421_soc()`
- **Line 1219:** `bq27421_reset()` — factory reset function, disabled (soft reset version exists and is active)
- **Line 1408:** `bq27421_writeBlockData()` — write a byte to the chip's memory, disabled
- **Line 1450:** `bq27421_writeBlockChecksum()` — write a checksum value, disabled

> **What this means:** The fuel gauge driver went through a refactor. Old functions were disabled rather than deleted. The active code uses newer/better versions.

### m1_infrared.c — Infrared (3 disabled blocks)

- **Line 217:** Entire IR transmit UI + encode + transmit sequence disabled (~20 lines) — the "Universal Remotes" feature's transmit logic is turned off
- **Line 241:** IR transmit call inside the main loop — disabled
- **Line 258:** LED blink off + cleanup on exit — disabled

> **What this means:** The "Universal Remotes" IR transmit feature is **structurally present but not functional**. The function exists and responds to button input, but the actual transmitting is turned off.

### m1_nfc.c — NFC

- **Line 201:** Empty `view_nfc_tools_table[]` and `view_nfc_saved_table[]` — placeholder arrays with no entries

> **What this means:** The NFC tools and saved-cards views have been stubbed out but not filled in yet.

### m1_power_ctl.c — Power Control

- **Line 34:** Status message strings for charger VBUS and charge state — disabled

> **What this means:** Human-readable status messages for the charger chip were defined but disabled (perhaps they were used for debugging and no longer needed on screen).

### m1_usb_cdc_msc.c — USB Serial (3+ disabled blocks)

- **Line 339:** Major block — an older USB CDC data handling approach with a mode-change feature, plus a nested `#if 0` debug mode toggle. The active code (after `#else`) uses a simpler `portMAX_DELAY` wait.
- **Line 378:** Semaphore-based mutex for USB-to-serial TX — disabled
- **Line 407:** Uses `#if 1` / `#else` to choose between polling-based DMA wait vs semaphore-based DMA wait — the semaphore approach is currently the disabled alternative.

> **What this means:** The USB serial module has been through iteration. There are two competing approaches left in the code (polling vs semaphore for DMA complete). The active one is the polling approach.

### uiView.c — UI Framework

- **Line 105:** Early-return-if-same-mode check disabled

### lfrfid/ — Low Frequency RFID (multiple files)

- **lfrfid.c line 537:** Part of the RFID write-done event flow is disabled
- **lfrfid.h line 42:** An old `RfidReadTypeDef` enum (state machine for RFID reading) — replaced
- **lfrfid_protocol_em4100.c line 192:** `setEm4100_bitrate()` — disabled
- **lfrfid_protocol_em4100.c line 960:** Wave TX data struct — disabled
- **lfrfid_protocol_em4100.c line 982:** `App_WaveTx_Init()` / `App_WaveTx_Start()` — old emulation init, disabled

---

## 2. Placeholder / "To Be Determined" Functions

### m1_nfc.c — `nfc_tools()` (line 1625)

The comment literally says **"NFC tools menu function (To Be Determined)"** and **"Placeholder function"**. It shows a menu structure and handles Back button, but **does nothing useful yet**. This is an unfinished feature.

### m1_bt.c — `menu_bluetooth_init()` (line 66)

The Bluetooth init function has an **empty body** (just `;`). It's declared and called but does nothing.

### m1_bt.c — `bluetooth_config()` (line 327)

The Bluetooth configuration function has a loop with placeholder comments (`; // Do other parts of this task here`) but **no actual configuration logic**. It only handles the Back button to exit.

---

## 3. Dummy/Stub Functions

### m1_esp_hosted_dummy.c — Entire file

Three functions that do nothing, exist purely to satisfy the linker (prevent compile errors):

- `get_esp32_ready_status()` — always returns 0
- `test_get_available_wifi()` — empty
- `esp32_app_init()` — empty

### m1_tasks.c — `m1_dummy_task()` and `m1_dummytimer_task()`

Lines 195 and 213: Both are infinite loops that just sleep forever. Comments say *"defined as a dummy task to avoid compile error!"*

---

## 4. Commented-Out Function Declarations (Possibly Unfinished)

### m1_bq25896.h — lines 215-216

`bq_enableBATFET()` and `bq_disableBATFET()` — declared but commented out. These would control the battery FET (the switch that connects/disconnects the battery). A `bq_shipMode()` function exists instead, which may be the replacement.

### m1_bq27421.h — lines 271 and 278

`bq27421_readStateofCharge_percent()` and `bq27421_readStateofHealth_percent()` — commented out in the header, matching the `#if 0` blocks in the .c file.

### m1_power_ctl.c (line 66) and m1_power_ctl.h (line 22)

`power_init()` — commented out in both files. The function was planned but never implemented or was removed.

---

## 5. "Needs Work" / Temporary Values

### m1_esp_hosted_config.h — lines 47-48

Two config values explicitly marked as **"Dummy number, should be adjusted properly later!"**:

- `CONFIG_ESP_HOSTED_MAX_SIMULTANEOUS_SYNC_RPC_REQUESTS = 10`
- `CONFIG_ESP_HOSTED_MAX_SIMULTANEOUS_ASYNC_RPC_REQUESTS = 10`

### m1_i2c.h — lines 41-43

`I2C_DEVICE_STC3115` and `I2C_DEVICE_LP5562` are both defined as `DUMMY_I2C_DEV (0xFF)`. These are I2C device addresses for chips (a battery gauge and an LED driver) that are probably on the hardware design but their drivers aren't wired up yet.

---

## 6. Explicit TODO

### bit_util.c — line 163

`// TODO: match the longest symbol first` — an optimization that hasn't been done yet.

---

## 7. Placeholder Comments Indicating Future Work

Several locations have lone semicolons with comments like:

- `; // Do other parts of this task here`
- `; // Display error here` (m1_fw_update_bl.c line 638)
- `; // Send warning message to display` (m1_infrared.c line 146)
- `; // exit` (m1_infrared.c line 147)

These are spots where someone intended to add real logic later but left an empty placeholder.

---

## Summary Table

| Area | Status | Risk Level |
|------|--------|-----------|
| **NFC Tools menu** | Unfinished (TBD placeholder) | Low — menu loads but does nothing |
| **IR Universal Remotes transmit** | Core logic disabled | Medium — feature appears in menu but won't work |
| **Bluetooth config** | Empty shell | Low — has UI loop but no config logic |
| **Bluetooth init** | Empty body | Low — called but does nothing |
| **ESP32 hosted config values** | Marked as dummy/temp | Medium — could cause issues under load |
| **I2C dummy devices** (STC3115, LP5562) | Not connected | Low — hardware not present or driver not integrated |
| **USB CDC DMA approach** | Two approaches left in code | Low — works, but dead code should be cleaned up |
| **Battery gauge old functions** | Disabled, replaced | Low — cleanup opportunity |
| **Firmware update error display** | Not implemented | Low-Medium — errors silently ignored |
| **ESP32 dummy functions** | Intentional stubs | Low — exist to prevent linker errors |
