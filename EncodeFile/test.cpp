#include "des.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#define SIZE 4096
using namespace std;

int encode_file(const char *fileInName, const char *fileOutName) {
    /*
     * 加密文件fileInName（必须已存在）并将加密内容输出到fileOutName中
     * 输入参数均为文件名称
     */
    int ret = 0;
    unsigned char inbuf[1024 * 4] = {0}, outbuf[1024 * 4] = {0};
    int inlen = 0, outlen = 0, donelen = 0;

    if (fileInName == NULL || fileOutName == NULL) {
        ret = -1;
        cout << __FILE__ << "in line " << __LINE__ << "NULL pointer error: " << ret;
        return ret;
    }
    FILE *fp1 = fopen(fileInName, "rb");
    FILE *fp2 = fopen(fileOutName, "wb");
    if (fp1 == NULL || fp2 == NULL) {
        ret = -2;
        cout << __FILE__ << "in line " << __LINE__ << "open file error: " << ret;
        goto END;
    }
    while (!feof(fp1)) {
        memset(inbuf, 0, 4 * 1024);
        inlen = fread(inbuf, 1, SIZE, fp1);
        if (inlen < SIZE)
            break;

        DesEnc_raw(inbuf, inlen, outbuf, &outlen);
        //strcpy((char*)outbuf, (char*)inbuf);
        donelen = fwrite(outbuf, 1, outlen, fp2);
        if (outlen != donelen) {
            ret = -3;
            cout << __FILE__ << "in line " << __LINE__ << "write file error: " << ret;
            goto END;
        }

    }
    DesEnc(inbuf, inlen, outbuf, &outlen);
    //strcpy((char*)outbuf, (char*)inbuf);
    donelen = fwrite(outbuf, 1, outlen, fp2);
    if (outlen != donelen) {
        ret = -3;
        cout << __FILE__ << "in line " << __LINE__ << "write file error: " << ret;
        goto END;
    }

    END:
    if (fp1)
        fclose(fp1);
    if (fp2)
        fclose(fp2);
    return ret;
}

int decode_file(const char *fileInName, const char *fileOutName) {
    /*
     * 解密文件fileInName（必须已存在）并将解密内容输出到fileOutName中
     * 输入参数均为文件名称
     */
    int ret = 0;
    int inlen = 0, outlen = 0, donelen = 0;
    unsigned char inbuf[SIZE] = {0};
    unsigned char outbuf[SIZE] = {0};

    if (fileInName == NULL || fileOutName == NULL) {
        ret = -1;
        cout << __FILE__ << "in line " << __LINE__ << "NULL pointer error: " << ret;
        return ret;
    }
    FILE *fp1 = fopen(fileInName, "rb");
    FILE *fp2 = fopen(fileOutName, "wb");
    if (fp1 == NULL || fp2 == NULL) {
        ret = -2;
        cout << __FILE__ << "in line " << __LINE__ << "open file error: " << ret;
        goto END;
    }
    while (!feof(fp1)) {
        inlen = fread(inbuf, 1, SIZE, fp1);
        if (inlen < SIZE)
            break;
        DesDec_raw(inbuf, inlen, outbuf, &outlen);
        donelen = fwrite(outbuf, 1, outlen, fp2);
        if (donelen != outlen) {
            ret = -3;
            cout << __FILE__ << "in line " << __LINE__ << "write file error: " << ret;
            goto END;

        }
    }
    DesDec(inbuf, inlen, outbuf, &outlen);
    donelen = fwrite(outbuf, 1, outlen, fp2);
    if (donelen != outlen) {
        ret = -3;
        cout << __FILE__ << "in line " << __LINE__ << "write file error: " << ret;
        goto END;
    }
    END:
    if (fp1)
        fclose(fp1);
    if (fp2)
        fclose(fp2);
    return ret;
}

void display(){
    cout << "=========请输入需要的功能========\n";
    cout << "0：退出\n";
    cout << "1：加密文件\n";
    cout << "2：解密文件\n";
    cout << "3：清屏\n";
}

int Encode(){
    char fileIN[128] = {0};
    char fileOUT[128] = {0};
    int ret = 0;
    cout << "请输入需要加密的文件名称：";
    cin >> fileIN;
    cout << "请输入加密后输出文件名称：";
    cin >> fileOUT;
    ret = encode_file(fileIN, fileOUT);
    if(ret != 0)
        cout << "加密失败！\n";
    else
        cout << "加密成功！\n";
    return ret;
}

int Decode(){
    char fileIN[128] = {0};
    char fileOUT[128] = {0};
    int ret = 0;
    cout << "请输入需要解密的文件名称：";
    cin >> fileIN;
    cout << "请输入解密后输出文件名称：";
    cin >> fileOUT;
    ret = decode_file(fileIN, fileOUT);
    if(ret != 0)
        cout << "解密失败！\n";
    else
        cout << "解密成功！\n";
    return ret;
}

int main() {
    int cmd = 0;
    while(true){
        display();
        cin>>cmd;
        switch (cmd) {
            case 0:
                exit(0);
                break;
            case 1:
                Encode();
                break;
            case 2:
                Decode();
                break;
            case 3:
                system("cls");
                break;
            default:
                cout << "请输入数字0-3！\n";
        }
    }
}
