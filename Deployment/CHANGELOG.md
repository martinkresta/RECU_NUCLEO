# RECU_NUCLEO firmware changelog

Newest version first. Versions before v001.01 are not listed.

## v001.04 (2026-11-13)

- Task: VARS2 step 6b part B (`UHA_COMMON/tasks/vars2_stop_legacy_tx.md`), branch `feature/vars2_stop_legacy_tx`
- CRC: `0xF8C3B49E`

- `UhaCommon` synced with `UHA_COMMON` `d0644cc`: COM, VARS2, uha_vars_gen, and MCAN + LOG (the new COM.c needs the MCAN diagnostics; LOG provides `MCAN_ReportError()`).
- Old variable frame `0x110+node` is no longer sent; only VARS2 `0x180+node`.
- `CMD_VAR_VALUE` RX case removed from `APP.c`. All remote inputs of RECON (`TEMP_KIDROOM`, `TEMP_DOWNSTAIRS`, `TEMP_OUTSIDE`, `BAT_SOC`) have a VARS2 row and are read through the read layer since v001.03.
- New on this node: MCAN CAN TX supervision (stall recovery, dead-TX self-reset, ABOM, TEC/REC monitor, `0x620` eleError reports 01..04). The CAN diagnostics SysInfo types (0x10..0x1A) are not hooked up in `APP.c` yet.

## v001.03 (2026-11-08)

- Task: VARS2 read layer, part B (`UHA_COMMON/tasks/vars2_read_layer.md`), branch `feature/vars2_read_recu`
- CRC: `0x990252B7`

- `VAR_EnableVars2Read()` after `COM_Init()`: the external inputs of RECON (`TEMP_KIDROOM`, `TEMP_DOWNSTAIRS`, `TEMP_OUTSIDE`, `BAT_SOC`) are now read from VARS2. When a producer stops sending (3 s for `BAT_SOC`, 9 s for temperatures) the value is flagged invalid and RECON goes to its safety configuration (supply fan 0, extract fan 10 %).
- `UhaCommon` synced with `UHA_COMMON` (VARS, VARS2, COM).

## v001.02 (2026-11-08)

- Task: VARS2 step 2 (`UHA_COMMON/tasks/elecon_vars2_tunel.md`), branch `feature/elecon_vars2_tunel`
- CRC: `0x3955D4B5`

- No functional change of the node, shared VARS2 code updated (frame hook for ELECON, unused here).
- VARS2 fix: a node's own value no longer reports a timeout in `VARS2_IsValid()` (own state refreshed when the frame is sent).
- VARS2 fix: `0x18x` frames with DLC != 8 or RTR are dropped in `COM_GetRxMessage()`.

## v001.01 (2026-11-01)

- Task: VARS2 step 1 (`UHA_COMMON/tasks/vars2_dual_transmission.md`), branch `vars2-step1`
- CRC: `0x06E6831F`

- Sends all variables in the old format (`0x110+node`) and the new VARS2 format (`0x180+node`).
- Received VARS2 frames are stored in `Vars2`; control logic still uses the old variables.
- `UhaCommon` copy of `COM`, `MCAN` and `VARS.h` synced with `UHA_COMMON`, which changes CAN behaviour beyond VARS2; VARS2 module added as a local copy.
