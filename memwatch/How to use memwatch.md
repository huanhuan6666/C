如何检测内存泄漏？memwatch的做法大概就是在memwatch.h中定义了一系列宏，像这样：

```c
#define malloc(n)       mwMalloc(n,__FILE__,__LINE__)
#define strdup(p)       mwStrdup(p,__FILE__,__LINE__)
#define realloc(p,n)    mwRealloc(p,n,__FILE__,__LINE__)
#define calloc(n,m)     mwCalloc(n,m,__FILE__,__LINE__)
#define free(p)         mwFree(p,__FILE__,__LINE__)
```

也就是说使用malloc等其他内存操纵函数，在预处理阶段全都会被替换成自己实现的函数，操纵内存的信息也就在函数中被记录下来了，这样就实现了监控内存。

下面是VS中使用memwatch的方法，最后会生成一个日志文件来显示项目的内存分配过程。

1）把 memwatch.h 和 memwatch.c 拷贝到所在代码同级目录上

![img](E:\md_image\wps3F8.tmp.jpg) 

 

2）代码添加到项目中

![img](E:\md_image\wps409.tmp.jpg) 

 

3）配置环境

Windows平台vs编译器可以在预处理器宏定义加入MEMWATCH和DMW_STDIO两项

![img](E:\md_image\wps40A.tmp.jpg) 

![img](E:\md_image\wps40B.tmp.jpg) 

![img](E:\md_image\wps40C.tmp.jpg) 

![img](E:\md_image\wps40D.tmp.jpg) 

 

4）在需要检查内存泄露中的代码中包含头文件：#include "memwatch.h"

![img](E:\md_image\wps40E.tmp.jpg) 

 

5）编译运行代码，在当前路径会生成如下文件，打开即可查看内存泄露情况

![img](E:\md_image\wps40F.tmp.jpg) 

 