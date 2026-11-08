# ASW_RECU_NUCLEO — Heat Recovery Ventilation Controller

## Hardware
- **MCU:** STM32F446RE (Nucleo dev board)
- **CAN Node:** 5
- **HWID:** 0x106
- **Version:** 100 (Dec 2025)

## Role
Automatic fan speed control for the house heat recovery ventilation (HRV) unit. Controls two fans (supply + extract) based on CO₂, humidity, temperature, outdoor conditions, and battery SOC.

---

## Sensors (inputs)

**1-Wire temperature:**
| VAR | Sensor location |
|-----|----------------|
| VAR_TEMP_RECU_FC | Fresh cold — supply inlet |
| VAR_TEMP_RECU_FH | Fresh hot — supply outlet |
| VAR_TEMP_RECU_WC | Waste cold — extract inlet |
| VAR_TEMP_RECU_WH | Waste hot — extract outlet |

**I2C sensors (two buses):**
| Sensor | VAR | Bus |
|--------|-----|-----|
| SDP810 differential pressure | VAR_DP_RECU_F | I2C1 |
| SCD4x CO₂ | VAR_CO2_RECU | I2C1 |
| SHT4x humidity+temp (waste air) | VAR_RH_RECU_WH, VAR_TEMP_RECU_WH | I2C1 |
| SDP810 differential pressure | VAR_DP_RECU_W | I2C2 |
| SHT4x humidity+temp (fresh air) | VAR_RH_RECU_FH, VAR_TEMP_RECU_FH | I2C2 |

**Read from CAN (other nodes):**
- VAR_TEMP_KIDROOM, VAR_TEMP_DOWNSTAIRS — indoor temperatures
- VAR_TEMP_OUTSIDE — outdoor temperature
- VAR_BAT_SOC — battery state of charge

---

## Outputs
- **FAN_IN** — supply/intake fan (PWM)
- **FAN_OUT** — extract/exhaust fan (PWM)
- Speed range: 10–55% daytime, 10–25% night

---

## Published VARS (CAN stream, every 3s)
VAR_TEMP_RECU_FC/FH/WC/WH, VAR_RH_RECU_FH/WH, VAR_CO2_RECU, VAR_DP_RECU_F/W, VAR_RECU_FAN_F, VAR_RECU_FAN_W

---

## Control Logic Overview
The RECON module runs a 1-second update with these priorities:

1. **Battery protection** — below 30% SOC: fans off; below 85% SOC: eco mode (35% max)
2. **Antifreeze** — fresh cold side minimum 4°C
3. **Humidity anti-dry** — below 50% RH: limit fans to 20%
4. **Season mode:**
   - *Winter (Sep–Apr):* CO₂-based control — speed = (CO₂ - 600) / 10, target 800 ppm, max 1000 ppm
   - *Summer (May–Aug):* night cooling — fans run when outdoor < indoor, disabled 9–23h
5. **Time limits** — 55% max daytime (6–21h), 25% night; eco reduces to 35%

**Remote override via CAN** — CMD_RECU_REMOTE_REQ can force: Auto / SlightOverpressure / MaxOverpressure / SlightUnderpressure / MaxUnderpressure (max 1000s duration)

**Manual override** — user button on board cycles fan speed +5% per press.

---

## Key Modules
| Module | Role |
|--------|------|
| RECON | Main ventilation control algorithm |
| SENSIRION | Sensirion SDP810 / SCD4x / SHT4x drivers |
| OW | 1-Wire temperature sensors |
| scheduler | Cooperative task scheduler |
