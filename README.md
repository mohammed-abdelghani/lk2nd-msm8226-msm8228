
<h1 align="center">lk2nd</h1>

<p align="center">
  A custom bootloader for Qualcomm SoC MSM8228 and MSM8226
</p>

<p align="center">
  <a href="https://github.com/mohammed-abdelghani/lk2nd-msm8226-msm8228/blob/main/LICENSE"><img src="https://img.shields.io/badge/license-MIT-blue.svg" alt="MIT License"></a>
  <a href="https://github.com/mohammed-abdelghani/lk2nd-msm8226-msm8228/releases"><img src="https://img.shields.io/badge/releases-latest-orange.svg" alt="Releases"></a>
  <a href="README.ar.md"><img src="https://img.shields.io/badge/docs-%D8%A7%D9%84%D8%B9%D8%B1%D8%A8%D9%8A%D8%A9-green.svg" alt="Arabic documentation"></a>
</p>

---

It makes booting standard operating systems simple by providing:

- **A unified boot and flashing interface** (Android Fastboot), independent
  of device-specific changes to the boot chain (e.g. Samsung's proprietary
  flashing interface)
- **Automatic device and hardware detection** (display panels, batteries, ...)
- **Extra boot options** (file system boot, booting from SD card)
- **Extra firmware features** (booting secondary CPU cores when the firmware
  lacks [PSCI] support)
- **Workarounds for device-specific issues** (USB on some Samsung devices,
  modem on Motorola phones, ...)
- **Debugging commands** for hardware research and development

---

## How it works

lk2nd comes in two configurations:

- **lk2nd** — a "secondary" bootloader for devices whose stock firmware
  cannot easily be replaced (most phones and tablets). It does **not**
  replace the stock bootloader. Instead, it is packaged into an Android
  boot image that the stock bootloader loads like any other OS image.
  The real operating system goes into the boot partition with a 512 KiB
  offset, or into an ext2 file system. It doesn't have to be Android or
  even Linux — any kernel can be packed into an Android boot image.

- **lk1st** — a primary bootloader for single-board computers (SBCs) and
  advanced users. It is the "first" bootloader responsible for loading the
  main operating system.

This repository is a fork of the [original open-source bootloader from
Qualcomm](https://git.codelinaro.org/clo/la/kernel/lk/-/tree/lk.lnx.1.0.r54-rel),
itself a heavily modified version of the [Little Kernel] embedded OS.

---

## Supported devices

lk2nd supports a large number of devices across multiple Qualcomm SoCs.
See [devices.md](Documentation/devices.md) for the full list.

---

## Installation

1. Download `lk2nd.img` from [Releases](https://github.com/mohammed-abdelghani/lk2nd-msm8226-msm8228/releases).
2. Flash it with the stock flashing interface:

   ```sh
   # Fastboot
   fastboot flash boot lk2nd.img

   # Samsung (Heimdall)
   heimdall flash --BOOT lk2nd.img

   # EDL
   edl w boot lk2nd.img
   ```

If you get `fastboot: error: Couldn't parse partition size '0x'`, try one of
the following workarounds:

```sh
fastboot flash:raw boot lk2nd.img
# or
fastboot boot lk2nd.img && fastboot flash lk2nd lk2nd.img
```

---

## Usage

Once installed, lk2nd presents the standard Android fastboot protocol for
flashing and booting operating systems.

- Press **Volume Down** during boot to enter Fastboot mode.
- Press **Volume Up** during boot to enter Recovery mode.

> [!TIP]
> If your stock bootloader uses the same key combinations, wait a moment
> before pressing the volume keys — typically until the screen turns on or
> the device vibrates — so the stock bootloader ignores them.

To update lk2nd itself through its own fastboot interface:

```sh
fastboot flash lk2nd lk2nd.img
```

> [!IMPORTANT]
> `fastboot flash boot boot.img` writes the boot image with a 512 KiB
> offset inside the boot partition, so that flashing an OS never
> overwrites lk2nd itself.

All other fastboot commands behave normally.

### Troubleshooting

If the device shows up in fastboot, you can retrieve an lk2nd log file:

```sh
fastboot oem log && fastboot get_staged output.txt
```

Use `/dev/stdout` instead of `output.txt` to print the log directly.

---

## Building from source

Requirements and build options are documented in
[building.md](Documentation/building.md). In short:

```sh
make TOOLCHAIN_PREFIX=arm-none-eabi- lk2nd-msmXXXX
```

The resulting image is written to `build-lk2nd-msmXXXX/lk2nd.img`.

See [targets.md](Documentation/targets.md) for the list of supported
build targets.

## Documentation

| Document | Contents |
| --- | --- |
| [boot.md](Documentation/boot.md) | Boot methods (boot image, fastboot, extlinux.conf) and cmdline arguments |
| [building.md](Documentation/building.md) | Build requirements and flags |
| [devices.md](Documentation/devices.md) | Supported devices |
| [targets.md](Documentation/targets.md) | Build targets |
| [dt-bindings.md](Documentation/dt-bindings.md) | Device-tree format |
| [fastboot.md](Documentation/fastboot.md) | lk2nd fastboot commands |

