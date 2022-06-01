@echo set environment variables
set MIOS32_GCC_PREFIX=arm-none-eabi
set MIOS32_FAMILY=STM32F4xx
set MIOS32_PROCESSOR=STM32F407VG
set MIOS32_BOARD=MBHP_CORE_STM32F4
set MIOS32_LCD=universal
set MIOS32_BIN_PATH=/D/Coding_projects/Cpp/midi_monitor/mios32/bin
set MIOS32_PATH=/D/Coding_projects/Cpp/midi_monitor/mios32

@echo start msys terminal
cd C:\msys\1.0\bin
START sh.exe --login -i -c "cd '/c/YOUR/DIRECTORY'; $SHELL"

@echo DELETE THIS PART AFTER CONFIGURING THE PATH ABOVE
pause