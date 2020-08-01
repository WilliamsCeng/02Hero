#ifndef MY_AES_H
#define MY_AES_H
#include <ap_int.h>
/**
 * ���� p: ���ĵ��ַ������顣
 * ���� plen: ���ĵĳ���,���ȱ���Ϊ16�ı�����
 * ���� key: ��Կ���ַ������顣
 */
//void aes(char *p, int plen, char *key);
void aes_return(char *p, int plen, char *key);
/**
 * ���� c: ���ĵ��ַ������顣
 * ���� clen: ���ĵĳ���,���ȱ���Ϊ16�ı�����
 * ���� key: ��Կ���ַ������顣
 */
//void deAes(char *c, int clen, char *key);
void deAes_return(char *c, int clen, char *key);

/**
 * ���� bit128in: ����/���� ���ַ������顣
 * ���� in_char_len:
 * 		in_char_len[0]:bit128in�ĳ���,��char��Ϊ������λ���㣬���ȱ���Ϊ16�ı�����
 * 		in_char_len[1]: ���ܻ��ǽ��� ���� 0������1
 * 		in_char_len[2]:������Ϣ�ķ���
 * ���� key: ��Կ���ַ������顣
 */
struct my_stream_type {
	char data[16];
	ap_uint<1> user;		// USER signal
	ap_uint<1> last; 	// TLAST signal
};
//void AES_En_De(char *bit128in_out, int in_char_len[3], char *key );
void AES_En_De( my_stream_type* in_stream, my_stream_type* out_stream, int in_char_len[3], char key[16] );

#endif
