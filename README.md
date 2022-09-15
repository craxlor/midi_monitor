# Midi Monitor
Inspired by Roland’s A-110 Midi-Display , this project is an implementation of a Midi-Monitoring device, based on MIOS32. Aiming for a presentation of in-time Midi-Data, two different visualization styles, interpret incoming Midi-packages and display them on an arry of OLED displays.
The first visualization interprets received midi-packages into easily readable plain-text-format, with additional hex-formatted text on another display.
In the second visualization-mode, a Keyboard is printed on all available displays, providing in-time animations, whenever a key is pressed and a Note-On-Event is passed.
Based on existing MIDIBox-Projecta by uCaps, the Midi-Monitor is based on a STM32F4-Coreboard, a Midi-I/O-Module and an OLED-Module for the connection of 4 displays. 
The standalone software package, which can be used directly with midi-devices through its 2-Ins/2-Outs Midi-Connectors, as well as via the MIOS-Studio's Interface, exceeds the possibilities of its Roland predecessor, featuring two Visualization-Modes, which can be switched by pressing the data-encoder, Midi-Data is not only presented on a keyboard giving information on Note-Events, but also providing a plain-text-visualization consisting of processed Midi-packages. Just like the Roland example, the user can choose between 16 Midi-Channels, for which each is presented at a time, and also choose an additional 17th MIDI-Channel in Text-Visualization, which disables the midi-channel filter, presenting all midi-channels at once.


## Summary
- [Midi Monitor](#midi-monitor)
  - [Summary](#summary)
  - [Requirements](#requirements)
    - [Required tools](#required-tools)
    - [Required hardware](#required-hardware)
  - [Setup Environment](#setup-environment)
  - [common pitfalls and errors](#common-pitfalls-and-errors)
    - [Some parts of mios32 won't compile anymore (i.e. datatypes like u32..)](#some-parts-of-mios32-wont-compile-anymore-ie-datatypes-like-u32)
    - [ARM-NONE-EABI-GCC not referenced errors](#arm-none-eabi-gcc-not-referenced-errors)
    - [to_string() method cannot be used when using <string> include with std::string](#to_string-method-cannot-be-used-when-using-string-include-with-stdstring)
  - [Git guidelines](#git-guidelines)
  - [License](#license)

## Requirements

### Required tools
  - [MIOS STUDIO](http://www.ucapps.de/mios_studio.html)
  - [MSYS 1.0](https://sourceforge.net/projects/mingw/files/MSYS/Base/msys-core/msys-1.0.11/MSYS-1.0.11.exe/download?use_mirror=netcologne)
  - [Arm GNU Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/downloads)

### Required hardware
  -  [MBHP_CORE_STM32F4XX](http://ucapps.de/mbhp_core_stm32f4.html)
  -  some displays

## Setup Environment
  1. install all required tools listed above whereever you want
  2. add the bin folders from the toolchain & msys to your user/system-path variable
  3. Visual Studio Code
     1. go to your settings.json 
        1. press `shift` + `ctrl` + `p`
        2. type `settings`
        3. click `Preferences: Open Settings (JSON)`
     2. add a new terminal to open msys in vsc
        ```
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
        }```
## common pitfalls and errors

### Some parts of mios32 won't compile anymore (i.e. datatypes like u32..)

<p> 
I assume it happens because some of the mios32 header files aren't supposed to be explicitly included alone due to internal dependencies, which can't be resolved anymore, because these header files don't include other header files. This is collectively done in mios32.h

To fix this, do not include single header files like mios32_midi.h  (at least without including mios32.h before).
Instead just include mios32.h :)
</p>

### ARM-NONE-EABI-GCC not referenced errors

<p>
this problem occurs due to the default setting of the compiler to use a socalled semi hosting feature. We dont need this feature and can disable it with "--specs=nosys.specs" flag. To set this flag using our makefile script, we can edit the cflags variable in common.mk like so:

```
<<CFLAGS += $(C_DEFINES) $(C_INCLUDE) -Wall -Wno-format -Wno-switch -Wno-strict-aliasing --specs=nosys.specs>>
```

hint: it seems to be the case, that the mios32_toolchain doesnt include such a nosys.specs configuration file or in another location.
</p>

### to_string() method cannot be used when using <string> include with std::string

<p>

The reason for this not to work is a known [bug](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52015) under mingw. The mios32 toolchain we are using provides an arm compiler (arm-none-eabi-gcc) with of version 4.7.4.

The problem was fixed with version 4.8.1+

To fix this, download a new version of the compiler we use for our target platform [here](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/downloads) (arm-none-eabi-gcc for windows) and replace your version in your mios32_toolchain/ folder.

alternatively you could just install the toolchain and remove the old path from to the mios32 toolchain and add the path to the new toolchain.

hint: when part of the project is already compiled with a different compiler, errors can occurr. If this is the case, try deleteing the project_build and recompile the entire project.
</p>

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
