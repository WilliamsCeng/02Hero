#include <stdio.h>
//#include "aes.h"
#include <ap_int.h>
// ����ͷ�ļ�

struct my_stream_type {
	char data[16];
	ap_uint<1> user;		// USER signal
	ap_uint<1> last; 	// TLAST signal
};
void AES_En_De( my_stream_type* in_stream, my_stream_type* out_stream, int in_char_len[3], char key[16] );
void AES_En_De_test(char bit128in_out[16], int in_char_len[3], char key[16] );

//axilite_and_16stream_main
int main(int argc, char const *argv[]) {
// ����, ����plain�������ַ����飬 len�ǳ��ȣ� key����Կ
char plain[17] = "abcdefghijklmnop";
int len = 16;
char key[17] = "hello_world_aes_";


//printf("���ģ�%s\n",plain);
//for(int i=0;i<16;i++)
//	printf("%x ",(unsigned char)plain[i]);
//printf("\n");
//
//printf("��Կ��%s\n",key);
//for(int i=0;i<16;i++)
//	printf("%x ",(unsigned char)key[i]);
//printf("\n");
//
//aes(plain, len, key);
//
//printf("���ģ�%s\n",plain);
//for(int i=0;i<16;i++)
//	printf("%x ",(unsigned char)plain[i]);
//printf("\n��������ĵ�hex�����֪�������ߵ�AES���ܽ����һ�µ�\n");
//
//
////���ܣ�����ciphertext�������ַ����飬 len�ǳ��ȣ� key����Կ
//// ciphertext
//deAes(plain, len, key);
//printf("���ģ�%s\n",plain);


/**
 * ���� bit128in: ����/���� ���ַ������顣
 * ���� in_char_len:
 * 		in_char_len[0]:bit128in�ĳ���,��char��Ϊ������λ���㣬���ȱ���Ϊ16�ı�����
 * 		in_char_len[1]: ���ܻ��ǽ��� ���� 0������1
 * 		in_char_len[2]:������Ϣ�ķ���
 * ���� key: ��Կ���ַ������顣
 */
//void AES_En_De_test(char *bit128in_out, int in_char_len[3], char *key )

//int in_char_len[3] = {16,0,0 };
//
//printf("���ģ�%s\n",plain);
//
//AES_En_De_test(plain,in_char_len,key);
//
//printf("���ģ�%s\n",plain);
//printf("\n���ĵ�hex��ʽ��\n");
//for(int i=0;i<16;i++)
//	printf("%x ",(unsigned char)plain[i]);
//printf("\n��������ĵ�hex�����֪�������ߵ�AES���ܽ����һ�µ�\n");
//
//in_char_len[1] = 1; //����
//
//AES_En_De_test(plain,in_char_len,key);
//printf("���ģ�%s\n",plain);
//printf("in_char_len[2]:������Ϣ�ķ��أ�%d\n",in_char_len[2]);



////void AES_En_De( my_stream_type* in_stream, my_stream_type* out_stream, int in_char_len[3], char key[16] )

int in_char_len[3] = {16,0,0 };
my_stream_type output_stream, input_stream;

printf("���ģ�%s\n",plain);

input_stream.user = 1;
input_stream.last = 1;

for(int i=0;i<16;i++)
	input_stream.data[i] = plain[i];

AES_En_De(&input_stream, &output_stream,in_char_len,key);

printf("���ģ�%s\n",output_stream.data);
printf("\n���ĵ�hex��ʽ��\n");
for(int i=0;i<16;i++)
	printf("%x ",(unsigned char)output_stream.data[i]);
printf("\n��������ĵ�hex�����֪�������ߵ�AES���ܽ����һ�µ�\n");

in_char_len[1] = 1; //����

for(int i=0;i<16;i++){
	input_stream.data[i] = output_stream.data[i];
	printf("%x ",(unsigned char)input_stream.data[i]);
}

AES_En_De(&input_stream, &output_stream,in_char_len,key);
printf("\n���ܺ����ģ�%s\n",output_stream.data);
printf("in_char_len[2]:������Ϣ�ķ��أ�%d\n",in_char_len[2]);

return 0;

}



int more_than16_stream_main(int argc, char const *argv[]) {
// ����, ����plain�������ַ����飬 len�ǳ��ȣ� key����Կ
char plain[33] = "abcdefghijklmnopabcdefghijklmnop";
const int len = 32;
char key[17] = "hello_world_aes_";

/**
 * ���� bit128in: ����/���� ���ַ������顣
 * ���� in_char_len:
 * 		in_char_len[0]:bit128in�ĳ���,��char��Ϊ������λ���㣬���ȱ���Ϊ16�ı�����
 * 		in_char_len[1]: ���ܻ��ǽ��� ���� 0������1
 * 		in_char_len[2]:������Ϣ�ķ���
 * ���� key: ��Կ���ַ������顣
 */
////void AES_En_De( my_stream_type* in_stream, my_stream_type* out_stream, int in_char_len[3], char key[16] )

int in_char_len[3] = {len,0,0 };
my_stream_type output_stream, input_stream;

printf("���ģ�%s\n",plain);
input_stream.user = 1;
for(int j=0;j<len;)
{
	for(int i=0;i<16;i++)
		input_stream.data[i] = plain[j];
	j++;
	input_stream.last = 1;
}
AES_En_De(&input_stream, &output_stream,in_char_len,key);

printf("���ģ�%s\n",output_stream.data);
printf("\n���ĵ�hex��ʽ��\n");
for(int i=0;i<16;i++)
	printf("%x ",(unsigned char)output_stream.data[i]);
printf("\n��������ĵ�hex�����֪�������ߵ�AES���ܽ����һ�µ�\n");

in_char_len[1] = 1; //����

for(int i=0;i<16;i++){
	input_stream.data[i] = output_stream.data[i];
	printf("%x ",(unsigned char)input_stream.data[i]);
}

AES_En_De(&input_stream, &output_stream,in_char_len,key);
printf("\n���ܺ����ģ�%s\n",output_stream.data);
printf("in_char_len[2]:������Ϣ�ķ��أ�%d\n",in_char_len[2]);

return 0;

}
