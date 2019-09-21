## Loss function & optimizer

### Main Loss Function

> 继承自`nn`。

#### MSELoss

> `nn.MSELoss(size_average=None, reduce=None, reduction='elementwise_mean')`
>
> `reduce`默认`True`即返回一个标量。
>
> `size_average`默认`True`，返回loss的平均值。

$$
{\rm MSE}(x, y)=\frac{\sum\left({\rm f}(x_i)-y_i\right)^2}{|\mathcal B|}
$$

> 对应反向传播

$$
w_k\leftarrow w_k-\frac{\eta}{|\mathcal B|}\frac{\partial{\rm MSE}}{\partial w_k}
$$

> L2正则化等价于权重衰减。($|\mathcal B|$代表batch size)
>
> L2正则化即在loss function后加上$\frac{\lambda ||w||^2}{2|\mathcal B|}$，权重衰减即每次权重减去$\frac{\lambda w}{|\mathcal B|}$。即前者求导后的结果。

##### CrossEntrophy

> `nn.CrossEntrophyLoss()` 交叉熵

> `nn.LogSoftmax()`和`nn.NLLLoss()`的结合。严格的交叉熵损失函数是`nn.NLLLoss()`。

#### NLLLoss

> `nn.NLLLoss`
>
> 对于单个样本而言，该函数即选择输出vector中，正确样本的output的负号作为loss值。

#### KLDivLoss

> KL散度。相对熵。
>
> 交叉熵 = 相对熵+信息熵

### Main Optimizer

#### Optimizer基础

##### Param_groups

> 参数组，将参数分为几组，设定不同的配置（比如学习率）。
>
> 每个参数组有如下配置：
>
> - `lr`
> - `weight_decay`
> - `momentum`

##### `zero_grad()`

将梯度清零。每次`loss.backward`前，记得把梯度清零。

##### `static_dict()`

一个字典，key是各层参数名，value即参数。

##### `add_param_group()`

给optimizer增加参数组，可用来定制lr等配置。

`opt.add_param_group({'param': w3, 'lr': 0.001})`

##### `step(closure)`

更新权值。其中可传入参数（一个闭包）。如，当采用 LBFGS优化方法时，需要多次计算，因此需要传入一个闭包去允许它们重新计算 loss。

```python
for input_, target_ in dataset:
    def closure():
		optimizer.zero_grad()
        out = model(input_)
        loss = loss_func(out, target_)
        loss.backward()
        return loss
    optimizer.step(closure)
```

#### 主要优化器

- `torch.optim.SGD(params, lr, momentum, dampening, weight_decay, nesterov)`

> pytorch的实现和别的框架稍有不同，在更新速度v的时候乘上的grad的部分会多乘一个lr。

$$
w_{i+1} = w_i-m*v_{i+1} - lr*grad_{i}\\
v_{i+1} = v_i*m+lr*grad_i
$$

- `torch.optim.Adam(..., eps=1e-08, amsgrad=False)`

> 一种自适应学习率的优化方法，Adam利用梯度的一阶矩估计和二阶矩估计动态调整学习率。Adam是结合了Momentum和RMSprop进行了偏差修正。

> Amsgrad- 表示是否采用AMSGrad方法，amsgrad方法是针对Adam的改进，通过添加额外的约束，使学习率始终为正值。
>
> > AMSGrad，ICLR-2018 Best-Pper 之一，《On the
> > convergence of Adam and Beyond》

- `torch.optim.adamax(..., betas=(0.9, 0.999), eps=1e-08, weight_decay=0`

> 对adam增加了一个学习率上限。（Adam）的论文中提出了adamax。

### 学习率调整

> 一般在训练初期给与大一点的学习率，到了后期给小一点的学习率。

#### `lr_scheduler.StepLR`

> `torch.optim.lr_scheduler.StepLR(optimizer, step_size, gamma=0.1, last_epoch=-1)`

> 这里step指的是epoch，每个epoch的学习率的调整，可以用adam的方法解决。

> 假设step=5，那么每5个epoch，我们就会将学习率改为$lr=lr*\gamma$。

#### `lr_scheduler.MultiStepLR`

> `torch.optim.lr_scheduler.MultiStepLR(.., milestones, gamma=0.1, last_epoch=-1)`
>
> 按设定的间隔调整学习率。这个方法适合后期调试使用，观察 loss 曲线，为每个实验定制学习率调整时机。

> `milestone`是一个`list`，里面的值是学习率改变的间隔。
>
> 比如`StepLR`的方法里面的`step_size=5`的时候，`milestone`等价于`[5, 5, 5, ...]`。

#### `lr_scheduler.ExponentialLR`

> `torch.optim.lr_scheduler.ExponentialLR(optimizer, gamma, last_epoch=-1)`
>
> `lr = lr*gamma^epoch`。按指数形式，缓慢变小。

#### `lr_scheduler.ReduceLROnPlateau`

> 当某指标不再变化(下降或升高)，调整学习率，这是非常实用的学习率调整策略。例如，当验证集的 loss 不再下降时，进行学习率调整;或者监测验证集的 accuracy，当accuracy 不再上升时，则调整学习率。

`torch.optim.lr_scheduler.ReduceLROnPlateau(optimizer, mode='min',factor=0.1, patience=10, verbose=False, threshold=0.0001, threshold_mode='rel', cooldown=0, min_lr=0, eps=1e-08)`

> mode(str)- 模式选择，有 min 和 max 两种模式，min 表示当指标不再降低(如监测loss)，max 表示当指标不再升高(如监测 accuracy)。
>
> factor(float)- 学习率调整倍数(等同于其它方法的 gamma)，即学习率更新为 lr = lr *factor。
>
> patience(int)- 直译——"耐心"，即忍受该指标多少个 step 不变化，当忍无可忍时，调整学习率。
>
> verbose(bool)- 是否打印学习率信息， print('Epoch {:5d}: reducing learning rate'' of group {} to {:.4e}.'.format(epoch, i, new_lr))
>
> threshold(float)- Threshold for measuring the new optimum，配合 threshold_mode 使用。
>
> threshold_mode(str)- 选择判断指标是否达最优的模式，有两种模式，rel 和 abs。
>
> 当 `threshold_mode=='rel'`，并且 `mode==max` 时，dynamic_threshold = best * ( 1 + threshold );
>
> 当 threshold_mode==rel，并且 mode==min 时，dynamic_threshold = best * ( 1 - threshold );
>
> 当 threshold_mode==abs，并且 mode==max 时，dynamic_threshold = best + threshold；
>
> 当 threshold_mode==rel，并且 mode==max 时，dynamic_threshold = best - threshold；
>
> cooldown(int)- “冷却时间“，当调整学习率之后，让学习率调整策略冷静一下，让模型再训练一段时间，再重启监测模式。
>
> min_lr(float or list)- 学习率下限，可为 float，或者 list，当有多个参数组时，可用 list 进行设置。
>
> eps(float)- 学习率衰减的最小值，当学习率变化小于 eps 时，则不调整学习率。

至此，可以开始读一个完整的`PyTorch`案例了，[传送门](https://github.com/tensor-yu/PyTorch_Tutorial/blob/master/Code/main_training/main.py)。