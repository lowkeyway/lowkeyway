Some useful webside.

1. [林智仁的官方网址](https://www.csie.ntu.edu.tw/~cjlin/libsvm/)
2. [libsvm_for_embeded](https://github.com/zhongcheng0519/libsvm_for_embeded) 

# LibSvm框架
java文件夹：是应用于java平台的实现SVM的源文件，/libsvm/：一些java库，Makefile：编译文件，svm_predict.java：测试文件，svm_toy.java：实现可视化工具的源文件，svm_train.java：训练的源码文件；

python文件夹：是用来参数优选的程序文件，cross_validation.py：交叉验证，easy.py： 总的调用程序，从参数优选，到文件训练预测均调用。svm.py：调用SVM主体文件， svm_test.py：测试svm模型的文件；

svm-toy文件夹：实现训练数据和分类界面可视化的工具的源码，其编译后的程序在windows文件夹下：svmtoy.exe；

windows文件夹：包含libSVM四个exe程序包，svmpredict.exe：svm-predict.c编译的可执行程序，实现SVM的预测。svmscale.exe：svm-scale.c编译的可执行程序，实现对数据的归一化缩放。svmtrain.exe：svm-train.c编译的可执行程序，调用SVM的数据训练接口。

当前文件夹： 
heart_scale：LibSvm自带的一个样本文件，可以用来测试和训练。   
Makefile：linux环境下，目录下几个.c,.h文件的编译规则文件。  
Makefile.win：windows环境下，目录下几个.c,.h文件的编译规则文件。  
svm.cpp/svm.h：svm算法的具体实现文件。  
svm-predict.c：svm模型的测试文件，调用svm.c文件中的测试接口。  
svm-scale.c：实现对样本数据的归一经缩放。缩放的规则如这个公式：  

# heart_scale文件

这个文件存储的是打过标签的libSVM的数据，可以作为train的input数据。

```
+1 1:0.708333 2:1 3:1 4:-0.320755 5:-0.105023 6:-1 7:1 8:-0.419847 9:-1 10:-0.225806 12:1 13:-1 
-1 1:0.583333 2:-1 3:0.333333 4:-0.603774 5:1 6:-1 7:1 8:0.358779 9:-1 10:-0.483871 12:-1 13:1 
+1 1:0.166667 2:1 3:-0.333333 4:-0.433962 5:-0.383562 6:-1 7:-1 8:0.0687023 9:-1 10:-0.903226 11:-1 12:-1 13:1 
-1 1:0.458333 2:1 3:1 4:-0.358491 5:-0.374429 6:-1 7:-1 8:-0.480916 9:1 10:-0.935484 12:-0.333333 13:1 
-1 1:0.875 2:-1 3:-0.333333 4:-0.509434 5:-0.347032 6:-1 7:1 8:-0.236641 9:1 10:-0.935484 11:-1 12:-0.333333 13:-1 
-1 1:0.5 2:1 3:1 4:-0.509434 5:-0.767123 6:-1 7:-1 8:0.0534351 9:-1 10:-0.870968 11:-1 12:-1 13:1 
+1 1:0.125 2:1 3:0.333333 4:-0.320755 5:-0.406393 6:1 7:1 8:0.0839695 9:1 10:-0.806452 12:-0.333333 13:0.5 
+1 1:0.25 2:1 3:1 4:-0.698113 5:-0.484018 6:-1 7:1 8:0.0839695 9:1 10:-0.612903 12:-0.333333 13:1 
+1 1:0.291667 2:1 3:1 4:-0.132075 5:-0.237443 6:-1 7:1 8:0.51145 9:-1 10:-0.612903 12:0.333333 13:1 
+1 1:0.416667 2:-1 3:1 4:0.0566038 5:0.283105 6:-1 7:1 8:0.267176 9:-1 10:0.290323 12:1 13:1 
```
它的数据格式可以表示为：
Label 1:value 2:value ….
Label：是类别的标识，比如如果做二分classification用到的1 -1，也可以自己随意定，比如-10，0，15。当然，如果是回归，这是目标值，就要实事求是了。 
Value：就是要训练的数据，从分类的角度来说就是特征值，数据之间用空格隔开  

比如: -15 1:0.708 2:1056 3:-0.3333

需要注意的是，如果特征值为0，特征冒号前面的(姑且称做序号)可以不连续。如：

       -15 1:0.708 3:-0.3333

表明第2个特征值为0，从编程的角度来说，这样做可以减少内存的使用，并提高做矩阵内积时的运算速度。我们平时在matlab中产生的数据都是没有序号的常规矩阵，所以为了方便最好编一个程序进行转化。


# train-model文件

这个文件存储的是SVM训练后的模型参数，可以作为train的output或者predict的input。

```
svm_type c_svc    //所选择的svm类型，默认为c_svc
kernel_type rbf   //训练采用的核函数类型，此处为RBF核
gamma 0.0769231   //RBF核的参数γ
nr_class 2        //类别数，此处为两分类问题
total_sv 132      //支持向量总个数
rho 0.424462      //判决函数的偏置项b
label 1 -1        //原始文件中的类别标识
nr_sv 64 68       //每个类的支持向量机的个数
SV                //以下为各个类的权系数及相应的支持向量
1 1:0.166667 2:1 3:-0.333333 … 10:-0.903226 11:-1 12:-1 13:1
0.5104832128985164 1:0.125 2:1 3:0.333333 … 10:-0.806452 12:-0.333333 13:0.5
………..
-1 1:-0.375 2:1 3:-0.333333…. 10:-1 11:-1 12:-1 13:1
-1 1:0.166667 2:1 3:1 …. 10:-0.870968 12:-1 13:0.5
```


# 编译
## Windows

Windows binaries are available in the directory `windows'. To re-build them via Visual C++, use the following steps:

### 1. Open a DOS command box (or Visual Studio Command Prompt) and change to libsvm directory. If environment variables of VC++ have not been set, type
```
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
```
You may have to modify the above command according which version of
VC++ or where it is installed.

### 2. Type
```
nmake -f Makefile.win clean all
```
### 3. (optional) To build shared library libsvm.dll, type
```
nmake -f Makefile.win lib
```
### 4. (optional) To build 32-bit windows binaries, you must
	+ (1) Setup "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat" instead of vcvars64.bat
	+ (2) Change CFLAGS in Makefile.win: /D _WIN64 to /D _WIN32

Another way is to build them from Visual C++ environment. See details in libsvm FAQ.


# svm-scale

svm-scale 是实现对数据进行缩放的。

<img src="https://latex.codecogs.com/svg.image?y^{\prime}=\text&space;{&space;lower&space;}&plus;(\text&space;{&space;upper&space;}-\text&space;{&space;lower&space;})&space;*&space;\frac{y-m&space;i&space;n}{\max&space;-\min&space;}" title="y^{\prime}=\text { lower }+(\text { upper }-\text { lower }) * \frac{y-m i n}{\max -\min }" />


式中，y 为缩放前的数据，y' 为缩放后的数据；lower 为参数中指定的数据下界，upper 为参数中指定的数据上界；min 为全部训练数据中的最小值，max 为全部训练数据中的最大值。

windows 文件夹下有一个 svm-scale.exe 程序，可以用 svm-scale 命令来执行该程序。

## 1. 先来看看 svm-scale 都有哪些参数，在 libsvm 的 windows 目录下输入命令svm-scale ：

```
D:\louis\06_Code\04_Louis\SVM\libsvm-3.24\libsvm-3.24\windows>svm-scale.exe
Usage: svm-scale [options] data_filename
options:
-l lower : x scaling lower limit (default -1)
-u upper : x scaling upper limit (default +1)
-y y_lower y_upper : y scaling limits (default: no y scaling)
-s save_filename : save scaling parameters to save_filename
-r restore_filename : restore scaling parameters from restore_filename
```

翻译一下：
```
lower --- upper：缩放后的数据范围（特征向量 x），默认 -1 到 1。

y_lower --- y_upper：缩放后的数据范围（类别标签 y），默认不对 y 进行缩放。
-s save_filename：将缩放的规则保存为文件save_filename。

-r restore_filename：按照restore_filename中的规则缩放数据。
```

## 2. 命令 svm-scale 要求至少输入一个文件名 data_filename，该文件中是要缩放的数据.
看示例：缩放 windows 文件夹下的 touch_heart 文件中的数据：
```
D:\louis\06_Code\04_Louis\SVM\libsvm-3.24\libsvm-3.24\windows>svm-scale.exe touch_heart
1 1:0.846154 2:0.888889 3:0.925926 4:0.806452 5:0.741935 6:0.885714 7:0.966387 8:1 9:1 10:1 11:1 12:1 13:0.947368 14:1 15:0.923077 16:0.934426 17:1 18:0.777778 19:0.785714 20:0.76 21:0.8 22:0.909091 23:0.907692 24:0.972028 25:0.972028 26:1 27:0.987179 28:0.985915 29:0.962617 30:0.949367 31:0.933333 32:0.954545 33:0.851852 34:0.925926 35:0.9 36:1 37:0.866667 38:0.866667 39:0.933333 40:0.9375 41:0.986486 42:0.986755 43:0.961538 44:1 45:1 46:1 47:0.977778 48:0.967742 49:0.76 50:0.882353 51:0.833333 52:0.833333 53:0.636364 54:0.714286 55:1 56:1 57:0.956044 58:0.985915 59:1 60:0.986207 61:1 62:1 63:0.980769 64:1 65:1 66:1 67:0.714286 68:0.6 69:0.75 70:0.777778 71:1 72:1 73:0.931034 74:0.981651 75:1 76:1 77:1 78:1 79:1 80:0.935484 81:0.857143 82:0.777778 83:0.555556 84:0.714286 85:0.555556 86:0.75 87:0.8 88:1 89:0.789474 90:0.961538 91:0.970803 92:1 93:0.986577 94:1 95:1 96:0.97561 97:1 98:0.555556 99:0.428571 100:0.5 101:0.555556 102:1 103:1 104:1 105:0.866667 106:0.846154 107:0.943662 108:1 109:0.972973 110:1 111:0.985507 112:0.929412 113:0.833333 114:0.454545 115:1 116:0.6 117:0.777778 118:0.8 119:0.818182 120:0.636364 121:0.866667 122:0.913043 123:0.923077 124:0.95122 125:0.972414 126:0.986111 127:0.985915 128:0.975309 129:0.846154 130:0.833333 131:0.6 132:1 133:1 134:1 135:1 136:1 137:0.846154 138:0.923077 139:0.92233 140:0.971014 141:1 142:1 143:1 144:0.955556 145:0.806452 146:0.826087 147:0.882353 148:0.866667 149:0.75 150:1 151:0.882353 152:0.789474 153:0.857143 154:0.846154 155:0.942857 156:0.971631 157:1 158:0.986928 159:0.986667 160:1
...
```

## 3. 我们使用命令 "> touch_heart.scaled" 将结果输出在文件 touch_heart.scaled中.
## 4. 我们使用命令 "-s touch_heart.range" 将缩放规则保存在文件touch_heart.range 中：
我们卡开看touch_heart.range的内容：
```
x
-1 1
1 -2 11
2 -1 17
3 -1 26
4 0 31
5 -1 30
6 1 36
7 0 119
8 -1 130
9 0 125
10 -1 151
11 -1 150
12 -1 101
13 0 38
14 0 22
15 1 27
16 0 61
17 -1 16
18 0 27
...
```
它的含义为：
```
第一行表示是对 x 进行的缩放；

第二行表示缩放的上下界分别是 1 和 -1；

第三行表示特征向量 x 的第一个分量 x1 在训练数据中的范围是 0~297.05，以此类推。
```

## 5. 还可以自定义 lower 和 upper 的值后再对数据进行缩放：
## 6. 还可以用-r命令制定.range文件对另外的测试文件test1做相同标准的缩放
```
svm-scale.exe -r touch_heart.range test1 > test1.scaled
```


svm-scale.exe 程序的源码是 libsvm 根目录下的 svm-scale.c 文件，可以通过研究该文件中的源码来进一步深入学习 libsvm 缩放数据的机制。

# svm-train

svm-train主要实现对训练数据集的训练，并可以获得SVM模型，我们可以通过执行svm-train.exe来看一看用法：
```
D:\louis\06_Code\04_Louis\SVM\libsvm-3.24\libsvm-3.24\windows>svm-train.exe
Usage: svm-train [options] training_set_file [model_file]
options:
-s svm_type : set type of SVM (default 0)
        0 -- C-SVC              (multi-class classification)
        1 -- nu-SVC             (multi-class classification)
        2 -- one-class SVM
        3 -- epsilon-SVR        (regression)
        4 -- nu-SVR             (regression)
-t kernel_type : set type of kernel function (default 2)
        0 -- linear: u'*v
        1 -- polynomial: (gamma*u'*v + coef0)^degree
        2 -- radial basis function: exp(-gamma*|u-v|^2)
        3 -- sigmoid: tanh(gamma*u'*v + coef0)
        4 -- precomputed kernel (kernel values in training_set_file)
-d degree : set degree in kernel function (default 3)
-g gamma : set gamma in kernel function (default 1/num_features)
-r coef0 : set coef0 in kernel function (default 0)
-c cost : set the parameter C of C-SVC, epsilon-SVR, and nu-SVR (default 1)
-n nu : set the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)
-p epsilon : set the epsilon in loss function of epsilon-SVR (default 0.1)
-m cachesize : set cache memory size in MB (default 100)
-e epsilon : set tolerance of termination criterion (default 0.001)
-h shrinking : whether to use the shrinking heuristics, 0 or 1 (default 1)
-b probability_estimates : whether to train a SVC or SVR model for probability estimates, 0 or 1 (default 0)
-wi weight : set the parameter C of class i to weight*C, for C-SVC (default 1)
-v n: n-fold cross validation mode
-q : quiet mode (no outputs)
```

用法： svm-train.exe [options] training_set_file [model_file]
中文解释一下就是：
```
-s 设置svm类型：
         0 – C-SVC
         1 – v-SVC
         2 – one-class-SVM
         3 – ε-SVR
         4 – n - SVR 

-t 设置核函数类型，默认值为2
         0 -- 线性核：u'*v
         1 -- 多项式核： (g*u'*v+ coef 0)degree
         2 -- RBF 核：exp(-γ*||u-v||2)
         3 -- sigmoid 核：tanh(γ*u'*v+ coef 0) 

-d degree: 设置多项式核中degree的值，默认为3
-gγ: 设置核函数中γ的值，默认为1/k，k为特征（或者说是属性）数；
-r coef 0:设置核函数中的coef 0，默认值为0；
-c cost：设置C-SVC、ε-SVR、n - SVR中从惩罚系数C，默认值为1；
-n v ：设置v-SVC、one-class-SVM 与n - SVR 中参数n ，默认值0.5；
-p ε ：设置v-SVR的损失函数中的e ，默认值为0.1；
-m cachesize：设置cache内存大小，以MB为单位，默认值为40；
-e ε ：设置终止准则中的可容忍偏差，默认值为0.001；
-h shrinking：是否使用启发式，可选值为0 或1，默认值为1；
-b 概率估计：是否计算SVC或SVR的概率估计，可选值0 或1，默认0；
-wi weight：对各类样本的惩罚系数C加权，默认值为1；
-v n：n折交叉验证模式；

model_file：可选项，为要保存的结果文件，称为模型文件，以便在预测时使用。
```

找个例子：
```
D:\louis\06_Code\04_Louis\SVM\libsvm-3.24\libsvm-3.24\windows>svm-train.exe heart_scale
[LOUIS]read_problem, 284 - filename is [heart_scale]
[LOUIS]read_problem, 312 - elements = 3648, prob.l = 270
*
optimization finished, #iter = 162
nu = 0.431029
obj = -100.877288, rho = 0.424462
nSV = 132, nBSV = 107
Total nSV = 132
```

运行后，我们可以在相同文件夹下生成train.mode文件。


# svm-predict

svmpredict 是根据训练获得的模型，对数据集合进行预测。 

用法：svmpredict [options] test_file model_file output_file

其中，options为操作参数，可用的选项即表示的涵义如下所示:

```
-b probability_estimates——是否需要进行概率估计预测，可选值为0 或者1，默认值为0。

model_file ——是由svmtrain 产生的模型文件；
test_file—— 是要进行预测的数据文件，格式也要符合libsvm格式，即使不知道label的值，也要任意填一个，svmpredict会在output_file中给出正确的label结果，如果知道label的值，就会输出正确率；
output_file ——是svmpredict 的输出文件，表示预测的结果值。
```

找个例子：
```
D:\louis\06_Code\04_Louis\SVM\libsvm-3.24\libsvm-3.24\windows>svm-predict.exe predict_input train.model louis.out
Accuracy = 89.2857% (25/28) (classification)
```

我们从train的heart_scale中摘抄出来一些作为输入文件，用来预测;
train.model作为SVM train后的模型参数作为输入文件；
louis.out作为输出文件，用来做预测结果的保存。看一看输出文件：

## predict_input
```
+1 1:0.708333 2:1 3:1 4:-0.320755 5:-0.105023 6:-1 7:1 8:-0.419847 9:-1 10:-0.225806 12:1 13:-1 
+1 1:0.166667 2:1 3:-0.333333 4:-0.433962 5:-0.383562 6:-1 7:-1 8:0.0687023 9:-1 10:-0.903226 11:-1 12:-1 13:1 
+1 1:0.125 2:1 3:0.333333 4:-0.320755 5:-0.406393 6:1 7:1 8:0.0839695 9:1 10:-0.806452 12:-0.333333 13:0.5 
+1 1:0.25 2:1 3:1 4:-0.698113 5:-0.484018 6:-1 7:1 8:0.0839695 9:1 10:-0.612903 12:-0.333333 13:1 
+1 1:0.291667 2:1 3:1 4:-0.132075 5:-0.237443 6:-1 7:1 8:0.51145 9:-1 10:-0.612903 12:0.333333 13:1 
+1 1:0.416667 2:-1 3:1 4:0.0566038 5:0.283105 6:-1 7:1 8:0.267176 9:-1 10:0.290323 12:1 13:1 
+1 1:0.333333 2:1 3:-1 4:-0.245283 5:-0.506849 6:-1 7:-1 8:0.129771 9:-1 10:-0.16129 12:0.333333 13:-1 
+1 1:-0.291667 2:1 3:1 4:-0.132075 5:-0.155251 6:-1 7:-1 8:-0.251908 9:1 10:-0.419355 12:0.333333 13:1 
+1 2:1 3:1 4:-0.132075 5:-0.648402 6:1 7:1 8:0.282443 9:1 11:1 12:-1 13:1 
+1 1:0.583333 2:1 3:1 4:-0.509434 5:-0.52968 6:-1 7:1 8:-0.114504 9:1 10:-0.16129 12:0.333333 13:1 
+1 1:0.208333 2:1 3:0.333333 4:-0.660377 5:-0.525114 6:-1 7:1 8:0.435115 9:-1 10:-0.193548 12:-0.333333 13:1 
+1 1:0.166667 2:1 3:0.333333 4:-0.358491 5:-0.52968 6:-1 7:1 8:0.206107 9:-1 10:-0.870968 12:-0.333333 13:1 
+1 1:0.25 2:1 3:1 4:0.433962 5:-0.086758 6:-1 7:1 8:0.0534351 9:1 10:0.0967742 11:1 12:-1 13:1 
+1 1:-0.125 2:1 3:1 4:-0.0566038 5:-0.6621 6:-1 7:1 8:-0.160305 9:1 10:-0.709677 12:-1 13:1 
+1 1:-0.208333 2:1 3:1 4:-0.320755 5:-0.406393 6:1 7:1 8:0.206107 9:1 10:-1 11:-1 12:0.333333 13:1 
+1 1:0.333333 2:1 3:1 4:-0.132075 5:-0.630137 6:-1 7:1 8:0.0229008 9:1 10:-0.387097 11:-1 12:-0.333333 13:1 
+1 1:0.25 2:1 3:-1 4:0.245283 5:-0.328767 6:-1 7:1 8:-0.175573 9:-1 10:-1 11:-1 12:-1 13:-1 
-1 1:-0.0833333 2:1 3:0.333333 4:-0.698113 5:-0.776256 6:-1 7:-1 8:-0.206107 9:-1 10:-0.806452 11:-1 12:-1 13:-1 
-1 1:0.25 2:1 3:0.333333 4:0.0566038 5:-0.607306 6:1 7:-1 8:0.312977 9:-1 10:-0.483871 11:-1 12:-1 13:-1 
-1 1:0.75 2:-1 3:-0.333333 4:0.245283 5:-0.196347 6:-1 7:-1 8:0.389313 9:-1 10:-0.870968 11:-1 12:0.333333 13:-1 
-1 1:0.333333 2:1 3:0.333333 4:0.0566038 5:-0.465753 6:1 7:-1 8:0.00763359 9:1 10:-0.677419 12:-1 13:-1 
-1 1:-0.416667 2:1 3:1 4:0.0566038 5:-0.447489 6:-1 7:-1 8:0.526718 9:-1 10:-0.516129 11:-1 12:-1 13:-1 
-1 1:0.208333 2:-1 3:0.333333 4:-0.509434 5:-0.0228311 6:-1 7:-1 8:0.541985 9:-1 10:-1 11:-1 12:-1 13:-1 
-1 1:-0.166667 2:1 3:-0.333333 4:-0.320755 5:-0.360731 6:-1 7:-1 8:0.526718 9:-1 10:-0.806452 11:-1 12:-1 13:-1 
-1 1:-0.0416667 2:1 3:0.333333 4:0.471698 5:-0.666667 6:1 7:-1 8:0.389313 9:-1 10:-0.83871 11:-1 12:-1 13:1 
-1 1:-0.375 2:1 3:-0.333333 4:-0.509434 5:-0.374429 6:-1 7:-1 8:0.557252 9:-1 10:-1 11:-1 12:-1 13:1 
-1 1:0.125 2:-1 3:-0.333333 4:-0.132075 5:-0.232877 6:-1 7:1 8:0.251908 9:-1 10:-0.580645 12:-1 13:-1 
-1 1:0.166667 2:1 3:1 4:-0.132075 5:-0.69863 6:-1 7:-1 8:0.175573 9:-1 10:-0.870968 12:-1 13:0.5 
```

## train.mode

```
svm_type c_svc
kernel_type rbf
gamma 0.076923076923076927
nr_class 2
total_sv 132
rho 0.42446205176771579
label 1 -1
nr_sv 64 68
SV
1 1:0.166667 2:1 3:-0.333333 4:-0.433962 5:-0.383562 6:-1 7:-1 8:0.0687023 9:-1 10:-0.903226 11:-1 12:-1 13:1 
0.51048321289851639 1:0.125 2:1 3:0.333333 4:-0.320755 5:-0.406393 6:1 7:1 8:0.0839695 9:1 10:-0.806452 12:-0.333333 13:0.5 
1 1:0.333333 2:1 3:-1 4:-0.245283 5:-0.506849 6:-1 7:-1 8:0.129771 9:-1 10:-0.16129 12:0.333333 13:-1 
1 1:0.208333 2:1 3:0.333333 4:-0.660377 5:-0.525114 6:-1 7:1 8:0.435115 9:-1 10:-0.193548 12:-0.333333 13:1 
1 1:0.166667 2:1 3:0.333333 4:-0.358491 5:-0.52968 6:-1 7:1 8:0.206107 9:-1 10:-0.870968 12:-0.333333 13:1 
1 1:0.25 2:1 3:-1 4:0.245283 5:-0.328767 6:-1 7:1 8:-0.175573 9:-1 10:-1 11:-1 12:-1 13:-1 
1 1:-0.541667 2:1 3:1 4:0.0943396 5:-0.557078 6:-1 7:-1 8:0.679389 9:-1 10:-1 11:-1 12:-1 13:1 
1 1:0.25 2:1 3:0.333333 4:-0.396226 5:-0.579909 6:1 7:-1 8:-0.0381679 9:-1 10:-0.290323 12:-0.333333 13:0.5 
1 1:-0.166667 2:1 3:0.333333 4:-0.54717 5:-0.894977 6:-1 7:1 8:-0.160305 9:-1 10:-0.741935 11:-1 12:1 13:-1 
...
```

## louis.out

```
1
-1
1
1
1
1
-1
1
1
1
1
1
1
1
1
1
-1
-1
-1
-1
-1
-1
-1
-1
-1
-1
-1
-1

```

# SVM类型 

1. SVC: support vector classication (two-class and multi-class).
2. SVR: support vector regression.
3. One-class SVM.
