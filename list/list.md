## 指针，数组，函数

### 指针数组
```cpp
const char* argv = {"asd", "qwe", "zcx"};
```
其中`argv`就是个指针数组，因为`[]`的优先级比`*`的要高，因此认为它是一个数组，也就是指针数组。
比如说`main()`函数的参数
```cpp
int main(int argc, char* argv[], char** env)
{
  int i = 0;
  for(i = 0; i< argc; i++)
    printf("%s ", argv[i]);
   //自停止
  for(i = 0; argv[i] != NULL; i++)
    printf("%s ", argv[i]);
  for(i = 0; env[i] != NULL; i++)
    printf("%s ", env[i]);
}
```
* `argc`即`argument count`即参数个数, `argv`即`argument vector`即参数向量, `env`即环境
* `argv`就是个指针数组，数组里面存的都是`char*`指针，也就是一个个C风格的字符串，也就是在命令行里运行时输入的参数，默认`argv[0]`为文件名。可以看到`argc`和`argv`是典型的风格**内存打包**，即确定数组的入口地址和数组元素的个数。
* `env`其实和`argv` 没有区别，里面存放的是环境相关的东西，比如找动态库的`path`等等，可以在我的电脑里看到
* 完全可以把`char* argv[]`写成`char **argv`，因为**数组做函数参数显然要退化**，写成和`char **env`一样的形式，按自己的习惯来吧
  > 退化？
  > 比如
  > ```cpp
  > void test(int array[])
  > {
  >   printf("the array's size in test is %d\n", sizeof(array));
  > }
  > int main()
  > {
  >   int a[10];
  >   printf("the array's size in main is %d\n", sizeof(array))
  >   test(a);
  > }
  > ```
  > `test`将数组作为函数参数，最后输出结果为4，也就是说被调用函数中只认为它是一个简单的指针变量而已，而不像main函数中结果为40.这就是数组做函数参数的退化。
  > 因此就有了经典的内存打包技术，即首地址+元素个数
* 为什么`char **env`不用内存打包？即不用传入参数个数`argc`。其实`argv`和`env`约定好都是自停止的指针，就算不用`argc`的话`argv`也完全可以正确停止。
  > 但是`argc`必须要传给`main()`函数才行，因为函数类型已经确定了。
* 内存如何分配？那只能是命令行输入参数后由系统分配参数字符串的内存，然后调起`main()`函数并且由系统把对应的指针当作参数传进去(更深入的我也不清楚。

### 数组指针
三种定义方式：
```cpp
//1定义数组类型
typedef int(ARRAY)[5]; //定义了数组类型
ARRAY myarr = {0};
ARRAY* p1 = &marry

//2直接定义数组指针类型
typedef int(*P_ARRAY)[5];
P_ARRAY p2 = &myarr;

//3不用typedef直接定义
int (*p3)[5] = &myarr;
```
* 数组名代表**数组首元素的地址**，即`myarr`在数值上等于`&myarr[0]`，而且是**指针常量**，如果是变量那一修改都没办法正确析构内存了。可以把这个看成个**语法糖**罢，毕竟写`&myarr[0]`的话有点麻烦
* 数组的地址写法为`&myarr`。
* 这两个东西数值上相等，但是是不同类型的指针。它们的区别就在于**指针步长不一样**，毕竟一个指向**首元素**一个指向**整个数组**。
这个东西就有点像Liunx内核链表，比如
```cpp
#include "list.h"

struct person
{
    struct list_head list; //list_head定义在list.h中
    int age;
};
int main(int argc,char **argv)
{
    int i;
    struct person *p;
    struct list_head *pos;

    INIT_LIST_HEAD(&person1.list);

    for (i = 0;i < 5;i++) {
        p = (struct person *)malloc(sizeof(struct person ));
        p->age=i*10;
        list_add(&p->list,&person1.list);
    }

    list_for_each(pos, &person1.list) {
        printf("age = %d\n",((struct person *)pos)->age);
    }

    return 0;
}
```
重点在于`printf("age = %d\n",((struct person *)pos)->age);`这一行，根据`person`结构体的定义可以看到`list`的地址和结构体的地址数值上是一样的，这不就是和`myarr`和`&myarr`值相等是一回事吗。然后来个强制类型转换`(struct person *)pos)->age`就按照结构体去访问成员了。
当然这Linux内核链表更加复杂完善，具体看这里[Linux内核链表](https://www.cnblogs.com/yangguang-it/p/11667772.html)

### 数组指针的数组
```cpp
int a[5] = {0};
int b[5] = {0};
int c[5] = {0};
int (*parr)[5] = &a;
```
其中`parr`是个**指向数组的指针变量**，重点是**变量**。是变量就能写成数组：
```cpp
int (*parr[3])[5] = &a;
```
这样parr就是个数组，里面存了三个指针，每个指针指向一个int[5]类型的数组。

这和常用的`const char* argv[] = {"asd", "qwe", "zxc"}`没有区别，**都是指针数组**，只不过指针比较复杂，是一个指向数组的指针。
它们的区别还是很大的，`const char *agrv[]`的内存模型是栈区三个指针，指向的是**常量区**三个常量字符串。
而`int (*parr[3])[5]`则是内存全部分配到栈区，先是三个`int [5]`类型的数组`a, b, c`，占用`15*4`个字节，然后parr压三个指针，这三个指针分别指向`a, b, c`整个数组。
> 既然这样直接二维数组`int parr[3][5]`不好么。。。还省了三个数组指针的栈空间呢。这里就当个概念辨析吧（不会真有人这么写吧

### 指针函数
返回指针的函数，比如
```cpp
int *p mycreate(int num)
{
  int *p = (int *)malloc(sizeof(int)* num);
  return p;
}
```
害不过这样子C函数唯一的返回值就被占用了，我的评价是不如间接赋值，写C语言函数参数不用指针有点说不过去
### 函数指针
和数组指针挺像的，定义方法也差不多
```cpp
int add(int a, int b)
{
  return a+b;
}
//1定义函数类型
typedef int(FUNC)(int a, int b);
FUNC* pfunc1 = &add;

//2定义函数指针类型
typedef int(P_FUNC)(int a, int b);
P_FUNC pfunc2 = &add;

//3直接定义
int (*pfunc3)(int a, int b) = &add;
```
上面这样写就和数组指针写法完全一致了，但是C还允许
```cpp
//赋值时
pfunc = add;
pfunc = &add;
//调用时
pfunc(1, 2);
(*pfunc)(1, 2);
```
这两种**没有任何区别**，但是为了形式上的统一，还是按照数组指针那种写法吧。
* 再有一点就是函数指针的**步长**好像也无法确定，这也是和数组指针的区别。 如果尝试输出`pfunc+1`时会报错`非法，左操作数包含“FUNC (__cdecl *)”类型`.

### 函数指针的数组
和之前数组指针的数组基本一致，
```cpp
int add();
int min();
int div();
int (*pfunc)() = &add;//函数指针
int (*pfuncarr[3])() = {&add, &min, &div};//函数指针的数组
```
这玩意的用处好像还大一点，如果多个条件下调用的函数类型一致，那么可以做成个“函数表”，下面这么写更方便：
```cpp
int (*pfuncarr[3])() = {add, min, div};
if(...)
  pfuncarr[0](); //调用add
else if(...)
  pfuncarr[1](); //调用min
else
  pfuncarr[2](); //调用div
```
