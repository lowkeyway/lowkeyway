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

### Makefile.win

windows 下的makefile文件，可以用来指导编译,从标准的libsvm中修改过来，只编译predict.exe
+ all: 编译出predict.exe
+ clean: 删除根目录下的obj文件以及，windows 下的 exp 和 lib 和 predict.exe 文件
