✅ Known problems in ESP32‑P4 rev 1.03 (rev 1.x)
1) MSPI power‑on / wake‑up faults

What happens:
On power‑on or wake‑up from deep sleep, MSPI access can trigger a load access fault, potentially causing boot failures or hangs.
Status: Affected in rev 1.x → Fixed in v3.2
Impact:
Unreliable boot or early crashes, especially with external flash/PSRAM activity.
Workaround:
Software sequencing changes (per errata) and conservative wake‑up flows.
 [docs.espressif.com]


2) PSRAM DMA read incoherency on unaligned/overlapping accesses

What happens:
Unaligned DMA reads from PSRAM—especially with overlapping addresses—may return stale data.
Status: Affected in rev 1.x
Impact:
Data corruption in high‑throughput or DMA‑heavy paths (graphics, media).
Workaround:
Ensure aligned DMA buffers and avoid overlapping regions.
Notes: Listed in errata under MSPI/PSRAM sections.
 [docs.espressif.com]


3) Asynchronous MSPI timing errors under overlap

What happens:
Certain overlapping read/write patterns at specific frequencies can yield data errors due to MSPI address overlap detection timing.
Status: Affected in rev 1.x
Impact:
Intermittent corruption under load.
Workaround:
Avoid overlapping MSPI transactions; adjust timing/clocking.
Notes: Errata MSPI‑749/750/751 cluster.
 [docs.espressif.com]


4) ROM secure‑boot / secure‑download edge cases

What happens:

Secure Boot verification can fail due to an internal ROM buffer address issue.
Secure Download Mode can fail during flash power‑on sequences.


Status: Affected in rev 1.x → Fixed in v3.x (incl. v3.2)
Impact:
Issues during secure provisioning/updates.
Workaround:
Follow errata‑specified flows; update silicon if possible.
Notes: ROM‑764 / ROM‑770.
 [docs.espressif.com]


5) DMA channel permission conflict

What happens:
DMA channel 0 transaction‑ID overlap can cause permission management issues.
Status: Affected in rev 1.x
Impact:
Faults or blocked transactions under certain DMA patterns.
Workaround:
Channel allocation discipline per errata guidance.
 [docs.espressif.com]


6) AHB permission block affecting Flash/PSRAM

What happens:
Unauthorized AHB accesses may block subsequent flash or PSRAM transactions.
Status: Affected in rev 1.x
Impact:
System stalls or memory access failures.
Workaround:
Strict access control and sequencing; see errata APM sections.
 [docs.espressif.com]


7) Analog regulator instability when a power domain is off

What happens:
On‑chip output regulators may not provide reliable supply if the peripheral power domain is disabled.
Status: Affected in rev 1.x
Impact:
Brown‑out‑like behaviors or peripheral malfunction.
Workaround:
Keep required domains powered per datasheet/errata.
 [docs.espressif.com]


Tooling / ecosystem realities for rev 1.03 (practical issues)
8) OpenOCD / USB‑JTAG early support caveats

What happens:
Older OpenOCD builds could fail to identify P4 targets or mishandle reset/flash for early silicon.
Status:
Resolved by updating OpenOCD‑ESP32 (adds revision reporting and proper handling).
Action:
Use a recent Espressif OpenOCD release that explicitly supports ESP32‑P4 revisions.
 [github.com]


Summary table


















































AreaRev 1.03 statusFixed laterMSPI boot/wake faults❌ Affected✅ v3.2PSRAM DMA coherency❌ Affected✅ v3.xMSPI overlap timing❌ Affected✅ v3.xSecure boot/download ROM bugs❌ Affected✅ v3.xDMA channel conflicts❌ Affected✅ v3.xAHB permission blocks❌ Affected✅ v3.xPower‑domain regulator issue❌ Affected✅ v3.xOpenOCD early P4 support⚠️ Tooling‑dependent✅ Updated tools

What this means for your project today

Rev 1.03 is usable, but you must follow errata workarounds, especially for MSPI/PSRAM and secure features.
If you control hardware choice, v3.x silicon is strongly recommended for stability, performance, and fewer guard‑rails.
Ensure your OpenOCD / ESP‑IDF versions are recent and match the target revision (you already saw the revision‑mismatch protection in program_esp).


Below is a practical, step‑by‑step guide to applying Espressif’s errata workarounds specifically for ESP32‑P4 chip revision v1.03 (rev 1.x).
Everything here maps directly to official Espressif errata entries, and I’ll show where the workaround lives (Kconfig, code, boot flow) and how to apply it safely. Source references are included at the end of each section.

How to Apply ESP32‑P4 Rev 1.03 Errata Workarounds
0️⃣ First: positively select “rev < 3” in ESP‑IDF
This single step enables many automatic safeguards.
✅ Set this in sdkconfig
INICONFIG_ESP32P4_SELECTS_REV_LESS_V3=yShow more lines
Why this matters

It tells ESP‑IDF you are running rev 1.x silicon
ESP‑IDF conditionally enables internal workarounds
Without this, the ROM/driver code may assume rev 3.x behaviour

📌 This flag is explicitly documented as required for rev 1.x silicon and must be cleared for v3.x chips [electronic...change.com]

1️⃣ MSPI power‑on / wake‑up faults (ERRATA: MSPI‑749)
❌ Problem (rev 1.03)

MSPI flash access can fault on power‑on or deep‑sleep wake
Leads to early boot crashes or “load access fault”

✅ Workaround (SOFTWARE)
Do not touch flash immediately after reset.
✅ ESP‑IDF already applies a delay if step 0 was done.
✅ If you do early MSPI access yourself:
C#include "esp_rom_delay_us.h"void app_main(void) {    esp_rom_delay_us(2000); // 2ms guard    // Safe flash activity starts here}Show more lines
✔ Fixed in later silicon (v3.2), but mandatory in rev 1.x [circuitlabs.net]

2️⃣ PSRAM DMA data corruption (ERRATA: MSPI‑750 / 751)
❌ Problem (rev 1.03)

Unaligned or overlapping DMA reads from PSRAM
Can return old or corrupted data

✅ Workaround (CODE DISCIPLINE)
✅ Always align DMA buffers
Cuint8_t buffer[1024] __attribute__((aligned(64)));Show more lines
✅ Never overlap DMA regions
❌ Bad:
Cdma_read(buf + 1, buf + 512);Show more lines
✅ Good:
Cdma_read(buf, other_buf);``Show more lines
ESP‑IDF drivers expect developers to enforce this on rev 1.x [circuitlabs.net]

3️⃣ MSPI timing / overlap errors
❌ Problem

Concurrent or overlapping MSPI reads/writes at some frequencies
Can produce intermittent corruption

✅ Workarounds

Avoid overlapping MSPI transactions
Use blocking flash APIs
Avoid manual MSPI register twiddling

If using LVGL, USB, camera, etc., serialise flash access via critical sections.
📌 These restrictions are listed under rev 1.x errata only [circuitlabs.net]

4️⃣ Secure Boot & Secure Download failures (ERRATA: ROM‑764 / 770)
❌ Problem

Secure boot verification may fail due to ROM buffer errors
Secure Download Mode can fail during flash power‑on

✅ Workarounds
✅ Enable ESP‑IDF ROM fixes via step 0
✅ Avoid custom secure‑boot staging code
✅ Keep secure operations single‑shot per boot
❗ Do not force‑flash mismatched bootloaders — OpenOCD correctly blocks this (you already saw it)
✔ Fixed in v3.x silicon [circuitlabs.net]

5️⃣ DMA channel permission conflicts (ERRATA: DMA‑767)
❌ Problem

DMA channel 0 may collide permission IDs

✅ Workarounds

Prefer non‑zero DMA channels
Use ESP‑IDF DMA APIs (do not hard‑code channel 0)
Avoid mixing DMA frameworks

Cspi_bus_config_t cfg = {    .dma_chan = SPI_DMA_CH_AUTO};``Show more lines
 [circuitlabs.net]

6️⃣ AHB permission blocks (ERRATA: APM‑560)
❌ Problem

Unauthorized AHB access can block future flash/PSRAM access

✅ Workarounds

Avoid raw AHB writes
Use driver APIs only
Don’t access uninitialised peripherals

✔ Automatically mitigated when using ESP‑IDF drivers [circuitlabs.net]

7️⃣ Analog regulator instability
❌ Problem

Regulators unstable when peripheral power domains are off

✅ Workarounds

Keep required power domains enabled
Avoid dynamic domain toggling unless documented safe

📌 Hardware + software constraint for rev 1.x [circuitlabs.net]

8️⃣ Tooling‑specific workaround (OpenOCD, USB‑JTAG)
✅ REQUIRED

Use updated OpenOCD‑ESP32 builds that understand P4 revisions
Do not bypass revision checks unless you are recovering hardware

These fixes were added recently in OpenOCD‑ESP32 to support rev 1.x reliably [esp32.com]

✅ Minimal Rev 1.03 Checklist
✅ CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y
✅ Aligned & non‑overlapping PSRAM DMA
✅ MSPI guard delay after reset
✅ No forced flash of mismatched binaries
✅ Current OpenOCD‑ESP32 toolchain

Final takeaway
ESP32‑P4 rev 1.03 is usable, but assumes disciplined software.
ESP‑IDF already applies many fixes if you identify the silicon correctly.

If you skip the revision flags, you get silent bugs.






### sdkconfig.cmake

Putting it together in order:

Defaults applied

sdkconfig.defaults
target-specific defaults


Kconfig resolution

All Kconfig files merged


Outputs generated

sdkconfig
sdkconfig.h
sdkconfig.cmake


CMake configuration continues
Compilation starts

This matches ESP‑IDF’s documented build flow. [docs.espressif.com]


---
### bootloader
```
esp-idf>python -m esptool --chip esp32 --port com7 write_flash -z 0x2000 c:/nf-interpreter/build/bootloader/bootloader.bin
```
### nanoCLR
```
python -m esptool --chip esp32p4 --port com7 write_flash -z 0x2000 c:/nf-interpreter/build/nanoCLR.bin
```
### Partitions
```
python -m esptool --chip esp32p4 --port com7 write_flash -z 0x2000 C:/nf-interpreter/build/partitions_8mb.bin
```
