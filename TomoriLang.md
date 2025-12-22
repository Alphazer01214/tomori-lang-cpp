# TomoriLang - 灯语言

一个基于C++和命令行的自娱自乐语言。

我希望更接近伪代码，减少符号的使用，且能够自动推断变量类型。

# 文件规范

后缀名为`*.tl`。



# 编写规范

语句结束可使用分号`;`或换行。

注释用//



# 语句关键字

- `if`
- `elseif`
- `else`
- `then`
- `end`
- `loop`
- `return`
- `var`
- `continue`
- `break`
- `function`
- `have`
- `default`



# 逻辑关键字

- `and`
- `or`
- `not`
- `is` ：等价于 == ，判断值是否相等。TomoriLang没有==，只有单个=用于赋值。
- `true / false`



# 运算符

- `+ - * /`
- `**` 
- `^ & |`
- `=`



## 运算符优先级

```tl
1. ()
2. **
3. * /
4. + -
5. < > is
6. not
7. and
8. or
```



# 变量类型

类型自动推断，但设计如下变量类型：

`````
INTEGER    使用 long long 存储
STRING     使用 std::string
FLOAT      使用 double
BOOLEAN    使用 bool
LIST       使用 std::vector
VOID
`````

**变量类型现在还不考虑，跳过**



# 函数库

\*.tlib文件，定义函数。

这个类似于C的\*.h

## tomorin.tlib - 基本输入输出

```tl
function input then
// 从console读取一个string
return string end
```



```tl
function gugugaga have string then
// 标准输出 string（或者其他类型变量）
end
```



# Parser 可靠性测试





# 案例

## 定义变量

int, double之类的是变量名，不是类型。

```tl
var int = 114514              // INTEGER
var double = 114.514          // FLOAT
var string = '114514'         // STRING
var bool = true               // BOOLEAN
```



## 定义函数

带参数：`function...have...then...return...end` / `function...have...then...end`

```tl
function adder have x y then return x + y end
```

不带参数： `function...then...return...end` / `function...have...then...end`

可以指定初始值/变量类型**(暂时不考虑实现这个)**：

```tl
function adder is INTEGER have x is INTEGER default 114 y is INTEGER default 514 then return x + y end
// 一个叫adder的函数接受 x y 两个整数变量，如果不指定那就是114和514，返回类型是整数，为x+y的值
```



## 条件语句

一个`if...then...end` 组成完整条件判断

注意：所有then都要对应一个end

```tl
if x > y and x > 0 then gugugaga('114514') end
```

还支持`if...elseif...else...end`

```tl
if x < y then gugugaga('bigger') elseif x is y then gugugaga('equal') else gugugaga('less') end
```



## 循环语句

无条件循环

```tl
loop then gugugaga('loop forever') end
```

有条件循环

```tl
loop if x < y then x = x + 1 end
```





# 实现