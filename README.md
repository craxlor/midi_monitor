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
  - [mios32 toolchain](http://www.midibox.org/mios32_toolchain/)

### Required hardware
  -  MBHP_CORE_STM32F4XX
  -  some displays

## Setup Environment
WIP

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