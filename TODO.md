> # TODO
> * test new pin def in pwm.c
> * finish USART with new pin def
> * make general purpose id_to_dev and dev_to_id functions
> 
> * add more options to fconfig_UART
> * add complementary support to PWM
> * move non PWM pins from pwm.h to their 
> * 
> * make it possible to disable the PLL and HSE
> * make use of the code stored in comments in "main.h"
> * implement printf: https://gitlab.com/mcturra2000/rpi/-/blob/master/stm32f411re/cmsis/printf.c https://gitlab.com/mcturra2000/rpi/-/blob/master/stm32f411re/cmsis/printf.h
> * add config locks
> * use the fpu: https://gitlab.com/mcturra2000/rpi/-/blob/master/stm32f411re/cmsis/12-fpu/main.c
> * External counters? (tim.h)
> ## DOCS
> * what does BSRR do
> * what is the difference between speed modes
> ## VISUAL
> * TypeDef -> _t
> ## CAR
> * connection guard
> * drive back in self driving mode on disconnect