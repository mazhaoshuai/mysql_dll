/*************************************************************************
**																		**
** Copyright (C) 2019 The Future Company Ltd.							**
** Author:   Ma zhaoShuai, 17090833800@163.com							**
** Contact: https://blog.csdn.net/weixin_41913666						**
**																		**
** mysql工具类库														**
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
//定义随机宏
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

/** 从命名类型关键字 */
typedef unsigned char			  byte;
typedef byte*					  bytes;
typedef unsigned short			  bit16;
typedef  int					  bit_32;
typedef unsigned int		      bit32;
typedef  long long				  bit_64;
typedef unsigned long long        bit64;


//base64加密
DLL_ int base64_encode(unsigned char const*, unsigned int, unsigned char*&);
DLL_ int base64_decode(unsigned char const*, unsigned int, unsigned char*&);
//sha1加密
DLL_ void calc(void const * src, size_t bytelength, unsigned char * hash);
//生成uid
DLL_ char* create_guid();
//获取根目录
DLL_ int get_root_path(char*&root_path);
//拼接目录
DLL_ int splicing_root_path(char* file_name, char*&complete_path);
//字符串转ascii码累加
DLL_ int ascii_value(char* str, int &add);
//位异或运算
DLL_ unsigned int bitXOR(char* str, char* key, unsigned int &scill);
//位或运算符
DLL_ unsigned int bitOR(char* str, char* key, unsigned int &scill);

/*
* @描述：左右去空格，包括\r\n 。
* 参数inbuf    ：去除前字符串；
* 参数outbuf   ：去除后字符串；
* return        状态码。
*/
DLL_  int trim_space(char* inbuf, char*&outbuf);

/*
* @描述：获取字符在字符串中的个数。
* 参数str  ：原始字符串；
* 参数mark ：截取字符串定位符号（一个字符，例如‘，’）;
* return mark在字符串出现次数。
*/
DLL_ int mark_count(char * str, const char * mark, int &count);

//释放二级指针内存
DLL_ int free_(void** items);

//二级指针计数
DLL_ int pointer_count(void **papszs, int &items);

/*
* @描述：原有基础空间后开辟空间追加。
* 参数list  ：字符串集合；
* 参数string   ：添加的字符串；
* return strList追加后的数据。
*/
DLL_ char **add_buffer(char** list, const char *string);

/*
* @描述：字符串标记截取计数，自动去头尾标记。
* 参数str  ：截取前字符串；
* 参数mark   ：字符串定位符号；
* 参数items   ：添加到开辟好内存二级指针；
* return         截取的计数。
*/
DLL_ int mark_split(const char* str, const char* mark, char***items);

/*
* @描述：字符串标记截取计数，头尾标记也会累加。
* 参数str  ：截取前字符串；
* 参数mark   ：字符串定位符号；
* 参数items   ：添加到开辟好内存二级指针；
* return         截取的计数。
*/
DLL_ int split_char(const char* str, const char* mark, char***items);

//内存大小不确定的情况下，自动开辟出下一次的内存
DLL_ int append_new_buffer(char* &beginBytes, int &beginLength, char *nextBytes, int nextLength);

DLL_ void* memory_copy(void *dest, const void *src, int count);

//获取时间精确到微秒
DLL_ char* get_slip_time_milliseconds();
//获取时间精确到毫秒
DLL_ char* get_slip_time();
//获取时间戳
DLL_ char* get_time();



_C_END_
#endif
