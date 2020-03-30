# Thrift

[Wiki](https://zh.wikipedia.org/wiki/Thrift)

[强烈推荐](https://diwakergupta.github.io/thrift-missing-guide/#_generated_code)

## Overview

- 「核心功能」
  - **RPC框架**
  - **多语言代码生成器**
  - **序列化框架**
- 跨语言

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/d/df/Apache_Thrift_architecture.png/400px-Apache_Thrift_architecture.png" style="zoom:50%;" />

- Thrift 支持众多**通讯协议**：
  - **TBinaryProtocal**： 一种简单的二进制格式，简单，但没有为空间效率而优化。比文本协议处理起来更快，但更难于调试；
  - **TCompactProtocol** – 更紧凑的二进制格式，处理起来通常同样高效。
  - **TDebugProtocol** – 一种人类可读的文本格式，用来协助调试。
  - **TDenseProtocol** – 与TCompactProtocol类似，将传输数据的元信息剥离。
  - **TJSONProtocol** – 使用[JSON](https://zh.wikipedia.org/wiki/JSON)对数据编码。
  - **TSimpleJSONProtocol** – 一种只写协议，它不能被Thrift解析，因为它使用JSON时丢弃了元数据。适合用脚本语言来解析。
- **传输协议**：
  - **TFileTransport**
  - **TFramedTransport**：当使用一个**<u>非阻塞服务器</u>**时，要求使用这个传输协议。它按帧来发送数据，其中每一帧的开头是长度信息。
  - **TMemoryTransport**：存储器映射输入输出；
  - **TSocket**：使用阻塞的套接字I/O来传输。
  - **TZlib**：用zlib做压缩。
- **服务器**：
  - **TNonblockingServer** – 一个**多线程服务器**，它使用**非阻塞**I/O（Java的实现使用了[NIO](https://zh.wikipedia.org/wiki/Java_NIO)通道）。TFramedTransport必须跟这个服务器配套使用。
  - **TSimpleServer** – 一个**单线程服务器**，它使用标准的阻塞I/O。测试时很有用。
  - **TThreadPoolServer** – 一个**多线程服务器**，它使用标准的**阻塞**I/O。

## Key Notes

### Theory

- String -> UTF8
- Binary Type ->`Array<byte>` (0.10.0被干掉了，type变成了string，val变成[]byte)
- 序列化协议主要是Binary/Compact/JSON
  - 数据类型：
    - 简单数据类型`bool | byte | i8 | i16 | i32 | i64 | double`
    - 复合数据类型`string | binary | map | set | list | struct`
    - 特殊数据类型`void | stop`
  - 二进制编码（Binary/Compact的实现，不同主要在于对于整数的处理）常用TLV编码，基本概念是`[Tag(1 byte) | 编号(2byte) | Length | Value]`（grpc直接把类型和编号合并了），可嵌套。
    - 详情看[这里](https://erikvanoosten.github.io/thrift-missing-specification/)。
  - Compat序列化的不同主要在于**整数类型**采用`zigzag` 和 `varint`压缩编码实现，细节略，说说好处：
    - `varint`: 小数字的字节数少；varint解决了无符号编码的问题，但有符号数的效果不好，所以`zigzag`先把有符号数映射到无符号数上；

### Usage

- `namespace cpp a.b.c`就是把这些代码搞成`a::b::c`这个空间去；
- 引号include
- 每个field必须有个唯一的整数标志位；
- Service：
  - `oneway`：client只打request不需要response，也不用等待；

## Quick Example

- 写IDL
- 生成rpc代码
- server端实现response
- client实现request