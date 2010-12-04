# 1 "FabOS.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "FabOS.c"
# 16 "FabOS.c"
# 1 "FabOS.h" 1
# 20 "FabOS.h"
# 1 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/io.h" 1 3
# 99 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/io.h" 3
# 1 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/sfr_defs.h" 1 3
# 126 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/sfr_defs.h" 3
# 1 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/inttypes.h" 1 3
# 37 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/inttypes.h" 3
# 1 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/stdint.h" 1 3
# 121 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/stdint.h" 3
typedef int int8_t __attribute__((__mode__(__QI__)));
typedef unsigned int uint8_t __attribute__((__mode__(__QI__)));
typedef int int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int uint16_t __attribute__ ((__mode__ (__HI__)));
typedef int int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int uint32_t __attribute__ ((__mode__ (__SI__)));

typedef int int64_t __attribute__((__mode__(__DI__)));
typedef unsigned int uint64_t __attribute__((__mode__(__DI__)));
# 142 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/stdint.h" 3
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;
# 159 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/stdint.h" 3
typedef int8_t int_least8_t;




typedef uint8_t uint_least8_t;




typedef int16_t int_least16_t;




typedef uint16_t uint_least16_t;




typedef int32_t int_least32_t;




typedef uint32_t uint_least32_t;







typedef int64_t int_least64_t;






typedef uint64_t uint_least64_t;
# 213 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/stdint.h" 3
typedef int8_t int_fast8_t;




typedef uint8_t uint_fast8_t;




typedef int16_t int_fast16_t;




typedef uint16_t uint_fast16_t;




typedef int32_t int_fast32_t;




typedef uint32_t uint_fast32_t;







typedef int64_t int_fast64_t;






typedef uint64_t uint_fast64_t;
# 273 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/stdint.h" 3
typedef int64_t intmax_t;




typedef uint64_t uintmax_t;
# 38 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/inttypes.h" 2 3
# 77 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/inttypes.h" 3
typedef int32_t int_farptr_t;



typedef uint32_t uint_farptr_t;
# 127 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/sfr_defs.h" 2 3
# 100 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/io.h" 2 3
# 376 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/io.h" 3
# 1 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 1 3
# 104 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef volatile uint8_t register8_t;
typedef volatile uint16_t register16_t;
typedef volatile uint32_t register32_t;
# 154 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct OCD_struct
{
    register8_t OCDR0;
    register8_t OCDR1;
} OCD_t;



typedef enum CCP_enum
{
    CCP_SPM_gc = (0x9D<<0),
    CCP_IOREG_gc = (0xD8<<0),
} CCP_t;
# 176 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct CLK_struct
{
    register8_t CTRL;
    register8_t PSCTRL;
    register8_t LOCK;
    register8_t RTCCTRL;
} CLK_t;
# 191 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct PR_struct
{
    register8_t PRGEN;
    register8_t PRPA;
    register8_t PRPB;
    register8_t PRPC;
    register8_t PRPD;
    register8_t PRPE;
    register8_t PRPF;
} PR_t;


typedef enum CLK_SCLKSEL_enum
{
    CLK_SCLKSEL_RC2M_gc = (0x00<<0),
    CLK_SCLKSEL_RC32M_gc = (0x01<<0),
    CLK_SCLKSEL_RC32K_gc = (0x02<<0),
    CLK_SCLKSEL_XOSC_gc = (0x03<<0),
    CLK_SCLKSEL_PLL_gc = (0x04<<0),
} CLK_SCLKSEL_t;


typedef enum CLK_PSADIV_enum
{
    CLK_PSADIV_1_gc = (0x00<<2),
    CLK_PSADIV_2_gc = (0x01<<2),
    CLK_PSADIV_4_gc = (0x03<<2),
    CLK_PSADIV_8_gc = (0x05<<2),
    CLK_PSADIV_16_gc = (0x07<<2),
    CLK_PSADIV_32_gc = (0x09<<2),
    CLK_PSADIV_64_gc = (0x0B<<2),
    CLK_PSADIV_128_gc = (0x0D<<2),
    CLK_PSADIV_256_gc = (0x0F<<2),
    CLK_PSADIV_512_gc = (0x11<<2),
} CLK_PSADIV_t;


typedef enum CLK_PSBCDIV_enum
{
    CLK_PSBCDIV_1_1_gc = (0x00<<0),
    CLK_PSBCDIV_1_2_gc = (0x01<<0),
    CLK_PSBCDIV_4_1_gc = (0x02<<0),
    CLK_PSBCDIV_2_2_gc = (0x03<<0),
} CLK_PSBCDIV_t;


typedef enum CLK_RTCSRC_enum
{
    CLK_RTCSRC_ULP_gc = (0x00<<1),
    CLK_RTCSRC_TOSC_gc = (0x01<<1),
    CLK_RTCSRC_RCOSC_gc = (0x02<<1),
    CLK_RTCSRC_TOSC32_gc = (0x05<<1),
} CLK_RTCSRC_t;
# 253 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct SLEEP_struct
{
    register8_t CTRL;
} SLEEP_t;


typedef enum SLEEP_SMODE_enum
{
    SLEEP_SMODE_IDLE_gc = (0x00<<1),
    SLEEP_SMODE_PDOWN_gc = (0x02<<1),
    SLEEP_SMODE_PSAVE_gc = (0x03<<1),
    SLEEP_SMODE_STDBY_gc = (0x06<<1),
    SLEEP_SMODE_ESTDBY_gc = (0x07<<1),
} SLEEP_SMODE_t;
# 276 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct OSC_struct
{
    register8_t CTRL;
    register8_t STATUS;
    register8_t XOSCCTRL;
    register8_t XOSCFAIL;
    register8_t RC32KCAL;
    register8_t PLLCTRL;
    register8_t DFLLCTRL;
} OSC_t;


typedef enum OSC_FRQRANGE_enum
{
    OSC_FRQRANGE_04TO2_gc = (0x00<<6),
    OSC_FRQRANGE_2TO9_gc = (0x01<<6),
    OSC_FRQRANGE_9TO12_gc = (0x02<<6),
    OSC_FRQRANGE_12TO16_gc = (0x03<<6),
} OSC_FRQRANGE_t;


typedef enum OSC_XOSCSEL_enum
{
    OSC_XOSCSEL_EXTCLK_gc = (0x00<<0),
    OSC_XOSCSEL_32KHz_gc = (0x02<<0),
    OSC_XOSCSEL_XTAL_256CLK_gc = (0x03<<0),
    OSC_XOSCSEL_XTAL_1KCLK_gc = (0x07<<0),
    OSC_XOSCSEL_XTAL_16KCLK_gc = (0x0B<<0),
} OSC_XOSCSEL_t;


typedef enum OSC_PLLSRC_enum
{
    OSC_PLLSRC_RC2M_gc = (0x00<<6),
    OSC_PLLSRC_RC32M_gc = (0x02<<6),
    OSC_PLLSRC_XOSC_gc = (0x03<<6),
} OSC_PLLSRC_t;
# 322 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct DFLL_struct
{
    register8_t CTRL;
    register8_t reserved_0x01;
    register8_t CALA;
    register8_t CALB;
    register8_t COMP0;
    register8_t COMP1;
    register8_t COMP2;
    register8_t reserved_0x07;
} DFLL_t;
# 342 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct RST_struct
{
    register8_t STATUS;
    register8_t CTRL;
} RST_t;
# 356 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct WDT_struct
{
    register8_t CTRL;
    register8_t WINCTRL;
    register8_t STATUS;
} WDT_t;


typedef enum WDT_PER_enum
{
    WDT_PER_8CLK_gc = (0x00<<2),
    WDT_PER_16CLK_gc = (0x01<<2),
    WDT_PER_32CLK_gc = (0x02<<2),
    WDT_PER_64CLK_gc = (0x03<<2),
    WDT_PER_128CLK_gc = (0x04<<2),
    WDT_PER_256CLK_gc = (0x05<<2),
    WDT_PER_512CLK_gc = (0x06<<2),
    WDT_PER_1KCLK_gc = (0x07<<2),
    WDT_PER_2KCLK_gc = (0x08<<2),
    WDT_PER_4KCLK_gc = (0x09<<2),
    WDT_PER_8KCLK_gc = (0x0A<<2),
} WDT_PER_t;


typedef enum WDT_WPER_enum
{
    WDT_WPER_8CLK_gc = (0x00<<2),
    WDT_WPER_16CLK_gc = (0x01<<2),
    WDT_WPER_32CLK_gc = (0x02<<2),
    WDT_WPER_64CLK_gc = (0x03<<2),
    WDT_WPER_128CLK_gc = (0x04<<2),
    WDT_WPER_256CLK_gc = (0x05<<2),
    WDT_WPER_512CLK_gc = (0x06<<2),
    WDT_WPER_1KCLK_gc = (0x07<<2),
    WDT_WPER_2KCLK_gc = (0x08<<2),
    WDT_WPER_4KCLK_gc = (0x09<<2),
    WDT_WPER_8KCLK_gc = (0x0A<<2),
} WDT_WPER_t;
# 403 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct MCU_struct
{
    register8_t DEVID0;
    register8_t DEVID1;
    register8_t DEVID2;
    register8_t REVID;
    register8_t JTAGUID;
    register8_t reserved_0x05;
    register8_t MCUCR;
    register8_t reserved_0x07;
    register8_t EVSYSLOCK;
    register8_t AWEXLOCK;
    register8_t reserved_0x0A;
    register8_t reserved_0x0B;
} MCU_t;
# 427 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct PMIC_struct
{
    register8_t STATUS;
    register8_t INTPRI;
    register8_t CTRL;
} PMIC_t;
# 442 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct DMA_CH_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t ADDRCTRL;
    register8_t TRIGSRC;
    __extension__ union { register16_t TRFCNT; struct { register8_t TRFCNTL; register8_t TRFCNTH; }; };
    register8_t REPCNT;
    register8_t reserved_0x07;
    register8_t SRCADDR0;
    register8_t SRCADDR1;
    register8_t SRCADDR2;
    register8_t reserved_0x0B;
    register8_t DESTADDR0;
    register8_t DESTADDR1;
    register8_t DESTADDR2;
    register8_t reserved_0x0F;
} DMA_CH_t;
# 468 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct DMA_struct
{
    register8_t CTRL;
    register8_t reserved_0x01;
    register8_t reserved_0x02;
    register8_t INTFLAGS;
    register8_t STATUS;
    register8_t reserved_0x05;
    __extension__ union { register16_t TEMP; struct { register8_t TEMPL; register8_t TEMPH; }; };
    register8_t reserved_0x08;
    register8_t reserved_0x09;
    register8_t reserved_0x0A;
    register8_t reserved_0x0B;
    register8_t reserved_0x0C;
    register8_t reserved_0x0D;
    register8_t reserved_0x0E;
    register8_t reserved_0x0F;
    DMA_CH_t CH0;
    DMA_CH_t CH1;
    DMA_CH_t CH2;
    DMA_CH_t CH3;
} DMA_t;


typedef enum DMA_CH_BURSTLEN_enum
{
    DMA_CH_BURSTLEN_1BYTE_gc = (0x00<<0),
    DMA_CH_BURSTLEN_2BYTE_gc = (0x01<<0),
    DMA_CH_BURSTLEN_4BYTE_gc = (0x02<<0),
    DMA_CH_BURSTLEN_8BYTE_gc = (0x03<<0),
} DMA_CH_BURSTLEN_t;


typedef enum DMA_CH_SRCRELOAD_enum
{
    DMA_CH_SRCRELOAD_NONE_gc = (0x00<<6),
    DMA_CH_SRCRELOAD_BLOCK_gc = (0x01<<6),
    DMA_CH_SRCRELOAD_BURST_gc = (0x02<<6),
    DMA_CH_SRCRELOAD_TRANSACTION_gc = (0x03<<6),
} DMA_CH_SRCRELOAD_t;


typedef enum DMA_CH_SRCDIR_enum
{
    DMA_CH_SRCDIR_FIXED_gc = (0x00<<4),
    DMA_CH_SRCDIR_INC_gc = (0x01<<4),
    DMA_CH_SRCDIR_DEC_gc = (0x02<<4),
} DMA_CH_SRCDIR_t;


typedef enum DMA_CH_DESTRELOAD_enum
{
    DMA_CH_DESTRELOAD_NONE_gc = (0x00<<2),
    DMA_CH_DESTRELOAD_BLOCK_gc = (0x01<<2),
    DMA_CH_DESTRELOAD_BURST_gc = (0x02<<2),
    DMA_CH_DESTRELOAD_TRANSACTION_gc = (0x03<<2),
} DMA_CH_DESTRELOAD_t;


typedef enum DMA_CH_DESTDIR_enum
{
    DMA_CH_DESTDIR_FIXED_gc = (0x00<<0),
    DMA_CH_DESTDIR_INC_gc = (0x01<<0),
    DMA_CH_DESTDIR_DEC_gc = (0x02<<0),
} DMA_CH_DESTDIR_t;


typedef enum DMA_CH_TRIGSRC_enum
{
    DMA_CH_TRIGSRC_OFF_gc = (0x00<<0),
    DMA_CH_TRIGSRC_EVSYS_CH0_gc = (0x01<<0),
    DMA_CH_TRIGSRC_EVSYS_CH1_gc = (0x02<<0),
    DMA_CH_TRIGSRC_EVSYS_CH2_gc = (0x03<<0),
    DMA_CH_TRIGSRC_ADCA_CH0_gc = (0x10<<0),
    DMA_CH_TRIGSRC_ADCA_CH1_gc = (0x11<<0),
    DMA_CH_TRIGSRC_ADCA_CH2_gc = (0x12<<0),
    DMA_CH_TRIGSRC_ADCA_CH3_gc = (0x13<<0),
    DMA_CH_TRIGSRC_ADCA_CH4_gc = (0x14<<0),
    DMA_CH_TRIGSRC_DACA_CH0_gc = (0x15<<0),
    DMA_CH_TRIGSRC_DACA_CH1_gc = (0x16<<0),
    DMA_CH_TRIGSRC_ADCB_CH0_gc = (0x20<<0),
    DMA_CH_TRIGSRC_ADCB_CH1_gc = (0x21<<0),
    DMA_CH_TRIGSRC_ADCB_CH2_gc = (0x22<<0),
    DMA_CH_TRIGSRC_ADCB_CH3_gc = (0x23<<0),
    DMA_CH_TRIGSRC_ADCB_CH4_gc = (0x24<<0),
    DMA_CH_TRIGSRC_DACB_CH0_gc = (0x25<<0),
    DMA_CH_TRIGSRC_DACB_CH1_gc = (0x26<<0),
    DMA_CH_TRIGSRC_TCC0_OVF_gc = (0x40<<0),
    DMA_CH_TRIGSRC_TCC0_ERR_gc = (0x41<<0),
    DMA_CH_TRIGSRC_TCC0_CCA_gc = (0x42<<0),
    DMA_CH_TRIGSRC_TCC0_CCB_gc = (0x43<<0),
    DMA_CH_TRIGSRC_TCC0_CCC_gc = (0x44<<0),
    DMA_CH_TRIGSRC_TCC0_CCD_gc = (0x45<<0),
    DMA_CH_TRIGSRC_TCC1_OVF_gc = (0x46<<0),
    DMA_CH_TRIGSRC_TCC1_ERR_gc = (0x47<<0),
    DMA_CH_TRIGSRC_TCC1_CCA_gc = (0x48<<0),
    DMA_CH_TRIGSRC_TCC1_CCB_gc = (0x49<<0),
    DMA_CH_TRIGSRC_SPIC_gc = (0x4A<<0),
    DMA_CH_TRIGSRC_USARTC0_RXC_gc = (0x4B<<0),
    DMA_CH_TRIGSRC_USARTC0_DRE_gc = (0x4C<<0),
    DMA_CH_TRIGSRC_USARTC1_RXC_gc = (0x4E<<0),
    DMA_CH_TRIGSRC_USARTC1_DRE_gc = (0x4F<<0),
    DMA_CH_TRIGSRC_TCD0_OVF_gc = (0x60<<0),
    DMA_CH_TRIGSRC_TCD0_ERR_gc = (0x61<<0),
    DMA_CH_TRIGSRC_TCD0_CCA_gc = (0x62<<0),
    DMA_CH_TRIGSRC_TCD0_CCB_gc = (0x63<<0),
    DMA_CH_TRIGSRC_TCD0_CCC_gc = (0x64<<0),
    DMA_CH_TRIGSRC_TCD0_CCD_gc = (0x65<<0),
    DMA_CH_TRIGSRC_TCD1_OVF_gc = (0x66<<0),
    DMA_CH_TRIGSRC_TCD1_ERR_gc = (0x67<<0),
    DMA_CH_TRIGSRC_TCD1_CCA_gc = (0x68<<0),
    DMA_CH_TRIGSRC_TCD1_CCB_gc = (0x69<<0),
    DMA_CH_TRIGSRC_SPID_gc = (0x6A<<0),
    DMA_CH_TRIGSRC_USARTD0_RXC_gc = (0x6B<<0),
    DMA_CH_TRIGSRC_USARTD0_DRE_gc = (0x6C<<0),
    DMA_CH_TRIGSRC_USARTD1_RXC_gc = (0x6E<<0),
    DMA_CH_TRIGSRC_USARTD1_DRE_gc = (0x6F<<0),
    DMA_CH_TRIGSRC_TCE0_OVF_gc = (0x80<<0),
    DMA_CH_TRIGSRC_TCE0_ERR_gc = (0x81<<0),
    DMA_CH_TRIGSRC_TCE0_CCA_gc = (0x82<<0),
    DMA_CH_TRIGSRC_TCE0_CCB_gc = (0x83<<0),
    DMA_CH_TRIGSRC_TCE0_CCC_gc = (0x84<<0),
    DMA_CH_TRIGSRC_TCE0_CCD_gc = (0x85<<0),
    DMA_CH_TRIGSRC_TCE1_OVF_gc = (0x86<<0),
    DMA_CH_TRIGSRC_TCE1_ERR_gc = (0x87<<0),
    DMA_CH_TRIGSRC_TCE1_CCA_gc = (0x88<<0),
    DMA_CH_TRIGSRC_TCE1_CCB_gc = (0x89<<0),
    DMA_CH_TRIGSRC_SPIE_gc = (0x8A<<0),
    DMA_CH_TRIGSRC_USARTE0_RXC_gc = (0x8B<<0),
    DMA_CH_TRIGSRC_USARTE0_DRE_gc = (0x8C<<0),
    DMA_CH_TRIGSRC_USARTE1_RXC_gc = (0x8E<<0),
    DMA_CH_TRIGSRC_USARTE1_DRE_gc = (0x8F<<0),
    DMA_CH_TRIGSRC_TCF0_OVF_gc = (0xA0<<0),
    DMA_CH_TRIGSRC_TCF0_ERR_gc = (0xA1<<0),
    DMA_CH_TRIGSRC_TCF0_CCA_gc = (0xA2<<0),
    DMA_CH_TRIGSRC_TCF0_CCB_gc = (0xA3<<0),
    DMA_CH_TRIGSRC_TCF0_CCC_gc = (0xA4<<0),
    DMA_CH_TRIGSRC_TCF0_CCD_gc = (0xA5<<0),
    DMA_CH_TRIGSRC_TCF1_OVF_gc = (0xA6<<0),
    DMA_CH_TRIGSRC_TCF1_ERR_gc = (0xA7<<0),
    DMA_CH_TRIGSRC_TCF1_CCA_gc = (0xA8<<0),
    DMA_CH_TRIGSRC_TCF1_CCB_gc = (0xA9<<0),
    DMA_CH_TRIGSRC_SPIF_gc = (0xAA<<0),
    DMA_CH_TRIGSRC_USARTF0_RXC_gc = (0xAB<<0),
    DMA_CH_TRIGSRC_USARTF0_DRE_gc = (0xAC<<0),
    DMA_CH_TRIGSRC_USARTF1_RXC_gc = (0xAE<<0),
    DMA_CH_TRIGSRC_USARTF1_DRE_gc = (0xAF<<0),
} DMA_CH_TRIGSRC_t;


typedef enum DMA_DBUFMODE_enum
{
    DMA_DBUFMODE_DISABLED_gc = (0x00<<2),
    DMA_DBUFMODE_CH01_gc = (0x01<<2),
    DMA_DBUFMODE_CH23_gc = (0x02<<2),
    DMA_DBUFMODE_CH01CH23_gc = (0x03<<2),
} DMA_DBUFMODE_t;


typedef enum DMA_PRIMODE_enum
{
    DMA_PRIMODE_RR0123_gc = (0x00<<0),
    DMA_PRIMODE_CH0RR123_gc = (0x01<<0),
    DMA_PRIMODE_CH01RR23_gc = (0x02<<0),
    DMA_PRIMODE_CH0123_gc = (0x03<<0),
} DMA_PRIMODE_t;


typedef enum DMA_CH_ERRINTLVL_enum
{
    DMA_CH_ERRINTLVL_OFF_gc = (0x00<<2),
    DMA_CH_ERRINTLVL_LO_gc = (0x01<<2),
    DMA_CH_ERRINTLVL_MED_gc = (0x02<<2),
    DMA_CH_ERRINTLVL_HI_gc = (0x03<<2),
} DMA_CH_ERRINTLVL_t;


typedef enum DMA_CH_TRNINTLVL_enum
{
    DMA_CH_TRNINTLVL_OFF_gc = (0x00<<0),
    DMA_CH_TRNINTLVL_LO_gc = (0x01<<0),
    DMA_CH_TRNINTLVL_MED_gc = (0x02<<0),
    DMA_CH_TRNINTLVL_HI_gc = (0x03<<0),
} DMA_CH_TRNINTLVL_t;
# 661 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct EVSYS_struct
{
    register8_t CH0MUX;
    register8_t CH1MUX;
    register8_t CH2MUX;
    register8_t CH3MUX;
    register8_t CH4MUX;
    register8_t CH5MUX;
    register8_t CH6MUX;
    register8_t CH7MUX;
    register8_t CH0CTRL;
    register8_t CH1CTRL;
    register8_t CH2CTRL;
    register8_t CH3CTRL;
    register8_t CH4CTRL;
    register8_t CH5CTRL;
    register8_t CH6CTRL;
    register8_t CH7CTRL;
    register8_t STROBE;
    register8_t DATA;
} EVSYS_t;


typedef enum EVSYS_QDIRM_enum
{
    EVSYS_QDIRM_00_gc = (0x00<<5),
    EVSYS_QDIRM_01_gc = (0x01<<5),
    EVSYS_QDIRM_10_gc = (0x02<<5),
    EVSYS_QDIRM_11_gc = (0x03<<5),
} EVSYS_QDIRM_t;


typedef enum EVSYS_DIGFILT_enum
{
    EVSYS_DIGFILT_1SAMPLE_gc = (0x00<<0),
    EVSYS_DIGFILT_2SAMPLES_gc = (0x01<<0),
    EVSYS_DIGFILT_3SAMPLES_gc = (0x02<<0),
    EVSYS_DIGFILT_4SAMPLES_gc = (0x03<<0),
    EVSYS_DIGFILT_5SAMPLES_gc = (0x04<<0),
    EVSYS_DIGFILT_6SAMPLES_gc = (0x05<<0),
    EVSYS_DIGFILT_7SAMPLES_gc = (0x06<<0),
    EVSYS_DIGFILT_8SAMPLES_gc = (0x07<<0),
} EVSYS_DIGFILT_t;


typedef enum EVSYS_CHMUX_enum
{
    EVSYS_CHMUX_OFF_gc = (0x00<<0),
    EVSYS_CHMUX_RTC_OVF_gc = (0x08<<0),
    EVSYS_CHMUX_RTC_CMP_gc = (0x09<<0),
    EVSYS_CHMUX_ACA_CH0_gc = (0x10<<0),
    EVSYS_CHMUX_ACA_CH1_gc = (0x11<<0),
    EVSYS_CHMUX_ACA_WIN_gc = (0x12<<0),
    EVSYS_CHMUX_ACB_CH0_gc = (0x13<<0),
    EVSYS_CHMUX_ACB_CH1_gc = (0x14<<0),
    EVSYS_CHMUX_ACB_WIN_gc = (0x15<<0),
    EVSYS_CHMUX_ADCA_CH0_gc = (0x20<<0),
    EVSYS_CHMUX_ADCA_CH1_gc = (0x21<<0),
    EVSYS_CHMUX_ADCA_CH2_gc = (0x22<<0),
    EVSYS_CHMUX_ADCA_CH3_gc = (0x23<<0),
    EVSYS_CHMUX_ADCB_CH0_gc = (0x24<<0),
    EVSYS_CHMUX_ADCB_CH1_gc = (0x25<<0),
    EVSYS_CHMUX_ADCB_CH2_gc = (0x26<<0),
    EVSYS_CHMUX_ADCB_CH3_gc = (0x27<<0),
    EVSYS_CHMUX_PORTA_PIN0_gc = (0x50<<0),
    EVSYS_CHMUX_PORTA_PIN1_gc = (0x51<<0),
    EVSYS_CHMUX_PORTA_PIN2_gc = (0x52<<0),
    EVSYS_CHMUX_PORTA_PIN3_gc = (0x53<<0),
    EVSYS_CHMUX_PORTA_PIN4_gc = (0x54<<0),
    EVSYS_CHMUX_PORTA_PIN5_gc = (0x55<<0),
    EVSYS_CHMUX_PORTA_PIN6_gc = (0x56<<0),
    EVSYS_CHMUX_PORTA_PIN7_gc = (0x57<<0),
    EVSYS_CHMUX_PORTB_PIN0_gc = (0x58<<0),
    EVSYS_CHMUX_PORTB_PIN1_gc = (0x59<<0),
    EVSYS_CHMUX_PORTB_PIN2_gc = (0x5A<<0),
    EVSYS_CHMUX_PORTB_PIN3_gc = (0x5B<<0),
    EVSYS_CHMUX_PORTB_PIN4_gc = (0x5C<<0),
    EVSYS_CHMUX_PORTB_PIN5_gc = (0x5D<<0),
    EVSYS_CHMUX_PORTB_PIN6_gc = (0x5E<<0),
    EVSYS_CHMUX_PORTB_PIN7_gc = (0x5F<<0),
    EVSYS_CHMUX_PORTC_PIN0_gc = (0x60<<0),
    EVSYS_CHMUX_PORTC_PIN1_gc = (0x61<<0),
    EVSYS_CHMUX_PORTC_PIN2_gc = (0x62<<0),
    EVSYS_CHMUX_PORTC_PIN3_gc = (0x63<<0),
    EVSYS_CHMUX_PORTC_PIN4_gc = (0x64<<0),
    EVSYS_CHMUX_PORTC_PIN5_gc = (0x65<<0),
    EVSYS_CHMUX_PORTC_PIN6_gc = (0x66<<0),
    EVSYS_CHMUX_PORTC_PIN7_gc = (0x67<<0),
    EVSYS_CHMUX_PORTD_PIN0_gc = (0x68<<0),
    EVSYS_CHMUX_PORTD_PIN1_gc = (0x69<<0),
    EVSYS_CHMUX_PORTD_PIN2_gc = (0x6A<<0),
    EVSYS_CHMUX_PORTD_PIN3_gc = (0x6B<<0),
    EVSYS_CHMUX_PORTD_PIN4_gc = (0x6C<<0),
    EVSYS_CHMUX_PORTD_PIN5_gc = (0x6D<<0),
    EVSYS_CHMUX_PORTD_PIN6_gc = (0x6E<<0),
    EVSYS_CHMUX_PORTD_PIN7_gc = (0x6F<<0),
    EVSYS_CHMUX_PORTE_PIN0_gc = (0x70<<0),
    EVSYS_CHMUX_PORTE_PIN1_gc = (0x71<<0),
    EVSYS_CHMUX_PORTE_PIN2_gc = (0x72<<0),
    EVSYS_CHMUX_PORTE_PIN3_gc = (0x73<<0),
    EVSYS_CHMUX_PORTE_PIN4_gc = (0x74<<0),
    EVSYS_CHMUX_PORTE_PIN5_gc = (0x75<<0),
    EVSYS_CHMUX_PORTE_PIN6_gc = (0x76<<0),
    EVSYS_CHMUX_PORTE_PIN7_gc = (0x77<<0),
    EVSYS_CHMUX_PORTF_PIN0_gc = (0x78<<0),
    EVSYS_CHMUX_PORTF_PIN1_gc = (0x79<<0),
    EVSYS_CHMUX_PORTF_PIN2_gc = (0x7A<<0),
    EVSYS_CHMUX_PORTF_PIN3_gc = (0x7B<<0),
    EVSYS_CHMUX_PORTF_PIN4_gc = (0x7C<<0),
    EVSYS_CHMUX_PORTF_PIN5_gc = (0x7D<<0),
    EVSYS_CHMUX_PORTF_PIN6_gc = (0x7E<<0),
    EVSYS_CHMUX_PORTF_PIN7_gc = (0x7F<<0),
    EVSYS_CHMUX_PRESCALER_1_gc = (0x80<<0),
    EVSYS_CHMUX_PRESCALER_2_gc = (0x81<<0),
    EVSYS_CHMUX_PRESCALER_4_gc = (0x82<<0),
    EVSYS_CHMUX_PRESCALER_8_gc = (0x83<<0),
    EVSYS_CHMUX_PRESCALER_16_gc = (0x84<<0),
    EVSYS_CHMUX_PRESCALER_32_gc = (0x85<<0),
    EVSYS_CHMUX_PRESCALER_64_gc = (0x86<<0),
    EVSYS_CHMUX_PRESCALER_128_gc = (0x87<<0),
    EVSYS_CHMUX_PRESCALER_256_gc = (0x88<<0),
    EVSYS_CHMUX_PRESCALER_512_gc = (0x89<<0),
    EVSYS_CHMUX_PRESCALER_1024_gc = (0x8A<<0),
    EVSYS_CHMUX_PRESCALER_2048_gc = (0x8B<<0),
    EVSYS_CHMUX_PRESCALER_4096_gc = (0x8C<<0),
    EVSYS_CHMUX_PRESCALER_8192_gc = (0x8D<<0),
    EVSYS_CHMUX_PRESCALER_16384_gc = (0x8E<<0),
    EVSYS_CHMUX_PRESCALER_32768_gc = (0x8F<<0),
    EVSYS_CHMUX_TCC0_OVF_gc = (0xC0<<0),
    EVSYS_CHMUX_TCC0_ERR_gc = (0xC1<<0),
    EVSYS_CHMUX_TCC0_CCA_gc = (0xC4<<0),
    EVSYS_CHMUX_TCC0_CCB_gc = (0xC5<<0),
    EVSYS_CHMUX_TCC0_CCC_gc = (0xC6<<0),
    EVSYS_CHMUX_TCC0_CCD_gc = (0xC7<<0),
    EVSYS_CHMUX_TCC1_OVF_gc = (0xC8<<0),
    EVSYS_CHMUX_TCC1_ERR_gc = (0xC9<<0),
    EVSYS_CHMUX_TCC1_CCA_gc = (0xCC<<0),
    EVSYS_CHMUX_TCC1_CCB_gc = (0xCD<<0),
    EVSYS_CHMUX_TCD0_OVF_gc = (0xD0<<0),
    EVSYS_CHMUX_TCD0_ERR_gc = (0xD1<<0),
    EVSYS_CHMUX_TCD0_CCA_gc = (0xD4<<0),
    EVSYS_CHMUX_TCD0_CCB_gc = (0xD5<<0),
    EVSYS_CHMUX_TCD0_CCC_gc = (0xD6<<0),
    EVSYS_CHMUX_TCD0_CCD_gc = (0xD7<<0),
    EVSYS_CHMUX_TCD1_OVF_gc = (0xD8<<0),
    EVSYS_CHMUX_TCD1_ERR_gc = (0xD9<<0),
    EVSYS_CHMUX_TCD1_CCA_gc = (0xDC<<0),
    EVSYS_CHMUX_TCD1_CCB_gc = (0xDD<<0),
    EVSYS_CHMUX_TCE0_OVF_gc = (0xE0<<0),
    EVSYS_CHMUX_TCE0_ERR_gc = (0xE1<<0),
    EVSYS_CHMUX_TCE0_CCA_gc = (0xE4<<0),
    EVSYS_CHMUX_TCE0_CCB_gc = (0xE5<<0),
    EVSYS_CHMUX_TCE0_CCC_gc = (0xE6<<0),
    EVSYS_CHMUX_TCE0_CCD_gc = (0xE7<<0),
    EVSYS_CHMUX_TCE1_OVF_gc = (0xE8<<0),
    EVSYS_CHMUX_TCE1_ERR_gc = (0xE9<<0),
    EVSYS_CHMUX_TCE1_CCA_gc = (0xEC<<0),
    EVSYS_CHMUX_TCE1_CCB_gc = (0xED<<0),
    EVSYS_CHMUX_TCF0_OVF_gc = (0xF0<<0),
    EVSYS_CHMUX_TCF0_ERR_gc = (0xF1<<0),
    EVSYS_CHMUX_TCF0_CCA_gc = (0xF4<<0),
    EVSYS_CHMUX_TCF0_CCB_gc = (0xF5<<0),
    EVSYS_CHMUX_TCF0_CCC_gc = (0xF6<<0),
    EVSYS_CHMUX_TCF0_CCD_gc = (0xF7<<0),
    EVSYS_CHMUX_TCF1_OVF_gc = (0xF8<<0),
    EVSYS_CHMUX_TCF1_ERR_gc = (0xF9<<0),
    EVSYS_CHMUX_TCF1_CCA_gc = (0xFC<<0),
    EVSYS_CHMUX_TCF1_CCB_gc = (0xFD<<0),
} EVSYS_CHMUX_t;
# 839 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct NVM_struct
{
    register8_t ADDR0;
    register8_t ADDR1;
    register8_t ADDR2;
    register8_t reserved_0x03;
    register8_t DATA0;
    register8_t DATA1;
    register8_t DATA2;
    register8_t reserved_0x07;
    register8_t reserved_0x08;
    register8_t reserved_0x09;
    register8_t CMD;
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t INTCTRL;
    register8_t reserved_0x0E;
    register8_t STATUS;
    register8_t LOCKBITS;
} NVM_t;
# 867 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct NVM_LOCKBITS_struct
{
    register8_t LOCKBITS;
} NVM_LOCKBITS_t;
# 879 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct NVM_FUSES_struct
{
    register8_t FUSEBYTE0;
    register8_t FUSEBYTE1;
    register8_t FUSEBYTE2;
    register8_t reserved_0x03;
    register8_t FUSEBYTE4;
    register8_t FUSEBYTE5;
} NVM_FUSES_t;
# 896 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct NVM_PROD_SIGNATURES_struct
{
    register8_t RCOSC2M;
    register8_t reserved_0x01;
    register8_t RCOSC32K;
    register8_t RCOSC32M;
    register8_t reserved_0x04;
    register8_t reserved_0x05;
    register8_t reserved_0x06;
    register8_t reserved_0x07;
    register8_t LOTNUM0;
    register8_t LOTNUM1;
    register8_t LOTNUM2;
    register8_t LOTNUM3;
    register8_t LOTNUM4;
    register8_t LOTNUM5;
    register8_t reserved_0x0E;
    register8_t reserved_0x0F;
    register8_t WAFNUM;
    register8_t reserved_0x11;
    register8_t COORDX0;
    register8_t COORDX1;
    register8_t COORDY0;
    register8_t COORDY1;
    register8_t reserved_0x16;
    register8_t reserved_0x17;
    register8_t reserved_0x18;
    register8_t reserved_0x19;
    register8_t reserved_0x1A;
    register8_t reserved_0x1B;
    register8_t reserved_0x1C;
    register8_t reserved_0x1D;
    register8_t reserved_0x1E;
    register8_t reserved_0x1F;
    register8_t ADCACAL0;
    register8_t ADCACAL1;
    register8_t reserved_0x22;
    register8_t reserved_0x23;
    register8_t ADCBCAL0;
    register8_t ADCBCAL1;
    register8_t reserved_0x26;
    register8_t reserved_0x27;
    register8_t reserved_0x28;
    register8_t reserved_0x29;
    register8_t reserved_0x2A;
    register8_t reserved_0x2B;
    register8_t reserved_0x2C;
    register8_t reserved_0x2D;
    register8_t TEMPSENSE0;
    register8_t TEMPSENSE1;
    register8_t DACAOFFCAL;
    register8_t DACAGAINCAL;
    register8_t DACBOFFCAL;
    register8_t DACBGAINCAL;
    register8_t reserved_0x34;
    register8_t reserved_0x35;
    register8_t reserved_0x36;
    register8_t reserved_0x37;
    register8_t reserved_0x38;
    register8_t reserved_0x39;
    register8_t reserved_0x3A;
    register8_t reserved_0x3B;
    register8_t reserved_0x3C;
    register8_t reserved_0x3D;
    register8_t reserved_0x3E;
} NVM_PROD_SIGNATURES_t;


typedef enum NVM_CMD_enum
{
    NVM_CMD_NO_OPERATION_gc = (0x00<<0),
    NVM_CMD_READ_CALIB_ROW_gc = (0x02<<0),
    NVM_CMD_READ_USER_SIG_ROW_gc = (0x01<<0),
    NVM_CMD_READ_EEPROM_gc = (0x06<<0),
    NVM_CMD_READ_FUSES_gc = (0x07<<0),
    NVM_CMD_WRITE_LOCK_BITS_gc = (0x08<<0),
    NVM_CMD_ERASE_USER_SIG_ROW_gc = (0x18<<0),
    NVM_CMD_WRITE_USER_SIG_ROW_gc = (0x1A<<0),
    NVM_CMD_ERASE_APP_gc = (0x20<<0),
    NVM_CMD_ERASE_APP_PAGE_gc = (0x22<<0),
    NVM_CMD_LOAD_FLASH_BUFFER_gc = (0x23<<0),
    NVM_CMD_WRITE_APP_PAGE_gc = (0x24<<0),
    NVM_CMD_ERASE_WRITE_APP_PAGE_gc = (0x25<<0),
    NVM_CMD_ERASE_FLASH_BUFFER_gc = (0x26<<0),
    NVM_CMD_ERASE_BOOT_PAGE_gc = (0x2A<<0),
    NVM_CMD_WRITE_BOOT_PAGE_gc = (0x2C<<0),
    NVM_CMD_ERASE_WRITE_BOOT_PAGE_gc = (0x2D<<0),
    NVM_CMD_ERASE_EEPROM_gc = (0x30<<0),
    NVM_CMD_ERASE_EEPROM_PAGE_gc = (0x32<<0),
    NVM_CMD_LOAD_EEPROM_BUFFER_gc = (0x33<<0),
    NVM_CMD_WRITE_EEPROM_PAGE_gc = (0x34<<0),
    NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc = (0x35<<0),
    NVM_CMD_ERASE_EEPROM_BUFFER_gc = (0x36<<0),
    NVM_CMD_APP_CRC_gc = (0x38<<0),
    NVM_CMD_BOOT_CRC_gc = (0x39<<0),
    NVM_CMD_FLASH_RANGE_CRC_gc = (0x3A<<0),
} NVM_CMD_t;


typedef enum NVM_SPMLVL_enum
{
    NVM_SPMLVL_OFF_gc = (0x00<<2),
    NVM_SPMLVL_LO_gc = (0x01<<2),
    NVM_SPMLVL_MED_gc = (0x02<<2),
    NVM_SPMLVL_HI_gc = (0x03<<2),
} NVM_SPMLVL_t;


typedef enum NVM_EELVL_enum
{
    NVM_EELVL_OFF_gc = (0x00<<0),
    NVM_EELVL_LO_gc = (0x01<<0),
    NVM_EELVL_MED_gc = (0x02<<0),
    NVM_EELVL_HI_gc = (0x03<<0),
} NVM_EELVL_t;


typedef enum NVM_BLBB_enum
{
    NVM_BLBB_NOLOCK_gc = (0x03<<6),
    NVM_BLBB_WLOCK_gc = (0x02<<6),
    NVM_BLBB_RLOCK_gc = (0x01<<6),
    NVM_BLBB_RWLOCK_gc = (0x00<<6),
} NVM_BLBB_t;


typedef enum NVM_BLBA_enum
{
    NVM_BLBA_NOLOCK_gc = (0x03<<4),
    NVM_BLBA_WLOCK_gc = (0x02<<4),
    NVM_BLBA_RLOCK_gc = (0x01<<4),
    NVM_BLBA_RWLOCK_gc = (0x00<<4),
} NVM_BLBA_t;


typedef enum NVM_BLBAT_enum
{
    NVM_BLBAT_NOLOCK_gc = (0x03<<2),
    NVM_BLBAT_WLOCK_gc = (0x02<<2),
    NVM_BLBAT_RLOCK_gc = (0x01<<2),
    NVM_BLBAT_RWLOCK_gc = (0x00<<2),
} NVM_BLBAT_t;


typedef enum NVM_LB_enum
{
    NVM_LB_NOLOCK_gc = (0x03<<0),
    NVM_LB_WLOCK_gc = (0x02<<0),
    NVM_LB_RWLOCK_gc = (0x00<<0),
} NVM_LB_t;


typedef enum BOOTRST_enum
{
    BOOTRST_BOOTLDR_gc = (0x00<<6),
    BOOTRST_APPLICATION_gc = (0x01<<6),
} BOOTRST_t;


typedef enum BOD_enum
{
    BOD_INSAMPLEDMODE_gc = (0x01<<0),
    BOD_CONTINOUSLY_gc = (0x02<<0),
    BOD_DISABLED_gc = (0x03<<0),
} BOD_t;


typedef enum WD_enum
{
    WD_8CLK_gc = (0x00<<4),
    WD_16CLK_gc = (0x01<<4),
    WD_32CLK_gc = (0x02<<4),
    WD_64CLK_gc = (0x03<<4),
    WD_128CLK_gc = (0x04<<4),
    WD_256CLK_gc = (0x05<<4),
    WD_512CLK_gc = (0x06<<4),
    WD_1KCLK_gc = (0x07<<4),
    WD_2KCLK_gc = (0x08<<4),
    WD_4KCLK_gc = (0x09<<4),
    WD_8KCLK_gc = (0x0A<<4),
} WD_t;


typedef enum SUT_enum
{
    SUT_0MS_gc = (0x03<<2),
    SUT_4MS_gc = (0x01<<2),
    SUT_64MS_gc = (0x00<<2),
} SUT_t;


typedef enum BODLVL_enum
{
    BODLVL_1V6_gc = (0x07<<0),
    BODLVL_1V9_gc = (0x06<<0),
    BODLVL_2V1_gc = (0x05<<0),
    BODLVL_2V4_gc = (0x04<<0),
    BODLVL_2V6_gc = (0x03<<0),
    BODLVL_2V9_gc = (0x02<<0),
    BODLVL_3V2_gc = (0x01<<0),
} BODLVL_t;
# 1106 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct AC_struct
{
    register8_t AC0CTRL;
    register8_t AC1CTRL;
    register8_t AC0MUXCTRL;
    register8_t AC1MUXCTRL;
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t WINCTRL;
    register8_t STATUS;
} AC_t;


typedef enum AC_INTMODE_enum
{
    AC_INTMODE_BOTHEDGES_gc = (0x00<<6),
    AC_INTMODE_FALLING_gc = (0x02<<6),
    AC_INTMODE_RISING_gc = (0x03<<6),
} AC_INTMODE_t;


typedef enum AC_INTLVL_enum
{
    AC_INTLVL_OFF_gc = (0x00<<4),
    AC_INTLVL_LO_gc = (0x01<<4),
    AC_INTLVL_MED_gc = (0x02<<4),
    AC_INTLVL_HI_gc = (0x03<<4),
} AC_INTLVL_t;


typedef enum AC_HYSMODE_enum
{
    AC_HYSMODE_NO_gc = (0x00<<1),
    AC_HYSMODE_SMALL_gc = (0x01<<1),
    AC_HYSMODE_LARGE_gc = (0x02<<1),
} AC_HYSMODE_t;


typedef enum AC_MUXPOS_enum
{
    AC_MUXPOS_PIN0_gc = (0x00<<3),
    AC_MUXPOS_PIN1_gc = (0x01<<3),
    AC_MUXPOS_PIN2_gc = (0x02<<3),
    AC_MUXPOS_PIN3_gc = (0x03<<3),
    AC_MUXPOS_PIN4_gc = (0x04<<3),
    AC_MUXPOS_PIN5_gc = (0x05<<3),
    AC_MUXPOS_PIN6_gc = (0x06<<3),
    AC_MUXPOS_DAC_gc = (0x07<<3),
} AC_MUXPOS_t;


typedef enum AC_MUXNEG_enum
{
    AC_MUXNEG_PIN0_gc = (0x00<<0),
    AC_MUXNEG_PIN1_gc = (0x01<<0),
    AC_MUXNEG_PIN3_gc = (0x02<<0),
    AC_MUXNEG_PIN5_gc = (0x03<<0),
    AC_MUXNEG_PIN7_gc = (0x04<<0),
    AC_MUXNEG_DAC_gc = (0x05<<0),
    AC_MUXNEG_BANDGAP_gc = (0x06<<0),
    AC_MUXNEG_SCALER_gc = (0x07<<0),
} AC_MUXNEG_t;


typedef enum AC_WINTMODE_enum
{
    AC_WINTMODE_ABOVE_gc = (0x00<<2),
    AC_WINTMODE_INSIDE_gc = (0x01<<2),
    AC_WINTMODE_BELOW_gc = (0x02<<2),
    AC_WINTMODE_OUTSIDE_gc = (0x03<<2),
} AC_WINTMODE_t;


typedef enum AC_WINTLVL_enum
{
    AC_WINTLVL_OFF_gc = (0x00<<0),
    AC_WINTLVL_LO_gc = (0x01<<0),
    AC_WINTLVL_MED_gc = (0x02<<0),
    AC_WINTLVL_HI_gc = (0x03<<0),
} AC_WINTLVL_t;


typedef enum AC_WSTATE_enum
{
    AC_WSTATE_ABOVE_gc = (0x00<<6),
    AC_WSTATE_INSIDE_gc = (0x01<<6),
    AC_WSTATE_BELOW_gc = (0x02<<6),
} AC_WSTATE_t;
# 1203 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct ADC_CH_struct
{
    register8_t CTRL;
    register8_t MUXCTRL;
    register8_t INTCTRL;
    register8_t INTFLAGS;
    __extension__ union { register16_t RES; struct { register8_t RESL; register8_t RESH; }; };
    register8_t reserved_0x6;
    register8_t reserved_0x7;
} ADC_CH_t;
# 1221 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct ADC_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t REFCTRL;
    register8_t EVCTRL;
    register8_t PRESCALER;
    register8_t CALCTRL;
    register8_t INTFLAGS;
    register8_t reserved_0x07;
    register8_t reserved_0x08;
    register8_t reserved_0x09;
    register8_t reserved_0x0A;
    register8_t reserved_0x0B;
    __extension__ union { register16_t CAL; struct { register8_t CALL; register8_t CALH; }; };
    register8_t reserved_0x0E;
    register8_t reserved_0x0F;
    __extension__ union { register16_t CH0RES; struct { register8_t CH0RESL; register8_t CH0RESH; }; };
    __extension__ union { register16_t CH1RES; struct { register8_t CH1RESL; register8_t CH1RESH; }; };
    __extension__ union { register16_t CH2RES; struct { register8_t CH2RESL; register8_t CH2RESH; }; };
    __extension__ union { register16_t CH3RES; struct { register8_t CH3RESL; register8_t CH3RESH; }; };
    __extension__ union { register16_t CMP; struct { register8_t CMPL; register8_t CMPH; }; };
    register8_t reserved_0x1A;
    register8_t reserved_0x1B;
    register8_t reserved_0x1C;
    register8_t reserved_0x1D;
    register8_t reserved_0x1E;
    register8_t reserved_0x1F;
    ADC_CH_t CH0;
    ADC_CH_t CH1;
    ADC_CH_t CH2;
    ADC_CH_t CH3;
} ADC_t;


typedef enum ADC_CH_MUXPOS_enum
{
    ADC_CH_MUXPOS_PIN0_gc = (0x00<<3),
    ADC_CH_MUXPOS_PIN1_gc = (0x01<<3),
    ADC_CH_MUXPOS_PIN2_gc = (0x02<<3),
    ADC_CH_MUXPOS_PIN3_gc = (0x03<<3),
    ADC_CH_MUXPOS_PIN4_gc = (0x04<<3),
    ADC_CH_MUXPOS_PIN5_gc = (0x05<<3),
    ADC_CH_MUXPOS_PIN6_gc = (0x06<<3),
    ADC_CH_MUXPOS_PIN7_gc = (0x07<<3),
} ADC_CH_MUXPOS_t;


typedef enum ADC_CH_MUXINT_enum
{
    ADC_CH_MUXINT_TEMP_gc = (0x00<<3),
    ADC_CH_MUXINT_BANDGAP_gc = (0x01<<3),
    ADC_CH_MUXINT_SCALEDVCC_gc = (0x02<<3),
    ADC_CH_MUXINT_DAC_gc = (0x03<<3),
} ADC_CH_MUXINT_t;


typedef enum ADC_CH_MUXNEG_enum
{
    ADC_CH_MUXNEG_PIN0_gc = (0x00<<0),
    ADC_CH_MUXNEG_PIN1_gc = (0x01<<0),
    ADC_CH_MUXNEG_PIN2_gc = (0x02<<0),
    ADC_CH_MUXNEG_PIN3_gc = (0x03<<0),
    ADC_CH_MUXNEG_PIN4_gc = (0x04<<0),
    ADC_CH_MUXNEG_PIN5_gc = (0x05<<0),
    ADC_CH_MUXNEG_PIN6_gc = (0x06<<0),
    ADC_CH_MUXNEG_PIN7_gc = (0x07<<0),
} ADC_CH_MUXNEG_t;


typedef enum ADC_CH_INPUTMODE_enum
{
    ADC_CH_INPUTMODE_INTERNAL_gc = (0x00<<0),
    ADC_CH_INPUTMODE_SINGLEENDED_gc = (0x01<<0),
    ADC_CH_INPUTMODE_DIFF_gc = (0x02<<0),
    ADC_CH_INPUTMODE_DIFFWGAIN_gc = (0x03<<0),
} ADC_CH_INPUTMODE_t;


typedef enum ADC_CH_GAIN_enum
{
    ADC_CH_GAIN_1X_gc = (0x00<<2),
    ADC_CH_GAIN_2X_gc = (0x01<<2),
    ADC_CH_GAIN_4X_gc = (0x02<<2),
    ADC_CH_GAIN_8X_gc = (0x03<<2),
    ADC_CH_GAIN_16X_gc = (0x04<<2),
    ADC_CH_GAIN_32X_gc = (0x05<<2),
    ADC_CH_GAIN_64X_gc = (0x06<<2),
} ADC_CH_GAIN_t;


typedef enum ADC_RESOLUTION_enum
{
    ADC_RESOLUTION_12BIT_gc = (0x00<<1),
    ADC_RESOLUTION_8BIT_gc = (0x02<<1),
    ADC_RESOLUTION_LEFT12BIT_gc = (0x03<<1),
} ADC_RESOLUTION_t;


typedef enum ADC_REFSEL_enum
{
    ADC_REFSEL_INT1V_gc = (0x00<<4),
    ADC_REFSEL_VCC_gc = (0x01<<4),
    ADC_REFSEL_AREFA_gc = (0x02<<4),
    ADC_REFSEL_AREFB_gc = (0x03<<4),
} ADC_REFSEL_t;


typedef enum ADC_SWEEP_enum
{
    ADC_SWEEP_0_gc = (0x00<<6),
    ADC_SWEEP_01_gc = (0x01<<6),
    ADC_SWEEP_012_gc = (0x02<<6),
    ADC_SWEEP_0123_gc = (0x03<<6),
} ADC_SWEEP_t;


typedef enum ADC_EVSEL_enum
{
    ADC_EVSEL_0123_gc = (0x00<<3),
    ADC_EVSEL_1234_gc = (0x01<<3),
    ADC_EVSEL_2345_gc = (0x02<<3),
    ADC_EVSEL_3456_gc = (0x03<<3),
    ADC_EVSEL_4567_gc = (0x04<<3),
    ADC_EVSEL_567_gc = (0x05<<3),
    ADC_EVSEL_67_gc = (0x06<<3),
    ADC_EVSEL_7_gc = (0x07<<3),
} ADC_EVSEL_t;


typedef enum ADC_EVACT_enum
{
    ADC_EVACT_NONE_gc = (0x00<<0),
    ADC_EVACT_CH0_gc = (0x01<<0),
    ADC_EVACT_CH01_gc = (0x02<<0),
    ADC_EVACT_CH012_gc = (0x03<<0),
    ADC_EVACT_CH0123_gc = (0x04<<0),
    ADC_EVACT_SWEEP_gc = (0x05<<0),
    ADC_EVACT_SYNCHSWEEP_gc = (0x06<<0),
} ADC_EVACT_t;


typedef enum ADC_CH_INTMODE_enum
{
    ADC_CH_INTMODE_COMPLETE_gc = (0x00<<2),
    ADC_CH_INTMODE_BELOW_gc = (0x01<<2),
    ADC_CH_INTMODE_ABOVE_gc = (0x03<<2),
} ADC_CH_INTMODE_t;


typedef enum ADC_CH_INTLVL_enum
{
    ADC_CH_INTLVL_OFF_gc = (0x00<<0),
    ADC_CH_INTLVL_LO_gc = (0x01<<0),
    ADC_CH_INTLVL_MED_gc = (0x02<<0),
    ADC_CH_INTLVL_HI_gc = (0x03<<0),
} ADC_CH_INTLVL_t;


typedef enum ADC_DMASEL_enum
{
    ADC_DMASEL_OFF_gc = (0x00<<6),
    ADC_DMASEL_CH01_gc = (0x01<<6),
    ADC_DMASEL_CH012_gc = (0x02<<6),
    ADC_DMASEL_CH0123_gc = (0x03<<6),
} ADC_DMASEL_t;


typedef enum ADC_PRESCALER_enum
{
    ADC_PRESCALER_DIV4_gc = (0x00<<0),
    ADC_PRESCALER_DIV8_gc = (0x01<<0),
    ADC_PRESCALER_DIV16_gc = (0x02<<0),
    ADC_PRESCALER_DIV32_gc = (0x03<<0),
    ADC_PRESCALER_DIV64_gc = (0x04<<0),
    ADC_PRESCALER_DIV128_gc = (0x05<<0),
    ADC_PRESCALER_DIV256_gc = (0x06<<0),
    ADC_PRESCALER_DIV512_gc = (0x07<<0),
} ADC_PRESCALER_t;
# 1409 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct DAC_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t EVCTRL;
    register8_t TIMCTRL;
    register8_t STATUS;
    register8_t reserved_0x06;
    register8_t reserved_0x07;
    register8_t GAINCAL;
    register8_t OFFSETCAL;
    register8_t reserved_0x0A;
    register8_t reserved_0x0B;
    register8_t reserved_0x0C;
    register8_t reserved_0x0D;
    register8_t reserved_0x0E;
    register8_t reserved_0x0F;
    register8_t reserved_0x10;
    register8_t reserved_0x11;
    register8_t reserved_0x12;
    register8_t reserved_0x13;
    register8_t reserved_0x14;
    register8_t reserved_0x15;
    register8_t reserved_0x16;
    register8_t reserved_0x17;
    __extension__ union { register16_t CH0DATA; struct { register8_t CH0DATAL; register8_t CH0DATAH; }; };
    __extension__ union { register16_t CH1DATA; struct { register8_t CH1DATAL; register8_t CH1DATAH; }; };
} DAC_t;


typedef enum DAC_CHSEL_enum
{
    DAC_CHSEL_SINGLE_gc = (0x00<<5),
    DAC_CHSEL_DUAL_gc = (0x02<<5),
} DAC_CHSEL_t;


typedef enum DAC_REFSEL_enum
{
    DAC_REFSEL_INT1V_gc = (0x00<<3),
    DAC_REFSEL_AVCC_gc = (0x01<<3),
    DAC_REFSEL_AREFA_gc = (0x02<<3),
    DAC_REFSEL_AREFB_gc = (0x03<<3),
} DAC_REFSEL_t;


typedef enum DAC_EVSEL_enum
{
    DAC_EVSEL_0_gc = (0x00<<0),
    DAC_EVSEL_1_gc = (0x01<<0),
    DAC_EVSEL_2_gc = (0x02<<0),
    DAC_EVSEL_3_gc = (0x03<<0),
    DAC_EVSEL_4_gc = (0x04<<0),
    DAC_EVSEL_5_gc = (0x05<<0),
    DAC_EVSEL_6_gc = (0x06<<0),
    DAC_EVSEL_7_gc = (0x07<<0),
} DAC_EVSEL_t;


typedef enum DAC_CONINTVAL_enum
{
    DAC_CONINTVAL_1CLK_gc = (0x00<<4),
    DAC_CONINTVAL_2CLK_gc = (0x01<<4),
    DAC_CONINTVAL_4CLK_gc = (0x02<<4),
    DAC_CONINTVAL_8CLK_gc = (0x03<<4),
    DAC_CONINTVAL_16CLK_gc = (0x04<<4),
    DAC_CONINTVAL_32CLK_gc = (0x05<<4),
    DAC_CONINTVAL_64CLK_gc = (0x06<<4),
    DAC_CONINTVAL_128CLK_gc = (0x07<<4),
} DAC_CONINTVAL_t;


typedef enum DAC_REFRESH_enum
{
    DAC_REFRESH_16CLK_gc = (0x00<<0),
    DAC_REFRESH_32CLK_gc = (0x01<<0),
    DAC_REFRESH_64CLK_gc = (0x02<<0),
    DAC_REFRESH_128CLK_gc = (0x03<<0),
    DAC_REFRESH_256CLK_gc = (0x04<<0),
    DAC_REFRESH_512CLK_gc = (0x05<<0),
    DAC_REFRESH_1024CLK_gc = (0x06<<0),
    DAC_REFRESH_2048CLK_gc = (0x07<<0),
    DAC_REFRESH_4086CLK_gc = (0x08<<0),
    DAC_REFRESH_8192CLK_gc = (0x09<<0),
    DAC_REFRESH_16384CLK_gc = (0x0A<<0),
    DAC_REFRESH_32768CLK_gc = (0x0B<<0),
    DAC_REFRESH_65536CLK_gc = (0x0C<<0),
    DAC_REFRESH_OFF_gc = (0x0F<<0),
} DAC_REFRESH_t;
# 1508 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct RTC_struct
{
    register8_t CTRL;
    register8_t STATUS;
    register8_t INTCTRL;
    register8_t INTFLAGS;
    register8_t TEMP;
    register8_t reserved_0x05;
    register8_t reserved_0x06;
    register8_t reserved_0x07;
    __extension__ union { register16_t CNT; struct { register8_t CNTL; register8_t CNTH; }; };
    __extension__ union { register16_t PER; struct { register8_t PERL; register8_t PERH; }; };
    __extension__ union { register16_t COMP; struct { register8_t COMPL; register8_t COMPH; }; };
} RTC_t;


typedef enum RTC_PRESCALER_enum
{
    RTC_PRESCALER_OFF_gc = (0x00<<0),
    RTC_PRESCALER_DIV1_gc = (0x01<<0),
    RTC_PRESCALER_DIV2_gc = (0x02<<0),
    RTC_PRESCALER_DIV8_gc = (0x03<<0),
    RTC_PRESCALER_DIV16_gc = (0x04<<0),
    RTC_PRESCALER_DIV64_gc = (0x05<<0),
    RTC_PRESCALER_DIV256_gc = (0x06<<0),
    RTC_PRESCALER_DIV1024_gc = (0x07<<0),
} RTC_PRESCALER_t;


typedef enum RTC_COMPINTLVL_enum
{
    RTC_COMPINTLVL_OFF_gc = (0x00<<2),
    RTC_COMPINTLVL_LO_gc = (0x01<<2),
    RTC_COMPINTLVL_MED_gc = (0x02<<2),
    RTC_COMPINTLVL_HI_gc = (0x03<<2),
} RTC_COMPINTLVL_t;


typedef enum RTC_OVFINTLVL_enum
{
    RTC_OVFINTLVL_OFF_gc = (0x00<<0),
    RTC_OVFINTLVL_LO_gc = (0x01<<0),
    RTC_OVFINTLVL_MED_gc = (0x02<<0),
    RTC_OVFINTLVL_HI_gc = (0x03<<0),
} RTC_OVFINTLVL_t;
# 1562 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct EBI_CS_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    __extension__ union { register16_t BASEADDR; struct { register8_t BASEADDRL; register8_t BASEADDRH; }; };
} EBI_CS_t;
# 1576 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct EBI_struct
{
    register8_t CTRL;
    register8_t SDRAMCTRLA;
    register8_t reserved_0x02;
    register8_t reserved_0x03;
    __extension__ union { register16_t REFRESH; struct { register8_t REFRESHL; register8_t REFRESHH; }; };
    __extension__ union { register16_t INITDLY; struct { register8_t INITDLYL; register8_t INITDLYH; }; };
    register8_t SDRAMCTRLB;
    register8_t SDRAMCTRLC;
    register8_t reserved_0x0A;
    register8_t reserved_0x0B;
    register8_t reserved_0x0C;
    register8_t reserved_0x0D;
    register8_t reserved_0x0E;
    register8_t reserved_0x0F;
    EBI_CS_t CS0;
    EBI_CS_t CS1;
    EBI_CS_t CS2;
    EBI_CS_t CS3;
} EBI_t;


typedef enum EBI_CS_ASIZE_enum
{
    EBI_CS_ASIZE_256B_gc = (0x00<<2),
    EBI_CS_ASIZE_512B_gc = (0x01<<2),
    EBI_CS_ASIZE_1KB_gc = (0x02<<2),
    EBI_CS_ASIZE_2KB_gc = (0x03<<2),
    EBI_CS_ASIZE_4KB_gc = (0x04<<2),
    EBI_CS_ASIZE_8KB_gc = (0x05<<2),
    EBI_CS_ASIZE_16KB_gc = (0x06<<2),
    EBI_CS_ASIZE_32KB_gc = (0x07<<2),
    EBI_CS_ASIZE_64KB_gc = (0x08<<2),
    EBI_CS_ASIZE_128KB_gc = (0x09<<2),
    EBI_CS_ASIZE_256KB_gc = (0x0A<<2),
    EBI_CS_ASIZE_512KB_gc = (0x0B<<2),
    EBI_CS_ASIZE_1MB_gc = (0x0C<<2),
    EBI_CS_ASIZE_2MB_gc = (0x0D<<2),
    EBI_CS_ASIZE_4MB_gc = (0x0E<<2),
    EBI_CS_ASIZE_8MB_gc = (0x0F<<2),
    EBI_CS_ASIZE_16M_gc = (0x10<<2),
} EBI_CS_ASIZE_t;


typedef enum EBI_CS_SRWS_enum
{
    EBI_CS_SRWS_0CLK_gc = (0x00<<0),
    EBI_CS_SRWS_1CLK_gc = (0x01<<0),
    EBI_CS_SRWS_2CLK_gc = (0x02<<0),
    EBI_CS_SRWS_3CLK_gc = (0x03<<0),
    EBI_CS_SRWS_4CLK_gc = (0x04<<0),
    EBI_CS_SRWS_5CLK_gc = (0x05<<0),
    EBI_CS_SRWS_6CLK_gc = (0x06<<0),
    EBI_CS_SRWS_7CLK_gc = (0x07<<0),
} EBI_CS_SRWS_t;


typedef enum EBI_CS_MODE_enum
{
    EBI_CS_MODE_DISABLED_gc = (0x00<<0),
    EBI_CS_MODE_SRAM_gc = (0x01<<0),
    EBI_CS_MODE_LPC_gc = (0x02<<0),
    EBI_CS_MODE_SDRAM_gc = (0x03<<0),
} EBI_CS_MODE_t;


typedef enum EBI_CS_SDMODE_enum
{
    EBI_CS_SDMODE_NORMAL_gc = (0x00<<0),
    EBI_CS_SDMODE_LOAD_gc = (0x01<<0),
} EBI_CS_SDMODE_t;


typedef enum EBI_SDDATAW_enum
{
    EBI_SDDATAW_4BIT_gc = (0x00<<6),
    EBI_SDDATAW_8BIT_gc = (0x01<<6),
} EBI_SDDATAW_t;


typedef enum EBI_LPCMODE_enum
{
    EBI_LPCMODE_ALE1_gc = (0x00<<4),
    EBI_LPCMODE_ALE12_gc = (0x02<<4),
} EBI_LPCMODE_t;


typedef enum EBI_SRMODE_enum
{
    EBI_SRMODE_ALE1_gc = (0x00<<2),
    EBI_SRMODE_ALE2_gc = (0x01<<2),
    EBI_SRMODE_ALE12_gc = (0x02<<2),
    EBI_SRMODE_NOALE_gc = (0x03<<2),
} EBI_SRMODE_t;


typedef enum EBI_IFMODE_enum
{
    EBI_IFMODE_DISABLED_gc = (0x00<<0),
    EBI_IFMODE_3PORT_gc = (0x01<<0),
    EBI_IFMODE_4PORT_gc = (0x02<<0),
    EBI_IFMODE_2PORT_gc = (0x03<<0),
} EBI_IFMODE_t;


typedef enum EBI_SDCOL_enum
{
    EBI_SDCOL_8BIT_gc = (0x00<<0),
    EBI_SDCOL_9BIT_gc = (0x01<<0),
    EBI_SDCOL_10BIT_gc = (0x02<<0),
    EBI_SDCOL_11BIT_gc = (0x03<<0),
} EBI_SDCOL_t;


typedef enum EBI_MRDLY_enum
{
    EBI_MRDLY_0CLK_gc = (0x00<<6),
    EBI_MRDLY_1CLK_gc = (0x01<<6),
    EBI_MRDLY_2CLK_gc = (0x02<<6),
    EBI_MRDLY_3CLK_gc = (0x03<<6),
} EBI_MRDLY_t;


typedef enum EBI_ROWCYCDLY_enum
{
    EBI_ROWCYCDLY_0CLK_gc = (0x00<<3),
    EBI_ROWCYCDLY_1CLK_gc = (0x01<<3),
    EBI_ROWCYCDLY_2CLK_gc = (0x02<<3),
    EBI_ROWCYCDLY_3CLK_gc = (0x03<<3),
    EBI_ROWCYCDLY_4CLK_gc = (0x04<<3),
    EBI_ROWCYCDLY_5CLK_gc = (0x05<<3),
    EBI_ROWCYCDLY_6CLK_gc = (0x06<<3),
    EBI_ROWCYCDLY_7CLK_gc = (0x07<<3),
} EBI_ROWCYCDLY_t;


typedef enum EBI_RPDLY_enum
{
    EBI_RPDLY_0CLK_gc = (0x00<<0),
    EBI_RPDLY_1CLK_gc = (0x01<<0),
    EBI_RPDLY_2CLK_gc = (0x02<<0),
    EBI_RPDLY_3CLK_gc = (0x03<<0),
    EBI_RPDLY_4CLK_gc = (0x04<<0),
    EBI_RPDLY_5CLK_gc = (0x05<<0),
    EBI_RPDLY_6CLK_gc = (0x06<<0),
    EBI_RPDLY_7CLK_gc = (0x07<<0),
} EBI_RPDLY_t;


typedef enum EBI_WRDLY_enum
{
    EBI_WRDLY_0CLK_gc = (0x00<<6),
    EBI_WRDLY_1CLK_gc = (0x01<<6),
    EBI_WRDLY_2CLK_gc = (0x02<<6),
    EBI_WRDLY_3CLK_gc = (0x03<<6),
} EBI_WRDLY_t;


typedef enum EBI_ESRDLY_enum
{
    EBI_ESRDLY_0CLK_gc = (0x00<<3),
    EBI_ESRDLY_1CLK_gc = (0x01<<3),
    EBI_ESRDLY_2CLK_gc = (0x02<<3),
    EBI_ESRDLY_3CLK_gc = (0x03<<3),
    EBI_ESRDLY_4CLK_gc = (0x04<<3),
    EBI_ESRDLY_5CLK_gc = (0x05<<3),
    EBI_ESRDLY_6CLK_gc = (0x06<<3),
    EBI_ESRDLY_7CLK_gc = (0x07<<3),
} EBI_ESRDLY_t;


typedef enum EBI_ROWCOLDLY_enum
{
    EBI_ROWCOLDLY_0CLK_gc = (0x00<<0),
    EBI_ROWCOLDLY_1CLK_gc = (0x01<<0),
    EBI_ROWCOLDLY_2CLK_gc = (0x02<<0),
    EBI_ROWCOLDLY_3CLK_gc = (0x03<<0),
    EBI_ROWCOLDLY_4CLK_gc = (0x04<<0),
    EBI_ROWCOLDLY_5CLK_gc = (0x05<<0),
    EBI_ROWCOLDLY_6CLK_gc = (0x06<<0),
    EBI_ROWCOLDLY_7CLK_gc = (0x07<<0),
} EBI_ROWCOLDLY_t;
# 1768 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct TWI_MASTER_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t STATUS;
    register8_t BAUD;
    register8_t ADDR;
    register8_t DATA;
} TWI_MASTER_t;
# 1786 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct TWI_SLAVE_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t STATUS;
    register8_t ADDR;
    register8_t DATA;
    register8_t ADDRMASK;
} TWI_SLAVE_t;
# 1803 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct TWI_struct
{
    register8_t CTRL;
    TWI_MASTER_t MASTER;
    TWI_SLAVE_t SLAVE;
} TWI_t;


typedef enum TWI_MASTER_INTLVL_enum
{
    TWI_MASTER_INTLVL_OFF_gc = (0x00<<6),
    TWI_MASTER_INTLVL_LO_gc = (0x01<<6),
    TWI_MASTER_INTLVL_MED_gc = (0x02<<6),
    TWI_MASTER_INTLVL_HI_gc = (0x03<<6),
} TWI_MASTER_INTLVL_t;


typedef enum TWI_MASTER_TIMEOUT_enum
{
    TWI_MASTER_TIMEOUT_DISABLED_gc = (0x00<<2),
    TWI_MASTER_TIMEOUT_50US_gc = (0x01<<2),
    TWI_MASTER_TIMEOUT_100US_gc = (0x02<<2),
    TWI_MASTER_TIMEOUT_200US_gc = (0x03<<2),
} TWI_MASTER_TIMEOUT_t;


typedef enum TWI_MASTER_CMD_enum
{
    TWI_MASTER_CMD_NOACT_gc = (0x00<<0),
    TWI_MASTER_CMD_REPSTART_gc = (0x01<<0),
    TWI_MASTER_CMD_RECVTRANS_gc = (0x02<<0),
    TWI_MASTER_CMD_STOP_gc = (0x03<<0),
} TWI_MASTER_CMD_t;


typedef enum TWI_MASTER_BUSSTATE_enum
{
    TWI_MASTER_BUSSTATE_UNKNOWN_gc = (0x00<<0),
    TWI_MASTER_BUSSTATE_IDLE_gc = (0x01<<0),
    TWI_MASTER_BUSSTATE_OWNER_gc = (0x02<<0),
    TWI_MASTER_BUSSTATE_BUSY_gc = (0x03<<0),
} TWI_MASTER_BUSSTATE_t;


typedef enum TWI_SLAVE_INTLVL_enum
{
    TWI_SLAVE_INTLVL_OFF_gc = (0x00<<6),
    TWI_SLAVE_INTLVL_LO_gc = (0x01<<6),
    TWI_SLAVE_INTLVL_MED_gc = (0x02<<6),
    TWI_SLAVE_INTLVL_HI_gc = (0x03<<6),
} TWI_SLAVE_INTLVL_t;


typedef enum TWI_SLAVE_CMD_enum
{
    TWI_SLAVE_CMD_NOACT_gc = (0x00<<0),
    TWI_SLAVE_CMD_COMPTRANS_gc = (0x02<<0),
    TWI_SLAVE_CMD_RESPONSE_gc = (0x03<<0),
} TWI_SLAVE_CMD_t;
# 1871 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct PORTCFG_struct
{
    register8_t MPCMASK;
    register8_t reserved_0x01;
    register8_t VPCTRLA;
    register8_t VPCTRLB;
    register8_t CLKEVOUT;
} PORTCFG_t;
# 1887 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct VPORT_struct
{
    register8_t DIR;
    register8_t OUT;
    register8_t IN;
    register8_t INTFLAGS;
} VPORT_t;
# 1902 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct PORT_struct
{
    register8_t DIR;
    register8_t DIRSET;
    register8_t DIRCLR;
    register8_t DIRTGL;
    register8_t OUT;
    register8_t OUTSET;
    register8_t OUTCLR;
    register8_t OUTTGL;
    register8_t IN;
    register8_t INTCTRL;
    register8_t INT0MASK;
    register8_t INT1MASK;
    register8_t INTFLAGS;
    register8_t reserved_0x0D;
    register8_t reserved_0x0E;
    register8_t reserved_0x0F;
    register8_t PIN0CTRL;
    register8_t PIN1CTRL;
    register8_t PIN2CTRL;
    register8_t PIN3CTRL;
    register8_t PIN4CTRL;
    register8_t PIN5CTRL;
    register8_t PIN6CTRL;
    register8_t PIN7CTRL;
} PORT_t;


typedef enum PORTCFG_VP0MAP_enum
{
    PORTCFG_VP0MAP_PORTA_gc = (0x00<<0),
    PORTCFG_VP0MAP_PORTB_gc = (0x01<<0),
    PORTCFG_VP0MAP_PORTC_gc = (0x02<<0),
    PORTCFG_VP0MAP_PORTD_gc = (0x03<<0),
    PORTCFG_VP0MAP_PORTE_gc = (0x04<<0),
    PORTCFG_VP0MAP_PORTF_gc = (0x05<<0),
    PORTCFG_VP0MAP_PORTG_gc = (0x06<<0),
    PORTCFG_VP0MAP_PORTH_gc = (0x07<<0),
    PORTCFG_VP0MAP_PORTJ_gc = (0x08<<0),
    PORTCFG_VP0MAP_PORTK_gc = (0x09<<0),
    PORTCFG_VP0MAP_PORTL_gc = (0x0A<<0),
    PORTCFG_VP0MAP_PORTM_gc = (0x0B<<0),
    PORTCFG_VP0MAP_PORTN_gc = (0x0C<<0),
    PORTCFG_VP0MAP_PORTP_gc = (0x0D<<0),
    PORTCFG_VP0MAP_PORTQ_gc = (0x0E<<0),
    PORTCFG_VP0MAP_PORTR_gc = (0x0F<<0),
} PORTCFG_VP0MAP_t;


typedef enum PORTCFG_VP1MAP_enum
{
    PORTCFG_VP1MAP_PORTA_gc = (0x00<<4),
    PORTCFG_VP1MAP_PORTB_gc = (0x01<<4),
    PORTCFG_VP1MAP_PORTC_gc = (0x02<<4),
    PORTCFG_VP1MAP_PORTD_gc = (0x03<<4),
    PORTCFG_VP1MAP_PORTE_gc = (0x04<<4),
    PORTCFG_VP1MAP_PORTF_gc = (0x05<<4),
    PORTCFG_VP1MAP_PORTG_gc = (0x06<<4),
    PORTCFG_VP1MAP_PORTH_gc = (0x07<<4),
    PORTCFG_VP1MAP_PORTJ_gc = (0x08<<4),
    PORTCFG_VP1MAP_PORTK_gc = (0x09<<4),
    PORTCFG_VP1MAP_PORTL_gc = (0x0A<<4),
    PORTCFG_VP1MAP_PORTM_gc = (0x0B<<4),
    PORTCFG_VP1MAP_PORTN_gc = (0x0C<<4),
    PORTCFG_VP1MAP_PORTP_gc = (0x0D<<4),
    PORTCFG_VP1MAP_PORTQ_gc = (0x0E<<4),
    PORTCFG_VP1MAP_PORTR_gc = (0x0F<<4),
} PORTCFG_VP1MAP_t;


typedef enum PORTCFG_VP2MAP_enum
{
    PORTCFG_VP2MAP_PORTA_gc = (0x00<<0),
    PORTCFG_VP2MAP_PORTB_gc = (0x01<<0),
    PORTCFG_VP2MAP_PORTC_gc = (0x02<<0),
    PORTCFG_VP2MAP_PORTD_gc = (0x03<<0),
    PORTCFG_VP2MAP_PORTE_gc = (0x04<<0),
    PORTCFG_VP2MAP_PORTF_gc = (0x05<<0),
    PORTCFG_VP2MAP_PORTG_gc = (0x06<<0),
    PORTCFG_VP2MAP_PORTH_gc = (0x07<<0),
    PORTCFG_VP2MAP_PORTJ_gc = (0x08<<0),
    PORTCFG_VP2MAP_PORTK_gc = (0x09<<0),
    PORTCFG_VP2MAP_PORTL_gc = (0x0A<<0),
    PORTCFG_VP2MAP_PORTM_gc = (0x0B<<0),
    PORTCFG_VP2MAP_PORTN_gc = (0x0C<<0),
    PORTCFG_VP2MAP_PORTP_gc = (0x0D<<0),
    PORTCFG_VP2MAP_PORTQ_gc = (0x0E<<0),
    PORTCFG_VP2MAP_PORTR_gc = (0x0F<<0),
} PORTCFG_VP2MAP_t;


typedef enum PORTCFG_VP3MAP_enum
{
    PORTCFG_VP3MAP_PORTA_gc = (0x00<<4),
    PORTCFG_VP3MAP_PORTB_gc = (0x01<<4),
    PORTCFG_VP3MAP_PORTC_gc = (0x02<<4),
    PORTCFG_VP3MAP_PORTD_gc = (0x03<<4),
    PORTCFG_VP3MAP_PORTE_gc = (0x04<<4),
    PORTCFG_VP3MAP_PORTF_gc = (0x05<<4),
    PORTCFG_VP3MAP_PORTG_gc = (0x06<<4),
    PORTCFG_VP3MAP_PORTH_gc = (0x07<<4),
    PORTCFG_VP3MAP_PORTJ_gc = (0x08<<4),
    PORTCFG_VP3MAP_PORTK_gc = (0x09<<4),
    PORTCFG_VP3MAP_PORTL_gc = (0x0A<<4),
    PORTCFG_VP3MAP_PORTM_gc = (0x0B<<4),
    PORTCFG_VP3MAP_PORTN_gc = (0x0C<<4),
    PORTCFG_VP3MAP_PORTP_gc = (0x0D<<4),
    PORTCFG_VP3MAP_PORTQ_gc = (0x0E<<4),
    PORTCFG_VP3MAP_PORTR_gc = (0x0F<<4),
} PORTCFG_VP3MAP_t;


typedef enum PORTCFG_CLKOUT_enum
{
    PORTCFG_CLKOUT_OFF_gc = (0x00<<0),
    PORTCFG_CLKOUT_PC7_gc = (0x01<<0),
    PORTCFG_CLKOUT_PD7_gc = (0x02<<0),
    PORTCFG_CLKOUT_PE7_gc = (0x03<<0),
} PORTCFG_CLKOUT_t;


typedef enum PORTCFG_EVOUT_enum
{
    PORTCFG_EVOUT_OFF_gc = (0x00<<4),
    PORTCFG_EVOUT_PC7_gc = (0x01<<4),
    PORTCFG_EVOUT_PD7_gc = (0x02<<4),
    PORTCFG_EVOUT_PE7_gc = (0x03<<4),
} PORTCFG_EVOUT_t;


typedef enum PORT_INT0LVL_enum
{
    PORT_INT0LVL_OFF_gc = (0x00<<0),
    PORT_INT0LVL_LO_gc = (0x01<<0),
    PORT_INT0LVL_MED_gc = (0x02<<0),
    PORT_INT0LVL_HI_gc = (0x03<<0),
} PORT_INT0LVL_t;


typedef enum PORT_INT1LVL_enum
{
    PORT_INT1LVL_OFF_gc = (0x00<<2),
    PORT_INT1LVL_LO_gc = (0x01<<2),
    PORT_INT1LVL_MED_gc = (0x02<<2),
    PORT_INT1LVL_HI_gc = (0x03<<2),
} PORT_INT1LVL_t;


typedef enum PORT_OPC_enum
{
    PORT_OPC_TOTEM_gc = (0x00<<3),
    PORT_OPC_BUSKEEPER_gc = (0x01<<3),
    PORT_OPC_PULLDOWN_gc = (0x02<<3),
    PORT_OPC_PULLUP_gc = (0x03<<3),
    PORT_OPC_WIREDOR_gc = (0x04<<3),
    PORT_OPC_WIREDAND_gc = (0x05<<3),
    PORT_OPC_WIREDORPULL_gc = (0x06<<3),
    PORT_OPC_WIREDANDPULL_gc = (0x07<<3),
} PORT_OPC_t;


typedef enum PORT_ISC_enum
{
    PORT_ISC_BOTHEDGES_gc = (0x00<<0),
    PORT_ISC_RISING_gc = (0x01<<0),
    PORT_ISC_FALLING_gc = (0x02<<0),
    PORT_ISC_LEVEL_gc = (0x03<<0),
    PORT_ISC_INPUT_DISABLE_gc = (0x07<<0),
} PORT_ISC_t;
# 2081 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct TC0_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t CTRLD;
    register8_t CTRLE;
    register8_t reserved_0x05;
    register8_t INTCTRLA;
    register8_t INTCTRLB;
    register8_t CTRLFCLR;
    register8_t CTRLFSET;
    register8_t CTRLGCLR;
    register8_t CTRLGSET;
    register8_t INTFLAGS;
    register8_t reserved_0x0D;
    register8_t reserved_0x0E;
    register8_t TEMP;
    register8_t reserved_0x10;
    register8_t reserved_0x11;
    register8_t reserved_0x12;
    register8_t reserved_0x13;
    register8_t reserved_0x14;
    register8_t reserved_0x15;
    register8_t reserved_0x16;
    register8_t reserved_0x17;
    register8_t reserved_0x18;
    register8_t reserved_0x19;
    register8_t reserved_0x1A;
    register8_t reserved_0x1B;
    register8_t reserved_0x1C;
    register8_t reserved_0x1D;
    register8_t reserved_0x1E;
    register8_t reserved_0x1F;
    __extension__ union { register16_t CNT; struct { register8_t CNTL; register8_t CNTH; }; };
    register8_t reserved_0x22;
    register8_t reserved_0x23;
    register8_t reserved_0x24;
    register8_t reserved_0x25;
    __extension__ union { register16_t PER; struct { register8_t PERL; register8_t PERH; }; };
    __extension__ union { register16_t CCA; struct { register8_t CCAL; register8_t CCAH; }; };
    __extension__ union { register16_t CCB; struct { register8_t CCBL; register8_t CCBH; }; };
    __extension__ union { register16_t CCC; struct { register8_t CCCL; register8_t CCCH; }; };
    __extension__ union { register16_t CCD; struct { register8_t CCDL; register8_t CCDH; }; };
    register8_t reserved_0x30;
    register8_t reserved_0x31;
    register8_t reserved_0x32;
    register8_t reserved_0x33;
    register8_t reserved_0x34;
    register8_t reserved_0x35;
    __extension__ union { register16_t PERBUF; struct { register8_t PERBUFL; register8_t PERBUFH; }; };
    __extension__ union { register16_t CCABUF; struct { register8_t CCABUFL; register8_t CCABUFH; }; };
    __extension__ union { register16_t CCBBUF; struct { register8_t CCBBUFL; register8_t CCBBUFH; }; };
    __extension__ union { register16_t CCCBUF; struct { register8_t CCCBUFL; register8_t CCCBUFH; }; };
    __extension__ union { register16_t CCDBUF; struct { register8_t CCDBUFL; register8_t CCDBUFH; }; };
} TC0_t;
# 2145 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct TC1_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t CTRLD;
    register8_t CTRLE;
    register8_t reserved_0x05;
    register8_t INTCTRLA;
    register8_t INTCTRLB;
    register8_t CTRLFCLR;
    register8_t CTRLFSET;
    register8_t CTRLGCLR;
    register8_t CTRLGSET;
    register8_t INTFLAGS;
    register8_t reserved_0x0D;
    register8_t reserved_0x0E;
    register8_t TEMP;
    register8_t reserved_0x10;
    register8_t reserved_0x11;
    register8_t reserved_0x12;
    register8_t reserved_0x13;
    register8_t reserved_0x14;
    register8_t reserved_0x15;
    register8_t reserved_0x16;
    register8_t reserved_0x17;
    register8_t reserved_0x18;
    register8_t reserved_0x19;
    register8_t reserved_0x1A;
    register8_t reserved_0x1B;
    register8_t reserved_0x1C;
    register8_t reserved_0x1D;
    register8_t reserved_0x1E;
    register8_t reserved_0x1F;
    __extension__ union { register16_t CNT; struct { register8_t CNTL; register8_t CNTH; }; };
    register8_t reserved_0x22;
    register8_t reserved_0x23;
    register8_t reserved_0x24;
    register8_t reserved_0x25;
    __extension__ union { register16_t PER; struct { register8_t PERL; register8_t PERH; }; };
    __extension__ union { register16_t CCA; struct { register8_t CCAL; register8_t CCAH; }; };
    __extension__ union { register16_t CCB; struct { register8_t CCBL; register8_t CCBH; }; };
    register8_t reserved_0x2C;
    register8_t reserved_0x2D;
    register8_t reserved_0x2E;
    register8_t reserved_0x2F;
    register8_t reserved_0x30;
    register8_t reserved_0x31;
    register8_t reserved_0x32;
    register8_t reserved_0x33;
    register8_t reserved_0x34;
    register8_t reserved_0x35;
    __extension__ union { register16_t PERBUF; struct { register8_t PERBUFL; register8_t PERBUFH; }; };
    __extension__ union { register16_t CCABUF; struct { register8_t CCABUFL; register8_t CCABUFH; }; };
    __extension__ union { register16_t CCBBUF; struct { register8_t CCBBUFL; register8_t CCBBUFH; }; };
} TC1_t;
# 2209 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct AWEX_struct
{
    register8_t CTRL;
    register8_t reserved_0x01;
    register8_t FDEVMASK;
    register8_t FDCTRL;
    register8_t STATUS;
    register8_t reserved_0x05;
    register8_t DTBOTH;
    register8_t DTBOTHBUF;
    register8_t DTLS;
    register8_t DTHS;
    register8_t DTLSBUF;
    register8_t DTHSBUF;
    register8_t OUTOVEN;
} AWEX_t;
# 2233 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct HIRES_struct
{
    register8_t CTRL;
} HIRES_t;


typedef enum TC_CLKSEL_enum
{
    TC_CLKSEL_OFF_gc = (0x00<<0),
    TC_CLKSEL_DIV1_gc = (0x01<<0),
    TC_CLKSEL_DIV2_gc = (0x02<<0),
    TC_CLKSEL_DIV4_gc = (0x03<<0),
    TC_CLKSEL_DIV8_gc = (0x04<<0),
    TC_CLKSEL_DIV64_gc = (0x05<<0),
    TC_CLKSEL_DIV256_gc = (0x06<<0),
    TC_CLKSEL_DIV1024_gc = (0x07<<0),
    TC_CLKSEL_EVCH0_gc = (0x08<<0),
    TC_CLKSEL_EVCH1_gc = (0x09<<0),
    TC_CLKSEL_EVCH2_gc = (0x0A<<0),
    TC_CLKSEL_EVCH3_gc = (0x0B<<0),
    TC_CLKSEL_EVCH4_gc = (0x0C<<0),
    TC_CLKSEL_EVCH5_gc = (0x0D<<0),
    TC_CLKSEL_EVCH6_gc = (0x0E<<0),
    TC_CLKSEL_EVCH7_gc = (0x0F<<0),
} TC_CLKSEL_t;


typedef enum TC_WGMODE_enum
{
    TC_WGMODE_NORMAL_gc = (0x00<<0),
    TC_WGMODE_FRQ_gc = (0x01<<0),
    TC_WGMODE_SS_gc = (0x03<<0),
    TC_WGMODE_DS_T_gc = (0x05<<0),
    TC_WGMODE_DS_TB_gc = (0x06<<0),
    TC_WGMODE_DS_B_gc = (0x07<<0),
} TC_WGMODE_t;


typedef enum TC_EVACT_enum
{
    TC_EVACT_OFF_gc = (0x00<<5),
    TC_EVACT_CAPT_gc = (0x01<<5),
    TC_EVACT_UPDOWN_gc = (0x02<<5),
    TC_EVACT_QDEC_gc = (0x03<<5),
    TC_EVACT_RESTART_gc = (0x04<<5),
    TC_EVACT_FRW_gc = (0x05<<5),
    TC_EVACT_PW_gc = (0x06<<5),
} TC_EVACT_t;


typedef enum TC_EVSEL_enum
{
    TC_EVSEL_OFF_gc = (0x00<<0),
    TC_EVSEL_CH0_gc = (0x08<<0),
    TC_EVSEL_CH1_gc = (0x09<<0),
    TC_EVSEL_CH2_gc = (0x0A<<0),
    TC_EVSEL_CH3_gc = (0x0B<<0),
    TC_EVSEL_CH4_gc = (0x0C<<0),
    TC_EVSEL_CH5_gc = (0x0D<<0),
    TC_EVSEL_CH6_gc = (0x0E<<0),
    TC_EVSEL_CH7_gc = (0x0F<<0),
} TC_EVSEL_t;


typedef enum TC_ERRINTLVL_enum
{
    TC_ERRINTLVL_OFF_gc = (0x00<<2),
    TC_ERRINTLVL_LO_gc = (0x01<<2),
    TC_ERRINTLVL_MED_gc = (0x02<<2),
    TC_ERRINTLVL_HI_gc = (0x03<<2),
} TC_ERRINTLVL_t;


typedef enum TC_OVFINTLVL_enum
{
    TC_OVFINTLVL_OFF_gc = (0x00<<0),
    TC_OVFINTLVL_LO_gc = (0x01<<0),
    TC_OVFINTLVL_MED_gc = (0x02<<0),
    TC_OVFINTLVL_HI_gc = (0x03<<0),
} TC_OVFINTLVL_t;


typedef enum TC_CCDINTLVL_enum
{
    TC_CCDINTLVL_OFF_gc = (0x00<<6),
    TC_CCDINTLVL_LO_gc = (0x01<<6),
    TC_CCDINTLVL_MED_gc = (0x02<<6),
    TC_CCDINTLVL_HI_gc = (0x03<<6),
} TC_CCDINTLVL_t;


typedef enum TC_CCCINTLVL_enum
{
    TC_CCCINTLVL_OFF_gc = (0x00<<4),
    TC_CCCINTLVL_LO_gc = (0x01<<4),
    TC_CCCINTLVL_MED_gc = (0x02<<4),
    TC_CCCINTLVL_HI_gc = (0x03<<4),
} TC_CCCINTLVL_t;


typedef enum TC_CCBINTLVL_enum
{
    TC_CCBINTLVL_OFF_gc = (0x00<<2),
    TC_CCBINTLVL_LO_gc = (0x01<<2),
    TC_CCBINTLVL_MED_gc = (0x02<<2),
    TC_CCBINTLVL_HI_gc = (0x03<<2),
} TC_CCBINTLVL_t;


typedef enum TC_CCAINTLVL_enum
{
    TC_CCAINTLVL_OFF_gc = (0x00<<0),
    TC_CCAINTLVL_LO_gc = (0x01<<0),
    TC_CCAINTLVL_MED_gc = (0x02<<0),
    TC_CCAINTLVL_HI_gc = (0x03<<0),
} TC_CCAINTLVL_t;


typedef enum TC_CMD_enum
{
    TC_CMD_NONE_gc = (0x00<<2),
    TC_CMD_UPDATE_gc = (0x01<<2),
    TC_CMD_RESTART_gc = (0x02<<2),
    TC_CMD_RESET_gc = (0x03<<2),
} TC_CMD_t;


typedef enum AWEX_FDACT_enum
{
    AWEX_FDACT_NONE_gc = (0x00<<0),
    AWEX_FDACT_CLEAROE_gc = (0x01<<0),
    AWEX_FDACT_CLEARDIR_gc = (0x03<<0),
} AWEX_FDACT_t;


typedef enum HIRES_HREN_enum
{
    HIRES_HREN_NONE_gc = (0x00<<0),
    HIRES_HREN_TC0_gc = (0x01<<0),
    HIRES_HREN_TC1_gc = (0x02<<0),
    HIRES_HREN_BOTH_gc = (0x03<<0),
} HIRES_HREN_t;
# 2384 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct USART_struct
{
    register8_t DATA;
    register8_t STATUS;
    register8_t reserved_0x02;
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t BAUDCTRLA;
    register8_t BAUDCTRLB;
} USART_t;


typedef enum USART_RXCINTLVL_enum
{
    USART_RXCINTLVL_OFF_gc = (0x00<<4),
    USART_RXCINTLVL_LO_gc = (0x01<<4),
    USART_RXCINTLVL_MED_gc = (0x02<<4),
    USART_RXCINTLVL_HI_gc = (0x03<<4),
} USART_RXCINTLVL_t;


typedef enum USART_TXCINTLVL_enum
{
    USART_TXCINTLVL_OFF_gc = (0x00<<2),
    USART_TXCINTLVL_LO_gc = (0x01<<2),
    USART_TXCINTLVL_MED_gc = (0x02<<2),
    USART_TXCINTLVL_HI_gc = (0x03<<2),
} USART_TXCINTLVL_t;


typedef enum USART_DREINTLVL_enum
{
    USART_DREINTLVL_OFF_gc = (0x00<<0),
    USART_DREINTLVL_LO_gc = (0x01<<0),
    USART_DREINTLVL_MED_gc = (0x02<<0),
    USART_DREINTLVL_HI_gc = (0x03<<0),
} USART_DREINTLVL_t;


typedef enum USART_CHSIZE_enum
{
    USART_CHSIZE_5BIT_gc = (0x00<<0),
    USART_CHSIZE_6BIT_gc = (0x01<<0),
    USART_CHSIZE_7BIT_gc = (0x02<<0),
    USART_CHSIZE_8BIT_gc = (0x03<<0),
    USART_CHSIZE_9BIT_gc = (0x07<<0),
} USART_CHSIZE_t;


typedef enum USART_CMODE_enum
{
    USART_CMODE_ASYNCHRONOUS_gc = (0x00<<6),
    USART_CMODE_SYNCHRONOUS_gc = (0x01<<6),
    USART_CMODE_IRDA_gc = (0x02<<6),
    USART_CMODE_MSPI_gc = (0x03<<6),
} USART_CMODE_t;


typedef enum USART_PMODE_enum
{
    USART_PMODE_DISABLED_gc = (0x00<<4),
    USART_PMODE_EVEN_gc = (0x02<<4),
    USART_PMODE_ODD_gc = (0x03<<4),
} USART_PMODE_t;
# 2458 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct SPI_struct
{
    register8_t CTRL;
    register8_t INTCTRL;
    register8_t STATUS;
    register8_t DATA;
} SPI_t;


typedef enum SPI_MODE_enum
{
    SPI_MODE_0_gc = (0x00<<2),
    SPI_MODE_1_gc = (0x01<<2),
    SPI_MODE_2_gc = (0x02<<2),
    SPI_MODE_3_gc = (0x03<<2),
} SPI_MODE_t;


typedef enum SPI_PRESCALER_enum
{
    SPI_PRESCALER_DIV4_gc = (0x00<<0),
    SPI_PRESCALER_DIV16_gc = (0x01<<0),
    SPI_PRESCALER_DIV64_gc = (0x02<<0),
    SPI_PRESCALER_DIV128_gc = (0x03<<0),
} SPI_PRESCALER_t;


typedef enum SPI_INTLVL_enum
{
    SPI_INTLVL_OFF_gc = (0x00<<0),
    SPI_INTLVL_LO_gc = (0x01<<0),
    SPI_INTLVL_MED_gc = (0x02<<0),
    SPI_INTLVL_HI_gc = (0x03<<0),
} SPI_INTLVL_t;
# 2501 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct IRCOM_struct
{
    register8_t CTRL;
    register8_t TXPLCTRL;
    register8_t RXPLCTRL;
} IRCOM_t;


typedef enum IRDA_EVSEL_enum
{
    IRDA_EVSEL_OFF_gc = (0x00<<0),
    IRDA_EVSEL_0_gc = (0x08<<0),
    IRDA_EVSEL_1_gc = (0x09<<0),
    IRDA_EVSEL_2_gc = (0x0A<<0),
    IRDA_EVSEL_3_gc = (0x0B<<0),
    IRDA_EVSEL_4_gc = (0x0C<<0),
    IRDA_EVSEL_5_gc = (0x0D<<0),
    IRDA_EVSEL_6_gc = (0x0E<<0),
    IRDA_EVSEL_7_gc = (0x0F<<0),
} IRDA_EVSEL_t;
# 2530 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/iox32a4.h" 3
typedef struct AES_struct
{
    register8_t CTRL;
    register8_t STATUS;
    register8_t STATE;
    register8_t KEY;
    register8_t INTCTRL;
} AES_t;


typedef enum AES_INTLVL_enum
{
    AES_INTLVL_OFF_gc = (0x00<<0),
    AES_INTLVL_LO_gc = (0x01<<0),
    AES_INTLVL_MED_gc = (0x02<<0),
    AES_INTLVL_HI_gc = (0x03<<0),
} AES_INTLVL_t;
# 377 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/io.h" 2 3
# 426 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/io.h" 3
# 1 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/portpins.h" 1 3
# 427 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/io.h" 2 3

# 1 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/common.h" 1 3
# 429 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/io.h" 2 3

# 1 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/version.h" 1 3
# 431 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/io.h" 2 3


# 1 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/fuse.h" 1 3
# 231 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/fuse.h" 3
typedef struct
{
    unsigned char byte[6];
} __fuse_t;
# 434 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/io.h" 2 3


# 1 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/lock.h" 1 3
# 437 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/io.h" 2 3
# 21 "FabOS.h" 2
# 1 "c:\\programme\\atmel\\avr tools\\avr toolchain\\bin\\../lib/gcc/avr/4.4.3/../../../../avr/include/avr/interrupt.h" 1 3
# 22 "FabOS.h" 2
# 1 "../FabOS_config.h" 1
# 23 "FabOS.h" 2
# 37 "FabOS.h"
typedef struct OS_Alarm_tag {
 uint8_t TaskID;
 uint16_t AlarmTicks;
} OS_Alarm_t;


typedef struct FabOS_tag
{

 volatile uint32_t OSTicks;

 volatile uint8_t EventMask[3] ;
 volatile uint8_t EventWaiting[3];

 volatile uint8_t MutexOwnedByTask[4] ;
 volatile uint8_t MutexTaskWaiting[3 +1] ;

 volatile uint8_t CurrTask;
 volatile uint8_t TaskReadyBits ;
 volatile uint16_t Stacks[3 +1];
 volatile OS_Alarm_t Alarms[6];




} FabOS_t;

typedef struct OS_Queue_tag {
   uint8_t read;
   uint8_t write;
   uint8_t chunk;
   uint8_t size;
   uint8_t* data;
 } OS_Queue_t;

extern FabOS_t MyOS;
# 94 "FabOS.h"
void OS_CustomISRCode();

void OS_StartExecution();

void OS_SetEvent(uint8_t TaskID, uint8_t EventMask);

uint8_t OS_WaitEvent(uint8_t EventMask);

void OS_MutexGet(int8_t mutexID);


void OS_MutexRelease(int8_t mutexID);

void OS_SetAlarm(uint8_t AlarmID, uint16_t numTicks );

void OS_WaitAlarm(uint8_t AlarmID);

uint8_t OS_QueueIn(OS_Queue_t* pQueue , uint8_t *pData);

uint8_t OS_QueueOut(OS_Queue_t* pQueue, uint8_t *pData);


 void OS_ErrorHook(uint8_t);
# 125 "FabOS.h"
void OS_GetTicks(uint32_t* pTime);
# 136 "FabOS.h"
uint8_t OS_WaitEventTimeout(uint8_t EventMask, uint8_t AlarmID, uint16_t numTicks );
# 147 "FabOS.h"
void __vector_22 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_22 (void)__attribute__ ((naked,signal));

void OS_TaskCreateInt( void (*t)(), uint8_t taskNum, uint8_t *stack, uint16_t stackSize ) ;

void OS_Reschedule(void)__attribute__ ((naked));

int8_t OS_GetNextTaskNumber();

void OS_Int_ProcessAlarms(void);
# 17 "FabOS.c" 2
# 1 "../FabOS_config.h" 1
# 18 "FabOS.c" 2

FabOS_t MyOS;


 uint8_t OS_Tracebuffer[100];

  uint8_t OS_TraceIdx;






extern unsigned char __heap_start;






void __vector_22 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_22 (void)
{

 asm volatile( "push	r0						\n\t" "in		r0, __SREG__			\n\t" "cli							\n\t" "push	r0						\n\t" "push	r1						\n\t" "clr	r1						\n\t" "push	r2						\n\t" "push	r3						\n\t" "push	r4						\n\t" "push	r5						\n\t" "push	r6						\n\t" "push	r7						\n\t" "push	r8						\n\t" "push	r9						\n\t" "push	r10						\n\t" "push	r11						\n\t" "push	r12						\n\t" "push	r13						\n\t" "push	r14						\n\t" "push	r15						\n\t" "push	r16						\n\t" "push	r17						\n\t" "push	r18						\n\t" "push	r19						\n\t" "push	r20						\n\t" "push	r21						\n\t" "push	r22						\n\t" "push	r23						\n\t" "push	r24						\n\t" "push	r25						\n\t" "push	r26						\n\t" "push	r27						\n\t" "push	r28						\n\t" "push	r29						\n\t" "push	r30						\n\t" "push	r31						\n\t" ); ;
 MyOS.Stacks[MyOS.CurrTask] = (*(volatile uint16_t *)(0x3D)) ;

 OS_Tracebuffer[OS_TraceIdx++] = 1 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 OS_CustomISRCode();

 OS_Tracebuffer[OS_TraceIdx++] = 2 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;


 MyOS.OSTicks++;


 OS_Int_ProcessAlarms();
 OS_Tracebuffer[OS_TraceIdx++] = 3 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;


 MyOS.CurrTask = OS_GetNextTaskNumber() ;

 (*(volatile uint16_t *)(0x3D)) = MyOS.Stacks[MyOS.CurrTask] ;
 asm volatile( "pop	r31						\n\t" "pop	r30						\n\t" "pop	r29						\n\t" "pop	r28						\n\t" "pop	r27						\n\t" "pop	r26						\n\t" "pop	r25						\n\t" "pop	r24						\n\t" "pop	r23						\n\t" "pop	r22						\n\t" "pop	r21						\n\t" "pop	r20						\n\t" "pop	r19						\n\t" "pop	r18						\n\t" "pop	r17						\n\t" "pop	r16						\n\t" "pop	r15						\n\t" "pop	r14						\n\t" "pop	r13						\n\t" "pop	r12						\n\t" "pop	r11						\n\t" "pop	r10						\n\t" "pop	r9						\n\t" "pop	r8						\n\t" "pop	r7						\n\t" "pop	r6						\n\t" "pop	r5						\n\t" "pop	r4						\n\t" "pop	r3						\n\t" "pop	r2						\n\t" "pop	r1						\n\t" "pop	r0						\n\t" "out	__SREG__, r0			\n\t" "pop	r0						\n\t" ); ;

 asm volatile("reti");
}



void OS_Int_ProcessAlarms(void)
{
 uint8_t alarmID;
 OS_Tracebuffer[OS_TraceIdx++] = 4 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;


 for(alarmID=0; alarmID < 6; alarmID++ )
 {
  if( MyOS.Alarms[alarmID].AlarmTicks > 0 )
  {
   OS_Tracebuffer[OS_TraceIdx++] = 5 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
   MyOS.Alarms[alarmID].AlarmTicks--;
   if( MyOS.Alarms[alarmID].AlarmTicks == 0 )
   {
    OS_Tracebuffer[OS_TraceIdx++] = 6 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
    MyOS.TaskReadyBits |= 1<<(MyOS.Alarms[alarmID].TaskID) ;
   }
  }
 }
}

void OS_Reschedule(void)
{
 __asm__ __volatile__ ("cli" ::: "memory");;


 asm volatile( "push	r0						\n\t" "in		r0, __SREG__			\n\t" "cli							\n\t" "push	r0						\n\t" "push	r1						\n\t" "clr	r1						\n\t" "push	r2						\n\t" "push	r3						\n\t" "push	r4						\n\t" "push	r5						\n\t" "push	r6						\n\t" "push	r7						\n\t" "push	r8						\n\t" "push	r9						\n\t" "push	r10						\n\t" "push	r11						\n\t" "push	r12						\n\t" "push	r13						\n\t" "push	r14						\n\t" "push	r15						\n\t" "push	r16						\n\t" "push	r17						\n\t" "push	r18						\n\t" "push	r19						\n\t" "push	r20						\n\t" "push	r21						\n\t" "push	r22						\n\t" "push	r23						\n\t" "push	r24						\n\t" "push	r25						\n\t" "push	r26						\n\t" "push	r27						\n\t" "push	r28						\n\t" "push	r29						\n\t" "push	r30						\n\t" "push	r31						\n\t" ); ;
 MyOS.Stacks[MyOS.CurrTask] = (*(volatile uint16_t *)(0x3D)) ;

 OS_Tracebuffer[OS_TraceIdx++] = 7 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;


 MyOS.CurrTask = OS_GetNextTaskNumber();

 OS_Tracebuffer[OS_TraceIdx++] = 8 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;

 (*(volatile uint16_t *)(0x3D)) = MyOS.Stacks[MyOS.CurrTask] ;
 asm volatile( "pop	r31						\n\t" "pop	r30						\n\t" "pop	r29						\n\t" "pop	r28						\n\t" "pop	r27						\n\t" "pop	r26						\n\t" "pop	r25						\n\t" "pop	r24						\n\t" "pop	r23						\n\t" "pop	r22						\n\t" "pop	r21						\n\t" "pop	r20						\n\t" "pop	r19						\n\t" "pop	r18						\n\t" "pop	r17						\n\t" "pop	r16						\n\t" "pop	r15						\n\t" "pop	r14						\n\t" "pop	r13						\n\t" "pop	r12						\n\t" "pop	r11						\n\t" "pop	r10						\n\t" "pop	r9						\n\t" "pop	r8						\n\t" "pop	r7						\n\t" "pop	r6						\n\t" "pop	r5						\n\t" "pop	r4						\n\t" "pop	r3						\n\t" "pop	r2						\n\t" "pop	r1						\n\t" "pop	r0						\n\t" "out	__SREG__, r0			\n\t" "pop	r0						\n\t" ); ;


 __asm__ __volatile__ ("sei" ::: "memory");;
 asm volatile("ret");
}

int8_t OS_GetNextTaskNumber()
{
 uint8_t Task;
 uint8_t next= 3;

 uint8_t ReadyMask = MyOS.TaskReadyBits;

 OS_Tracebuffer[OS_TraceIdx++] = 9 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;

 for (Task=0;Task<3;Task++)
 {
  if (ReadyMask & 0x01)
  {
   OS_Tracebuffer[OS_TraceIdx++] = 10 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
   next = Task;
   break;
  }
  else
  {
   OS_Tracebuffer[OS_TraceIdx++] = 11 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
   ReadyMask= (ReadyMask>>1);
  }
 }




 if (MyOS.MutexTaskWaiting[next] != 0xff)
 {
  OS_Tracebuffer[OS_TraceIdx++] = 12 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
  if(MyOS.MutexOwnedByTask[MyOS.MutexTaskWaiting[next]]!=0xff)
  {
   OS_Tracebuffer[OS_TraceIdx++] = 13 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;

   next = MyOS.MutexOwnedByTask[MyOS.MutexTaskWaiting[next]];


   if(((1<<next)&MyOS.TaskReadyBits) == 0)
   {
    OS_Tracebuffer[OS_TraceIdx++] = 14 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
    next = 3;
   }
  }
 }
 return next;
}


void OS_TaskCreateInt( void (*t)(), uint8_t TaskID, uint8_t *stack, uint16_t stackSize )
{
 uint16_t z ;
 OS_Tracebuffer[OS_TraceIdx++] = 15 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
# 174 "FabOS.c"
 for (z=stackSize-35;z<stackSize;z++)
 {
  stack[z] = 0;
 }

 MyOS.TaskReadyBits |= 1<<TaskID ;

 MyOS.Stacks[TaskID] = (uint16_t)stack + stackSize - 1 ;
# 191 "FabOS.c"
 *(uint8_t*)(MyOS.Stacks[TaskID]-1) = ((uint16_t)(t)) >> 8;
 *(uint8_t*)(MyOS.Stacks[TaskID]) = ((uint16_t)(t)) & 0x00ff;
 MyOS.Stacks[TaskID] -= 35;



}



void OS_StartExecution()
{
 uint8_t i;
 OS_Tracebuffer[OS_TraceIdx++] = 16 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 for(i=0; i < 3 +1; i++ )
 {
  MyOS.MutexTaskWaiting[i] = 0xff;
 }
 for(i=0; i < 4; i++ )
 {
  MyOS.MutexOwnedByTask[i] = 0xff;
 }
# 225 "FabOS.c"
 MyOS.OSTicks = 0L;



 MyOS.CurrTask = 3;
 OS_Reschedule();
 __asm__ __volatile__ ("sei" ::: "memory");
}





void OS_MutexGet(int8_t mutexID)
{

 if(mutexID >= 4)
 {
  OS_ErrorHook(5);
  return;
 }

 __asm__ __volatile__ ("cli" ::: "memory");;
 OS_Tracebuffer[OS_TraceIdx++] = 17 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 while( MyOS.MutexOwnedByTask[mutexID] != 0xff)
 {
  OS_Tracebuffer[OS_TraceIdx++] = 18 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
  MyOS.MutexTaskWaiting[MyOS.CurrTask] = mutexID;
  OS_Reschedule();
  __asm__ __volatile__ ("cli" ::: "memory");;
  OS_Tracebuffer[OS_TraceIdx++] = 19 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;

  MyOS.MutexTaskWaiting[MyOS.CurrTask] = 0xff;
 }
 OS_Tracebuffer[OS_TraceIdx++] = 20 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 MyOS.MutexOwnedByTask[mutexID] = MyOS.CurrTask;
 __asm__ __volatile__ ("sei" ::: "memory");;
}


void OS_MutexRelease(int8_t mutexID)
{

 if(mutexID >= 4)
 {
  OS_ErrorHook(6);
  return;
 }

 __asm__ __volatile__ ("cli" ::: "memory");;
 OS_Tracebuffer[OS_TraceIdx++] = 21 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 MyOS.MutexOwnedByTask[mutexID] = 0xff;
 OS_Reschedule() ;
}



void OS_SetEvent(uint8_t TaskID, uint8_t EventMask)
{
 __asm__ __volatile__ ("cli" ::: "memory");;
 OS_Tracebuffer[OS_TraceIdx++] = 22 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 MyOS.EventMask[TaskID] |= EventMask;

 if(EventMask & MyOS.EventWaiting[TaskID])
 {
  OS_Tracebuffer[OS_TraceIdx++] = 23 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;

  MyOS.TaskReadyBits |= 1<<TaskID ;



  OS_Reschedule() ;
 }
 else
 {
  OS_Tracebuffer[OS_TraceIdx++] = 24 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;

  __asm__ __volatile__ ("sei" ::: "memory");;
 }
}

uint8_t OS_WaitEvent(uint8_t EventMask)
{

 if(MyOS.CurrTask == 3)
 {
  OS_ErrorHook(2);
  return 0;
 }


 uint8_t ret;
 __asm__ __volatile__ ("cli" ::: "memory");;
 OS_Tracebuffer[OS_TraceIdx++] = 25 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;

 if(!(EventMask & MyOS.EventMask[MyOS.CurrTask]))
 {
  OS_Tracebuffer[OS_TraceIdx++] = 26 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
  MyOS.EventWaiting[MyOS.CurrTask] = EventMask;

  MyOS.TaskReadyBits &= ~(1<<MyOS.CurrTask) ;

  OS_Reschedule() ;
  __asm__ __volatile__ ("cli" ::: "memory");;
  OS_Tracebuffer[OS_TraceIdx++] = 27 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;

  MyOS.EventWaiting[MyOS.CurrTask] = 0;
 }
 ret = MyOS.EventMask[MyOS.CurrTask] & EventMask;

 MyOS.EventMask[MyOS.CurrTask] &= ~EventMask;
 OS_Tracebuffer[OS_TraceIdx++] = 28 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 __asm__ __volatile__ ("sei" ::: "memory");;
 return ret;
}





void OS_SetAlarm(uint8_t AlarmID, uint16_t numTicks )
{

 if(AlarmID >= 6)
 {
  OS_ErrorHook(7);
  return;
 }

 __asm__ __volatile__ ("cli" ::: "memory");;
 OS_Tracebuffer[OS_TraceIdx++] = 29 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 MyOS.Alarms[AlarmID].AlarmTicks = numTicks ;
 __asm__ __volatile__ ("sei" ::: "memory");;
}

void OS_WaitAlarm(uint8_t AlarmID)
{

 if(AlarmID >= 6)
 {
  OS_ErrorHook(7);
  return;
 }
 if(MyOS.CurrTask == 3)
 {
  OS_ErrorHook(4);
  return;
 }
 if(MyOS.Alarms[AlarmID].TaskID != MyOS.CurrTask)
 {
  OS_Tracebuffer[OS_TraceIdx++] = 30 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
  OS_ErrorHook(9);
  return;
 }


 __asm__ __volatile__ ("cli" ::: "memory");;
 OS_Tracebuffer[OS_TraceIdx++] = 31 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 if(MyOS.Alarms[AlarmID].AlarmTicks == 0 )
 {

  OS_ErrorHook(8);

  __asm__ __volatile__ ("sei" ::: "memory");;
 }
 else
 {
  OS_Tracebuffer[OS_TraceIdx++] = 32 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
  MyOS.TaskReadyBits &= ~(1<<MyOS.CurrTask) ;
  OS_Reschedule();
 }
}
# 409 "FabOS.c"
uint8_t OS_QueueIn(OS_Queue_t* pQueue , uint8_t* pByte)
{
 uint8_t i;
 __asm__ __volatile__ ("cli" ::: "memory");;
 OS_Tracebuffer[OS_TraceIdx++] = 33 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 if (pQueue->write + pQueue->chunk == pQueue->read || (pQueue->read == 0 && pQueue->write + pQueue->chunk == pQueue->size))
 {
  OS_Tracebuffer[OS_TraceIdx++] = 34 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
  __asm__ __volatile__ ("sei" ::: "memory");;
  return 1;
 }

 for(i=0;i<pQueue->chunk;i++)
 {
  pQueue->data[pQueue->write] = *pByte++;
  pQueue->write = pQueue->write + 1;
  if (pQueue->write >= pQueue->size)
   pQueue->write = 0;
 }
 OS_Tracebuffer[OS_TraceIdx++] = 35 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 __asm__ __volatile__ ("sei" ::: "memory");;
 return 0;
}

uint8_t OS_QueueOut(OS_Queue_t* pQueue , uint8_t* pByte)
{
 uint8_t i;
 __asm__ __volatile__ ("cli" ::: "memory");;
 OS_Tracebuffer[OS_TraceIdx++] = 36 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 if (pQueue->read == pQueue->write)
 {
  OS_Tracebuffer[OS_TraceIdx++] = 37 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
  __asm__ __volatile__ ("sei" ::: "memory");;
  return 1;
 }

 for(i=0;i<pQueue->chunk;i++)
 {
  *pByte++ = pQueue->data[pQueue->read];
  pQueue->read = pQueue->read + 1;
  if (pQueue->read >= pQueue->size)
   pQueue->read = 0;
 }
 OS_Tracebuffer[OS_TraceIdx++] = 38 ; if(OS_TraceIdx >= sizeof(OS_Tracebuffer)) OS_TraceIdx = 0;;
 __asm__ __volatile__ ("sei" ::: "memory");;
 return 0;
}
# 491 "FabOS.c"
void OS_GetTicks(uint32_t* pTime)
{
 __asm__ __volatile__ ("cli" ::: "memory");;
  *pTime = MyOS.OSTicks;
 __asm__ __volatile__ ("sei" ::: "memory");;
}




uint8_t OS_WaitEventTimeout(uint8_t EventMask, uint8_t AlarmID, uint16_t numTicks )
{
 uint8_t ret;
 OS_SetAlarm(AlarmID,numTicks);
 ret = OS_WaitEvent(EventMask);
 if(ret & EventMask)
 {

  OS_SetAlarm(AlarmID,0);
  return ret;
 }
 else
 {

  return ret;
 }
}
