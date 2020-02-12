# REST&RPC

[TOC]

## REST

- REST不是协议，也不是RPC架构，而是一种指导Web APP设计API的“API设计风格”。
  - HTTP Methods:
    - **GET**: 输入参数，从特定的URL得到返回数据；
    - **POST**: 提交数据（放到body里）；
    - **PUT**: Update a specified resource(tell me which one do you want to update);
    - **DELETE**
  - Relies on a stateless, client-server protocal, almost always HTTP.
  - Treats server obj. as resources that can be created or destroyed.
- 特点就是**简单直接**；
- Authentication: Some API require authentications.
  - `curl -H "Authorization: tocken OAUTH-TOKEN" https://apo.github.com`
  - `curl https://api.github/com/?access_token=OAUTH-TOKEN`

## REST & RPC

![img](https://pic1.zhimg.com/80/v2-94f2d3a1a96ce10d0b40fa7d17a78d00_hd.jpg)

[Credit](https://zhuanlan.zhihu.com/p/66311537)

- RPC面向**过程**，只使用GET/POST：
  - GET用于读取信息；
  - 其他一律用POST；
- RESTful**面向资源**，使用POST/DELETE/GET/PUT
- 如`POST /removeItem?itemId=456`，那么实际上是远程调用了`removeItem(itemId=456)`的这种方法；
- RPC本身也只是一种通讯方式/概念（面向过程），而并不是协议；

## RPC框架与RPC

以阿里的Dubbo为例，Dubbo是一个实现了RPC的框架（包括了其自定义的协议），与RPC是实现与被实现的关系。Dubbo自己定义了一个应用层协议，传输层依旧是TCP，与HTTP，FTP，SMTP是并列的概念。

- RPC是一种编程模型的概念，而HTTP是一种应用层协议。我们完全可以基于HTTP协议实现RPC通信，这两者并不矛盾。

RPC框架要解决3个问题：

- **协议约定**：语法、如何传参等等；
- **传输协议**：网络发生错误等问题时如何处理；
- **服务发现**：服务端哪些端口可以调用，从哪个端口进行访问；

## gRPC

#### 协议约定

- 给予二进制的序列化，具有更好的性能；
- 协议给予Protocol Buffers，压缩率极高的一种序列化协议；

#### 传输协议

- 每个gRPC请求将封装成一个**HTTP 2.0**的Stream；
- HTTP2.0的好处：
  - 单向RPC
  - 客户端流式RPC
  - 服务端流式RPC
  - 双向流式RPC

#### 服务发现

gRPC 本身没有提供服务发现的机制，需要通过其他组件。一个比较高性能的服务发现和负载均衡器是 **Envoy**，可以灵活配置转发规则，有兴趣的可以去了解下。

#### 优点总结

- 性能高
- HTTP 2.0流传输
- 支持多语言（嗯，他有个很牛逼的gRPC编译器）