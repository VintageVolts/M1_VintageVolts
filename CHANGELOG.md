<!-- See COPYING.txt for license details. -->

# Changelog

All notable changes to this fork of the M1 project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project (mostly) adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.8.1.2] - 2026-03-05

### Added

- Initial fork of Monstatek M1 source code (0.8.0.0)
- Modified versioning display to include Major, Minor, Build, and RC number on Home screen
- Implemented Universal IR functionality, re-enabled from disabled code in original source. RAW mode STILL not functioning.
- Load the **IR/** folder on your SD card with any number of the folders found at [Flipper-IRDB](https://github.com/Lucaslhm/Flipper-IRDB) to add Universal IR codes
- Implemented a more natural folder flow when browsing the SD card in most functions
- BACK button in file browser after selecting a file for a particular M1 function takes you one level up instead of instantly out of the file browser.

## [0.8.2.1] - 2026-03-05

### Added

- "Dual Boot" feature. When each or the two banks in the M1 contain a bank switch feature, like the feature added to this release, you can switch between the two banks in the **Settings** menu function, **Swap FW Bank**.
- Dual booting is best acheived by two sequential SD card firmware updates to get both updates into the two separate banks.
- Personalized the **About** page
- Compiled .bin and .hex files for v0.8.2.1 can be found in the /Release folder
