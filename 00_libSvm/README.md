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
