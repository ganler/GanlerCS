[TOC]

## TODO List

- https://zhuanlan.zhihu.com/p/31547842
- https://zhuanlan.zhihu.com/p/51682879

## NLP Tasks

- **序列标注**：分词／词性标注／命名实体识别...
- **分类任务**：文本分类／情感分析...
- **句子关系判断**：自然语言推理／深度文本匹配／问答系统...
- **生成式任务**：机器翻译／文本摘要生成...

## Cosine Similarity In NLP

> Measure the distance of 2 sentence.

Cosine Similarity is a **scalar**.

- 2 lists of tokens. 
- Say the diversity of tokens is N.
- We can have a vector(say $V$) of length N for token list.
  - $V=\{F_1,\cdots,F_N\}$. F stands for the frequency.
  - $c=\frac{V_1V_2}{|V_1||V_2|}$

## NLP@Word2Vec

> https://easyai.tech/ai-definition/word2vec/
>
> https://towardsdatascience.com/word-embedding-with-word2vec-and-fasttext-a209c1d3e12c

- word2vec: word2embedding.
  - Just map the word to a n-dimension embedding vector.

![word2vec在nlp中的位置](https://easy-ai.oss-cn-shanghai.aliyuncs.com/2020-02-17-w2v-guanxi.png)

- Skip-gram
  - Input: one-hot word. **dim-v**
  - Outputs: one-hot words. **dim-v**
  - Result vector: Hidden layer embedding vector. **dim-n**

![2019-09-26-Skip-gram](https://easy-ai.oss-cn-shanghai.aliyuncs.com/2019-09-26-Skip-gram.png)

![img](https://miro.medium.com/max/552/1*TbjQNQLuyEW-cgsofyDioQ.png)

- CBOW

![img](https://miro.medium.com/max/660/1*UdLFo8hgsX0a1NKKuf_n9Q.png)

![2019-09-26-cbow](https://easy-ai.oss-cn-shanghai.aliyuncs.com/2019-09-26-cbow.png)

- Accelerate: 

  - Negative Sample
  - Hierachical Softmax

- Advantages: 

  - Taking context information into consideration.
  - Less dimensions.
  - General.

- Disadvantages:

  - Word <-> Vec : One 2 One

## [Zhihu@1](https://zhuanlan.zhihu.com/p/43493999)

### Seq2Seq

e.g., 机器学习(Chinese) -> Machine Learning(English)

### RNN@Seq2Seq

![img](https://pic1.zhimg.com/80/v2-e258d6cd046c0567ad72a8fe930807cc_1440w.jpg)

### Attenion-Based@Seq2Seq

- RNN: Source => Embedding
- Attention(NxN fully Connected): Embedding => Embedding
- RNN: Embedding => Output

![img](https://pic4.zhimg.com/80/v2-f0a7c907fca9301a628ac3a5bfe04ac7_1440w.jpg)

## Bert

![img](https://pic3.zhimg.com/80/v2-54f38912df10ac86c410bb3cccbd13e2_1440w.jpg)

ELMo

- Single directed.
- Sequential Model(Word N to predict word N+1).

Bert

- Use transformers to do encoding stuff.
- Parallel!

![img](https://pic2.zhimg.com/80/v2-5b7d456a24ba76d22f4ff8830683a1a9_1440w.jpg)

### What is Transformer

> https://jalammar.github.io/illustrated-transformer/

**CNN**: Local information.

**RNN**: Sequential not parallel.

**Transformer**: Local + long-distance information + Parallel.

#### Overview

The encoders have independent parameters(they do not share the same parameters).

![img](https://jalammar.github.io/images/t/The_transformer_encoder_decoder_stack.png)

#### Encoder

![img](https://jalammar.github.io/images/t/Transformer_encoder.png)

#### Decoder

![img](https://jalammar.github.io/images/t/Transformer_decoder.png)

#### Input

- For each token: a one-hot vector of size N. (N = the number of word classes)
- Where: The bottom-most encoder.