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



# svm-train

我们可以通过执行svm-train.exe来看一看：
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
