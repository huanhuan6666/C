## 配置文件读写

### 接口设计

* 写配置文件
```cpp
int wirte_config(const char* filename/*in*/, const char* key/*in*/, const char* value/*in*/);
```
* 读配置文件
```cpp
int read_config(const char* filename/*in*/, const char* key/*in*/, char** value/*out*/);
```
这里是`value`的获取是输出指针，利用间接赋值，被调用函数分配内存。当然直接用一级指针在主调函数中分配内存也可以，如下：
```cpp
int read_config(const char* filename/*in*/, const char* key/*in*/, char* value/*in*/);
```
* 修改配置文件
```cpp
int modify_config(ocnst char* filename/*int*/, const char* key/*in*/, char* new_value/*in*/);
```

> 接口设计要紧，因此修改配置文件和写配置文件应该合二为一，底层API实现时应该完成这部分工作：如果key存在则修改，如果不存在则添加新的项

因此最终只用`read_config()`和`write_config()`两个函数。


