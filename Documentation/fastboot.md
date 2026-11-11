# lk2nd fastboot commands
[العربية](fastboot.ar.md)

lk2nd includes a fastboot implementation and provides a selection of additional
commands for debugging and development.

> [!NOTE]
> Not all fastboot commands may be enabled on a given build of lk2nd.
> Use `fastboot oem help` to find which commands are available.

- `oem battery` - Show battery voltage, state of charge and status.
- `oem dtb` - Stage dtb.
- `oem hash` - Hash staged data using hardware crypto.
- `oem log` - Stage lk log.
- `oem reboot-edl` - Reboot into EDL mode.
- `oem screenshot` - Stage a screenshot.
- `oem debug cpuid` - Dump CPUID registers.
- `oem debug (read|write)(b|hw|l|q|pmic)` - Peek/Poke memory.
- `oem debug spmi-regulators` - Dump regulstors state.
