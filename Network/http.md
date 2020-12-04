### Content

- Start line;
- Headers;
- Body;

#### Request

Start line:

* Method (RESTful API)
  * GET: Read data;
  * POST: Read/Write data;
* URI: path/to/file
* HTTP Version

Headers: A list of KVs;

* Host: address of the server;
* Type of files: 
  * image/gif
  * text/html

There's no content in the body;

#### Response

Start line:

* HTTP V
* Status code;

Headers: KVs;

Body: file requested;

---



### 面试常问

#### HTTP的方法

GET: 通常用于请求服务器发送某些资源

**HEAD**: 请求资源的头部信息, 并且这些头部与 HTTP GET 方法请求时返回的一致. 该请求方法的一个使用场景是在**下载一个大文件前先获取其大小再决定是否要下载**, 以此可以节约带宽资源；

OPTIONS: 用于获取目的资源所支持的通信选项

POST: 发送数据给服务器

PUT: 用于新增资源或者使用请求中的有效负载替换目标资源的表现形式

DELETE: 用于删除指定的资源

PATCH: 用于对资源进行部分修改

CONNECT: HTTP/1.1协议中预留给能够将连接改为管道方式的代理服务器

TRACE: 回显服务器收到的请求，主要用于测试或诊断

#### GET和POST的区别

* 传输方式：
  * Get没有用body，直接通过uri传达数据；
  * post使用body的部分进行传输；
* 安全性：
  * 一般来说uri会被cache（浏览记录），而body不会；
* 数据类型：
  * get只用用ascii字符；post没有限制

#### PUT和POST的区别

* POST可以一次请求一群文件，而PUT只能请求一个；
* PUT=>幂等；

#### 常见状态码

* 2xx：成功
  * 200: OK
* 3xx：重定向
  * 301：永久的被分配了新的url；
* 4xx：客户端（原因引起的）错误
  * 400：报文语法错误
  * 401：需要认证；
  * 403：server rej；
  * 404：not found;
  * 408：timeout;
* 5xx：服务器（原因引起的）错误
  * 500 internal sever error，表示服务器端在执行请求时发生了错误；
  * 505：http版本不支持；

#### Keep alive

在早期的HTTP/1.0中，每次http请求都要创建一个连接，而创建连接的过程需要消耗资源和时间；reuse连接可以降低资源消耗和延迟；在后来的HTTP/1.0中以及HTTP/1.1中，引入了重用连接的机制，就是在http请求头中加入Connection: keep-alive来告诉对方这个请求响应完成后不要关闭，下一次咱们还用这个请求继续交流。协议规定HTTP/1.0如果想要保持长连接，需要在请求头中加上Connection: keep-alive。

#### HTTP2的优势

1. 可以存binary；协议解析更加高效；
2. 分帧；
3. 流：存在于连接中的一个虚拟通道。流可以承载双向消息，每个流都有一个唯一的整数ID；
4. 头部压缩；（只发diff）
5. HTTP 1.x 中，如果想并发多个请求，必须使用多个 TCP 链接，且浏览器为了控制资源，还会对单个域名有 6-8个的TCP链接请求限制。HTTP一个流只用一个连接；

> GRPC基于http2!