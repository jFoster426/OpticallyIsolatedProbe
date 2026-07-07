cls
@echo ---- Beginning Build ----
@echo off

xc8-cc -O0 -mcpu=pic16f15323 main.c -o FW_PIC.hex

del *.cmf *.d *.elf *.hxl *.p1 *.sdb *.sym *.lst *.o *.s *.rlf