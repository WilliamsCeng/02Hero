#ifndef MY_AES_H
#define MY_AES_H
#include <ap_int.h>
/**
 * 参数 p: 明文的字符串数组。
 * 参数 plen: 明文的长度,长度必须为16的倍数。
 * 参数 key: 密钥的字符串数组。
 */
//void aes(char *p, int plen, char *key);
void aes_return(char *p, int plen, char *key);
/**
 * 参数 c: 密文的字符串数组。
 * 参数 clen: 密文的长度,长度必须为16的倍数。
 * 参数 key: 密钥的字符串数组。
 */
//void deAes(char *c, int clen, char *key);
void deAes_return(char *c, int clen, char *key);

/**
 * 参数 bit128in: 待加/解密 的字符串数组。
 * 参数 in_char_len:
 * 		in_char_len[0]:bit128in的长度,以char作为基本单位计算，长度必须为16的倍数。
 * 		in_char_len[1]: 加密还是解密 加密 0，解密1
 * 		in_char_len[2]:调试信息的返回
 * 参数 key: 密钥的字符串数组。
 */
struct my_stream_type {
	char data[16];
	ap_uint<1> user;		// USER signal
	ap_uint<1> last; 	// TLAST signal
};
//void AES_En_De(char *bit128in_out, int in_char_len[3], char *key );
void AES_En_De( my_stream_type* in_stream, my_stream_type* out_stream, int in_char_len[3], char key[16] );

#endif
