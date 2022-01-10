## 用DES加密文件
> 其实还是个文件操作的练习

* `des.h`和`des.cpp`里面实现了DES对称算法的加密解密过程，加密解密API各有两套，
```cpp
int DesEnc(
        unsigned char *pInData,
        int            nInDataLen,
        unsigned char *pOutData,
        int           *pOutDataLen); 

int DesEnc_raw(
        unsigned char *pInData,
        int            nInDataLen,
        unsigned char *pOutData,
        int           *pOutDataLen);
```
其中`DesEnc()`实现加密不满足4KB的文件，`DesEnc_raw()`则加密大小正好为4KB的文件。这样**对于大文件的加密操作可以按4KB分成多份一份份加密**，最后输出到目标文件中即可。
解密算法同理，API也是两套。

* `test.cpp`测试文件中的API设计如下：
  * 加密文件
  ```cpp
    int encode_file(const char *fileInName, const char *fileOutName);
  ```
  * 解密文件
  ```cpp
    int decode_file(const char *fileInName, const char *fileOutName)
  ```
所有参数均为输入指针，其实就是文件名（传文件指针耦合性太高。

* 界面简陋，按提示来即可