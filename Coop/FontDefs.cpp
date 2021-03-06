#include "font.h"

void CFont::_FillForArial()
{
	m_iHeight = 28;
	m_iNormalDistance = 3;
	m_iSpaceDistance = 5;

	m_iCharColumn[0] = 0;
	m_iCharWidth[0] = 4;
	m_iCharRow[0] = 0;
	m_iCharColumn[1] = 4;
	m_iCharWidth[1] = 10;
	m_iCharRow[1] = 0;
	m_iCharColumn[2] = 14;
	m_iCharWidth[2] = 16;
	m_iCharRow[2] = 0;
	m_iCharColumn[3] = 30;
	m_iCharWidth[3] = 15;
	m_iCharRow[3] = 0;
	m_iCharColumn[4] = 45;
	m_iCharWidth[4] = 22;
	m_iCharRow[4] = 0;
	m_iCharColumn[5] = 67;
	m_iCharWidth[5] = 17;
	m_iCharRow[5] = 0;
	m_iCharColumn[6] = 84;
	m_iCharWidth[6] = 5;
	m_iCharRow[6] = 0;
	m_iCharColumn[7] = 89;
	m_iCharWidth[7] = 8;
	m_iCharRow[7] = 0;
	m_iCharColumn[8] = 97;
	m_iCharWidth[8] = 8;
	m_iCharRow[8] = 0;
	m_iCharColumn[9] = 105;
	m_iCharWidth[9] = 11;
	m_iCharRow[9] = 0;
	m_iCharColumn[10] = 116;
	m_iCharWidth[10] = 14;
	m_iCharRow[10] = 0;
	m_iCharColumn[11] = 130;
	m_iCharWidth[11] = 4;
	m_iCharRow[11] = 0;
	m_iCharColumn[12] = 134;
	m_iCharWidth[12] = 9;
	m_iCharRow[12] = 0;
	m_iCharColumn[13] = 143;
	m_iCharWidth[13] = 4;
	m_iCharRow[13] = 0;
	m_iCharColumn[14] = 147;
	m_iCharWidth[14] = 10;
	m_iCharRow[14] = 0;
	m_iCharColumn[15] = 157;
	m_iCharWidth[15] = 14;
	m_iCharRow[15] = 0;
	m_iCharColumn[16] = 171;
	m_iCharWidth[16] = 9;
	m_iCharRow[16] = 0;
	m_iCharColumn[17] = 180;
	m_iCharWidth[17] = 14;
	m_iCharRow[17] = 0;
	m_iCharColumn[18] = 194;
	m_iCharWidth[18] = 13;
	m_iCharRow[18] = 0;
	m_iCharColumn[19] = 207;
	m_iCharWidth[19] = 14;
	m_iCharRow[19] = 0;
	m_iCharColumn[20] = 221;
	m_iCharWidth[20] = 13;
	m_iCharRow[20] = 0;
	m_iCharColumn[21] = 234;
	m_iCharWidth[21] = 14;
	m_iCharRow[21] = 0;
	m_iCharColumn[22] = 0;
	m_iCharWidth[22] = 13;
	m_iCharRow[22] = 1;
	m_iCharColumn[23] = 13;
	m_iCharWidth[23] = 15;
	m_iCharRow[23] = 1;
	m_iCharColumn[24] = 28;
	m_iCharWidth[24] = 15;
	m_iCharRow[24] = 1;
	m_iCharColumn[25] = 43;
	m_iCharWidth[25] = 4;
	m_iCharRow[25] = 1;
	m_iCharColumn[26] = 47;
	m_iCharWidth[26] = 4;
	m_iCharRow[26] = 1;
	m_iCharColumn[27] = 51;
	m_iCharWidth[27] = 14;
	m_iCharRow[27] = 1;
	m_iCharColumn[28] = 65;
	m_iCharWidth[28] = 14;
	m_iCharRow[28] = 1;
	m_iCharColumn[29] = 79;
	m_iCharWidth[29] = 14;
	m_iCharRow[29] = 1;
	m_iCharColumn[30] = 93;
	m_iCharWidth[30] = 13;
	m_iCharRow[30] = 1;
	m_iCharColumn[31] = 106;
	m_iCharWidth[31] = 26;
	m_iCharRow[31] = 1;
	m_iCharColumn[32] = 132;
	m_iCharWidth[32] = 19;
	m_iCharRow[32] = 1;
	m_iCharColumn[33] = 151;
	m_iCharWidth[33] = 16;
	m_iCharRow[33] = 1;
	m_iCharColumn[34] = 167;
	m_iCharWidth[34] = 18;
	m_iCharRow[34] = 1;
	m_iCharColumn[35] = 185;
	m_iCharWidth[35] = 17;
	m_iCharRow[35] = 1;
	m_iCharColumn[36] = 202;
	m_iCharWidth[36] = 16;
	m_iCharRow[36] = 1;
	m_iCharColumn[37] = 218;
	m_iCharWidth[37] = 15;
	m_iCharRow[37] = 1;
	m_iCharColumn[38] = 233;
	m_iCharWidth[38] = 19;
	m_iCharRow[38] = 1;
	m_iCharColumn[39] = 0;
	m_iCharWidth[39] = 17;
	m_iCharRow[39] = 2;
	m_iCharColumn[40] = 17;
	m_iCharWidth[40] = 4;
	m_iCharRow[40] = 2;
	m_iCharColumn[41] = 21;
	m_iCharWidth[41] = 12;
	m_iCharRow[41] = 2;
	m_iCharColumn[42] = 33;
	m_iCharWidth[42] = 18;
	m_iCharRow[42] = 2;
	m_iCharColumn[43] = 51;
	m_iCharWidth[43] = 14;
	m_iCharRow[43] = 2;
	m_iCharColumn[44] = 65;
	m_iCharWidth[44] = 21;
	m_iCharRow[44] = 2;
	m_iCharColumn[45] = 86;
	m_iCharWidth[45] = 17;
	m_iCharRow[45] = 2;
	m_iCharColumn[46] = 103;
	m_iCharWidth[46] = 20;
	m_iCharRow[46] = 2;
	m_iCharColumn[47] = 123;
	m_iCharWidth[47] = 16;
	m_iCharRow[47] = 2;
	m_iCharColumn[48] = 139;
	m_iCharWidth[48] = 21;
	m_iCharRow[48] = 2;
	m_iCharColumn[49] = 160;
	m_iCharWidth[49] = 18;
	m_iCharRow[49] = 2;
	m_iCharColumn[50] = 178;
	m_iCharWidth[50] = 16;
	m_iCharRow[50] = 2;
	m_iCharColumn[51] = 194;
	m_iCharWidth[51] = 16;
	m_iCharRow[51] = 2;
	m_iCharColumn[52] = 210;
	m_iCharWidth[52] = 17;
	m_iCharRow[52] = 2;
	m_iCharColumn[53] = 227;
	m_iCharWidth[53] = 17;
	m_iCharRow[53] = 2;
	m_iCharColumn[54] = 0;
	m_iCharWidth[54] = 28;
	m_iCharRow[54] = 3;
	m_iCharColumn[55] = 28;
	m_iCharWidth[55] = 17;
	m_iCharRow[55] = 3;
	m_iCharColumn[56] = 45;
	m_iCharWidth[56] = 18;
	m_iCharRow[56] = 3;
	m_iCharColumn[57] = 63;
	m_iCharWidth[57] = 17;
	m_iCharRow[57] = 3;
	m_iCharColumn[58] = 80;
	m_iCharWidth[58] = 7;
	m_iCharRow[58] = 3;
	m_iCharColumn[59] = 87;
	m_iCharWidth[59] = 9;
	m_iCharRow[59] = 3;
	m_iCharColumn[60] = 96;
	m_iCharWidth[60] = 7;
	m_iCharRow[60] = 3;
	m_iCharColumn[61] = 103;
	m_iCharWidth[61] = 12;
	m_iCharRow[61] = 3;
	m_iCharColumn[62] = 115;
	m_iCharWidth[62] = 17;
	m_iCharRow[62] = 3;
	m_iCharColumn[63] = 132;
	m_iCharWidth[63] = 6;
	m_iCharRow[63] = 3;
	m_iCharColumn[64] = 138;
	m_iCharWidth[64] = 14;
	m_iCharRow[64] = 3;
	m_iCharColumn[65] = 152;
	m_iCharWidth[65] = 14;
	m_iCharRow[65] = 3;
	m_iCharColumn[66] = 166;
	m_iCharWidth[66] = 13;
	m_iCharRow[66] = 3;
	m_iCharColumn[67] = 179;
	m_iCharWidth[67] = 14;
	m_iCharRow[67] = 3;
	m_iCharColumn[68] = 193;
	m_iCharWidth[68] = 15;
	m_iCharRow[68] = 3;
	m_iCharColumn[69] = 208;
	m_iCharWidth[69] = 10;
	m_iCharRow[69] = 3;
	m_iCharColumn[70] = 218;
	m_iCharWidth[70] = 14;
	m_iCharRow[70] = 3;
	m_iCharColumn[71] = 232;
	m_iCharWidth[71] = 13;
	m_iCharRow[71] = 3;
	m_iCharColumn[72] = 245;
	m_iCharWidth[72] = 4;
	m_iCharRow[72] = 3;
	m_iCharColumn[73] = 249;
	m_iCharWidth[73] = 7;
	m_iCharRow[73] = 3;
	m_iCharColumn[74] = 0;
	m_iCharWidth[74] = 13;
	m_iCharRow[74] = 4;
	m_iCharColumn[75] = 13;
	m_iCharWidth[75] = 4;
	m_iCharRow[75] = 4;
	m_iCharColumn[76] = 17;
	m_iCharWidth[76] = 20;
	m_iCharRow[76] = 4;
	m_iCharColumn[77] = 37;
	m_iCharWidth[77] = 13;
	m_iCharRow[77] = 4;
	m_iCharColumn[78] = 50;
	m_iCharWidth[78] = 15;
	m_iCharRow[78] = 4;
	m_iCharColumn[79] = 65;
	m_iCharWidth[79] = 14;
	m_iCharRow[79] = 4;
	m_iCharColumn[80] = 79;
	m_iCharWidth[80] = 14;
	m_iCharRow[80] = 4;
	m_iCharColumn[81] = 93;
	m_iCharWidth[81] = 9;
	m_iCharRow[81] = 4;
	m_iCharColumn[82] = 102;
	m_iCharWidth[82] = 14;
	m_iCharRow[82] = 4;
	m_iCharColumn[83] = 116;
	m_iCharWidth[83] = 8;
	m_iCharRow[83] = 4;
	m_iCharColumn[84] = 124;
	m_iCharWidth[84] = 13;
	m_iCharRow[84] = 4;
	m_iCharColumn[85] = 137;
	m_iCharWidth[85] = 15;
	m_iCharRow[85] = 4;
	m_iCharColumn[86] = 152;
	m_iCharWidth[86] = 20;
	m_iCharRow[86] = 4;
	m_iCharColumn[87] = 172;
	m_iCharWidth[87] = 12;
	m_iCharRow[87] = 4;
	m_iCharColumn[88] = 184;
	m_iCharWidth[88] = 14;
	m_iCharRow[88] = 4;
	m_iCharColumn[89] = 198;
	m_iCharWidth[89] = 13;
	m_iCharRow[89] = 4;
	m_iCharColumn[90] = 211;
	m_iCharWidth[90] = 9;
	m_iCharRow[90] = 4;
	m_iCharColumn[91] = 220;
	m_iCharWidth[91] = 4;
	m_iCharRow[91] = 4;
	m_iCharColumn[92] = 224;
	m_iCharWidth[92] = 9;
	m_iCharRow[92] = 4;
	m_iCharColumn[93] = 233;
	m_iCharWidth[93] = 16;
	m_iCharRow[93] = 4;

	m_iMaxWidth = 0;
	for(int i = 0; i < g_iCharCount; i++)
	{
		m_iMaxWidth = max(m_iMaxWidth, m_iCharWidth[i]);
	}
}

void CFont::_FillForCourier()
{
}
