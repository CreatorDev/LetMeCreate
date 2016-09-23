#define SWITCH_INTERVAL 8 * 1000
#define STR_MAX 10

/* Init the alphanum clicker */
int alphanum_init(uint8_t);

/* Converts a char into a 14 segment display value */
uint16_t alphanum_get_char(char);

/* Write the two chars to */
int alphanum_write(char, char);

/* Raw write  */
int alphanum_raw_write(uint16_t, uint16_t);

/* This peridically switch between segment a and b to keep the illusion of a 
 * simultanous display of both values
 */
void alphanum_switch_cycles(int);

/* Write a text with more than two characters */
void alphanum_write_running_text(const char *, int);

/*
 * Translation table between a character and a 14 segment display value.
 */
const uint16_t alphanum_char_table[51] = {
 //-------------------
 // Hex			Symbol 
 //-------------------
	0x0003,		// '-' 
	0x0040,		// '.' 
	0x7621,		// '/' 
	0x3F00,		// '0' 
	0x0600,		// '1' 
	0x1B03,		// '2' 
	0x0F03,		// '3' 
	0x6013,		// '4' 
	0x2D03,		// '5' 
	0x3D03,		// '6' 
	0x0700,		// '7' 
	0x3F03,		// '8' 
	0x2F03,		// '9' 
	0x4010,		// ':' 
	0x4008,		// ';' 
	0x8004,		// '<' 
	0x7607,		// '=' 
	0x0028,		// '>' 
	0x2312,		// '?' 
	0x0000,		// '@' 
	0x3703,		// 'A' 
	0x4F12,		// 'B' 
	0x3900,		// 'C' 
	0x4F01,		// 'D' 
	0x3903,		// 'E' 
	0x0133,		// 'F' 
	0x3D02,		// 'G' 
	0x3603,		// 'H' 
	0x4010,		// 'I' 
	0x1E00,		// 'J' 
	0x0B05,		// 'K' 
	0x3800,		// 'L' 
	0xB620,		// 'M' 
	0x3624,		// 'N' 
	0x3F00,		// 'O' 
	0x3303,		// 'P' 
	0x3F44,		// 'Q' 
	0x3307,		// 'R' 
	0x2D03,		// 'S' 
	0x6310,		// 'T' 
	0x3E00,		// 'U' 
	0xB008,		// 'V' 
	0x360C,		// 'W' 
	0x802C,		// 'X' 
	0x8030,		// 'Y' 
	0x8908,		// 'Z' 
	0x3900,		// '[' 
	0x8008,		// '\' 
	0x0F00,		// ']' 
	0x8100,		// '^' 
	0x0800,		// '_' 
};

