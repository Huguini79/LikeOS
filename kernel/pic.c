#include <ioport.h>

#define MASTER_PIC_CONTROL 0x20
#define SLAVE_PIC_CONTROL 0xA0

#define MASTER_PIC_DATA 0x21
#define SLAVE_PIC_DATA 0xA1

void remapPIC()
{
    /* ICW 1 */
    unsigned char icw1_packet = 0x11;
    outb(MASTER_PIC_CONTROL, icw1_packet);
    outb(SLAVE_PIC_CONTROL, icw1_packet);

    /* ICW 2 */
    unsigned char icw2_packet_master = 0x20;
    unsigned char icw2_packet_slave = 0x28;
    outb(MASTER_PIC_DATA, icw2_packet_master);
    outb(SLAVE_PIC_DATA, icw2_packet_slave);

    /* ICW 3 */
    unsigned char icw3_packet_master = 0x4;
    unsigned char icw3_packet_slave = 0x2;
    outb(MASTER_PIC_DATA, icw3_packet_master);
    outb(SLAVE_PIC_DATA, icw3_packet_slave);

    /* ICW 4 */
    unsigned char icw4_packet = 1;
    outb(MASTER_PIC_DATA, icw4_packet);
    outb(SLAVE_PIC_DATA, icw4_packet);
}