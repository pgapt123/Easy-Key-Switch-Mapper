# Easy-Key-Switch-Mapper

这是由APT开发的快速为Key Switch建立Cakewalk表情映射的小工具！有了它，只需编写简洁的文本代码，就可以建立表情映射，而不需要纠结于操作繁杂的Cakewalk自带表情映射编辑器！

### 使用方法

将ezksmp代码文档拖到 `ezksmp.exe` 上即可！

使用命令行执行 `ezksmp -v` 可查看版本信息，执行 `ezksmp -h` 可打开本帮助文件。



### 脚本

ezksmp脚本的结构形如

```
[Art Map Name]

-[Group 1 Name]

[Key] [Art Name] v[Velocity] c[Color]
[Key] [Art Name] v[Velocity] c[Color]
...

-[Group 2 Name]

[Key] [Art Name] v[Velocity] c[Color]
[Key] [Art Name] v[Velocity] c[Color]
...

......
```

每行的末尾请直接换行，不要有空白字符！

只要换行正确，各参数之间的任意空白字符都不影响ezksmp的正确工作！你可以任意加空白行以及缩进！

#### 头部

[Art Map Name] 是必需的，这是表情映射显示在管理器中的名字。可以包含空格，**不可以**使用中文或特殊字符。

#### 组

组名称由新的一行开头的`-`标示。即，当新的一行以`-`开头，意味着一个新的组开始了，下面的技法都属于这个组，直到再新建一个组。`-`的后面不要有空格，组名称内也不要有空格，可以使用中文或特殊字符。如果整个映射中只有一个组，那么组名称可以省略，缺省值为`Articulations`。

#### 技法

技法最多支持4个参数，按顺序分别为：键位、名称、力度、颜色

##### 键位

键位标记处于新的一行开头，意味着本行是一个新的技法。一个技法必须至少有键位这个参数。

键位有三种表示方法：

**音名指定**：使用标准的绝对音名记号，例如 `C1, D#0, Bb-1` 。MIDI中最低音为C-2

**音编号指定**：使用MIDI中的音编号，即 0~127

**步进**：使用 `=` 会使得当前的键位与上一个技法相同；使用 `+` 会使得当前的键位比上一个技法高一个半音，可以连续使用多个 `+` ，形如 `+++`

##### 名称

技法名称跟在键位标记后，中间以任意非换行的空白字符隔开（可以使用空格或tab，也可以打任意多个）。技法的名称不能有空格，可以使用中文或特殊字符。可以不设置技法名称，其缺省值为 `Art [技法序号]`。

##### 力度

力度是可选参数，默认值为100。如果要指定力度，使用 `v` 紧跟力度值进行设定。

##### 颜色

颜色是可选参数，默认值是一个随机数。如果要指定颜色，使用 `c` 紧跟颜色的16进制RGB表示，即 `cRRGGBB`。



#### 一个具体的例子

```
Ilya Hulusi

-Articulations
    C2 Natural_Dyn
    D2 Vibrato
    B1 Mordent_Fast
    A#1 Mordant_Slow
    G1 Slide_Up
    A1 Forshlag
    F#1 Staccato

-Trill
    E2 Trill

-Drone
    B0 Drone1-OFF v0
    = Drone1-ON v127
    + Drone2-OFF v0
    = Drone2-ON v127

-Tune
    G0 Tune_Down
    ++ Tune_Up
```


