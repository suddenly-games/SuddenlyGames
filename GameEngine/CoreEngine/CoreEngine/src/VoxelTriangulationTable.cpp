#include "VoxelTriangulation.h"

namespace Engine
{
	void VoxelTriangulation::InitializePermutations()
	{
		Permutations.reserve(256);
		Permutations.push_back(Permutation(-1, -1));

		Permutations.push_back(Permutation(  0, Transformation::Euler___0P___0Y___0R ));		/* 00000001 | 001 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P___0Y__90R ));		/* 00000010 | 002 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P___0Y___0R ));		/* 00000011 | 003 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P___0Y_180R ));		/* 00000100 | 004 */
		Permutations.push_back(Permutation(  5, Transformation::Euler___0P___0Y___0R ));		/* 00000101 | 005 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P___0Y__90R ));		/* 00000110 | 006 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P___0Y_270R ));		/* 00000111 | 007 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P___0Y_270R ));		/* 00001000 | 008 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P___0Y_270R ));		/* 00001001 | 009 */
		Permutations.push_back(Permutation(  5, Transformation::Euler___0P___0Y__90R ));		/* 00001010 | 010 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P___0Y_180R ));		/* 00001011 | 011 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P___0Y_180R ));		/* 00001100 | 012 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P___0Y__90R ));		/* 00001101 | 013 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P___0Y___0R ));		/* 00001110 | 014 */
		Permutations.push_back(Permutation(  2, Transformation::Euler___0P_270Y__90R ));		/* 00001111 | 015 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P_180Y__90R ));		/* 00010000 | 016 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P_270Y___0R ));		/* 00010001 | 017 */
		Permutations.push_back(Permutation(  5, Transformation::Euler__90P___0Y___0R ));		/* 00010010 | 018 */
		Permutations.push_back(Permutation(  3, Transformation::Euler__90P___0Y__90R ));		/* 00010011 | 019 */
		Permutations.push_back(Permutation(  6, Transformation::Euler___0P_270Y___0R ));		/* 00010100 | 020 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P_270Y___0R ));		/* 00010101 | 021 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P__90Y_270R ));		/* 00010110 | 022 */
		Permutations.push_back(Permutation( 10, Transformation::Euler___0P___0Y___0R ));		/* 00010111 | 023 */
		Permutations.push_back(Permutation(  5, Transformation::Euler___0P_270Y___0R ));		/* 00011000 | 024 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_270Y_270R ));		/* 00011001 | 025 */
		Permutations.push_back(Permutation( 14, Transformation::Euler___0P_180Y_270R, true ));	/* 00011010 | 026 */
		Permutations.push_back(Permutation(  4, Transformation::Euler___0P___0Y___0R ));		/* 00011011 | 027 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P___0Y_180R ));		/* 00011100 | 028 */
		Permutations.push_back(Permutation( 11, Transformation::Euler___0P___0Y_270R ));		/* 00011101 | 029 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P_180Y__90R, true ));	/* 00011110 | 030 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_180Y__90R, true ));	/* 00011111 | 031 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P_180Y___0R ));		/* 00100000 | 032 */
		Permutations.push_back(Permutation(  5, Transformation::Euler__90P___0Y__90R ));		/* 00100001 | 033 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P__90Y___0R ));		/* 00100010 | 034 */
		Permutations.push_back(Permutation(  3, Transformation::Euler__90P___0Y___0R ));		/* 00100011 | 035 */
		Permutations.push_back(Permutation(  5, Transformation::Euler___0P__90Y__90R ));		/* 00100100 | 036 */
		Permutations.push_back(Permutation( 14, Transformation::Euler___0P_270Y_270R, true ));	/* 00100101 | 037 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P__90Y_180R ));		/* 00100110 | 038 */
		Permutations.push_back(Permutation(  4, Transformation::Euler___0P___0Y__90R ));		/* 00100111 | 039 */
		Permutations.push_back(Permutation(  6, Transformation::Euler___0P_180Y___0R ));		/* 00101000 | 040 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P___0Y_270R ));		/* 00101001 | 041 */
		Permutations.push_back(Permutation( 16, Transformation::Euler__90P___0Y__90R ));		/* 00101010 | 042 */
		Permutations.push_back(Permutation( 11, Transformation::Euler___0P___0Y___0R ));		/* 00101011 | 043 */
		Permutations.push_back(Permutation( 16, Transformation::Euler_270P___0Y___0R ));		/* 00101100 | 044 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P_180Y___0R, true ));	/* 00101101 | 045 */
		Permutations.push_back(Permutation( 10, Transformation::Euler___0P___0Y__90R ));		/* 00101110 | 046 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_180Y___0R, true ));	/* 00101111 | 047 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P_180Y___0R ));		/* 00110000 | 048 */
		Permutations.push_back(Permutation(  3, Transformation::Euler__90P___0Y_180R ));		/* 00110001 | 049 */
		Permutations.push_back(Permutation(  3, Transformation::Euler__90P___0Y_270R ));		/* 00110010 | 050 */
		Permutations.push_back(Permutation(  2, Transformation::Euler___0P___0Y___0R ));		/* 00110011 | 051 */
		Permutations.push_back(Permutation( 16, Transformation::Euler__90P___0Y___0R ));		/* 00110100 | 052 */
		Permutations.push_back(Permutation( 12, Transformation::Euler_270P___0Y__90R, true ));	/* 00110101 | 053 */
		Permutations.push_back(Permutation( 11, Transformation::Euler___0P__90Y___0R ));		/* 00110110 | 054 */
		Permutations.push_back(Permutation(  3, Transformation::Euler_270P___0Y__90R, true ));	/* 00110111 | 055 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P_180Y___0R ));		/* 00111000 | 056 */
		Permutations.push_back(Permutation( 10, Transformation::Euler___0P_270Y___0R ));		/* 00111001 | 057 */
		Permutations.push_back(Permutation( 12, Transformation::Euler_270P___0Y___0R, true ));	/* 00111010 | 058 */
		Permutations.push_back(Permutation(  3, Transformation::Euler_270P___0Y___0R, true ));	/* 00111011 | 059 */
		Permutations.push_back(Permutation( 13, Transformation::Euler___0P___0Y___0R, true ));	/* 00111100 | 060 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P_180Y_180R, true ));	/* 00111101 | 061 */
		Permutations.push_back(Permutation(  7, Transformation::Euler_270P___0Y_180R, true ));	/* 00111110 | 062 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P_180Y_180R, true ));	/* 00111111 | 063 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P_180Y_270R ));		/* 01000000 | 064 */
		Permutations.push_back(Permutation(  6, Transformation::Euler___0P___0Y___0R ));		/* 01000001 | 065 */
		Permutations.push_back(Permutation(  5, Transformation::Euler___0P__90Y___0R ));		/* 01000010 | 066 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P___0Y___0R ));		/* 01000011 | 067 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P__90Y_180R ));		/* 01000100 | 068 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P__90Y_180R ));		/* 01000101 | 069 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P__90Y__90R ));		/* 01000110 | 070 */
		Permutations.push_back(Permutation( 11, Transformation::Euler___0P___0Y__90R ));		/* 01000111 | 071 */
		Permutations.push_back(Permutation(  5, Transformation::Euler_270P___0Y___0R ));		/* 01001000 | 072 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P_270Y__90R ));		/* 01001001 | 073 */
		Permutations.push_back(Permutation( 14, Transformation::Euler___0P_270Y___0R, true ));	/* 01001010 | 074 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P_180Y_270R, true ));	/* 01001011 | 075 */
		Permutations.push_back(Permutation(  3, Transformation::Euler_270P___0Y_270R ));		/* 01001100 | 076 */
		Permutations.push_back(Permutation( 10, Transformation::Euler___0P___0Y_180R ));		/* 01001101 | 077 */
		Permutations.push_back(Permutation(  4, Transformation::Euler___0P___0Y_180R ));		/* 01001110 | 078 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_180Y_270R, true ));	/* 01001111 | 079 */
		Permutations.push_back(Permutation(  5, Transformation::Euler___0P_180Y__90R ));		/* 01010000 | 080 */
		Permutations.push_back(Permutation( 16, Transformation::Euler__90P___0Y_270R ));		/* 01010001 | 081 */
		Permutations.push_back(Permutation( 14, Transformation::Euler_270P___0Y___0R, true ));	/* 01010010 | 082 */
		Permutations.push_back(Permutation( 12, Transformation::Euler_270P___0Y_180R, true ));	/* 01010011 | 083 */
		Permutations.push_back(Permutation( 16, Transformation::Euler_270P___0Y__90R ));		/* 01010100 | 084 */
		Permutations.push_back(Permutation( 13, Transformation::Euler_270P___0Y_270R, true ));	/* 01010101 | 085 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P_270Y___0R, true ));	/* 01010110 | 086 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P_270Y_180R, true ));	/* 01010111 | 087 */
		Permutations.push_back(Permutation( 14, Transformation::Euler___0P__90Y___0R, true ));	/* 01011000 | 088 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P__90Y_180R, true ));	/* 01011001 | 089 */
		Permutations.push_back(Permutation(  9, Transformation::Euler___0P___0Y___0R, true ));	/* 01011010 | 090 */
		Permutations.push_back(Permutation(  8, Transformation::Euler___0P_180Y___0R, true ));	/* 01011011 | 091 */
		Permutations.push_back(Permutation( 12, Transformation::Euler__90P___0Y___0R, true ));	/* 01011100 | 092 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P__90Y___0R, true ));	/* 01011101 | 093 */
		Permutations.push_back(Permutation(  8, Transformation::Euler__90P___0Y__90R, true ));	/* 01011110 | 094 */
		Permutations.push_back(Permutation( 15, Transformation::Euler___0P_180Y___0R ));		/* 01011111 | 095 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P_180Y_270R ));		/* 01100000 | 096 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P_180Y_270R ));		/* 01100001 | 097 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P__90Y_270R ));		/* 01100010 | 098 */
		Permutations.push_back(Permutation( 10, Transformation::Euler___0P__90Y___0R ));		/* 01100011 | 099 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P__90Y___0R ));		/* 01100100 | 100 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P_270Y__90R, true ));	/* 01100101 | 101 */
		Permutations.push_back(Permutation(  2, Transformation::Euler___0P___0Y__90R ));		/* 01100110 | 102 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_270Y__90R, true ));	/* 01100111 | 103 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P__90Y__90R ));		/* 01101000 | 104 */
		Permutations.push_back(Permutation( 13, Transformation::Euler___0P___0Y__90R, true ));	/* 01101001 | 105 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P_270Y_180R, true ));	/* 01101010 | 106 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P_180Y__90R, true ));	/* 01101011 | 107 */
		Permutations.push_back(Permutation( 11, Transformation::Euler___0P__90Y_180R ));		/* 01101100 | 108 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P_270Y_270R, true ));	/* 01101101 | 109 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_270Y_180R, true ));	/* 01101110 | 110 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P_180Y__90R, true ));	/* 01101111 | 111 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_180Y_180R ));		/* 01110000 | 112 */
		Permutations.push_back(Permutation( 11, Transformation::Euler___0P_180Y___0R ));		/* 01110001 | 113 */
		Permutations.push_back(Permutation(  4, Transformation::Euler___0P_180Y___0R ));		/* 01110010 | 114 */
		Permutations.push_back(Permutation(  3, Transformation::Euler_270P___0Y_180R, true ));	/* 01110011 | 115 */
		Permutations.push_back(Permutation( 10, Transformation::Euler___0P__90Y__90R ));		/* 01110100 | 116 */
		Permutations.push_back(Permutation(  7, Transformation::Euler_270P___0Y_270R, true ));	/* 01110101 | 117 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_270Y___0R, true ));	/* 01110110 | 118 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P_270Y_180R, true ));	/* 01110111 | 119 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P___0Y_270R, true ));	/* 01111000 | 120 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P___0Y__90R, true ));	/* 01111001 | 121 */
		Permutations.push_back(Permutation(  8, Transformation::Euler___0P___0Y___0R, true ));	/* 01111010 | 122 */
		Permutations.push_back(Permutation( 15, Transformation::Euler_270P___0Y__90R ));		/* 01111011 | 123 */
		Permutations.push_back(Permutation(  7, Transformation::Euler__90P___0Y_180R, true ));	/* 01111100 | 124 */
		Permutations.push_back(Permutation(  6, Transformation::Euler___0P__90Y___0R, true ));	/* 01111101 | 125 */
		Permutations.push_back(Permutation( 15, Transformation::Euler___0P_270Y__90R ));		/* 01111110 | 126 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P_180Y_180R, true ));	/* 01111111 | 127 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P_180Y_180R ));		/* 10000000 | 128 */
		Permutations.push_back(Permutation(  5, Transformation::Euler___0P_270Y__90R ));		/* 10000001 | 129 */
		Permutations.push_back(Permutation(  6, Transformation::Euler___0P__90Y___0R ));		/* 10000010 | 130 */
		Permutations.push_back(Permutation( 16, Transformation::Euler__90P___0Y_180R ));		/* 10000011 | 131 */
		Permutations.push_back(Permutation(  5, Transformation::Euler_270P___0Y__90R ));		/* 10000100 | 132 */
		Permutations.push_back(Permutation( 14, Transformation::Euler___0P_180Y___0R, true ));	/* 10000101 | 133 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P___0Y__90R ));		/* 10000110 | 134 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P_180Y_180R, true ));	/* 10000111 | 135 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P_270Y_180R ));		/* 10001000 | 136 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_270Y___0R ));		/* 10001001 | 137 */
		Permutations.push_back(Permutation( 16, Transformation::Euler_270P___0Y_270R ));		/* 10001010 | 138 */
		Permutations.push_back(Permutation( 10, Transformation::Euler___0P___0Y_270R ));		/* 10001011 | 139 */
		Permutations.push_back(Permutation(  3, Transformation::Euler_270P___0Y_180R ));		/* 10001100 | 140 */
		Permutations.push_back(Permutation(  4, Transformation::Euler___0P___0Y_270R ));		/* 10001101 | 141 */
		Permutations.push_back(Permutation( 11, Transformation::Euler___0P___0Y_180R ));		/* 10001110 | 142 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_180Y_180R, true ));	/* 10001111 | 143 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P_180Y__90R ));		/* 10010000 | 144 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_270Y_180R ));		/* 10010001 | 145 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P_270Y_270R ));		/* 10010010 | 146 */
		Permutations.push_back(Permutation( 11, Transformation::Euler___0P_270Y___0R ));		/* 10010011 | 147 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P_180Y__90R ));		/* 10010100 | 148 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P__90Y_270R, true ));	/* 10010101 | 149 */
		Permutations.push_back(Permutation( 13, Transformation::Euler___0P___0Y_270R, true ));	/* 10010110 | 150 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P__90Y__90R, true ));	/* 10010111 | 151 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_270Y__90R ));		/* 10011000 | 152 */
		Permutations.push_back(Permutation(  2, Transformation::Euler___0P_180Y__90R ));		/* 10011001 | 153 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P__90Y___0R, true ));	/* 10011010 | 154 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P__90Y___0R, true ));	/* 10011011 | 155 */
		Permutations.push_back(Permutation( 10, Transformation::Euler___0P_270Y_180R ));		/* 10011100 | 156 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P__90Y_270R, true ));	/* 10011101 | 157 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P_180Y_270R, true ));	/* 10011110 | 158 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P_180Y_270R, true ));	/* 10011111 | 159 */
		Permutations.push_back(Permutation(  5, Transformation::Euler___0P_180Y___0R ));		/* 10100000 | 160 */
		Permutations.push_back(Permutation( 14, Transformation::Euler_270P___0Y__90R, true ));	/* 10100001 | 161 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P__90Y___0R ));		/* 10100010 | 162 */
		Permutations.push_back(Permutation( 12, Transformation::Euler_270P___0Y_270R, true ));	/* 10100011 | 163 */
		Permutations.push_back(Permutation( 14, Transformation::Euler___0P___0Y___0R, true ));	/* 10100100 | 164 */
		Permutations.push_back(Permutation(  9, Transformation::Euler___0P__90Y___0R, true ));	/* 10100101 | 165 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P_270Y_270R, true ));	/* 10100110 | 166 */
		Permutations.push_back(Permutation(  8, Transformation::Euler___0P_270Y___0R, true ));	/* 10100111 | 167 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P_270Y_180R ));		/* 10101000 | 168 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P__90Y__90R, true ));	/* 10101001 | 169 */
		Permutations.push_back(Permutation( 13, Transformation::Euler__90P___0Y_270R, true ));	/* 10101010 | 170 */
		Permutations.push_back(Permutation(  7, Transformation::Euler_270P___0Y__90R, true ));	/* 10101011 | 171 */
		Permutations.push_back(Permutation( 12, Transformation::Euler__90P___0Y__90R, true ));	/* 10101100 | 172 */
		Permutations.push_back(Permutation(  8, Transformation::Euler__90P___0Y_180R, true ));	/* 10101101 | 173 */
		Permutations.push_back(Permutation(  7, Transformation::Euler__90P___0Y_270R, true ));	/* 10101110 | 174 */
		Permutations.push_back(Permutation( 15, Transformation::Euler___0P_180Y__90R ));		/* 10101111 | 175 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_180Y_270R ));		/* 10110000 | 176 */
		Permutations.push_back(Permutation(  4, Transformation::Euler___0P_180Y__90R ));		/* 10110001 | 177 */
		Permutations.push_back(Permutation( 10, Transformation::Euler___0P_180Y___0R ));		/* 10110010 | 178 */
		Permutations.push_back(Permutation(  3, Transformation::Euler_270P___0Y_270R, true ));	/* 10110011 | 179 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P___0Y_180R, true ));	/* 10110100 | 180 */
		Permutations.push_back(Permutation(  8, Transformation::Euler___0P__90Y___0R, true ));	/* 10110101 | 181 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P_270Y__90R, true ));	/* 10110110 | 182 */
		Permutations.push_back(Permutation( 15, Transformation::Euler_270P___0Y___0R ));		/* 10110111 | 183 */
		Permutations.push_back(Permutation( 11, Transformation::Euler___0P_180Y__90R ));		/* 10111000 | 184 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P__90Y__90R, true ));	/* 10111001 | 185 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P__90Y_180R, true ));	/* 10111010 | 186 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P__90Y_180R, true ));	/* 10111011 | 187 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P___0Y___0R, true ));	/* 10111100 | 188 */
		Permutations.push_back(Permutation( 15, Transformation::Euler___0P__90Y___0R ));		/* 10111101 | 189 */
		Permutations.push_back(Permutation(  6, Transformation::Euler___0P___0Y___0R, true ));	/* 10111110 | 190 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P_180Y_270R, true ));	/* 10111111 | 191 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P_180Y_180R ));		/* 11000000 | 192 */
		Permutations.push_back(Permutation( 16, Transformation::Euler_270P___0Y_180R ));		/* 11000001 | 193 */
		Permutations.push_back(Permutation( 16, Transformation::Euler___0P_180Y_180R ));		/* 11000010 | 194 */
		Permutations.push_back(Permutation( 13, Transformation::Euler___0P___0Y_180R, true ));	/* 11000011 | 195 */
		Permutations.push_back(Permutation(  3, Transformation::Euler_270P___0Y___0R ));		/* 11000100 | 196 */
		Permutations.push_back(Permutation( 12, Transformation::Euler__90P___0Y_270R, true ));	/* 11000101 | 197 */
		Permutations.push_back(Permutation( 10, Transformation::Euler___0P__90Y_180R ));		/* 11000110 | 198 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P_180Y___0R, true ));	/* 11000111 | 199 */
		Permutations.push_back(Permutation(  3, Transformation::Euler_270P___0Y__90R ));		/* 11001000 | 200 */
		Permutations.push_back(Permutation( 11, Transformation::Euler___0P_270Y_180R ));		/* 11001001 | 201 */
		Permutations.push_back(Permutation( 12, Transformation::Euler__90P___0Y_180R, true ));	/* 11001010 | 202 */
		Permutations.push_back(Permutation(  7, Transformation::Euler__90P___0Y___0R, true ));	/* 11001011 | 203 */
		Permutations.push_back(Permutation(  2, Transformation::Euler___0P___0Y_180R ));		/* 11001100 | 204 */
		Permutations.push_back(Permutation(  3, Transformation::Euler__90P___0Y_270R, true ));	/* 11001101 | 205 */
		Permutations.push_back(Permutation(  3, Transformation::Euler__90P___0Y_180R, true ));	/* 11001110 | 206 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P_180Y___0R, true ));	/* 11001111 | 207 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_180Y___0R ));		/* 11010000 | 208 */
		Permutations.push_back(Permutation( 10, Transformation::Euler___0P_180Y__90R ));		/* 11010001 | 209 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P___0Y__90R, true ));	/* 11010010 | 210 */
		Permutations.push_back(Permutation(  7, Transformation::Euler_270P___0Y___0R, true ));	/* 11010011 | 211 */
		Permutations.push_back(Permutation( 11, Transformation::Euler___0P_180Y_180R ));		/* 11010100 | 212 */
		Permutations.push_back(Permutation(  7, Transformation::Euler__90P___0Y__90R, true ));	/* 11010101 | 213 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P___0Y_270R, true ));	/* 11010110 | 214 */
		Permutations.push_back(Permutation(  6, Transformation::Euler___0P_180Y___0R, true ));	/* 11010111 | 215 */
		Permutations.push_back(Permutation(  4, Transformation::Euler___0P_180Y_180R ));		/* 11011000 | 216 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P__90Y_180R, true ));	/* 11011001 | 217 */
		Permutations.push_back(Permutation(  8, Transformation::Euler__90P___0Y_270R, true ));	/* 11011010 | 218 */
		Permutations.push_back(Permutation( 15, Transformation::Euler___0P__90Y__90R ));		/* 11011011 | 219 */
		Permutations.push_back(Permutation(  3, Transformation::Euler__90P___0Y___0R, true ));	/* 11011100 | 220 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P__90Y___0R, true ));	/* 11011101 | 221 */
		Permutations.push_back(Permutation( 15, Transformation::Euler__90P___0Y__90R ));		/* 11011110 | 222 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P_180Y___0R, true ));	/* 11011111 | 223 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_180Y__90R ));		/* 11100000 | 224 */
		Permutations.push_back(Permutation( 12, Transformation::Euler___0P___0Y___0R, true ));	/* 11100001 | 225 */
		Permutations.push_back(Permutation( 11, Transformation::Euler___0P__90Y__90R ));		/* 11100010 | 226 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P___0Y_180R, true ));	/* 11100011 | 227 */
		Permutations.push_back(Permutation(  4, Transformation::Euler___0P_180Y_270R ));		/* 11100100 | 228 */
		Permutations.push_back(Permutation(  8, Transformation::Euler__90P___0Y___0R, true ));	/* 11100101 | 229 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P_270Y_270R, true ));	/* 11100110 | 230 */
		Permutations.push_back(Permutation( 15, Transformation::Euler___0P_270Y___0R ));		/* 11100111 | 231 */
		Permutations.push_back(Permutation( 10, Transformation::Euler___0P_180Y_180R ));		/* 11101000 | 232 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P__90Y_270R, true ));	/* 11101001 | 233 */
		Permutations.push_back(Permutation(  7, Transformation::Euler___0P_270Y___0R, true ));	/* 11101010 | 234 */
		Permutations.push_back(Permutation(  6, Transformation::Euler___0P_270Y___0R, true ));	/* 11101011 | 235 */
		Permutations.push_back(Permutation(  3, Transformation::Euler__90P___0Y__90R, true ));	/* 11101100 | 236 */
		Permutations.push_back(Permutation( 15, Transformation::Euler__90P___0Y___0R ));		/* 11101101 | 237 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P_270Y___0R, true ));	/* 11101110 | 238 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P_180Y__90R, true ));	/* 11101111 | 239 */
		Permutations.push_back(Permutation(  2, Transformation::Euler___0P__90Y__90R ));		/* 11110000 | 240 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P___0Y___0R, true ));	/* 11110001 | 241 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P___0Y__90R, true ));	/* 11110010 | 242 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P___0Y_180R, true ));	/* 11110011 | 243 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P___0Y_180R, true ));	/* 11110100 | 244 */
		Permutations.push_back(Permutation( 15, Transformation::Euler___0P___0Y__90R ));		/* 11110101 | 245 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P___0Y_270R, true ));	/* 11110110 | 246 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P___0Y_270R, true ));	/* 11110111 | 247 */
		Permutations.push_back(Permutation(  3, Transformation::Euler___0P___0Y_270R, true ));	/* 11111000 | 248 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P__90Y_270R, true ));	/* 11111001 | 249 */
		Permutations.push_back(Permutation( 15, Transformation::Euler___0P___0Y___0R ));		/* 11111010 | 250 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P___0Y_180R, true ));	/* 11111011 | 251 */
		Permutations.push_back(Permutation(  1, Transformation::Euler___0P___0Y___0R, true ));	/* 11111100 | 252 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P___0Y__90R, true ));	/* 11111101 | 253 */
		Permutations.push_back(Permutation(  0, Transformation::Euler___0P___0Y___0R, true ));	/* 11111110 | 254 */
	}
}