# Noise Contrastive Estimation

## Softmax

$$
\frac{\exp\left(f(X_i;\theta)\right)}{\sum_j^N\exp (f(X_j;\theta))}
$$

## Word2Vec Softmax: Negative Sampling

#### Ideal

$$
\frac{\exp\Big(f(w_i;c_i)\Big)}{\sum_{w'\in U}^N\exp \Big(f(w';c_i)\Big)}
$$

- c: Center word.
- w: Normal words(the others).
- U: Dictionary.

> "I love the **red** shirt"
>
> if "**red**" is the center word, the others are the normal words.

- Problem: The size of U can be massive, making it impossible to stay on memory.

![img](https://pic1.zhimg.com/80/v2-bcdc2c7a9b6738682ee0316257103a40_1440w.jpg)

#### Sampled Softmax

Sample and do softmax.

#### Negative Sampling

Instead of N classification, we can do binary classification, only telling if it is our target.

![img](https://pic2.zhimg.com/80/v2-3d3b6bddc9796d510732bfbbb8ed4251_1440w.jpg)

The loss function can be:

![img](https://pic2.zhimg.com/80/v2-bf442ea267b80f4fd6513d6221dfe961_1440w.jpg)

- h:context
- \theta: predicted embedding
- w_t: ground truth.
- When training, we just randomly sample some **negative samples(noise)** and use their expectation.

## NCE

$$
p(x)=\frac{\exp \hat p(x)}{Z}
$$

