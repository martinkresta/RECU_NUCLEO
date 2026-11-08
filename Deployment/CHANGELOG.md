# RECU_NUCLEO firmware changelog

Newest version first. Versions before v001.01 are not listed.

## v001.01 (2026-11-01)

- Task: VARS2 step 1 (`UHA_COMMON/tasks/vars2_dual_transmission.md`), branch `vars2-step1`
- CRC: `0x06E6831F`

- Sends all variables in the old format (`0x110+node`) and the new VARS2 format (`0x180+node`).
- Received VARS2 frames are stored in `Vars2`; control logic still uses the old variables.
- `UhaCommon` copy of `COM`, `MCAN` and `VARS.h` synced with `UHA_COMMON`, which changes CAN behaviour beyond VARS2; VARS2 module added as a local copy.
