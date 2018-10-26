#ifndef _DELAYNANO_h
#define _DELAYNANO_h

#define delay1tick __asm__("nop\n\t");
#define delay2tick __asm__("nop\n\t""nop\n\t");
#define delay3tick __asm__("nop\n\t""nop\n\t""nop\n\t");
#define delay4tick __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
#define delay5tick __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
#define delay6tick __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
#define delay7tick __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
#define delay8tick __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");

// delay 0.0625 us == 62.5 ns == 1 tick(16MHz)
#define delayns062  delay1tick
// delay 0.1250 us == 125 ns  == 2 tick(16MHz)
#define delayns125  delay2tick
// delay 0.2500 us == 250 ns  == 4 tick(16MHz)
#define delayns250  delay4tick
// delay 0.3750 us == 375 ns  == 6 tick(16MHz)
#define delayns375  delay6tick
// delay 0.5000 us == 500 ns  == 8 tick(16MHz)
#define delayns500  delay8tick
// delay 0.6250 us == 625 ns  == 10 tick(16MHz)
#define delayns625  delayns500 delayns125
// delay 0.7500 us == 750 ns  == 12 tick(16MHz)
#define delayns750  delayns500 delayns250
// delay 0.8750 us == 875 ns  == 14 tick(16MHz)
#define delayns875  delayns500 delayns375
// delay 1.0000 us == 1000 ns == 16 tick(16MHz)
#define delayns1000 delayns500 delayns500

#endif