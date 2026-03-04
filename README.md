<!-- See COPYING.txt for license details. -->

**This is a FORK of the original MonstaTek M1 source code. It is intended for personal use, but I will make updates available as I add and vet them.**

There are no guarantees to the accuracy, availability, or functionality of this code, and its use is AT YOUR OWN RISK!!! I will also be using my own versioning sequence, which most likely will not match those of Monstatek, or other forked versions of the project.

The following content, and links to additional pages, is from the original Mostatek site for your convenience to learn about the M1 device. I highly recommend you first look at the primary project at the MostaTek GitHub site at: [https://github.com/Monstatek/M1](https://github.com/Monstatek/M1) before continuing through this personal repository.


# M1 Firmware

Firmware for the M1 NFC/RFID multi-protocol device, built on STM32H5.

## Overview

The M1 firmware provides support for:

- **NFC** (13.56 MHz)
- **LF RFID** (125 kHz)
- **Sub-GHz** (315–915 MHz)
- **Infrared** (IR transmit/receive)
- **Bluetooth** (BLE scan, advertise, GATT; via ESP32 co-processor)
- **WiFi** (AP scan, station/AP modes; via ESP32 co-processor)
- **Battery** monitoring
- **Display** (ST7586s ERC240160)
- **USB** (CDC, MSC)

## Hardware

- **MCU:** STM32H573VIT6 (32-bit, 2MB Flash, 100LQFP)
- **Hardware revision:** 2.x

See [HARDWARE.md](HARDWARE.md) for more details.

## Documentation

- [Build Tool (mbt)](documentation/mbt.md) – Build with STM32CubeIDE or VS Code
- [Architecture](ARCHITECTURE.md) – Project structure
- [Development](DEVELOPMENT.md) – Development guidelines

## Building

**See [documentation/mbt.md](documentation/mbt.md) for full build instructions** (STM32CubeIDE and VS Code setup, extensions, and optional post-build CRC).

### Prerequisites

- **STM32CubeIDE 1.17+** (recommended), or  
- **VS Code** with ARM GCC 14.2, CMake Tools, Cortex-Debug, and Ninja, or
- **Linux** with ARM GCC toolchain and Ninja

### Build steps

**Linux:**  
```bash
make
```

Output: `./artifacts/` (MonstaTek_M1_v0800.elf, .bin, .hex)

**STM32CubeIDE:**  
Open the project and build in the IDE.

**VS Code:**  
1. Configure the project (e.g. `gcc-14_2_build-release` or `gcc-14_2_build-debug`)  
2. Build via the Build icon  

Output: `./out/build/gcc-14_2_build-release` (VS Code) or `./Release` (STM32CubeIDE)

## Contributing

Contributions are welcome. See [CONTRIBUTING.md](.github/CONTRIBUTING.md) and the [Code of Conduct](.github/CODE_OF_CONDUCT.md).

## License

See [LICENSE](LICENSE) for details.
