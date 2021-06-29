# How to use it

## Train

### raw2heart.py 

可以把raw/test目录下的数据转换成一个libsvm的数据格式，并且按照一定的缩放格式进行scale，目标文件保存在数据所在的目录下

+ touch_heart：raw data转换成的libsvm格式的数据，并且打过了标签
+ touch_heart_scale：对touch_heart经过归一化的数据
+ touch_heart_scale_range：归一化的规则

### train.py 

把touch_heart_scale先利用grid.py进行选参，找到最优参数后利用svm-train.exe进行train

+ touch_heart_scale.out： 选参的过程文件
+ touch_heart_scale.model： train过的模型


## Predict

### model2C.py

这个脚本可以把train生成的model文件转换成svm_model.h，用头文件的格式存放model信息。

### heart2H.py

这个脚本可以把要predict的数据转换成svm_predict.h，用头文件的格式存放要predict的数据信息。

### Makefile.win

windows 下的makefile文件，可以用来指导编译,从标准的libsvm中修改过来，只编译predict.exe
+ all: 编译出predict.exe
+ clean: 删除根目录下的obj文件以及，windows 下的 exp 和 lib 和 predict.exe 文件

### svm-predict.c/svm.h
改编自libsvm，把svm-predict相关的函数从svm.cpp中提取出来。这样可以只编译svm-predict.c即可了，方便以后的移植。


## 步骤：

### 1. Train

```
python raw2heart.py
python train.py
```

### 2. Predict

#### 2.1 生成头文件

```
python model2C.py
python raw2heart.py test
python heart2H.py
```

#### 2.2 生成predict.exe
```
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
nmake -f Makefile.win clean all
```

这样就可以在windows目录下看到predict.exe文件

#### 2.3 做预测

接着就可以用这个exe文件做预测了，命令如下：
```
cd windows && svm-predict.exe
```
