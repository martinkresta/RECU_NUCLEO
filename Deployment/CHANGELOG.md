# RECU_NUCLEO firmware changelog

Newest version first. Versions before v001.01 are not listed.

## v002.02 (2026-11-17)

- Task: commands step 3c (`UHA_COMMON/tasks/cmd_inverters_recu.md`, mapping agreed in `UHA_COMMON/tasks/cmd_inverters_recu_report.md`),
  branch `feature/cmd-3c` (UHA_COMMON + ASW_ELECON + ASW_ELECON_D + ASW_RECU_NUCLEO)
- CRC: `0xB7D6275B`

- Commands on CAN `0x205` (`APP_OnCommand()`): `CMD_RECU_CO2_SETPOINT` (501, 400..1200 ppm) and
  `CMD_RECU_ANTIDRY_RH` (502, 20..70 %). No mode commands (the remote modes stay on `0x140` from TECHM).
- RECON: the CO2 start point (was the constant 600: fans `(CO2 - 600) / 10` % above 610 ppm) and the anti dry RH
  limit (was `RH_ANTI_DRY` 50 %, hysteresis 2 % unchanged) are variables with setters `RECON_SetCo2Setpoint()` /
  `RECON_SetAntiDryRh()` (clamp to `CMD_RECU_*_MIN/_MAX`), used from the next `RECON_Update_1s()`. Defaults = the
  old values, so nothing changes without a command. RAM only.
- New states `ST_RECU_CO2_SETPOINT` (512), `ST_RECU_ANTIDRY_RH` (513) in `States.Recu` (`PublishStates()`).
- Flash +444 B, RAM +40 B (since the 3a-state build of v002.01).

## v002.01 (2026-11-15)

- Task: VARS2 states step 2b (`UHA_COMMON/tasks/states_nodes.md`), branch `feature/vars2-states-2b`
  (UHA_COMMON + this project); report `UHA_COMMON/tasks/states_nodes_report.md`
- CRC: `0x0DCEC5AA`

- New VARS2 group `States.Recu` (ids 510–511), `send: on_change` (on change, min 1 s apart, + every 20 s).
- Remote request mode (`mRemoteMode`) as enum `RecuRemoteMode`.
- Active fan limitation as enum `RecuLimit`: set where each `RECON` branch is taken, the highest priority of the
  cycle wins (InvalidInputs > BatteryOff > AntifreezeStop > AntifreezeReduced > AntiDry > Eco > None);
  written at every exit of `RECON_Update_1s()`. `RECON_Init()` gets the states group pointer.
- Shared code: VARS2 `send: on_change` (step 2a), new YAML rows of all nodes. No change of control behaviour.
- Flash +1.4 KB, RAM +0.5 KB (since v002.00).

## v001.05 (2026-11-14)

- Task: VARS2 direct access part B (`UHA_COMMON/tasks/vars2_direct_access.md`), branch `feature/vars2_direct_access`
- CRC: `0xE65E9036`

- `UhaCommon/` synced with `UHA_COMMON` `main` (direct access part A + METEO).
- Variables live in `Vars2` (value in physical units + valid flag); the old `Vars[]` / `VAR_*` API is no longer used here.
- RECON reads its sensors (`Recu.Air`), the room / outside temperatures (`Temperatures`) and the battery SOC
  (`GlobalEnergy.Battery`) directly and writes the fan levels to `Recu.Fans`. Temperatures and thresholds in degC
  (were 0.1 degC integers); same safety fallback when one of the 7 inputs is invalid.
- TEMP and SENSIRION sensors configured by `VID_*`.
- VARS2 sends this node's variables by the YAML `period_ms` (same 11 variables at 3 s as the removed
  `COM_AddStreamedVariable()` list). CAN frames unchanged.
- Flash -0.3 KB, RAM -1.7 KB.

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
