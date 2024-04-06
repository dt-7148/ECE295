#include "Si5351.h"

static void write(byte addr, byte data);
static byte read(byte addr);
static void enableSpreadSpectrum(bool enabled);
static void write_bulk(byte addr, byte* data, int len);

uint32_t lastRdivValue[3];

/* 
 * write procedure for PLL device
 * works very similar to encoder procedure
 * refer to encoder procedure to see how it works
 *
 */

void write(byte addr, byte data)
{
    twi_start();
    twi_MT_SLA_W(SI5351_ADDR); 
    twi_MT_write(addr);
    twi_MT_write(data);
    twi_stop();
}

/*
 * write procedure but multiple data points
 */

void write_bulk(byte addr, byte* data, int len)
{
    twi_start();
    twi_MT_SLA_W(SI5351_ADDR);
    twi_MT_write(addr);
    for(int i = 0; i < len; ++i)
        twi_MT_write(data[i]);

    twi_stop();
}

/*
 * set phase by writing 0 and mult (previously set to guarantee 90 deg)
 * to the two different clocks
 * 
 */

void set_phase(word mult) 
{
    mult &= 0b1111111;
    write(SI5351_REGISTER_165_CLK0_INITIAL_PHASE_OFFSET, 0);
    write(SI5351_REGISTER_166_CLK1_INITIAL_PHASE_OFFSET, mult);
    write(SI5351_REGISTER_177_PLL_RESET, (1 << 7) | (1 << 5));
}

/*
 * read procedure for PLL
 * very similar to encoder, see reference
 *
 */

byte read(byte addr)
{
    volatile byte ret;
    volatile byte ret_MT_write;
    volatile byte ret_SLA_R;
    twi_start();
    ret = twi_MT_SLA_W(SI5351_ADDR);
	if(ret != 0x18){
		return 0xFF;
	}
    ret_MT_write = twi_MT_write(addr);
	if(ret_MT_write != 0x28){
		return 0xFF;
	}
    twi_repeat_start();
    ret_SLA_R = twi_MR_SLA_R(SI5351_ADDR);	
	if(ret_SLA_R != 0x40){
		return 0xFF;
	}
    ret = twi_MR_read_NACK();
    twi_stop();
    return ret;
}

/*
 * Code to turn on/off spectrum feature. Not used in the project
 */
void enableSpreadSpectrum(bool enabled) {
    byte regval = read(SI5351_REGISTER_149_SPREAD_SPECTRUM_PARAMETERS);
    if(enabled) 
        regval |= 0x80;
    else 
        regval &= ~0x80;

    write(SI5351_REGISTER_149_SPREAD_SPECTRUM_PARAMETERS, regval);
}

/* 
 * Initialise the device in our project
 * Reset all the internal clocks 
 */ 
void si5351_init()
{
    for(int i = 0; i < 3; ++i) lastRdivValue[i] = 0;
    byte status = 0;
    /* wait for device to start */
    do {
	    status = read(SI5351_REGISTER_0_DEVICE_STATUS);
	    //status = read(SI5351_REGISTER_1_INTERRUPT_STATUS_STICKY);
		_delay_ms(1);
    } while (status >> 7 == 1);

    write(SI5351_REGISTER_183_CRYSTAL_INTERNAL_LOAD_CAPACITANCE, 
        (SI5351_CRYSTAL_LOAD_10PF & SI5351_CRYSTAL_LOAD_MASK) | 0b00010010);

    write(SI5351_REGISTER_16_CLK0_CONTROL, 0x80); 
    write(SI5351_REGISTER_17_CLK1_CONTROL, 0x80); 
    write(SI5351_REGISTER_18_CLK2_CONTROL, 0x80);
    write(SI5351_REGISTER_19_CLK3_CONTROL, 0x80);
    write(SI5351_REGISTER_20_CLK4_CONTROL, 0x80);
    write(SI5351_REGISTER_21_CLK5_CONTROL, 0x80);
    write(SI5351_REGISTER_22_CLK6_CONTROL, 0x80);
    write(SI5351_REGISTER_23_CLK7_CONTROL, 0x80);

    write(16, 0x0c);
	write(17, 0x0c);
	write(18, 0x0c);
	write(19, 0x0c);
	write(20, 0x0c);
	write(21, 0x0c);
	write(22, 0x0c);
	write(23, 0x0c);

    enableSpreadSpectrum(false);  
}

/*
 * Turn off/on the clocks
 */

void enable_clocks(bool enabled)
{
    write(SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL, enabled ? 0x00 : 0xFF);
}

/*
 * Setup reference frequency (fvco) based on the XTAL crystal frequency (25MHz)
 * Code taken and slightly modified from adafruit si5351 library
 * 
 */

void setup_PLL(plldev_t pll, byte mult, uint32_t num, uint32_t denom)
{
    
    if( mult < 15 || mult > 90 ) return;    // multiple not supported
    if( (denom == 0) || (denom > 0xFFFFF) ) return;
    if( num > 0xFFFFF) return;

    uint32_t P1, P2, P3;
	

	// TODO: calculate P1, P2, and P3 and write to memory
	P1 = 128 * 28 + floor(128 * 0 / 1) - 512;
    P2 = 128 * 0 - 1 * floor(128 * 0 / 1);
    P3 = 1;
	
	/*
		P1 = 128 * 28 + floor(128 * 0 / 1) - 512;
		P2 = 128 * num - denom * floor(128 * 0 / 1);
		P3 = 1;
	*/

    write(SI5351_PLLA_ADDR + 0, (P3 & 0b1111111100000000) >> 8);
    write(SI5351_PLLA_ADDR + 1, P3 & 0b0000000011111111);
    write(SI5351_PLLA_ADDR + 2, (P1 & 0b110000000000000000) >> 16);
    write(SI5351_PLLA_ADDR + 3, (P1 & 0b1111111100000000) >> 8);
    write(SI5351_PLLA_ADDR + 4, P1 & 0b0000000011111111);
    write(SI5351_PLLA_ADDR + 5, (P3 & 0b11110000000000000000) >> 12 | (P2 & 0b11110000000000000000) >> 16);
    write(SI5351_PLLA_ADDR + 6, (P2 & 0b1111111100000000) >> 8);
    write(SI5351_PLLA_ADDR + 7, P2 & 0b0000000011111111);
    
}

/*
 * Set up out clock frequencies
 * Code taken and slightly modified from adafruit si5351 library
 */

void setup_clock(plldev_t pll, byte port, uint32_t div, uint32_t num, uint32_t denom)
{
    uint32_t a;
	a = denom;
	if( port > 2 ) return;
    if( div < 4 || div > 2048 ) return;
    if( (denom == 0) || (denom > 0xFFFFF) ) return;
    if( num > 0xFFFFF) return;

    uint32_t P1, P2, P3;
	uint8_t clkCtrlReg = 0; 
	
	// TODO: calculate P1, P2, and P3 and write to memory
	P1 = 128 * div + floor(128 * num / denom) - 512;
    P2 = 128 * num - denom * floor(128 * num/denom);
    P3 = denom;


	// TODO: Write the correct value for clock control register
    // clock control reg = MS0, MS1, change depending on port
    switch (port) {
        case SI5351_PORT0:
            #define MS0 42

            write(MS0 + 0, (P3 & 0b1111111100000000) >> 8);
            write(MS0 + 1, P3 & 0b0000000011111111);
            write(MS0 + 2, (P1 & 0b110000000000000000) >> 16);
            write(MS0 + 3, (P1 & 0b1111111100000000) >> 8);
            write(MS0 + 4, P1 & 0b0000000011111111);
            write(MS0 + 5, (P3 & 0b11110000000000000000) >> 12 | (P2 & 0b11110000000000000000) >> 16);
            write(MS0 + 6, (P2 & 0b1111111100000000) >> 8);
            write(MS0 + 7, P2 & 0b0000000011111111);
			
			clkCtrlReg = MS0;
            break;
        case SI5351_PORT1:
            #define MS1 50
            write(MS1 + 0, (P3 & 0b1111111100000000) >> 8);
            write(MS1 + 1, P3 & 0b0000000011111111);
            write(MS1 + 2, (P1 & 0b110000000000000000) >> 16);
            write(MS1 + 3, (P1 & 0b1111111100000000) >> 8);
            write(MS1 + 4, P1 & 0b0000000011111111);
            write(MS1 + 5, (P3 & 0b11110000000000000000) >> 12 | (P2 & 0b11110000000000000000) >> 16);
            write(MS1 + 6, (P2 & 0b1111111100000000) >> 8);
            write(MS1 + 7, P2 & 0b0000000011111111);
            
			clkCtrlReg = MS1;
            break;
        default:
            break;
    } 

    switch(port) 
    {
        case SI5351_PORT0: 
            write(SI5351_REGISTER_16_CLK0_CONTROL, clkCtrlReg);
            break;
        case SI5351_PORT1:
            write(SI5351_REGISTER_17_CLK1_CONTROL, clkCtrlReg);
            break;
        case SI5351_PORT2:
            write(SI5351_REGISTER_18_CLK2_CONTROL, clkCtrlReg);
            break;
    }
}

void reset_pll()
{
    write(SI5351_REGISTER_177_PLL_RESET, (1 << 7) | (1 << 5));
}

/* 
 * Functions below are not used for the project 
 */
word choose_rdiv(uint32_t *freq)
{
    uint8_t r_div = SI5351_R_DIV_1;

    // Choose the correct R divider
               
    if(*freq >= 4000 && *freq < 8000) {
        r_div = SI5351_R_DIV_128;
        *freq *= 128;
    } else if(*freq >= 8000  && *freq < 16000) {
        r_div = SI5351_R_DIV_64;
        *freq *= 64;
    } else if(*freq >= 16000 && *freq < 32000) {
        r_div = SI5351_R_DIV_32;
        *freq *= 32;
    } else if(*freq >= 32000 && *freq < 64000) {
        r_div = SI5351_R_DIV_16;
        *freq *= 16;
    } else if(*freq >= 64000 && *freq < 128000) {
        r_div = SI5351_R_DIV_8;
        *freq *= 8;
    } else if(*freq >= 128000 && *freq < 256000) {
        r_div = SI5351_R_DIV_4;
        *freq *= 4;
     } else if(*freq >= 256000 && *freq < 512000) {
        r_div = SI5351_R_DIV_2;
        *freq *= 2;
    }
    return r_div;

}

void setup_rdiv(byte port, byte div)
{
    uint8_t Rreg = SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3, regval;

    switch(port)
    {
        case SI5351_PORT0: 
            Rreg = SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3;
            break;
        case SI5351_PORT1:
            Rreg = SI5351_REGISTER_52_MULTISYNTH1_PARAMETERS_3;
            break;
        case SI5351_PORT2:
            Rreg = SI5351_REGISTER_60_MULTISYNTH2_PARAMETERS_3;
            break;
    }

    regval = read(Rreg);
    regval &= 0x0F;
    uint8_t divider = div;
    divider &= 0x07;
    divider <<= 4;
    regval |= divider;
    lastRdivValue[port] = divider;
    write(Rreg, regval);
}

uint32_t _gcd(uint32_t a, uint32_t b)
{
	return (a == 0) ? b: _gcd(b % a, a);    /* gcd algorithm based on Euclid */
}


void set_LO_freq(uint32_t freq)
{
    /* Set PLL frequency. By default, 700MHz is set for the fvco */
    uint32_t fvco = 700000000, div, num, denom, gcf, rdiv;

	// TODO: Calculate required divider, num and denom for clock
    div = (int)fvco / (int)freq;        // Integer division results in an integer without remainders
    uint32_t mod = fvco % freq;
    gcf = _gcd(mod, freq);

    num = (mod) / gcf;
    denom = (float) freq / gcf;  // ************************************************************

    /* Setup two clock frequencies and 90 degree phase shift */
    setup_clock(SI5351_PLL_A, SI5351_PORT0, div, num, denom);
    setup_clock(SI5351_PLL_A, SI5351_PORT1, div, num, denom);
    
	// TODO: figure out what phase offset to send
	set_phase(90);
}

void A(uint32_t freq)
{
	/* Set PLL frequency. By default, 700MHz is set for the fvco */
	uint32_t fvco = 700000000, div, num, denom, gcf, rdiv;

	// TODO: Calculate required divider, num and denom for clock
	div = fvco / freq;        // Integer division results in an integer without remainders
	uint32_t mod = fvco % freq;
	gcf = _gcd(mod, freq);

	num = (mod) / gcf;
	denom = (float) freq / gcf;  // ************************************************************

	/* Setup two clock frequencies and 90 degree phase shift */
	setup_clock(SI5351_PLL_A, SI5351_PORT0, div, num, denom);
	setup_clock(SI5351_PLL_A, SI5351_PORT1, div, num, denom);
	
	// TODO: figure out what phase offset to send
	set_phase(90);
}