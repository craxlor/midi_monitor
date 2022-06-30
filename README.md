# Midi Monitor
MISSING DESCRIPTION

## Summary
- [Midi Monitor](#midi-monitor)
  - [Summary](#summary)
  - [Requirements](#requirements)
    - [Required tools](#required-tools)
    - [Required hardware](#required-hardware)
  - [Setup Environment](#setup-environment)
  - [Git guidelines](#git-guidelines)
  - [License](#license)

## Requirements

### Required tools
  - [MIOS STUDIO](http://www.ucapps.de/mios_studio.html)
  - [MSYS 1.0](https://sourceforge.net/projects/mingw/files/MSYS/Base/msys-core/msys-1.0.11/MSYS-1.0.11.exe/download?use_mirror=netcologne)
  - [Arm GNU Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/downloads)

### Required hardware
  -  MBHP_CORE_STM32F4XX
  -  some displays

## Setup Environment
  1. install all required tools listed above whereever you want
  2. add the bin folders from the toolchain & msys to your user/system-path variable
  3. Visual Studio Code
     1. go to your settings.json 
        1. (press `shift` + `ctrl` + `p`)
        2. type `settings`
        3. click `Preferences: Open Settings (JSON)`
     2. add a new terminal to open msys in vsc
        1. ```
            "terminal.integrated.profiles.windows": {
                "Msys 1.0 MIOS": {
                    "overrideName": true,
                    "color": "terminal.ansiGreen",
                    "env": {
                        "MIOS32_GCC_PREFIX": "arm-none-eabi",
                        "MIOS32_FAMILY": "STM32F4xx",
                        "MIOS32_PROCESSOR": "STM32F407VG",
                        "MIOS32_BOARD": "MBHP_CORE_STM32F4",
                        "MIOS32_LCD": "universal",
                        "MIOS32_BIN_PATH": "/D/Coding_projects/Cpp/midi_monitor/mios32/bin",
                        "MIOS32_PATH": "/D/Coding_projects/Cpp/midi_monitor/mios32",
                    },
                    "path": "C:\\msys\\1.0\\bin\\sh.exe",
                    "args": [
                        "--login",
                        "-i",
                        "-c",
                        "cd '/d/Coding_projects/Cpp/midi_monitor'; $SHELL"
                    ]
                }
              }
              ```



## Git guidelines
inspired by [blackfalcon](https://gist.github.com/blackfalcon/8428401) & [angular](https://github.com/angular/angular/blob/main/CONTRIBUTING.md#commit)

<> aren't part of the name, only mark variables
- branch hierachy
  - master
    - `<feature>`
      - `<type>`-`<titel-kebab-case>`
- pull requests
  - have to be reviewed by at least one other person
  - rebase, merge
- Commit & pull request format
  - `<type>`:`<short summary>`
  - `<type>`
    - docs      - documentation only
    - feat      - new feature
    - fix       - a bug fix
    - refactor  - a code change that neither fixes a bug nor adds a feature
    - perf      - a code change that improves performance
  - `<short summary>`
    - present tense
    - not capitalized
    - no period at the end

## License
Copyright (C) <2022> <your name> (<your email address>)
Licensed for personal non-commercial use only.
All other rights reserved.