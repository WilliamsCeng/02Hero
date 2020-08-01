/*
*
* 站在前人的肩膀上 https://blog.csdn.net/qq_28205153/article/details/55798628
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ap_int.h>

//#include "aes.h"
// 定义缓冲区  一次存1280 * 720 * 3 = 2764800 个 bytes 的信息 除以16，就是 172800
// 2764800 资源爆了 /8  = 345600
#define Max_bytes 345600
char bit128in_buffer[ Max_bytes ];

static char temp_key[16];

struct my_stream_type {
	char data[16];
	ap_uint<1> user;		// USER signal
	ap_uint<1> last; 	// TLAST signal
};

/**
 * S盒
 */
static const int S[16][16] = { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

/**
 * 逆S盒
 */
static const int S2[16][16] = { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };


static int getNumFromSBox(char index) {
	char row =   (index & 0xf0)>>4; // getLeft4Bit(index);
	char col =  index & 0x0f;  //getRight4Bit(index);
	return S[row][col];
}


/**
 * 把一个字符转变成整型
 */
static int getIntFromChar(char c) {
	int result = (int) c;
	return result & 0x000000ff;
}

/**
 * 把16个字符转变成4X4的数组，
 * 该矩阵中字节的排列顺序为从上到下，
 * 从左到右依次排列。
 */
static void convertToIntArray(char *str, int pa[4][4]) {
	int k = 0;
	int i,j;
	convertToIntArray_label0:for(i = 0; i < 4; i++)
		convertToIntArray_label1:for(j = 0; j < 4; j++) {
			pa[j][i] = getIntFromChar(str[k]);
			k++;
		}
}


/**
 * 把连续的4个字符合并成一个4字节的整型
 */
static int getWordFromStr(char *str) {
	int one, two, three, four;
	one = getIntFromChar(str[0]);
	one = one << 24;
	two = getIntFromChar(str[1]);
	two = two << 16;
	three = getIntFromChar(str[2]);
	three = three << 8;
	four = getIntFromChar(str[3]);
	return one | two | three | four;
}

/**
 * 把一个4字节的数的第一、二、三、四个字节取出，
 * 入进一个4个元素的整型数组里面。
 */
static void splitIntToArray(int num, int array[4]) {
	int one, two, three;
	one = num >> 24;
	array[0] = one & 0x000000ff;
	two = num >> 16;
	array[1] = two & 0x000000ff;
	three = num >> 8;
	array[2] = three & 0x000000ff;
	array[3] = num & 0x000000ff;
}

/**
 * 将数组中的元素循环左移step位
 */
static void leftLoop4int(int array[4], int step) {
	int temp[4];
	int i;
	int index;
	leftLoop4int_label3:for(i = 0; i < 4; i++)
		temp[i] = array[i];

	index = step % 4 == 0 ? 0 : step % 4;
	leftLoop4int_label2:for(i = 0; i < 4; i++){
		array[i] = temp[index];
		index++;
		index = index % 4;
	}
}

/**
 * 把数组中的第一、二、三和四元素分别作为
 * 4字节整型的第一、二、三和四字节，合并成一个4字节整型
 */
static int mergeArrayToInt(int array[4]) {
	int one = array[0] << 24;
	int two = array[1] << 16;
	int three = array[2] << 8;
	int four = array[3];
	return one | two | three | four;
}

/**
 * 常量轮值表
 */
static const unsigned int Rcon[10] = { 0x01000000, 0x02000000,
	0x04000000, 0x08000000,
	0x10000000, 0x20000000,
	0x40000000, 0x80000000,
	0x1b000000, 0x36000000 };
/**
 * 密钥扩展中的T函数
 */
static int T(int num, int round) {
	int numArray[4];
	int i;
	int result;
	splitIntToArray(num, numArray);
	leftLoop4int(numArray, 1);//字循环

	//字节代换
	T_label4:for(i = 0; i < 4; i++)
		numArray[i] = getNumFromSBox(numArray[i]);

	result = mergeArrayToInt(numArray);
	return result ^ Rcon[round];
}

//密钥对应的扩展数组
int w[44]; // static int w[44]; 有static 之后，加密会出问题


/**
 * 扩展密钥，结果是把w[44]中的每个元素初始化
 */
static void extendKey(char *key) {
	int i,j;
	extendKey_label5:for(i = 0; i < 4; i++)
		w[i] = getWordFromStr(key + i * 4);

	extendKey_label0:
	for(i = 4, j = 0; i < 44; i++) {
		if( i % 4 == 0) {
			w[i] = w[i - 4] ^ T(w[i - 1], j);
			j++;//下一轮
		}else {
			w[i] = w[i - 4] ^ w[i - 1];
		}
	}

}

/**
 * 轮密钥加
 */
static void addRoundKey(int array[4][4], int round) {
	int warray[4];
	int i,j;
	addRoundKey_label0:for(i = 0; i < 4; i++) {

		splitIntToArray(w[ round * 4 + i], warray);

		addRoundKey_label6:for(j = 0; j < 4; j++) {
			array[j][i] = array[j][i] ^ warray[j];
		}
	}
}

/**
 * 字节代换
 */
static void subBytes(int array[4][4]){
	int i,j;
	subBytes_label0:for(i = 0; i < 4; i++)
		subBytes_label7:for(j = 0; j < 4; j++)
			array[i][j] = getNumFromSBox(array[i][j]);
}

/**
 * 行移位
 */
static void shiftRows(int array[4][4]) {
	int rowTwo[4], rowThree[4], rowFour[4];
	int i;
	shiftRows_label1:for(i = 0; i < 4; i++) {
		rowTwo[i] = array[1][i];
		rowThree[i] = array[2][i];
		rowFour[i] = array[3][i];
	}

	leftLoop4int(rowTwo, 1);
	leftLoop4int(rowThree, 2);
	leftLoop4int(rowFour, 3);

	shiftRows_label8:for(i = 0; i < 4; i++) {
		array[1][i] = rowTwo[i];
		array[2][i] = rowThree[i];
		array[3][i] = rowFour[i];
	}
}

/**
 * 列混合要用到的矩阵
 */
static const char colM[4][4] = { 2, 3, 1, 1,
	1, 2, 3, 1,
	1, 1, 2, 3,
	3, 1, 1, 2 };

static int GFMul2(int s) {
	int result = s << 1;
	int a7 = result & 0x00000100;

	if(a7 != 0) {
		result = result & 0x000000ff;
		result = result ^ 0x1b;
	}

	return result;
}

static int GFMul3(int s) {
	return GFMul2(s) ^ s;
}

static int GFMul4(int s) {
	return GFMul2(GFMul2(s));
}

static int GFMul8(int s) {
	return GFMul2(GFMul4(s));
}

static int GFMul9(int s) {
	return GFMul8(s) ^ s;
}

static int GFMul11(int s) {
	return GFMul9(s) ^ GFMul2(s);
}

static int GFMul12(int s) {
	return GFMul8(s) ^ GFMul4(s);
}

static int GFMul13(int s) {
	return GFMul12(s) ^ s;
}

static int GFMul14(int s) {
	return GFMul12(s) ^ GFMul2(s);
}

/**
 * GF上的二元运算
 */
static int GFMul(int n, int s) {
	int result;

	if(n == 1)
		result = s;
	else if(n == 2)
		result = GFMul2(s);
	else if(n == 3)
		result = GFMul3(s);
	else if(n == 0x9)
		result = GFMul9(s);
	else if(n == 0xb)//11
		result = GFMul11(s);
	else if(n == 0xd)//13
		result = GFMul13(s);
	else if(n == 0xe)//14
		result = GFMul14(s);

	return result;
}
/**
 * 列混合
 */
static void mixColumns(int array[4][4]) {

	int tempArray[4][4];
	int i,j;
	mixColumns_label4:for(i = 0; i < 4; i++)
		mixColumns_label9:for(j = 0; j < 4; j++)
			tempArray[i][j] = array[i][j];

	mixColumns_label3:for(i = 0; i < 4; i++)
		mixColumns_label10:for(j = 0; j < 4; j++){
			array[i][j] = GFMul(colM[i][0],tempArray[0][j]) ^ GFMul(colM[i][1],tempArray[1][j])
				^ GFMul(colM[i][2],tempArray[2][j]) ^ GFMul(colM[i][3], tempArray[3][j]);
		}
}
/**
 * 把4X4数组转回字符串
 */
static void convertArrayToStr(int array[4][4], char *str) {
	int i,j;
	convertArrayToStr_label2:for(i = 0; i < 4; i++)
		convertArrayToStr_label11:for(j = 0; j < 4; j++)
			*str++ = (char)array[j][i];
}

/**
 * 参数 p: 明文的字符串数组。
 * 参数 plen: 明文的长度。
 * 参数 key: 密钥的字符串数组。
 */
void aes_return(char *p, int plen){

	int pArray[4][4];
	int k,i;

//	extendKey(key);//扩展密钥

	aes_return_label23:for(k = 0; k < plen; k += 16) {
		convertToIntArray(p + k, pArray);

		addRoundKey(pArray, 0);//一开始的轮密钥加

		aes_return_label12:for(i = 1; i < 10; i++){

			subBytes(pArray);//字节代换

			shiftRows(pArray);//行移位

			mixColumns(pArray);//列混合

			addRoundKey(pArray, i);

		}

		subBytes(pArray);//字节代换

		shiftRows(pArray);//行移位

		addRoundKey(pArray, 10);

		convertArrayToStr(pArray, p + k);
	}
//	return info;
}


static int getNumFromS1Box(char index) {
	char row =   (index & 0xf0)>>4; // getLeft4Bit(index);
	char col =  index & 0x0f;  //getRight4Bit(index);
	return S2[row][col];
}

/**
 * 逆字节变换
 */
static void deSubBytes(int array[4][4]) {
	int i,j;
	deSubBytes_label1:for(i = 0; i < 4; i++)
		deSubBytes_label13:for(j = 0; j < 4; j++)
			array[i][j] = getNumFromS1Box(array[i][j]);
}
/**
 * 把4个元素的数组循环右移step位
 */
static void rightLoop4int(int array[4], int step) {
	int temp[4];
	int i;
	int index;
	rightLoop4int_label14:for(i = 0; i < 4; i++)
		temp[i] = array[i];

	index = step % 4 == 0 ? 0 : step % 4;
	index = 3 - index;
	rightLoop4int_label15:for(i = 3; i >= 0; i--) {
		array[i] = temp[index];
		index--;
		index = index == -1 ? 3 : index;
	}
}

/**
 * 逆行移位
 */
static void deShiftRows(int array[4][4]) {
	int rowTwo[4], rowThree[4], rowFour[4];
	int i;
	deShiftRows_label16:for(i = 0; i < 4; i++) {
		rowTwo[i] = array[1][i];
		rowThree[i] = array[2][i];
		rowFour[i] = array[3][i];
	}

	rightLoop4int(rowTwo, 1);
	rightLoop4int(rowThree, 2);
	rightLoop4int(rowFour, 3);

	deShiftRows_label17:for(i = 0; i < 4; i++) {
		array[1][i] = rowTwo[i];
		array[2][i] = rowThree[i];
		array[3][i] = rowFour[i];
	}
}
/**
 * 逆列混合用到的矩阵
 */

static const char deColM[4][4] = { 0xe, 0xb, 0xd, 0x9,
	0x9, 0xe, 0xb, 0xd,
	0xd, 0x9, 0xe, 0xb,
	0xb, 0xd, 0x9, 0xe };

/**
 * 逆列混合
 */
static void deMixColumns(int array[4][4]) {
	int tempArray[4][4];
	int i,j;
	deMixColumns_label5:for(i = 0; i < 4; i++)
		deMixColumns_label18:for(j = 0; j < 4; j++)
			tempArray[i][j] = array[i][j];

	deMixColumns_label6:
	for(i = 0; i < 4; i++){
		//因为deColM都是比较大的数，所以会反复迭代很多次 列混合计算中的二元运算GF
		//因此，不能随便对这里进行Unroll或者pipeline，会消耗巨大的资源
		deMixColumns_label3:for(j = 0; j < 4; j++){
			array[i][j] = GFMul(deColM[i][0],tempArray[0][j]) ^ GFMul(deColM[i][1],tempArray[1][j])
				^ GFMul(deColM[i][2],tempArray[2][j]) ^ GFMul(deColM[i][3], tempArray[3][j]);
		}
	}

}

/**
 * 把两个4X4数组进行异或
 */
static void addRoundTowArray(int aArray[4][4],int bArray[4][4]) {
	int i,j;
	addRoundTowArray_label2:for(i = 0; i < 4; i++)
		addRoundTowArray_label20:for(j = 0; j < 4; j++)
			aArray[i][j] = aArray[i][j] ^ bArray[i][j];
}
/**
 * 从4个32位的密钥字中获得4X4数组，
 * 用于进行逆列混合
 */
static void getArrayFrom4W(int i, int array[4][4]) {
	int index,j;
	int colOne[4], colTwo[4], colThree[4], colFour[4];
	index = i * 4;
	splitIntToArray(w[index], colOne);
	splitIntToArray(w[index + 1], colTwo);
	splitIntToArray(w[index + 2], colThree);
	splitIntToArray(w[index + 3], colFour);

	getArrayFrom4W_label21:for(j = 0; j < 4; j++) {
		array[j][0] = colOne[j];
		array[j][1] = colTwo[j];
		array[j][2] = colThree[j];
		array[j][3] = colFour[j];
	}

}

/**
 * 参数 c: 密文的字符串数组。
 * 参数 clen: 密文的长度。
 * 参数 key: 密钥的字符串数组。
 */
void deAes_return(char *c, int clen) {
	int cArray[4][4];
	int keylen,k;

//	extendKey(key);//扩展密钥

	deAes_return_label24:for(k = 0; k < clen; k += 16) {
		int i;
		int wArray[4][4];

		convertToIntArray(c + k, cArray);
		addRoundKey(cArray, 10);

		deAes_return_label22:for(i = 9; i >= 1; i--) {
			deSubBytes(cArray);

			deShiftRows(cArray);

			deMixColumns(cArray);
			//因为deColM都是比较大的数，所以会反复迭代很多次 列混合计算中的二元运算GF
     		//因此，不能随便对这里进行Unroll或者pipeline，会消耗巨大的资源
			getArrayFrom4W(i, wArray);
			deMixColumns(wArray);

			addRoundTowArray(cArray, wArray);
		}

		deSubBytes(cArray);

		deShiftRows(cArray);

		addRoundKey(cArray, 0);

		convertArrayToStr(cArray, c + k);

	}
//	return info;
}


/**
 * 参数 bit128in: 待加/解密 的字符串数组。
 * 参数 in_char_len:
 * 		in_char_len[0]:bit128in的长度,以char作为基本单位计算，长度必须为16的倍数。
 * 		in_char_len[1]: 加密还是解密 加密 0，解密1
 * 		in_char_len[2]:调试信息的返回
 * 参数 key: 密钥的字符串数组。
 */
////backup 避免重复extendkey
void AES_En_De_test(char bit128in_out[16], int in_char_len[3], char key[16] )
{
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=bit128in_out
	int char_len = in_char_len[0];
	int En_Decryption = in_char_len[1];
	in_char_len[2]=9;

	//避免重复 extendkey
	if( strcmp(temp_key,key)!=0 )  //strcmp 两字符串 相同时，返回 0
	{
		extendKey(key);//扩展密钥
		strcpy(temp_key,key);			//char* strcpy(char* strDestination, const char* strSource);
		printf("***extendkey\n");
	}

	if(En_Decryption == 0)
	{
		aes_return(bit128in_out, char_len);

	}
	else
	{
		deAes_return(bit128in_out, char_len);
	}

}

// //可用版 stream backup
//void AES_En_De( my_stream_type* in_stream, my_stream_type* out_stream, int in_char_len[3], char key[16] )
//{
//#pragma HLS INTERFACE s_axilite port=key
//#pragma HLS INTERFACE s_axilite port=in_char_len
//#pragma HLS ARRAY_PARTITION variable=w complete dim=1
//#pragma HLS PIPELINE enable_flush
//#pragma HLS INTERFACE axis register both port=out_stream
//#pragma HLS INTERFACE axis register both port=in_stream
//#pragma HLS INTERFACE s_axilite port=return
//
//
//	// pipeline enable_flush 如果流水线输入不有效，则清空流水线。
//
//	int char_len = in_char_len[0];
//	int En_Decryption = in_char_len[1];
//	in_char_len[2]=9;
//	int i;
//
//	my_stream_type input, output;
//	input = *in_stream;
//	output = input;
//
////	char bit128in_out[16];
////	for(int i =0;i<16;i++)
////		bit128in_out[i] = in_stream->data[i];
//
//	//避免重复 extendkey     使用这个时，需要把 w变量 的static 取消掉
//	// strcmp  strcpy HLS 不支持
////	if( strcmp(temp_key,key)!=0 )  //strcmp 两字符串 相同时，返回 0
////	{
////		extendKey(key);//扩展密钥
////		strcpy(temp_key,key);			//char* strcpy(char* strDestination, const char* strSource);
////		printf("***extendkey\n");
////	}
//
//	char key_change_flag = 0;
//	for(i = 0;i<16;i++)
//	{
//		if(temp_key[i]!=key[i])
//		{
//			temp_key[i]=key[i];
//			key_change_flag = 1;
//		}
//	}
//	if(key_change_flag==1)
//	{
//		extendKey(key);//扩展密钥
////		printf("***extendkey\n");
//	}
//
//
//	if(En_Decryption == 0)
//	{
//		aes_return(input.data, char_len);
//
//	}
//	else
//	{
//		deAes_return(input.data, char_len);
//	}
//
////	for(int i =0;i<16;i++)
////		out_stream->data[i] = bit128in_out[i];
//	*out_stream = input;
//}


//// 在函数内，定义了大的 输入缓冲区，报错，莫名其妙，放到开头定义了

void AES_En_De( my_stream_type* in_stream, my_stream_type* out_stream, int in_char_len[3], char key[16] )
{
#pragma HLS INTERFACE s_axilite port=key
#pragma HLS INTERFACE s_axilite port=in_char_len
#pragma HLS ARRAY_PARTITION variable=w complete dim=1
#pragma HLS PIPELINE enable_flush
#pragma HLS INTERFACE axis register both port=out_stream
#pragma HLS INTERFACE axis register both port=in_stream
#pragma HLS INTERFACE s_axilite port=return

	// pipeline enable_flush 如果流水线输入不有效，则清空流水线。

	int char_len = in_char_len[0];
	int En_Decryption = in_char_len[1];
	in_char_len[2]=9;
	int i,r;

	my_stream_type input, output;
#pragma HLS ARRAY_PARTITION variable=input.data complete dim=1
#pragma HLS ARRAY_PARTITION variable=output.data complete dim=1
//	input = *in_stream;
//	*out_stream = input; //  主要是为了给user 和 last 信号赋值

	// 输入 待加解密的 数据串的 缓冲区
    // 一次最多 存1280 * 720 * 3 = 2764800 个 bytes 的信息 除以16，就是 172800
	// 用完一定要清零啊，不然 一次传大的，后面传小的，小的就受影响了
//    char bit128in_buffer[ Max_bytes ]={0}; //在这里定义就出问题，报错，莫名其妙，放到开头了
#pragma HLS ARRAY_PARTITION variable=bit128in_buffer cyclic factor=16 dim=1
//#pragma HLS RESOURCE variable=bit128in_buffer core=RAM_2P_BRAM

//    int num_of_128bits = char_len/16;
	for( r = 0; r < char_len;  ){
		input = *in_stream;
		for( i =0; i< 16; i++){
#pragma HLS UNROLL
			bit128in_buffer[r] = input.data[i];
			r = r+1;
		}
		++in_stream;
	}

	//避免重复 extendkey     使用这个时，需要把 w变量 的static 取消掉
	char key_change_flag = 0;
	for(i = 0;i<16;i++)
	{
#pragma HLS PIPELINE rewind
		if(temp_key[i]!=key[i])
		{
			temp_key[i]=key[i];
			key_change_flag = 1;
		}
	}
	if(key_change_flag==1)
	{
		extendKey(key);//扩展密钥
	}


	if(En_Decryption == 0)
	{
		aes_return(bit128in_buffer, char_len);
	}
	else
	{
		deAes_return(bit128in_buffer, char_len);
	}


	for( r = 0; r < char_len;  ){
		for( i =0; i< 16; i++){
#pragma HLS UNROLL
			output.data[i] = bit128in_buffer[r] ;
			output.user = (r == 0 && i == 0)? 1: 0;
			output.last = (r == (char_len-1) && i == 15)? 1: 0;
			r = r+1;
		}
		*out_stream = output;
		++out_stream;
	}

}



//// 逐步改，找bug所在
//void AES_En_De( my_stream_type* in_stream, my_stream_type* out_stream, int in_char_len[3], char key[16] )
//{
//#pragma HLS INTERFACE s_axilite port=key
//#pragma HLS INTERFACE s_axilite port=in_char_len
//#pragma HLS ARRAY_PARTITION variable=w complete dim=1
//#pragma HLS PIPELINE enable_flush
//#pragma HLS INTERFACE axis register both port=out_stream
//#pragma HLS INTERFACE axis register both port=in_stream
//#pragma HLS INTERFACE s_axilite port=return
//
//
//	int char_len = in_char_len[0];
//	int En_Decryption = in_char_len[1];
//	in_char_len[2]=9;
//	int i,r;
//
//	my_stream_type input, output;
//#pragma HLS ARRAY_PARTITION variable=input.data complete dim=1
//#pragma HLS ARRAY_PARTITION variable=output.data complete dim=1
//	input = *in_stream;
//	output = input;
//
//
////    char bit128in_buffer[ 2764800 ]={0};
//#pragma HLS ARRAY_PARTITION variable=bit128in_buffer cyclic factor=16 dim=1
//#pragma HLS RESOURCE variable=bit128in_buffer core=RAM_2P_BRAM
//
//    int num_of_128bits = char_len/16;
//	char key_change_flag = 0;
//	for(i = 0;i<16;i++)
//	{
//		if(temp_key[i]!=key[i])
//		{
//			temp_key[i]=key[i];
//			key_change_flag = 1;
//		}
//	}
//	if(key_change_flag==1)
//	{
//		extendKey(key);//扩展密钥
//	}
//
//	if(En_Decryption == 0)
//	{
//		aes_return(input.data, char_len);
//	}
//	else
//	{
//		deAes_return(input.data, char_len);
//	}
//
//	*out_stream = input;
//}
