/*******************************************************************************************
  Font name: Arial
  Font width: varialbe (proportional font)
  Font height: 14
  Encode: ANSI+

  Data length: 8 bits
  Data format: Big Endian, Row based, Row preferred, Packed
 *******************************************************************************************/

/*******************************************************************************************
  Data table provides the bitmap data of each character.

  To get the starting data offset of character 'A', you can use the following expression:

     const unsigned char index = index_table['A'];
     const unsigned int offset = offset_table[index];
     const unsigned char *pData = data_table[offset];

 *******************************************************************************************/
const unsigned short Arial8DataLength = 8; // bits
const unsigned short Arial8CharHeight = 14;
const unsigned char Arial8Data_table[] = {

/* character 0x0020 (' '): (width=3, offset=0) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0021 ('!'): (width=2, offset=6) */
0x02, 0xAA, 0x88, 0x00, 

/* character 0x0022 ('"'): (width=4, offset=10) */
0x00, 0x0A, 0xAA, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0023 ('#'): (width=6, offset=17) */
0x00, 0x00, 0x0A, 0x2B, 0xE5, 0x14, 0xFA, 0x8A, 
0x00, 0x00, 0x00, 

/* character 0x0024 ('$'): (width=6, offset=28) */
0x00, 0x00, 0x1C, 0xAA, 0x87, 0x0A, 0x2A, 0xA7, 
0x08, 0x00, 0x00, 

/* character 0x0025 ('%'): (width=10, offset=39) */
0x00, 0x00, 0x00, 0x01, 0x88, 0x94, 0x25, 0x06, 
0x80, 0x2C, 0x14, 0x85, 0x22, 0x30, 0x00, 0x00, 
0x00, 0x00, 

/* character 0x0026 ('&'): (width=7, offset=57) */
0x00, 0x00, 0x01, 0x84, 0x89, 0x0C, 0x28, 0x8D, 
0x11, 0xD0, 0x00, 0x00, 0x00, 

/* character 0x0027 ('''): (width=2, offset=70) */
0x02, 0xA0, 0x00, 0x00, 

/* character 0x0028 ('('): (width=4, offset=74) */
0x00, 0x02, 0x48, 0x88, 0x88, 0x84, 0x20, 

/* character 0x0029 (')'): (width=4, offset=81) */
0x00, 0x08, 0x42, 0x22, 0x22, 0x24, 0x80, 

/* character 0x002A ('*'): (width=4, offset=88) */
0x00, 0x04, 0xE4, 0xA0, 0x00, 0x00, 0x00, 

/* character 0x002B ('+'): (width=6, offset=95) */
0x00, 0x00, 0x00, 0x00, 0x82, 0x3E, 0x20, 0x80, 
0x00, 0x00, 0x00, 

/* character 0x002C (','): (width=3, offset=106) */
0x00, 0x00, 0x00, 0x01, 0x24, 0x00, 

/* character 0x002D ('-'): (width=4, offset=112) */
0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 

/* character 0x002E ('.'): (width=3, offset=119) */
0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 

/* character 0x002F ('/'): (width=3, offset=125) */
0x00, 0x12, 0x92, 0x52, 0x00, 0x00, 

/* character 0x0030 ('0'): (width=6, offset=131) */
0x00, 0x00, 0x1C, 0x8A, 0x28, 0xA2, 0x8A, 0x27, 
0x00, 0x00, 0x00, 

/* character 0x0031 ('1'): (width=6, offset=142) */
0x00, 0x00, 0x08, 0x62, 0x82, 0x08, 0x20, 0x82, 
0x00, 0x00, 0x00, 

/* character 0x0032 ('2'): (width=6, offset=153) */
0x00, 0x00, 0x1C, 0x88, 0x20, 0x84, 0x21, 0x0F, 
0x80, 0x00, 0x00, 

/* character 0x0033 ('3'): (width=6, offset=164) */
0x00, 0x00, 0x1C, 0x88, 0x23, 0x02, 0x0A, 0x27, 
0x00, 0x00, 0x00, 

/* character 0x0034 ('4'): (width=6, offset=175) */
0x00, 0x00, 0x04, 0x31, 0x45, 0x24, 0xF8, 0x41, 
0x00, 0x00, 0x00, 

/* character 0x0035 ('5'): (width=6, offset=186) */
0x00, 0x00, 0x1E, 0x42, 0x0F, 0x02, 0x0A, 0x27, 
0x00, 0x00, 0x00, 

/* character 0x0036 ('6'): (width=6, offset=197) */
0x00, 0x00, 0x1C, 0x8A, 0x0F, 0x22, 0x8A, 0x27, 
0x00, 0x00, 0x00, 

/* character 0x0037 ('7'): (width=6, offset=208) */
0x00, 0x00, 0x3E, 0x10, 0x42, 0x08, 0x41, 0x04, 
0x00, 0x00, 0x00, 

/* character 0x0038 ('8'): (width=6, offset=219) */
0x00, 0x00, 0x1C, 0x8A, 0x27, 0x22, 0x8A, 0x27, 
0x00, 0x00, 0x00, 

/* character 0x0039 ('9'): (width=6, offset=230) */
0x00, 0x00, 0x1C, 0x8A, 0x28, 0x9E, 0x0A, 0x27, 
0x00, 0x00, 0x00, 

/* character 0x003A (':'): (width=3, offset=241) */
0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 

/* character 0x003B (';'): (width=3, offset=247) */
0x00, 0x01, 0x00, 0x02, 0x48, 0x00, 

/* character 0x003C ('<'): (width=6, offset=253) */
0x00, 0x00, 0x00, 0x00, 0x27, 0x20, 0x70, 0x20, 
0x00, 0x00, 0x00, 

/* character 0x003D ('='): (width=6, offset=264) */
0x00, 0x00, 0x00, 0x00, 0x0F, 0x80, 0xF8, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x003E ('>'): (width=6, offset=275) */
0x00, 0x00, 0x00, 0x02, 0x07, 0x02, 0x72, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x003F ('?'): (width=6, offset=286) */
0x00, 0x00, 0x1C, 0x88, 0x21, 0x08, 0x20, 0x02, 
0x00, 0x00, 0x00, 

/* character 0x0040 ('@'): (width=11, offset=297) */
0x00, 0x00, 0x00, 0x00, 0x0F, 0x86, 0x08, 0x9A, 
0xA4, 0xD5, 0x12, 0xA2, 0x54, 0xD2, 0x6C, 0x20, 
0x23, 0xF8, 0x00, 0x00, 

/* character 0x0041 ('A'): (width=8, offset=317) */
0x00, 0x00, 0x00, 0x10, 0x28, 0x28, 0x28, 0x44, 
0x7C, 0x82, 0x82, 0x00, 0x00, 0x00, 

/* character 0x0042 ('B'): (width=7, offset=331) */
0x00, 0x00, 0x07, 0xC8, 0x50, 0xBF, 0x42, 0x85, 
0x0B, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0043 ('C'): (width=7, offset=344) */
0x00, 0x00, 0x01, 0xC4, 0x50, 0x20, 0x40, 0x80, 
0x88, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0044 ('D'): (width=7, offset=357) */
0x00, 0x00, 0x07, 0x88, 0x90, 0xA1, 0x42, 0x85, 
0x13, 0xC0, 0x00, 0x00, 0x00, 

/* character 0x0045 ('E'): (width=6, offset=370) */
0x00, 0x00, 0x3E, 0x82, 0x0F, 0xA0, 0x82, 0x0F, 
0x80, 0x00, 0x00, 

/* character 0x0046 ('F'): (width=6, offset=381) */
0x00, 0x00, 0x3E, 0x82, 0x0F, 0x20, 0x82, 0x08, 
0x00, 0x00, 0x00, 

/* character 0x0047 ('G'): (width=8, offset=392) */
0x00, 0x00, 0x00, 0x38, 0x44, 0x82, 0x80, 0x8E, 
0x82, 0x44, 0x38, 0x00, 0x00, 0x00, 

/* character 0x0048 ('H'): (width=7, offset=406) */
0x00, 0x00, 0x04, 0x28, 0x50, 0xBF, 0x42, 0x85, 
0x0A, 0x10, 0x00, 0x00, 0x00, 

/* character 0x0049 ('I'): (width=2, offset=419) */
0x02, 0xAA, 0xA8, 0x00, 

/* character 0x004A ('J'): (width=5, offset=423) */
0x00, 0x00, 0x21, 0x08, 0x42, 0x94, 0x98, 0x00, 
0x00, 

/* character 0x004B ('K'): (width=7, offset=432) */
0x00, 0x00, 0x04, 0x28, 0x92, 0x2C, 0x68, 0x89, 
0x12, 0x10, 0x00, 0x00, 0x00, 

/* character 0x004C ('L'): (width=6, offset=445) */
0x00, 0x00, 0x20, 0x82, 0x08, 0x20, 0x82, 0x0F, 
0x80, 0x00, 0x00, 

/* character 0x004D ('M'): (width=8, offset=456) */
0x00, 0x00, 0x00, 0x82, 0xC6, 0xC6, 0xAA, 0xAA, 
0xAA, 0x92, 0x92, 0x00, 0x00, 0x00, 

/* character 0x004E ('N'): (width=7, offset=470) */
0x00, 0x00, 0x04, 0x2C, 0x54, 0xA9, 0x4A, 0x95, 
0x1A, 0x10, 0x00, 0x00, 0x00, 

/* character 0x004F ('O'): (width=8, offset=483) */
0x00, 0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 
0x82, 0x44, 0x38, 0x00, 0x00, 0x00, 

/* character 0x0050 ('P'): (width=6, offset=497) */
0x00, 0x00, 0x3C, 0x8A, 0x28, 0xBC, 0x82, 0x08, 
0x00, 0x00, 0x00, 

/* character 0x0051 ('Q'): (width=8, offset=508) */
0x00, 0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 
0x9A, 0x44, 0x3A, 0x00, 0x00, 0x00, 

/* character 0x0052 ('R'): (width=7, offset=522) */
0x00, 0x00, 0x07, 0xC8, 0x50, 0xBE, 0x48, 0x89, 
0x12, 0x10, 0x00, 0x00, 0x00, 

/* character 0x0053 ('S'): (width=7, offset=535) */
0x00, 0x00, 0x03, 0xC8, 0x50, 0x18, 0x0C, 0x05, 
0x09, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0054 ('T'): (width=6, offset=548) */
0x00, 0x00, 0x1F, 0x10, 0x41, 0x04, 0x10, 0x41, 
0x00, 0x00, 0x00, 

/* character 0x0055 ('U'): (width=7, offset=559) */
0x00, 0x00, 0x04, 0x28, 0x50, 0xA1, 0x42, 0x85, 
0x09, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0056 ('V'): (width=8, offset=572) */
0x00, 0x00, 0x00, 0x82, 0x82, 0x44, 0x44, 0x28, 
0x28, 0x10, 0x10, 0x00, 0x00, 0x00, 

/* character 0x0057 ('W'): (width=10, offset=586) */
0x00, 0x00, 0x00, 0x00, 0x21, 0x14, 0x65, 0x29, 
0x4A, 0x8A, 0xA2, 0x90, 0x44, 0x10, 0x00, 0x00, 
0x00, 0x00, 

/* character 0x0058 ('X'): (width=7, offset=604) */
0x00, 0x00, 0x04, 0x24, 0x89, 0x0C, 0x18, 0x48, 
0x92, 0x10, 0x00, 0x00, 0x00, 

/* character 0x0059 ('Y'): (width=8, offset=617) */
0x00, 0x00, 0x00, 0x82, 0x44, 0x44, 0x28, 0x10, 
0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 

/* character 0x005A ('Z'): (width=7, offset=631) */
0x00, 0x00, 0x03, 0xE0, 0x82, 0x04, 0x10, 0x20, 
0x83, 0xF0, 0x00, 0x00, 0x00, 

/* character 0x005B ('['): (width=3, offset=644) */
0x00, 0x69, 0x24, 0x92, 0x4C, 0x00, 

/* character 0x005C ('\'): (width=3, offset=650) */
0x00, 0x48, 0x92, 0x44, 0x80, 0x00, 

/* character 0x005D (']'): (width=3, offset=656) */
0x00, 0x64, 0x92, 0x49, 0x2C, 0x00, 

/* character 0x005E ('^'): (width=5, offset=662) */
0x00, 0x00, 0x45, 0x2A, 0x20, 0x00, 0x00, 0x00, 
0x00, 

/* character 0x005F ('_'): (width=6, offset=671) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x7C, 0x00, 

/* character 0x0060 ('`'): (width=4, offset=682) */
0x00, 0x08, 0x40, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0061 ('a'): (width=6, offset=689) */
0x00, 0x00, 0x00, 0x01, 0xC8, 0x9E, 0x8A, 0x66, 
0x80, 0x00, 0x00, 

/* character 0x0062 ('b'): (width=6, offset=700) */
0x00, 0x00, 0x20, 0x82, 0xCC, 0xA2, 0x8B, 0x2B, 
0x00, 0x00, 0x00, 

/* character 0x0063 ('c'): (width=6, offset=711) */
0x00, 0x00, 0x00, 0x01, 0xC8, 0xA0, 0x82, 0x27, 
0x00, 0x00, 0x00, 

/* character 0x0064 ('d'): (width=6, offset=722) */
0x00, 0x00, 0x02, 0x09, 0xA9, 0xA2, 0x8A, 0x66, 
0x80, 0x00, 0x00, 

/* character 0x0065 ('e'): (width=6, offset=733) */
0x00, 0x00, 0x00, 0x01, 0xC8, 0xBE, 0x82, 0x27, 
0x00, 0x00, 0x00, 

/* character 0x0066 ('f'): (width=4, offset=744) */
0x00, 0x02, 0x4E, 0x44, 0x44, 0x40, 0x00, 

/* character 0x0067 ('g'): (width=6, offset=751) */
0x00, 0x00, 0x00, 0x01, 0xA9, 0xA2, 0x8A, 0x66, 
0x82, 0xF0, 0x00, 

/* character 0x0068 ('h'): (width=6, offset=762) */
0x00, 0x00, 0x20, 0x82, 0xCC, 0xA2, 0x8A, 0x28, 
0x80, 0x00, 0x00, 

/* character 0x0069 ('i'): (width=2, offset=773) */
0x02, 0x2A, 0xA8, 0x00, 

/* character 0x006A ('j'): (width=2, offset=777) */
0x01, 0x15, 0x55, 0x80, 

/* character 0x006B ('k'): (width=5, offset=781) */
0x00, 0x01, 0x08, 0x4A, 0x98, 0xA5, 0x24, 0x00, 
0x00, 

/* character 0x006C ('l'): (width=2, offset=790) */
0x02, 0xAA, 0xA8, 0x00, 

/* character 0x006D ('m'): (width=8, offset=794) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xBC, 0xD2, 0x92, 
0x92, 0x92, 0x92, 0x00, 0x00, 0x00, 

/* character 0x006E ('n'): (width=6, offset=808) */
0x00, 0x00, 0x00, 0x03, 0xC8, 0xA2, 0x8A, 0x28, 
0x80, 0x00, 0x00, 

/* character 0x006F ('o'): (width=6, offset=819) */
0x00, 0x00, 0x00, 0x01, 0xC8, 0xA2, 0x8A, 0x27, 
0x00, 0x00, 0x00, 

/* character 0x0070 ('p'): (width=6, offset=830) */
0x00, 0x00, 0x00, 0x02, 0xCC, 0xA2, 0x8B, 0x2B, 
0x20, 0x80, 0x00, 

/* character 0x0071 ('q'): (width=6, offset=841) */
0x00, 0x00, 0x00, 0x01, 0xA9, 0xA2, 0x8A, 0x66, 
0x82, 0x08, 0x00, 

/* character 0x0072 ('r'): (width=4, offset=852) */
0x00, 0x00, 0x0A, 0xC8, 0x88, 0x80, 0x00, 

/* character 0x0073 ('s'): (width=6, offset=859) */
0x00, 0x00, 0x00, 0x01, 0xC8, 0x98, 0x12, 0x27, 
0x00, 0x00, 0x00, 

/* character 0x0074 ('t'): (width=3, offset=870) */
0x00, 0x49, 0xA4, 0x93, 0x00, 0x00, 

/* character 0x0075 ('u'): (width=6, offset=876) */
0x00, 0x00, 0x00, 0x02, 0x28, 0xA2, 0x8A, 0x66, 
0x80, 0x00, 0x00, 

/* character 0x0076 ('v'): (width=6, offset=887) */
0x00, 0x00, 0x00, 0x02, 0x28, 0x94, 0x50, 0x82, 
0x00, 0x00, 0x00, 

/* character 0x0077 ('w'): (width=10, offset=898) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x29, 
0x49, 0x54, 0x55, 0x08, 0x82, 0x20, 0x00, 0x00, 
0x00, 0x00, 

/* character 0x0078 ('x'): (width=6, offset=916) */
0x00, 0x00, 0x00, 0x02, 0x25, 0x08, 0x21, 0x48, 
0x80, 0x00, 0x00, 

/* character 0x0079 ('y'): (width=6, offset=927) */
0x00, 0x00, 0x00, 0x02, 0x28, 0x94, 0x50, 0x82, 
0x08, 0x40, 0x00, 

/* character 0x007A ('z'): (width=6, offset=938) */
0x00, 0x00, 0x00, 0x03, 0xE1, 0x08, 0x21, 0x0F, 
0x80, 0x00, 0x00, 

/* character 0x007B ('{'): (width=4, offset=949) */
0x00, 0x02, 0x44, 0x48, 0x44, 0x44, 0x20, 

/* character 0x007C ('|'): (width=2, offset=956) */
0x02, 0xAA, 0xAA, 0x80, 

/* character 0x007D ('}'): (width=4, offset=960) */
0x00, 0x04, 0x22, 0x21, 0x22, 0x22, 0x40, 

/* character 0x007E ('~'): (width=6, offset=967) */
0x00, 0x00, 0x00, 0x00, 0x0E, 0xAC, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x007F (''): (width=8, offset=978) */
0x00, 0x00, 0x00, 0xE0, 0xA0, 0xA0, 0xA0, 0xA0, 
0xA0, 0xA0, 0xE0, 0x00, 0x00, 0x00, 

};

/*******************************************************************************************
  Offset table provides the starting offset of each character in the data table.

  To get the starting offset of character 'A', you can use the following expression:

     const unsigned char index = index_table['A'];
     const unsigned int offset = offset_table[index]

 *******************************************************************************************/
const unsigned int Arial8Offset_table[] = {
/*		offset    char    hexcode */
/*		======    ====    ======= */
  		     0, /*          0020    */
  		     6, /*   !      0021    */
  		    10, /*   "      0022    */
  		    17, /*   #      0023    */
  		    28, /*   $      0024    */
  		    39, /*   %      0025    */
  		    57, /*   &      0026    */
  		    70, /*   '      0027    */
  		    74, /*   (      0028    */
  		    81, /*   )      0029    */
  		    88, /*   *      002A    */
  		    95, /*   +      002B    */
  		   106, /*   ,      002C    */
  		   112, /*   -      002D    */
  		   119, /*   .      002E    */
  		   125, /*   /      002F    */
  		   131, /*   0      0030    */
  		   142, /*   1      0031    */
  		   153, /*   2      0032    */
  		   164, /*   3      0033    */
  		   175, /*   4      0034    */
  		   186, /*   5      0035    */
  		   197, /*   6      0036    */
  		   208, /*   7      0037    */
  		   219, /*   8      0038    */
  		   230, /*   9      0039    */
  		   241, /*   :      003A    */
  		   247, /*   ;      003B    */
  		   253, /*   <      003C    */
  		   264, /*   =      003D    */
  		   275, /*   >      003E    */
  		   286, /*   ?      003F    */
  		   297, /*   @      0040    */
  		   317, /*   A      0041    */
  		   331, /*   B      0042    */
  		   344, /*   C      0043    */
  		   357, /*   D      0044    */
  		   370, /*   E      0045    */
  		   381, /*   F      0046    */
  		   392, /*   G      0047    */
  		   406, /*   H      0048    */
  		   419, /*   I      0049    */
  		   423, /*   J      004A    */
  		   432, /*   K      004B    */
  		   445, /*   L      004C    */
  		   456, /*   M      004D    */
  		   470, /*   N      004E    */
  		   483, /*   O      004F    */
  		   497, /*   P      0050    */
  		   508, /*   Q      0051    */
  		   522, /*   R      0052    */
  		   535, /*   S      0053    */
  		   548, /*   T      0054    */
  		   559, /*   U      0055    */
  		   572, /*   V      0056    */
  		   586, /*   W      0057    */
  		   604, /*   X      0058    */
  		   617, /*   Y      0059    */
  		   631, /*   Z      005A    */
  		   644, /*   [      005B    */
  		   650, /*   \      005C    */
  		   656, /*   ]      005D    */
  		   662, /*   ^      005E    */
  		   671, /*   _      005F    */
  		   682, /*   `      0060    */
  		   689, /*   a      0061    */
  		   700, /*   b      0062    */
  		   711, /*   c      0063    */
  		   722, /*   d      0064    */
  		   733, /*   e      0065    */
  		   744, /*   f      0066    */
  		   751, /*   g      0067    */
  		   762, /*   h      0068    */
  		   773, /*   i      0069    */
  		   777, /*   j      006A    */
  		   781, /*   k      006B    */
  		   790, /*   l      006C    */
  		   794, /*   m      006D    */
  		   808, /*   n      006E    */
  		   819, /*   o      006F    */
  		   830, /*   p      0070    */
  		   841, /*   q      0071    */
  		   852, /*   r      0072    */
  		   859, /*   s      0073    */
  		   870, /*   t      0074    */
  		   876, /*   u      0075    */
  		   887, /*   v      0076    */
  		   898, /*   w      0077    */
  		   916, /*   x      0078    */
  		   927, /*   y      0079    */
  		   938, /*   z      007A    */
  		   949, /*   {      007B    */
  		   956, /*   |      007C    */
  		   960, /*   }      007D    */
  		   967, /*   ~      007E    */
  		   978, /*         007F    */
};

/*******************************************************************************************
  Index table is used to find the mapping index of a character.

  If you can find a simple mathematical expression for index mapping, you can use that.
  If you do not have such a mathematical expression, this index table is just for you.

  To get the index of character 'A', you can use the following expression:

     const unsigned char index = index_table['A'];

 *******************************************************************************************/
const unsigned char Arial8Index_table[] = {
/*		index   hexcode   decimal  char */
/*		=====   =======   =======  ==== */
  		  0, /*   00          0     .   */
  		  0, /*   01          1     .   */
  		  0, /*   02          2     .   */
  		  0, /*   03          3     .   */
  		  0, /*   04          4     .   */
  		  0, /*   05          5     .   */
  		  0, /*   06          6     .   */
  		  0, /*   07          7     .   */
  		  0, /*   08          8     .   */
  		  0, /*   09          9     .   */
  		  0, /*   0A         10     .   */
  		  0, /*   0B         11     .   */
  		  0, /*   0C         12     .   */
  		  0, /*   0D         13     .   */
  		  0, /*   0E         14     .   */
  		  0, /*   0F         15     .   */
  		  0, /*   10         16     .   */
  		  0, /*   11         17     .   */
  		  0, /*   12         18     .   */
  		  0, /*   13         19     .   */
  		  0, /*   14         20     .   */
  		  0, /*   15         21     .   */
  		  0, /*   16         22     .   */
  		  0, /*   17         23     .   */
  		  0, /*   18         24     .   */
  		  0, /*   19         25     .   */
  		  0, /*   1A         26     .   */
  		  0, /*   1B         27     .   */
  		  0, /*   1C         28     .   */
  		  0, /*   1D         29     .   */
  		  0, /*   1E         30     .   */
  		  0, /*   1F         31     .   */
  		  0, /*   20         32         */
  		  1, /*   21         33     !   */
  		  2, /*   22         34     "   */
  		  3, /*   23         35     #   */
  		  4, /*   24         36     $   */
  		  5, /*   25         37     %   */
  		  6, /*   26         38     &   */
  		  7, /*   27         39     '   */
  		  8, /*   28         40     (   */
  		  9, /*   29         41     )   */
  		 10, /*   2A         42     *   */
  		 11, /*   2B         43     +   */
  		 12, /*   2C         44     ,   */
  		 13, /*   2D         45     -   */
  		 14, /*   2E         46     .   */
  		 15, /*   2F         47     /   */
  		 16, /*   30         48     0   */
  		 17, /*   31         49     1   */
  		 18, /*   32         50     2   */
  		 19, /*   33         51     3   */
  		 20, /*   34         52     4   */
  		 21, /*   35         53     5   */
  		 22, /*   36         54     6   */
  		 23, /*   37         55     7   */
  		 24, /*   38         56     8   */
  		 25, /*   39         57     9   */
  		 26, /*   3A         58     :   */
  		 27, /*   3B         59     ;   */
  		 28, /*   3C         60     <   */
  		 29, /*   3D         61     =   */
  		 30, /*   3E         62     >   */
  		 31, /*   3F         63     ?   */
  		 32, /*   40         64     @   */
  		 33, /*   41         65     A   */
  		 34, /*   42         66     B   */
  		 35, /*   43         67     C   */
  		 36, /*   44         68     D   */
  		 37, /*   45         69     E   */
  		 38, /*   46         70     F   */
  		 39, /*   47         71     G   */
  		 40, /*   48         72     H   */
  		 41, /*   49         73     I   */
  		 42, /*   4A         74     J   */
  		 43, /*   4B         75     K   */
  		 44, /*   4C         76     L   */
  		 45, /*   4D         77     M   */
  		 46, /*   4E         78     N   */
  		 47, /*   4F         79     O   */
  		 48, /*   50         80     P   */
  		 49, /*   51         81     Q   */
  		 50, /*   52         82     R   */
  		 51, /*   53         83     S   */
  		 52, /*   54         84     T   */
  		 53, /*   55         85     U   */
  		 54, /*   56         86     V   */
  		 55, /*   57         87     W   */
  		 56, /*   58         88     X   */
  		 57, /*   59         89     Y   */
  		 58, /*   5A         90     Z   */
  		 59, /*   5B         91     [   */
  		 60, /*   5C         92     \   */
  		 61, /*   5D         93     ]   */
  		 62, /*   5E         94     ^   */
  		 63, /*   5F         95     _   */
  		 64, /*   60         96     `   */
  		 65, /*   61         97     a   */
  		 66, /*   62         98     b   */
  		 67, /*   63         99     c   */
  		 68, /*   64        100     d   */
  		 69, /*   65        101     e   */
  		 70, /*   66        102     f   */
  		 71, /*   67        103     g   */
  		 72, /*   68        104     h   */
  		 73, /*   69        105     i   */
  		 74, /*   6A        106     j   */
  		 75, /*   6B        107     k   */
  		 76, /*   6C        108     l   */
  		 77, /*   6D        109     m   */
  		 78, /*   6E        110     n   */
  		 79, /*   6F        111     o   */
  		 80, /*   70        112     p   */
  		 81, /*   71        113     q   */
  		 82, /*   72        114     r   */
  		 83, /*   73        115     s   */
  		 84, /*   74        116     t   */
  		 85, /*   75        117     u   */
  		 86, /*   76        118     v   */
  		 87, /*   77        119     w   */
  		 88, /*   78        120     x   */
  		 89, /*   79        121     y   */
  		 90, /*   7A        122     z   */
  		 91, /*   7B        123     {   */
  		 92, /*   7C        124     |   */
  		 93, /*   7D        125     }   */
  		 94, /*   7E        126     ~   */
  		 95, /*   7F        127        */
  		  0, /*   80        128   ? */
  		  0, /*   81        129   � */
  		  0, /*   82        130   ? */
  		  0, /*   83        131   ? */
  		  0, /*   84        132   ? */
  		  0, /*   85        133   ? */
  		  0, /*   86        134   ? */
  		  0, /*   87        135   ? */
  		  0, /*   88        136   ? */
  		  0, /*   89        137   ? */
  		  0, /*   8A        138   ? */
  		  0, /*   8B        139   ? */
  		  0, /*   8C        140   ? */
  		  0, /*   8D        141   � */
  		  0, /*   8E        142   ? */
  		  0, /*   8F        143   � */
  		  0, /*   90        144   � */
  		  0, /*   91        145   ? */
  		  0, /*   92        146   ? */
  		  0, /*   93        147   ? */
  		  0, /*   94        148   ? */
  		  0, /*   95        149   ? */
  		  0, /*   96        150   ? */
  		  0, /*   97        151   ? */
  		  0, /*   98        152   ? */
  		  0, /*   99        153   ? */
  		  0, /*   9A        154   ? */
  		  0, /*   9B        155   ? */
  		  0, /*   9C        156   ? */
  		  0, /*   9D        157   � */
  		  0, /*   9E        158   ? */
  		  0, /*   9F        159   ? */
  		  0, /*   A0        160   � */
  		  0, /*   A1        161   � */
  		  0, /*   A2        162   � */
  		  0, /*   A3        163   � */
  		  0, /*   A4        164   � */
  		  0, /*   A5        165   � */
  		  0, /*   A6        166   � */
  		  0, /*   A7        167   � */
  		  0, /*   A8        168   � */
  		  0, /*   A9        169   � */
  		  0, /*   AA        170   � */
  		  0, /*   AB        171   � */
  		  0, /*   AC        172   � */
  		  0, /*   AD        173   � */
  		  0, /*   AE        174   � */
  		  0, /*   AF        175   � */
  		  0, /*   B0        176   � */
  		  0, /*   B1        177   � */
  		  0, /*   B2        178   � */
  		  0, /*   B3        179   � */
  		  0, /*   B4        180   � */
  		  0, /*   B5        181   � */
  		  0, /*   B6        182   � */
  		  0, /*   B7        183   � */
  		  0, /*   B8        184   � */
  		  0, /*   B9        185   � */
  		  0, /*   BA        186   � */
  		  0, /*   BB        187   � */
  		  0, /*   BC        188   � */
  		  0, /*   BD        189   � */
  		  0, /*   BE        190   � */
  		  0, /*   BF        191   � */
  		  0, /*   C0        192   � */
  		  0, /*   C1        193   � */
  		  0, /*   C2        194   � */
  		  0, /*   C3        195   � */
  		  0, /*   C4        196   � */
  		  0, /*   C5        197   � */
  		  0, /*   C6        198   � */
  		  0, /*   C7        199   � */
  		  0, /*   C8        200   � */
  		  0, /*   C9        201   � */
  		  0, /*   CA        202   � */
  		  0, /*   CB        203   � */
  		  0, /*   CC        204   � */
  		  0, /*   CD        205   � */
  		  0, /*   CE        206   � */
  		  0, /*   CF        207   � */
  		  0, /*   D0        208   � */
  		  0, /*   D1        209   � */
  		  0, /*   D2        210   � */
  		  0, /*   D3        211   � */
  		  0, /*   D4        212   � */
  		  0, /*   D5        213   � */
  		  0, /*   D6        214   � */
  		  0, /*   D7        215   � */
  		  0, /*   D8        216   � */
  		  0, /*   D9        217   � */
  		  0, /*   DA        218   � */
  		  0, /*   DB        219   � */
  		  0, /*   DC        220   � */
  		  0, /*   DD        221   � */
  		  0, /*   DE        222   � */
  		  0, /*   DF        223   � */
  		  0, /*   E0        224   � */
  		  0, /*   E1        225   � */
  		  0, /*   E2        226   � */
  		  0, /*   E3        227   � */
  		  0, /*   E4        228   � */
  		  0, /*   E5        229   � */
  		  0, /*   E6        230   � */
  		  0, /*   E7        231   � */
  		  0, /*   E8        232   � */
  		  0, /*   E9        233   � */
  		  0, /*   EA        234   � */
  		  0, /*   EB        235   � */
  		  0, /*   EC        236   � */
  		  0, /*   ED        237   � */
  		  0, /*   EE        238   � */
  		  0, /*   EF        239   � */
  		  0, /*   F0        240   � */
  		  0, /*   F1        241   � */
  		  0, /*   F2        242   � */
  		  0, /*   F3        243   � */
  		  0, /*   F4        244   � */
  		  0, /*   F5        245   � */
  		  0, /*   F6        246   � */
  		  0, /*   F7        247   � */
  		  0, /*   F8        248   � */
  		  0, /*   F9        249   � */
  		  0, /*   FA        250   � */
  		  0, /*   FB        251   � */
  		  0, /*   FC        252   � */
  		  0, /*   FD        253   � */
  		  0, /*   FE        254   � */
  		  0, /*   FF        255   � */
};

/*******************************************************************************************
  Width table provides the width of each character. It's useful for proportional font only.
  For monospaced font, the width of each character is the same.

  To get the width of character 'A', you can use the following expression:

     const unsigned char index = index_table['A'];
     const unsigned char width = width_table[index];

 *******************************************************************************************/
const unsigned char Arial8Width_table[] = {
/*		width    char    hexcode */
/*		=====    ====    ======= */
  		  3, /*          0020    */
  		  2, /*   !      0021    */
  		  4, /*   "      0022    */
  		  6, /*   #      0023    */
  		  6, /*   $      0024    */
  		 10, /*   %      0025    */
  		  7, /*   &      0026    */
  		  2, /*   '      0027    */
  		  4, /*   (      0028    */
  		  4, /*   )      0029    */
  		  4, /*   *      002A    */
  		  6, /*   +      002B    */
  		  3, /*   ,      002C    */
  		  4, /*   -      002D    */
  		  3, /*   .      002E    */
  		  3, /*   /      002F    */
  		  6, /*   0      0030    */
  		  6, /*   1      0031    */
  		  6, /*   2      0032    */
  		  6, /*   3      0033    */
  		  6, /*   4      0034    */
  		  6, /*   5      0035    */
  		  6, /*   6      0036    */
  		  6, /*   7      0037    */
  		  6, /*   8      0038    */
  		  6, /*   9      0039    */
  		  3, /*   :      003A    */
  		  3, /*   ;      003B    */
  		  6, /*   <      003C    */
  		  6, /*   =      003D    */
  		  6, /*   >      003E    */
  		  6, /*   ?      003F    */
  		 11, /*   @      0040    */
  		  8, /*   A      0041    */
  		  7, /*   B      0042    */
  		  7, /*   C      0043    */
  		  7, /*   D      0044    */
  		  6, /*   E      0045    */
  		  6, /*   F      0046    */
  		  8, /*   G      0047    */
  		  7, /*   H      0048    */
  		  2, /*   I      0049    */
  		  5, /*   J      004A    */
  		  7, /*   K      004B    */
  		  6, /*   L      004C    */
  		  8, /*   M      004D    */
  		  7, /*   N      004E    */
  		  8, /*   O      004F    */
  		  6, /*   P      0050    */
  		  8, /*   Q      0051    */
  		  7, /*   R      0052    */
  		  7, /*   S      0053    */
  		  6, /*   T      0054    */
  		  7, /*   U      0055    */
  		  8, /*   V      0056    */
  		 10, /*   W      0057    */
  		  7, /*   X      0058    */
  		  8, /*   Y      0059    */
  		  7, /*   Z      005A    */
  		  3, /*   [      005B    */
  		  3, /*   \      005C    */
  		  3, /*   ]      005D    */
  		  5, /*   ^      005E    */
  		  6, /*   _      005F    */
  		  4, /*   `      0060    */
  		  6, /*   a      0061    */
  		  6, /*   b      0062    */
  		  6, /*   c      0063    */
  		  6, /*   d      0064    */
  		  6, /*   e      0065    */
  		  4, /*   f      0066    */
  		  6, /*   g      0067    */
  		  6, /*   h      0068    */
  		  2, /*   i      0069    */
  		  2, /*   j      006A    */
  		  5, /*   k      006B    */
  		  2, /*   l      006C    */
  		  8, /*   m      006D    */
  		  6, /*   n      006E    */
  		  6, /*   o      006F    */
  		  6, /*   p      0070    */
  		  6, /*   q      0071    */
  		  4, /*   r      0072    */
  		  6, /*   s      0073    */
  		  3, /*   t      0074    */
  		  6, /*   u      0075    */
  		  6, /*   v      0076    */
  		 10, /*   w      0077    */
  		  6, /*   x      0078    */
  		  6, /*   y      0079    */
  		  6, /*   z      007A    */
  		  4, /*   {      007B    */
  		  2, /*   |      007C    */
  		  4, /*   }      007D    */
  		  6, /*   ~      007E    */
  		  8, /*         007F    */
};

