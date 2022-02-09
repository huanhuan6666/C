# C相关

## 杂
### 关于gcc/make
```cpp
gcc -E hello.c > hello.i //预处理
gcc -S hello.i -o hello.s //编译
gcc -c hello.s -o hello.o //汇编
gcc hello.o -o hello //链接生成可执行文件hello
//当然也可以一步到位：
gcc hello.c -o hello
```
用gcc和make都能编译生成可执行文件，make基本用法如下：
```cpp
make hello
```
会自动寻找hello.c文件并且`cc hello.c -o hello`，其中cc默认就是gcc。

make的强大之处在于可以设置复杂的编译选项，即**makefile文件**，需要手动编写。

### 关于makefile文件的编写

### 头文件一定要包
gcc的报错很圆滑，很多错误不报错而是报警告，比如下面的代码：
```cpp
#include<stdio.h>
#include<errno.h>
int main()
{
	FILE *fp;
	fp = fopen("tmp", "r");
	if(fp == NULL)
	{
		fprintf(stderr, "ERROR: %s", strerror(errno));
		exit(1);
	}
	puts("OK!");
	exit(0);
}
```
只是简单报警告如下：
```cpp
a.c:9:28: warning: format ‘%s’ expects argument of type ‘char *’, but argument 3 has type ‘int’ [-Wformat=]
   fprintf(stderr, "ERROR: %s", strerror(errno));
                           ~^   ~~~~~~~~~~~~~~~
                           %d
```
从这里可以看出，对于没有包头文件的库函数，编译器认为它**返回int**，从警告也能看出，虽然警告但是`./a.out`就会**段错误**。

因此警告的风险也很大，要千万记住库函数包好对应头文件：

最最常见的，`malloc`函数返回`void *`，空指针可以赋值给任何类型指针并且任何类型指针都能给空指针赋值。因此`(int *)malloc`的强转**完全多此一举**，警告多半是因为没有包`stdlib.h`头文件，**根本不需要强转**。

### 变量相关
#### 存储类型
变量定义的基本写法：[存储类型] 数据类型 变量名 = 值。

其中存储类型可省，存储类型分为`auto, static , register, extern(说明型)`。其中extern不是用于定义的而是**用于说明**的，说明为一个外部变量，定义在外部。

* auto类型：
就是**默认的**类型，`int a = 1;`则a就是个auto类型，**存储在栈上**，**自动**分配和回收内存。
* register类型：
建议型关键字，表示建议将变量直接放在寄存器里，但仅仅只是建议。`register int i = 0;`如果gcc确实把i分配到了寄存器上，那么i就无法查看地址了`&i`；并且只能定义**32位类型**的数据；并且**只能定义局部变量**，不能定义全局变量。

内联函数`inline`一样也是建议型关键字，到底内不内敛完全取决于gcc的判断。
* static类型：
静态类型，**自动初始化**0值或空值，`static`变量的值有**继承性**。因为**存储在.data段的确定空间**，**只定义一次**，因此值的改变也会确实体现在.data段，只要在程序中被修改那么修改一定会保留下来。而且`static`局部变量具有**全局寿命**，也就是说只有在程序结束时静态区的对应内存才会释放。

而auto类型的变量在栈中，程序运行时auto变量**连名字都没有**，全是寄存器压栈出栈操作，退栈后就灰飞烟灭了。

用例：

比如同一个项目的多个源文件中定义了**同名的全局变量**，那么就会报错**重定义**。如果想让每个源文件都用**本文件内定义的**全局变量，那么就用`static`修饰即可，即**本地全局变量**，**防止全局变量对外扩展**。

而`static`也可以修饰函数，作用和上面类似，如果不同源文件**函数同名的话**，那么用`static`修饰就保证使用的是本文件内的函数，**防止当前函数对外扩展**，如果我们还想用某个源文件内的本地函数，那么可以定义个**套皮**的普通函数，然后调用普通函数即可，因为**默认是对外扩展**的。

这就有点像C++类的`private`数据没法直接访问，所以定义public属性的`getattr`和`setattr`来访问私有数据，这其实**也是个套皮**。


* extern类型：
前三种都是定义型关键字，而extern是说明型，因此**不能改变变量的类型和值**，也就是说`extern int i = 100;`是错误的，只能`extern int i;`声明。

如果一个源文件中有`extern int i;`语句，这**不是定义而是声明**；表示本文件使用一个**外部的**允许拓展的全局变量i，在项目的其他文件中寻找i。


#### union共用体
和结构体struct不一样，结构体中的所有成员是**并列存在**的；而共用体则是成员之间**你死我活**，定义了一个成员后访问其他成员是没有意义的，因为他们**共享同一块内存**，内存大小按照**最大的**那个成员来分配。

```cpp
union name_un
{
	int a;
	int b;
	double c;
};
sizeof(nama_un) = 8; //这个共用体的大小就是double的大小8
```


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
int (*parr[3])[5] = {&a, &b, &c};
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
pfunc就是个**变量**，变量当然可以后面加上[3]写成数组的样子啊~

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

### 回调函数
**函数指针**作为函数的参数，（哈哈原来C语言也能函数式啊

我们有了函数指针变量，变量当然可以作为函数的参数啊，本质还是传指针：
```cpp
int add(int a, int b)
{
  return a + b;
}
int div(int a, int b)
{
  return a/b;
}
void myfunc(int a, int b, int(*pfunc)(int a, int b))
{
  int a = 0;
  a = pfunc(x, y); //这就是回调！
  printf("%d", a);
}
int main()
{
  int a = 1, b = 2;
  myfunc(a, b, add);
  myfunc(a, b, div);//所谓多态
}
```
突然觉得C语言有了回调也能实现多态。。
**多态**的核心就在于子类对于父类的虚函数重写以及父类指针或引用指向子类对象。

回调的话：
`myfunc(int a, int b, int(*pfunc)(int a, int b))`中的`int(*pfunc)(int a, int b)`参数相当于定义了一个虚函数，我们自己实现了`add`和`div`不就是虚函数重写么。调用`myfunc`就相当于父类指针去调用子类所实现的`pfunc`，传入参数不同就相当于父类指针指向不同的子类，这不就多态了么。

而且看样子还符合开闭原则，如果这样的话那回调属于是神中神了，哈哈面向对象太多了（

# C++相关

## C++的常量折叠
这个地方说法有点多，好像用指针修改const是UB，编译器不同表现也不同。。。我就按照自己实验来吧
按照C语言来讲的话，`const`修饰的全局变量将存储在**常量/只读区**,`const`修饰的局部变量将存储在**栈区**。并且C语言的常量本质上还是**变量**，编译之后可以通过指针修改，无法真正做到常量。
但是C++为了实现真正的常量，对C的const机制做了**加强**，也就是**常量折叠**，做法为：
* 建立**符号表**，声明`const`时就**放入符号表中**，之后访问常量时会进行替换，也就是**在编译阶段**，查找符号表对该变量进行值替换，同时，该常量**拥有自己的内存空间**。
  > 这部分内存分配在哪里？局部变量在栈区，全局变量在只读区。

**局部常量被折叠**的例子：
```cpp
int main()
{
	const int i = 5;
	int* p = (int*)(&i);
	*p = 2;
	printf("%d,%d,%p,%p", *p, i, p, &i);
	return 0;
}
```
这样的代码，最后输出`2,5,006FFC44,006FFC44`。输出挺奇怪，因为地址一样，但是输出的值反而不一样，怎么会事呢？
反汇编是这样的:
```assembly
	const int i = 5;
00E92102  mov         dword ptr [i],5  #可以看到是在栈区分配内存，把5赋值给i
	int* p = (int*)(&i);
00E92109  lea         eax,[i]  #lea传址，把i的地址传给eax再传给p
00E9210C  mov         dword ptr [p],eax  
	*p = 2;
00E9210F  mov         eax,dword ptr [p]  
00E92112  mov         dword ptr [eax],2  
	printf("%d,%d,%p,%p", *p, i, p, &i);
00E92118  lea         eax,[i]  
00E9211B  push        eax  
00E9211C  mov         ecx,dword ptr [p]  
00E9211F  push        ecx  
00E92120  push        5  #！！！这里压栈的直接就是5，也就是在编译期直接换成5了！！！
00E92122  mov         edx,dword ptr [p]  
00E92125  mov         eax,dword ptr [edx]  #*p还是照常寻址
00E92127  push        eax  
00E92128  push        offset string "S is " (0E99B30h)  
00E9212D  call        _printf (0E9146Ah)  
```
* 可以看到参数i压栈的时候就直接是5了，而不像参数* p 那样还要压变量。也就是所谓常量折叠，在编译期的时候i就被直接替换了。
* 有点像宏定义，因为读取的时候编译器进行替换优化，区别在于可以进行类型检测而不是简单的字符串替换
* 这里也说明了，局部常量放在了栈区，通过指针照样可以被修改，C语言就是这样。C++同样**无法避免**栈区的内存被修改，但是它增加了常量折叠的机制，使得即使内存被修改也没关系，因为编译期早就根据符号表替换了。

**全局常量**的例子：

全局常量分配在**只读区**，无法被修改。
比如：
```cpp
const int i = 0;
int main()
{
	int* p = (int*)(&i);
	*p = 2;
	printf("%d,%d,%p,%p", *p, i, p, &i);
	return 0;
}
```
编译可以通过，但是一运行就宕，因为修改只读区了。

> 关于常量还有个问题，就是比如：
> ```cpp
> int a = 0;
> int func(const int* p);
> int func1(const int &p); 
> ```
> 这样的函数我们调用时`func(&p)`和`func1(p)`都是正确的做法，但它们的类型是不一样的，一个`const int*`一个`int *`。
> 有关这个问题更深入的理解是关于ANSI C的**指针赋值**标准，事实上：
> ```cpp
> const int* p1;
> int* p2 = NULL;
> p1 = p2; //正确
> p2 = p1; //报错，无法将int* 转换成const int*
> ```
> 而下面这样
> ```cpp
> const int** p1;
> int** p2 = NULL;
> p1 = p2; //报错！
> p2 = p1; //报错，无法将int** 转换成const int**
> ```
> ANSI C标准规定，要想指针赋值成立必须满足：两个操作数都是指向有限定符或无限定符的**相容类型**的指针，左边指针所指向的类型必须具有右边指针所指向类型的全部限定符。
> 例一中`p1`和`p2`都指向`char`，因此相容，并且`p1`有`const`限定而`p2`没有，即`p1`包含`p2`的所有限定符，因此完全合法。
> 而例二中`p1`指向的是`const char*`变量，`p2`指向的是`char*`，两个指针不相容，并且`p1`和`p2`**都没有限定符**，注意`p1`的`const`和例一中的区别！不相容，所以赋值会报错！

## 引用
> 引用就是给内存起别名
> 使用引用一方面是为了方便间接赋值，C++编译器帮我们做了指针间接赋值的后两步而已。指针还是重中之重，为了方便可以用引用。
> 引用另一方面是为了链式编程，通过函数返回引用实现的。
### 基本概念
引用是C++新增的概念，变量名是一段连续**存储空间的别名**，**同一段内存**的别名可以有多个，用引用来实现。
即：引用可以看作一个已定义变量的别名
```cpp
int a = 10;
int &b = a;
b = 5;
cout << a;
```
输出`5`，a和b代表**同一段**内存。

引用最大意义和指针一样，都是作为函数参数来修改实参的值。只不过指针的间接赋值写起来比较麻烦：
* 定义实参和形参
* 实参**取地址**传给新参
* 在被调用函数中通过(* 形参)的方式间接修改实参的值

引用就简单多了：
* 形参设置成引用，实参**直接**传给形参
* 在被调用函数中，修改形参就直接修改了实参

引用的本质：
引用必须要初始化，这点就非常**像常量**。实际上引用就是个**指针常量**：
```cpp
int a = 5;
//C++
int &b = a;
b = 10;
//编译器看到的
int *const b = &a;
*b = 10;
```
C++编译器屏蔽了引用的实现细节而已，深究其修改实参的方法和指针的间接赋值完全一致。

### 引用做函数返回值
捏麻麻的，**不要返回局部对象的引用和地址！！！**
> 就是说，网上误人子弟的太多辣
返回引用和返回指针没什么本质区别，在release环境下编译
```cpp
int &func()
{
	int a = 10;
	return a;
}
int main()
{
	int ret = func();
	cout << ret;
}
```
是确实输出了10，但是
这个行为是UB！！！`func()`返回的是局部变量a的别名，而`func()`已经结束，事实上返回了对一个**已经被回收的栈内存的指针**然后使用，能够正确返回是因为那块内存还没有被覆盖掉，而且你又直接赋值给了ret，因此显示不出错误，但这是有风险的！

更有甚者：
```cpp
int &func()
{
	int a = 10;
	return a;
}
int main()
{
	int &ret = func();
	cout << ret;
	cout << ret; //debug编译下多输出几次就会出现乱码！
	cout << ret;
	cout << ret;
}
```
捏麻麻用引用去接局部变量的引用，这个比上个例子更哈人，上一个由于是赋值操作，ret确确实实值为10。这个相当于给局部变量a的内存起了个别名ret，而a的内存早就随着func结束释放掉了。就是说ret是在疯狂地访问一块未定义的内存。

debug编译下第一次输出确实是10，但是但凡你多输出几次就会出现乱码，因为你用ret访问的那块未定义的内存终于被冲烂辣嘻嘻。

返回引用安全的示例，也就是返回一个静态变量的引用，静态变量内存分配在全局区由系统接管，因此不必担心出现访问未定义的内存
可以当右值，也可以当左值。如：
```cpp
int &func()
{
	static int a = 0;
	return a;
}
int main()
{
	func() = 10; //可以做左值，因为返回的是a的别名，代表的就是a本身
	int ret = func(); //可以做右值，func()返回的是a的别名，代表的就是a，也就是和ret = a等价，简单的赋值操作，ret和a没有关系
	int& ret1 = func(); //把func()看作a本身即可，ret1是别名
	ret1 = 20; //将修改a的值
}
```
**把func()看作a本身即可**。

再有就是返回指针形参的引用，比如：
```cpp
int& g2(int* p) //
{
	*p = 100;
	return *p;
}
int main()
{
	int a1 = 10;
	int &a2 = g2(&a1);
}
```
这也是正确的做法，返回的就是形参指针**指向的变量本身**。关注内存的状况即可判断什么才是正确的行为。

### 指针的引用
C语言修改指针的话，间接赋值就得传二级指针了，指针的引用写法: &要和变量名**紧挨**
```cpp
int getNew(node* &p)
{
	p = new node;
	p->data = 14;
}
int main()
{
	node* myp = NULL;
	getNew(myp);
}
```
写引用的时候简单当个别名就OK了，出现奇怪现象的时候再去分析，它们只是**同一块内存的不同名称**而已。

### 常引用
常引用名只能读变量，不能修改变量，一般用于做**函数形参**。
```cpp
int x = 10;
const int &y = x;
y = 20; //error:不能通过y修改x
```
常引用更深的意义在于：**常引用兼容左值和右值**，比如重载赋值运算符时：
```cpp
Test operator=(Test& a);
```
这样的函数原型`t1 = t2 = t3`就会报错，因为`t2 = t3`返回一个匿名对象是**右值**，而函数参数是**非常量引用**，**非常量引用的初始值必须为左值**，因此类型不匹配。详见C++部分。

因此**常引用做函数参数**好处多多，嘻嘻。

## 函数重载&重写
### 重载
overload
* **返回值不是**函数重载的判断标准，只有参数的类型，个数和顺序才是。
* 重载函数是**相互独立**的函数，编译器在静态链编的时候就确定了各个函数，是一种**静态的多态**。

### 重写
override
