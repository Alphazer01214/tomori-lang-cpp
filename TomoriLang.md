# TomoriLang - 灯语言

一个基于C++和命令行的自娱自乐语言。

我希望更接近伪代码，减少符号的使用，且能够自动推断变量类型。

# 文件规范

后缀名为`*.tl`。



# 编写规范

语句结束可使用分号`;`或换行。

注释用//

# 关键字

## 语句关键字

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



## 逻辑关键字

- `and`
- `or`
- `not`
- `is` ：等价于 == ，判断值是否相等。TomoriLang没有==，只有单个=用于赋值。
- `isnot` 等价于 !=
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
9. =
```



# 变量类型

类型自动推断，但设计如下变量类型：

`````
Integer    使用 long long 存储
String     使用 std::string
Float      使用 double
Boolean    使用 bool
List       使用 std::vector
Mygo       使用 void
`````



# 函数库

这个类似于C的\*.h

## tomorin.tl - 基本输入输出

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

## 条件语句

```tl
if a then b elseif c then d else e end
if a then b elseif c then d end
if x isnot 1 then x = 1 end
if not (x is 1) then x = 1 end
```

## 函数定义

```
function func then end
function acceed have x y then return end
function acceed have x y then return x + y end 

// recursive function
function acceed have x then
if x is 1 then return 0
else return acceed(x-1) + acceed(x-2) end
end
// end
```

## 函数调用

```tl
func(a, b, c)
func(fun(a is b, c isnot d), e)
```



## 循环语句

```tl
loop then
    gugugaga('gugugaga')
end

var i = 0
loop i < 5 then
    gugugaga('i = ' + i)
    i = i + 1
end

var x = 0
loop x < 3 then
    var y = 0
    loop y < 2 then
        gugugaga('x=' + x + ', y=' + y)
        y = y + 1
    end
    x = x + 1
end

loop x < 3 then
if x is 2 then break end
if x isnot 1 then continue end
end
```



## 一个比较完整的程序

```tl
// 导入基础库
// tomorin.tl 提供了 gugugaga (输出) 和 input (输入)

// --- 函数定义：计算斐波那契数列 (展示递归与条件分支) ---
function get_member_energy have days then
    if days is 0 then
        return 0
    elseif days is 1 or days is 2 then
        return 1
    else
        // 递归调用
        return get_member_energy(days - 1) + get_member_energy(days - 2)
    end
end

// --- 函数定义：判断练习状态 (展示逻辑运算符与字符串) ---
function check_practice_status have member_name energy then
    var threshold = 10
    if energy > threshold and member_name isnot 'Soyo' then
        return member_name + ' is ready to practice!'
    elseif member_name is 'Soyo' then
        return 'Soyo is drinking tea...'
    else
        return member_name + ' needs more rest.'
    end
end

// --- 主程序逻辑 ---

gugugaga('--- Welcome to TomoriLang Interpreter ---')

// 1. 变量定义与自动推断
var member = 'Anon'           // String
var practice_days = 7         // Integer
var performance_ratio = 0.85  // Float
var is_active = true          // Boolean

// 2. 循环与计算
gugugaga('Calculating energy for ' + member + '...')

var total_energy = 0
var i = 1
loop i < practice_days then
    // 调用递归函数
    var daily_boost = get_member_energy(i)
    total_energy = total_energy + daily_boost
    
    gugugaga('Day ' + i + ' boost: ' + daily_boost)
    i = i + 1
end

// 3. 复杂逻辑判断
var result_message = check_practice_status(member, total_energy)
gugugaga('Final Status: ' + result_message)

// 4. 运算符优先级测试 ( (10+2)*3**2 )
var math_test = (10 + 2) * 2 ** 3
gugugaga('Math logic test (12 * 2^3): ' + math_test)

// 5. 无限循环与 break 示例
var count = 0
loop then
    if count is 3 then
        gugugaga('Loop reached 3, breaking...')
        break
    end
    count = count + 1
end

gugugaga('TomoriLang execution finished.')
```

log:

```tl
FunctionStatement
  Name:
        get_member_energy
  Parameters:
        days
  Body:
    IfStatement
      Condition:
        Binary(is)
          Variable(days)
          Literal(0)
      Then:
        ReturnStatement
          Literal(0)
      ElseIf:
        Binary(or)
          Binary(is)
            Variable(days)
            Literal(1)
          Binary(is)
            Variable(days)
            Literal(2)
        ReturnStatement
          Literal(1)
      Else:
        ReturnStatement
          Binary(+)
            CallExpression
              Variable(get_member_energy)
              Arguments:
                Binary(-)
                  Variable(days)
                  Literal(1)
            CallExpression
              Variable(get_member_energy)
              Arguments:
                Binary(-)
                  Variable(days)
                  Literal(2)
FunctionStatement
  Name:
        check_practice_status
  Parameters:
        member_name
        energy
  Body:
    VariableStatement(threshold)
      Literal(10)
    IfStatement
      Condition:
        Binary(and)
          Binary(>)
            Variable(energy)
            Variable(threshold)
          Binary(isnot)
            Variable(member_name)
            Literal(Soyo)
      Then:
        ReturnStatement
          Binary(+)
            Variable(member_name)
            Literal( is ready to practice!)
      ElseIf:
        Binary(is)
          Variable(member_name)
          Literal(Soyo)
        ReturnStatement
          Literal(Soyo is drinking tea...)
      Else:
        ReturnStatement
          Binary(+)
            Variable(member_name)
            Literal( needs more rest.)
ExpressionStatement
  CallExpression
    Variable(gugugaga)
    Arguments:
      Literal(--- Welcome to TomoriLang Interpreter ---)
VariableStatement(member)
  Literal(Anon)
VariableStatement(practice_days)
  Literal(7)
VariableStatement(performance_ratio)
  Literal(0.85)
VariableStatement(is_active)
  Literal(true)
ExpressionStatement
  CallExpression
    Variable(gugugaga)
    Arguments:
      Binary(+)
        Binary(+)
          Literal(Calculating energy for )
          Variable(member)
        Literal(...)
VariableStatement(total_energy)
  Literal(0)
VariableStatement(i)
  Literal(1)
LoopStatement
  Condition:
    Binary(<)
      Variable(i)
      Variable(practice_days)
  Body:
    VariableStatement(daily_boost)
      CallExpression
        Variable(get_member_energy)
        Arguments:
          Variable(i)
    ExpressionStatement
      AssignExpression(total_energy)
          Binary(+)
            Variable(total_energy)
            Variable(daily_boost)
    ExpressionStatement
      CallExpression
        Variable(gugugaga)
        Arguments:
          Binary(+)
            Binary(+)
              Binary(+)
                Literal(Day )
                Variable(i)
              Literal( boost: )
            Variable(daily_boost)
    ExpressionStatement
      AssignExpression(i)
          Binary(+)
            Variable(i)
            Literal(1)
VariableStatement(result_message)
  CallExpression
    Variable(check_practice_status)
    Arguments:
      Variable(member)
      Variable(total_energy)
ExpressionStatement
  CallExpression
    Variable(gugugaga)
    Arguments:
      Binary(+)
        Literal(Final Status: )
        Variable(result_message)
VariableStatement(math_test)
  Binary(*)
    Group
      Binary(+)
        Literal(10)
        Literal(2)
    Binary(**)
      Literal(2)
      Literal(3)
ExpressionStatement
  CallExpression
    Variable(gugugaga)
    Arguments:
      Binary(+)
        Literal(Math logic test (12 * 2^3): )
        Variable(math_test)
VariableStatement(count)
  Literal(0)
LoopStatement
  Condition:
  Body:
    IfStatement
      Condition:
        Binary(is)
          Variable(count)
          Literal(3)
      Then:
        ExpressionStatement
          CallExpression
            Variable(gugugaga)
            Arguments:
              Literal(Loop reached 3, breaking...)
        BreakStatement
    ExpressionStatement
      AssignExpression(count)
          Binary(+)
            Variable(count)
            Literal(1)
ExpressionStatement
  CallExpression
    Variable(gugugaga)
    Arguments:
      Literal(TomoriLang execution finished.)
```



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
function adder is Integer have x is Integer default 114 y is Integer default 514 then return x + y end
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
loop x < y then x = x + 1 end
```



# 实现