//
// Created by Administrator on 2022/1/8.
//
#ifndef TESTCONFIG_CONFIG_OP_H
#define TESTCONFIG_CONFIG_OP_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace std;

#define IN
#define OUT
#define MAXSIZE (1024)
#define FILEMAX_SIZE (8*1024)
//写配置项
int wirte_config(IN const char *filename, IN const char *key, IN const char *value);

//获取配置项
int read_config(IN const char *filename, IN const char *key, OUT char **value);


#endif //TESTCONFIG_CONFIG_OP_H
