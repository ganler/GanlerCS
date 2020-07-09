[toc]

# ssl && adversarial training

> https://medium.com/inside-machine-learning/placeholder-3557ebb3d470
>
> http://www.twistedwg.com/2018/12/04/VAT.html

### Status/Challenges

- Limited labeled data.
- No labels needed: Unsupervised Learning. e.g., K-Means && Latent Dirichlet Allocation algo.
- ssl: small amount of data(oartially labelled data sets) to construct the categories.

### Adversarial Training

- Suitable for supervised & semi-supervised learning.

2017.Miyato et al: [*Virtual Adversarial Training: A Regularization Method for Supervised and Semi-Supervised Learning*](https://arxiv.org/pdf/1704.03976.pdf)*.* 

- Goodfellow: We can easily perturb a neural image classifier by adding some random noise.

![img](https://miro.medium.com/max/3156/1*twpZiTw1PAJOcf2jW5gUhg.png)

- How2fix:

  - **virtual adversarial training(VAT)**: Add adversarial noise to the training data. (adversarial training)

    - For labeled images, train it with adversarial noise.
      - We need to maximize the "noise" to make our model more robust.
      - How define "noisiness"? The divergence between the predicted label & the correct label/prediction.
      - D stands for distance. We can make use of KL.
      - Given the best perturbation we need to minimize the loss function. 

    $$
    perturbation_{best} = \arg \max_{perturbation}\left(\mathbb D[P(label|feature), P(label|feature+perturbation)]\right)
    $$
$$
    L=L_{original}+L_{VAT}\\
    L_{VAT}=\mathbb D [P(label|feature+perturbation_{best}), P(label|feature)]
    $$
    
- For unlabeled images, we let our model to predict the labels.

