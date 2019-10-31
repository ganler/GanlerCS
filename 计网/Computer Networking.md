# Computer Networking

## Introduction

### Introduction

Computer communicates just like human do. They follow some rules which are called **protocols**.

And we can use some models to describe Network systems(protocols are just some rules used in such models).

![](https://i.loli.net/2019/07/07/5d2195016f76894989.png)

### TCP/IP 5 layers Network Model

#### Physical layer

> Represents <u>the physical devices</u> that interconnect computers.(Describing how signals are sent over these connections)
>
> - The specification(规格) for the Network cables(电缆).
> - The connector that join devices together.

#### Data link layer

> The network interface or network access layer.

> Define **a common way** of interpreting these signals.
>
> This has something to do with lots of **protocols**.
>
> The most common one is called **Ethernet**.
>
> > Specified <u>physical layer attributes</u> and a <u>protocol</u> for getting data to nodes on the same network or link(single link).

#### Network layer

Deliver data between 2 individual nodes.

> Allow different networks to communicate with each other through devices known as routers. 
>
> Internetwork: 
>
> **A collection of networks** connected through routers, the most famous of these being the Internet.
>
> Get data across a collection of the network. (your computer and a server. The network layer helps you get data between the 2 locations.)

> The most common protocol used at this layer: **IP**(Internet Protocol)
>
> <u>IP is the heart of the Internet and most smaller networks around the world.</u>

> Network software is usually divided into client and server categories.
>
> - Client : initiate a request for data;
> - Server : anwer the request.
>
> A single node may be running multiple client or server APPs. 

#### Transport layer

Sorts out which client and server programs are supposed to get that data. 

Ensure the data are transmitted between the right applications on the nodes.

> **TCP** : Transmission Control Protocol
>
> **UDP** : User Datagram Protocol
>
> **Difference **: TCP makes sure that data is **reliably** delivered while UDP does not.

#### Application layer

Application-specific.

![](https://i.loli.net/2019/07/07/5d2194b841cb511485.png)

#### The OSI Networking Model

> The primary difference between our five layer model and the seven layer OSI model is that the OSI model **abstracts the application layer into three layers total.**

The traditional TCP/IP Model only has **four** layers, as it doesn’t differentiate <u>between the physical layer and the data link layer</u>, but is otherwise very similar to the one we’ll be working with. 

### Data Link Layer and Physical Layer

#### LAN

局域网；

#### Ethernet

电脑之间用一条以太电线连接。当传输数据的时候，所有电脑都能收到数据，但这样就不知道数据该给谁了，所以这个时候我们使用MAC(Media Access Control)地址作为传递数据的前缀来进行接收确认。

![](https://i.loli.net/2019/07/08/5d221a9b8d66682544.png)

#### CSMA

Carrier Sense Multiple Access 载波侦听多路访问

> **侦听 **是指电脑能通过侦听传输媒介获取信息；
>
> **多路访问** 是指多台电脑可以同时享用这一媒介。

指多台电脑共享一个传输媒介。

> 对于Ethernet来说，这个媒介就是铜线；
>
> 对于WIFI来说，这个媒介就是空气中的无限电波。

> 这种传输有个缺点就是一次只能服务一台机器，当多台电脑同时企图占用媒介的时候就会发生冲突（多台电脑同时占用媒介发报）。

> Ethernet的解决办法：
>
> 当检测到冲突的时候，会在重传之前，等待一小段时间（这一小段时间一定<u>不能一样</u>，因为如果时间一样会导致再度冲突，简单的解决办法就是使用随机的等待时间（比如说先等`1s+rand()`，如果继续发生了冲突，就等待`2s+rand()`，如果还发生就等待`4s+rand()`，以此类推。这种指数递增等待时间的方法叫做：**指数避退**））。
>
> > 指数避退的方法在Ethernet和 WIFI 以及其他协议上都被广泛使用。

#### 冲突域

一个载体中设备数量过多会导致更多的冲突，也会使整体的通讯变得低效，所以<u>我们需要降低一个载体下的设备数量</u>，即设置"**冲突域**"。

![](https://i.loli.net/2019/07/08/5d221f549a68032881.png)

比如，我们假设6个电脑的LAN经常容易发生冲突，所以我们分2组，在中间加上一个**交换机**(switch)：只有当需要跨界传输的时候才打开同理，否则不打开。(交换机会记录所需要的MAC地址是在A side还是B side，当A side穿过来的prefix中的MAC地址是b side电脑的地址，那么交换机就暂时打开，允许双方进行沟通)。

#### 路由 routing

早起通讯是通过A到B之间搭建各种**直连**路线（当然是有限的），当路线有空闲的时候，就使用它，否则就等到空闲为止。当然为了连接的方便与高效(特别对于军用系统和银行来说)，会建立专有路线，即只有特定人士才能使用的路线。

#### 报文交换 Message Switching

想想成邮政系统。

![](https://i.loli.net/2019/07/08/5d222200d2e6213033.png)

即使用不同的路由的**跳转**，提高通信系统的鲁棒性（如果某一个地方的路由系统坏了，那么就走另外一条路）。

消息连着路由进行跳转的速度叫做"跳数"(hop count)：

hop count会被记录在消息中，在传输的过程中会更新hop count。如果一个信息的hop count很高，那么很有可能就是出问题了（跳数限制）。

> 报文交换的缺点：有时报文很大，会堵塞线路。

#### 带宽 Band width

传输数据的速度。

### Network Layer

#### 分组交换 Packet Switching

报文交换的缺点：有时报文很大，会堵塞线路。所以我们将数据切成小小的数据包：packets。

##### IP决定目标地址

每个packet有个**目标地址**，即IP(由IP: Internet Protocol决定)。

##### 阻塞控制 congestion control

对于有些软件来说，他们要求收到的信息是可以按一定顺序的，而如果收到了乱序的信息，那么他们可能就凉了。TCP/IP协议的定制就是为了解决乱序问题。

### Internet

#### WAN

LANs were connected to Wide Area Network.

> WAN的路由器一般属于ISP(互联网服务提供商)

#### Packet传输

假如我们要访问YouTube服务器，我们先跳转N次到互联网主干(Backbone)，然后在主干里跳M次，然后再跳出来，最后继续跳K次跳到YouTube的服务器处。

 ![](https://i.loli.net/2019/07/08/5d2334cf2f39b53740.png)

我们知道IP就知道要给哪台电脑，但我们依旧不知道给哪个程序，这个时候，我们就需要更加高层的协议。

#### UDP

IP告诉我们数据是给哪台电脑的，UDP告诉我们数据是给哪个程序的。

需要联网的程序会从操作系统处申请一个端口，而UDP协议就要求packet需要携带对应的端口，当packet被传达到正确的端系统上时，通过匹配port，将数据传给对应的程序。

![](https://i.loli.net/2019/07/08/5d23360beceb654054.png)

UDP Header有很多元数据，除了port以外，还有checksum校验，即将所有的数据加起来用16位数保存(溢出就溢出)。

**缺点：**

- 不提供数据修复/数据重发；(坏数据直接丢掉)
- UDP无法得知数据是否到达对应的程序(无反馈)

> UDP: Simple and fast.
>
> 对于有的应用这些缺点很无所谓，比如视频直播，坏掉的数据丢掉就好了（卡一下），因为如果这个时候再重新请求，重新接受，会导致视频极其不流畅(无法保证实效性)。
>
> 而对于邮件而言，我们必须保证数据都到达且是正确的。

#### TCP

> "所有数据必须到达"

TCP在UDP的基础上加了很多特性比如：

- TCP包序号

即使数据到达时间不同，但是数据是被标上序号的，我们可以对其进行重新排序。

- 确认码

checksum对了后会回发一个确认码（ACK）；得知上一个数据包成功抵达后，发送方会发送下一个数据包。

在一定时间内如果没有收到ACK，就重发。

> 通过接受packet和ACK的效率，系统可以推导网络拥堵情况，这样就可以自动调整传输率。

**缺点**：

数据包大小翻倍，信息更加冗余；

#### 访问网页

需要：

- IP
- port

> 但难以记忆；

#### DNS

> 由DNS服务商提供。

```c++
ip_and_port_u_need = DNS[domain_name];
```

> 找不到网址的时候就会告知你：`DNS address couldn't be found.`

#### Domain Structure

![](https://i.loli.net/2019/07/08/5d233af195fb385274.png)

### Again: OSI 7 layers Network Model

- **Physical layer**: signal stuffs.
- **Data link layer**: MAC address, collision detection, exponential backoff ...
- **Network layer**: Routing and switching.
- **Transport layer**: UDP TCP … 描述了点到点传输的细节
- **Session layer**: TCP UDP: Open connection, pass information back and forth and then close the connection when finished.

![](https://i.loli.net/2019/07/08/5d233c4d04d2850406.png)

> What about WWW(World Wide Web)？
>
> **WWW runs on Internet.** Just an application.

