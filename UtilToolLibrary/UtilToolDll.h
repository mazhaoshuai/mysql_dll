/*************************************************************************
**																		**
** Copyright (C) 2019 The Future Company Ltd.							**
** Author:   Ma zhaoShuai, 17090833800@163.com							**
** Contact: https://blog.csdn.net/weixin_41913666						**
**																		**
** mysql�������														**
**																		**
**                                                   Date 20190727		**
*************************************************************************/
#ifndef __UTIL_TOOL_DLL_H_
#define __UTIL_TOOL_DLL_H_

#ifdef __cplusplus
/** Macro to start a block of C symbols */
#  define _C_START_           extern "C" {
/** Macro to end a block of C symbols */
#  define _C_END_             }
#else
#  define _C_START_
#  define _C_END_
#endif

#define __DLL___
#ifdef	__DLL___
#define DLL_  __declspec(dllexport)
#else
#define DLL_ __declspec(dllimport)
#endif

_C_START_


/** Alias for strcasecmp() == 0 */
#define Equal(a,b))              (stricmp((a),(b))==0)
/** Alias for strcasecmp() == 0 */
#define EQUAL(a,b)              (strcmp((a),(b))==0)
//���������
#define random(x) (rand()%x)


#define BYTE_4	4
#define BYTE_9	9
#define BYTE_16   16
#define BYTE_32   32
#define BYTE_64   64
#define BYTE_1024 1024
#define BYTE_2048 2048
#define BYTE_5120 5120
#define BYTE_10K 10240
#define BYTE_20K 20480
#define BYTE_128kb 131072//131072
#define BYTE_128_kb 131086//131072+14
#define TCP_FRAME_SIZE 4096

/** ���������͹ؼ��� */
typedef unsigned char			  byte;
typedef byte*					  bytes;
typedef unsigned short			  bit16;
typedef  int					  bit_32;
typedef unsigned int		      bit32;
typedef  long long				  bit_64;
typedef unsigned long long        bit64;


//base64����
DLL_ int base64_encode(unsigned char const*, unsigned int, unsigned char*&);
DLL_ int base64_decode(unsigned char const*, unsigned int, unsigned char*&);
//sha1����
DLL_ void calc(void const * src, size_t bytelength, unsigned char * hash);
//����uid
DLL_ char* create_guid();
//��ȡ��Ŀ¼
DLL_ int get_root_path(char*&root_path);
//ƴ��Ŀ¼
DLL_ int splicing_root_path(char* file_name, char*&complete_path);
//�ַ���תascii���ۼ�
DLL_ int ascii_value(char* str, int &add);
//λ�������
DLL_ unsigned int bitXOR(char* str, char* key, unsigned int &scill);
//λ�������
DLL_ unsigned int bitOR(char* str, char* key, unsigned int &scill);

/*
* @����������ȥ�ո񣬰���\r\n ��
* ����inbuf    ��ȥ��ǰ�ַ�����
* ����outbuf   ��ȥ�����ַ�����
* return        ״̬�롣
*/
DLL_  int trim_space(char* inbuf, char*&outbuf);

/*
* @��������ȡ�ַ����ַ����еĸ�����
* ����str  ��ԭʼ�ַ�����
* ����mark ����ȡ�ַ�����λ���ţ�һ���ַ������确������;
* return mark���ַ������ִ�����
*/
DLL_ int mark_count(char * str, const char * mark, int &count);

//�ͷŶ���ָ���ڴ�
DLL_ int free_(void** items);

//����ָ�����
DLL_ int pointer_count(void **papszs, int &items);

/*
* @������ԭ�л����ռ�󿪱ٿռ�׷�ӡ�
* ����list  ���ַ������ϣ�
* ����string   ����ӵ��ַ�����
* return strList׷�Ӻ�����ݡ�
*/
DLL_ char **add_buffer(char** list, const char *string);

/*
* @�������ַ�����ǽ�ȡ�������Զ�ȥͷβ��ǡ�
* ����str  ����ȡǰ�ַ�����
* ����mark   ���ַ�����λ���ţ�
* ����items   ����ӵ����ٺ��ڴ����ָ�룻
* return         ��ȡ�ļ�����
*/
DLL_ int mark_split(const char* str, const char* mark, char***items);

/*
* @�������ַ�����ǽ�ȡ������ͷβ���Ҳ���ۼӡ�
* ����str  ����ȡǰ�ַ�����
* ����mark   ���ַ�����λ���ţ�
* ����items   ����ӵ����ٺ��ڴ����ָ�룻
* return         ��ȡ�ļ�����
*/
DLL_ int split_char(const char* str, const char* mark, char***items);

//�ڴ��С��ȷ��������£��Զ����ٳ���һ�ε��ڴ�
DLL_ int append_new_buffer(char* &beginBytes, int &beginLength, char *nextBytes, int nextLength);

DLL_ void* memory_copy(void *dest, const void *src, int count);

//��ȡʱ�侫ȷ��΢��
DLL_ char* get_slip_time_milliseconds();
//��ȡʱ�侫ȷ������
DLL_ char* get_slip_time();
//��ȡʱ���
DLL_ char* get_time();



_C_END_
#endif
