# 用户至少要这么做

+ 1. 对数据执行简单的scaling
+ 2. 考虑 **RBF**  kernel <img src="https://latex.codecogs.com/svg.image?K(x,&space;y)=e^{-\gamma\|x-y\|^{2}}" title="K(x, y)=e^{-\gamma\|x-y\|^{2}}" />
+ 3. 使用 cross-validation 找到最优的参数 C 和 <img src="https://latex.codecogs.com/svg.image?\gamma" title="\gamma" />
+ 4. 使用最优参数 C 和 <img src="https://latex.codecogs.com/svg.image?\gamma" title="\gamma" /> 训练整个数据集
+ 5. 测试
