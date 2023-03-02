# 24points game

24点小游戏，算是C++ Qt5复健（？）随手写出的小玩意。

该小游戏设置了4个难度，其中：
* `easy`与`hard`生成数字范围为[1,10],`normal`与`lunatic`生成数字范围为[-10,-1]U[1,10]
* `easy`与`normal`仅有4则运算符，`hard`与`lunatic`则加入了乘方与整除[^1]运算。

反馈bug请前往Issues。

![image](https://user-images.githubusercontent.com/88281489/171977138-04977a55-a3c2-4f94-a44e-1878580435c4.png)

[^1]: a // b means floor(a / b)
