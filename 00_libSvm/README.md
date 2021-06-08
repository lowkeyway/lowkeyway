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



# train-model文件
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
