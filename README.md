# STM32F_CMSIS
CMSIS HAL library for STM32F4
>## Features
>* GPIO
>* EXTI
>* TIM
>* * PWM
>* * S0S90 (quadrature) Encoders
>* SYS_CLOCK
>* * SYS_TICK
>* CRC
>* USART
>* * Buffered RX IRQ
>* I2C (master only)
>* UUID
>* * RNG
>* WATCHDOG

# GPIO
```C
#include "gpio.h"

int main(void) {
    fconfig_GPIO(PORT, PIN, MODE, PULLUP, OUTPUT_TYPE, SPEED, ALT_FUNC);
    config_GPIO(PORT, PIN, MODE, PULLUP, OUTPUT_TYPE);
    // MODE:        GPIO_input, GPIO_output, GPIO_alt_func, GPIO_analog
    // PULLUP:      GPIO_no_pull, GPIO_pull_up, GPIO_pull_down
    // OUTPUT_TYPE: GPIO_push_pull, GPIO_open_drain
    // SPEED:       GPIO_low_speed (default), GPIO_medium_speed, GPIO_high_speed, GPIO_very_high_speed,
    // ALT_FUNC:    uint8_t [0, 15]
    GPIO_write(PORT, PIN, DATA);
    // DATA:        uint8_t [0, 1]
    GPIO_toggle(PORT, PIN);
    uint8_t data = GPIO_read(PORT, PIN);
    
    disable_GPIO(PORT);
}
```

# EXTI
```C
#include "gpio.h"
#include "exti.h"


int main(void) {
    config_GPIO(PORT, PIN, MODE, PULLUP, OUTPUT_TYPE);
    config_EXTI(EXTI_LINE, PORT, FALLING, RISING);
    // EXTI_LINE = PIN
    // FALLING:    uint8_t [0, 1]  // falling edge trigger
    // RISING:     uint8_t [0, 1]  // rising edge trigger
    start_EXTI(EXTI_LINE);
    stop_EXTI(EXTI_LINE);
    disable_EXTI();
}
```

# TIM
```C
#include "tim.h"
#include "pwm.h"
#include "encoder.h"


void TIMx_IRQHandler(void) {
    TIM->SR &= ~TIM_SR_UIF;  // reset interupt flag
}

int main(void) {
    void config_TIM(TIM, PRESCALER, LIMIT);
    // PRESCALER:  tim_clk = clk / prescaler
    // LIMIT:      max counter value
    disable_TIM(TIM);
    start_TIM(TIM);
    stop_TIM(TIM);
    start_TIM_update_irq(TIM);
    start_TIM_update_irq(TIM);
    
    // PWM
    config_PWM(PWM_PIN, PRESCALER, PERIOD);
    // PWM_PIN:         pre defined timer pins (TIMx_CHx_xx)
    // PERIOD:          uint32_t
    TIM->CCRx = data;  // write PWM data
    // CCRx:            the capture compare register for chanel x
    // data:            uint32_t [0, PERIOD]
    
    // ENCODER
    config_encoder_S0S90(PIN_A, PIN_B);
    // PIN_A, PIN_B:    pre defined timer pins (TIMx_CHx_xx)
    start_encoder_S0S90(TIM);
    stop_encoder_S0S90(TIM);
    data = TIM->cnt;  // read encoder
    
    disable_TIM(TIM);
}
```

# SYS_CLOCK
```C
#include "sys.h"


int main(void) {
    SYS_CLK_Config_t* sys_config = new_SYS_CLK_config();
    set_SYS_PLL_config(sys_config, M, N, P, Q, PLL_SRC);
    // PLL_FREQ = PLL_SRC_FREQ / M * N / P
    // M:   uint8_t [0, 63]
    // N:   uint16_t [0, 511]
    // P:   PLL_P_DIV2, PLL_P_DIV4, PLL_P_DIV6, PLL_P_DIV8
    // Q:   uint8_t [0, 255]
    set_SYS_CLOCK_config(sys_config, SRC, AHB_PRESCALER, APB1_PRESCALER, APB2_PRESCALER, RTC_PRESCALER);
    // SRC:             SYS_CLK_SRC_HSI, SYS_CLK_SRC_HSE, SYS_CLK_SRC_PLL
    // AHB_PRESCALER:   AHB_CLK_NO_DIV, AHB_CLK_DIV2, ..., AHB_CLK_DIV512
    // APB1_PRESCALER:  APBx_CLK_NO_DIV, APBx_CLK_DIV2, ..., APBx_CLK_DIV16
    // APB2_PRESCALER:  APBx_CLK_NO_DIV, APBx_CLK_DIV2, ..., APBx_CLK_DIV16
    // RTC_PRESCALER:   int [0, 255]
    set_SYS_FLASH_config(sys_config, LATENCY, PREFETCH, ICACHE, DCACHE);  // latency is set automatically (if it needs to be higher)
    // LATENCY:         FLASH_LATENCY1, ..., FLASH_LATENCY8
    // PREFETCH:        uint8_t [0, 1]  // enable flash prefetch
    // ICACHE:          uint8_t [0, 1]  // enable instruction cache
    // DCACHE:          uint8_t [0, 1]  // enable data cache
    set_SYS_tick_config(sys_config, ENABLE, ENABLE_IRQ);
    // ENABLE:          uint8_t [0, 1]
    // ENABLE_IRQ:      uint8_t [0, 1]
    sys_clock_init(sys_config);  // configure clocks
    free(sys_config);
    
    // SYS_TICK
    uint64_t now = tick;
    // 'tick' is incremented by systick (with interrupt)
}
```
### Flash Latency Table (in sys.h)
```C
typedef enum {
	//================================================================================================================|
	// flash access latency |                                            H_CLK (MHz)                                  |
	//      (table 6)       |              voltage range      voltage range      voltage range      voltage range     |
	//    in CPU cycles     |__________      2V7 - 3V6          2V4 - 2V7          2V1 - 2V4          1V71 - 2V1      |
	FLASH_LATENCY1 =         0b000,  //    0 < H_CLK ≤ 30     0 < H_CLK ≤ 24     0 < H_CLK ≤ 18     0 < H_CLK ≤ 16    |
	FLASH_LATENCY2 =         0b001,  //    30 < H_CLK ≤ 60    24 < H_CLK ≤ 48    18 < H_CLK ≤ 36    16 < H_CLK ≤ 32   |
	FLASH_LATENCY3 =         0b010,  //    60 < H_CLK ≤ 90    48 < H_CLK ≤ 72    36 < H_CLK ≤ 54    32 < H_CLK ≤ 48   |
	FLASH_LATENCY4 =         0b011,  //    90 < H_CLK ≤ 120   72 < H_CLK ≤ 96    54 < H_CLK ≤ 72    48 < H_CLK ≤ 64   |
	FLASH_LATENCY5 =         0b100,  //    120 < H_CLK ≤ 150  96 < H_CLK ≤ 120   72 < H_CLK ≤ 90    64 < H_CLK ≤ 80   |
	FLASH_LATENCY6 =         0b101,  //    150 < H_CLK ≤ 168  120 < H_CLK ≤ 144  90 < H_CLK ≤ 108   80 < H_CLK ≤ 96   |
	FLASH_LATENCY7 =         0b110,  //           -           144 < H_CLK ≤ 168  108 < H_CLK ≤ 120  96 < H_CLK ≤ 112  |
	FLASH_LATENCY8 =         0b111   //           -                  -           120 < H_CLK ≤ 138  112 < H_CLK ≤ 128 |
	//================================================================================================================|
} FLASH_LATENCY_t;
```

# CRC
```C
#include "crc.h"


int main(void) {
    void enable_CRC(void);
    CRC->DR = data; // loading data into the crc device
    // data: uint32_t
    uint32_t crc = CRC->DR;  // reading the crc result
    void reset_CRC(void);
    void disable_CRC(void);
}
```

# USART
```C
#include "usart.h"


int main(void) {
    fconfig_UART(TX_PIN, RX_PIN, BAUD, OVERSAMPLING);
    // TX_PIN:        pre defined usart pins (USARTx_TX_xx)
    // RX_PIN:        pre defined usart pins (USARTx_RX_xx)
    // BAUD:          uint32_t
    // OVERSAMPLING:  USART_OVERSAMPLING_16 (default), USART_OVERSAMPLING_8
    config_UART(TX_PIN, RX_PIN, BAUD);
    USART_write(USART, BUFFER, SIZE, TIMEOUT);
    // TIMOUT:        uint32_t (ms)
    USART_read(USART, BUFFER, SIZE, TIMEOUT);
    USART_print(USART, STR, TIMEOUT);
    
    // Buffered RX IRQ
    io_buffer_t* uart_buf = new_buffer(SIZE);
    start_USART_read_irq(USART, uart_buf, FIFO);
    // FIFO:          uint8_t [0, 1]  // enable First In First Out (cyclic buffer)
    uint8_t data = ((uint8_t*)io_buffer->ptr)[x]  // read data
    
    disable_USART(USART);
}
```

# I2C
```C
#include "i2c.h"


int main(void) {
	fconfig_I2C(SCL_PIN, SDA_PIN, OWN_ADDRESS, ADDRESS_TYPE, DUAL_ADDRESS);
	// SCL_PIN:         pre defined usart pins (I2Cx_SCL_xx)
	// SDA_PIN:         pre defined usart pins (I2Cx_SDA_xx)
	// OWN_ADDRESS:     uint8_t [0, 127] (default type), uint16_t [0, 1023]
	// ADDRESS_TYPE:    I2C_ADDR_7BIT (default), I2C_ADDR_10BIT
	// DUAL_ADDRESS:    uint8_t [0, 127]  // second own address
	config_I2C(SCL_PIN, SDA_PIN, OWN_ADDRESS);
	I2C_master_address(I2C, ADDD, TIMEOUT);
	I2C_master_write(I2C, ADDR, BUFFER, SIZE, TIMEOUT);
	I2C_master_read(I2C, ADDR, BUFFER, SIZE, TIMEOUT);
	I2C_master_write_reg(I2C, ADDR, REG, BUFFER, SIZE, TIMEOUT);
	I2C_master_read_reg(I2C, ADDR, REG, BUFFER, SIZE, TIMEOUT);
	disable_I2C(I2C);
}
```

# UUID
```C
#include "sys.h"
#include "rng.h"


int main(void) {
    UID->ID0;   // low 32 bits of uid
    UID->ID1;
    UID->ID2;   // high 32 bits of uid
    
    // RNG
    start_RNG();
    // set_RNG(SEED);  // already initialized with UID
    data = RNG_generate(MAX);
    // data:    uint32_t [0, MAX]
}
```

# WATCHDOG
```C
#include "watchdog.h"


int main(void) {
    config_watchdog(PRESCALER, RELOAD);
    // PRESCALER:       IWDG_FREQ = 32 kHz / (4 << PRESCALER)
    // RELOAD:          uint16_t [0, 4095]
    start_watchdog();
    reset_watchdog();
}
```