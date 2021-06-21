# 用户至少要这么做

+ 1. 对数据执行简单的scaling
+ 2. 考虑 **RBF**  kernel <img src="https://latex.codecogs.com/svg.image?K(x,&space;y)=e^{-\gamma\|x-y\|^{2}}" title="K(x, y)=e^{-\gamma\|x-y\|^{2}}" />
+ 3. 使用 cross-validation 找到最优的参数 C 和 <img src="https://latex.codecogs.com/svg.image?\gamma" title="\gamma" />
+ 4. 使用最优参数 C 和 <img src="https://latex.codecogs.com/svg.image?\gamma" title="\gamma" /> 训练整个数据集
+ 5. 测试

# Scaling

我们在[README](https://github.com/lowkeyway/lowkeyway/blob/main/00_libSvm/README.md)中就提过了了scale的用法以及原理，让我们看一个例子：
```
$./svm-scale -s range1 train.1 > train.1.scale
$./svm-scale -r range1 test.1 > test.1.scale
$./svm-train train.1.scale
$./svm-predict test.1.scale train.1.scale.model test.1.predict
! Accuracy = 96.15%
```

它的大概意思就是，
+ 对训练数据train.1进行默认scale，把scale的结果保存在train.1.scale，把scale的规则保存成range1
+ 对测试数据test.1按照train.1的scale规则range1来进行相同的缩放，保存结果为test.1.scale
+ 对train.1.scale使用默认参数进行训练，训练的模型默认保存成train.1.scale.model
+ 对test.1.scale数据使用训练的参数train.1.scale.model进行预测，预测结果保存成test.1.predict


