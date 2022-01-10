//
// Created by Administrator on 2022/1/8.
//
#include "config_op.h"

//写配置项
int wirte_config(const char *filename/*in*/, const char *key/*in*/, const char *value/*in*/) {
    int     ret = 0, key_exist = 0;
    FILE    *fp = NULL;
    char    linebuf[MAXSIZE]; //行buffer
    char    filebuf[FILEMAX_SIZE]; //文件buffer
    if(filename == NULL || key == NULL || value == NULL)
    {
        ret = -1;
        cout << "NULL pointer EORROR:" << ret << endl;
        goto END;
    }
    fp = fopen(filename, "r+");
    if(fp == NULL) {
        ret = -2;
        cout << "open file EORROR:" << ret << endl;
        goto END;
    }
    // 设置修改后的文件内容filebuf
    while(!feof(fp)) {
        memset(linebuf, 0, MAXSIZE);
        fgets(linebuf, MAXSIZE, fp);
        if (strstr(linebuf, key) != NULL) {
            memset(linebuf, 0, MAXSIZE);
            sprintf(linebuf, "%s = %s\n", key, value); //format中的"\n"结束标志不要忘！
            key_exist = 1;
        }
        strcat(filebuf, linebuf);
    }

    if(key_exist == 0){ //加到文件末尾
        fprintf(fp, "%s = %s", key, value);
    }
    else{ //修改对应项，w清空文件后重写内容
        fp = fopen(filename, "w+");
        if(fp == NULL){
            ret = -2;
            cout << "open file EORROR:" << ret << endl;
            goto END;
        }
        fputs(filebuf, fp);
    }

    END:
    if(fp != NULL)
        fclose(fp);

    return ret;
}

//获取配置项
int read_config(const char *filename/*in*/, const char *key/*in*/, char **value/*out*/) {
    int ret = 0;
    FILE *fp = NULL;
    char buf[MAXSIZE] = {0};
    char *ptmp = NULL, *res = NULL;
    if (filename == NULL || key == NULL || value == NULL) {
        ret = -1;
        cout << "NULL pointer EORROR:" << ret << endl;
        goto END;
    }
    fp = fopen(filename, "r+");
    if (fp == NULL) {
        ret = -2;
        cout << "open file EORROR:" << ret << endl;
        goto END;
    }
    while (!feof(fp)) {
        memset(buf, 0, MAXSIZE);
        fgets(buf, MAXSIZE, fp);

        ptmp = strstr(buf, key);
        if (ptmp == NULL)
            continue;
        ptmp = strchr(buf, '=');
        if (ptmp == NULL) //没有等号
            continue;
        else {
            ptmp += 1;
            res = (char *) malloc(sizeof(char) * 30);
            strcpy(res, ptmp);
            *value = res;
            break;
        }
    }

    END:
    if (fp != NULL)
        fclose(fp);
    return ret;
}