# TF-Serving

## Features

- Workes well for large models up to 2GB. Because the protobuf file format has 32 bit limit.
- Model versioning: using the newest model.

## Pipeline

- Export model
  - Variables.
  - Checkpoint.
  - Model as protobuf(`.pb`).
  - Assets contains additional files.
- Serving:

```shell
docker pull tensorflow/serving
docker run ... -t tensorflow/serving:latest-gpu
```

- Inference with REST/RPC


```python
import json
from requests import HTTPSession

# Default REST API port is 8051.
def rest_request(text, url=None):
  	if url is None:
      	url = 'http://localhost:8051/v1/models/my_model:predict'
    payload = json.dump({'instances': [text]})
    response = http.request('post', url, payload) # 提交数据
    return response

rs = rest_request(text='classify my text')
print(rs.json)
```

- Inference with gRPC
  - Inference data needs to be converted to the protobuf format.
  - Request types have designated types, e.g. float, int, bytes.
  - Payloads need to be converted to base64.
    - Base64 review：可打印的asic字符有64个，完整的asic字符理论上最多有256个，那么我们可以用6bit而不是8bit来表示一个「可打印的」字符。现在我们有一串byte buffer，我们每6位将其填充成8位即能完成base64编码。
  - Connect to the server via gRPC stubs.

## Arch

- 当TF Serving发现磁盘上的模型文件，该模型服务的生命周期就开始了。

- **Source & Loader**

  - 负责发现模型文件，找出需要加载的新模型。实际上，该组件**监控文件系统**，当发现一个新的模型版本，就为该模型创建一个Loader。

  - Loader可以知道模型的相关信息，包括估算需要的RAM/显存等。每个loader带一个指针，指向磁盘上存储的模型。但一开始时，Loader并不会加载模型。

  - Loader创建完成之后，Source通知Manager，告诉他这是一个待加载的版本。

- **Manager**

  - Case 1: 模型首次推送部署，Manager要先获取相应的资源，于是告诉loader去加载该模型；

  - Case 2：模型已被加载，Manager会查询Version Policy插件，决定加载模型的流程该如何进行；

  - 具体来说，当加载新模型时，可选择保持 **(1) 可用性** 或 **(2) 资源**。

    - 如果选(1)可用性，意味着我们倾向于保证系统对用户的响应。Manager让Loader实例化新的计算图和新的权重。此时模型的**两个版本被都被加载**，也就是说Manager**先加载新版本模型确保其可以安全服务后，然后再卸载原版本模型**。

    - 如果我们希望**在切换模型时节约资源**，可以选择模式（2）。对于重量级模型也许挺有用，模型切换间会有极短的可用性缺口，不过可以换取内存不足。

- **CLIENT**：

  - 当用户请求模型inference的时候，Manager会返回句柄给Servable；

## Quick Start

```shell
# Download the TensorFlow Serving Docker image and repo
docker pull tensorflow/serving # full gpu version: tensorflow/serving:latest-gpu
git clone https://github.com/tensorflow/serving
# Location of demo models
TESTDATA="$(pwd)/serving/tensorflow_serving/servables/tensorflow/testdata"
# Start TensorFlow Serving container and open the REST API port
docker run -t --rm -p 8501:8501 \
  -v "$TESTDATA/saved_model_half_plus_two_cpu:/models/half_plus_two" \
  -e MODEL_NAME=half_plus_two \
  tensorflow/serving &
# Query the model using the predict API
curl -d '{"instances": [1.0, 2.0, 5.0]}' \
  -X POST http://localhost:8501/v1/models/half_plus_two:predict
# Returns => { "predictions": [2.5, 3.0, 4.5] }
```

### TF Serving  Configuration

#### Model Server Config

##### Command Line Args

- **Docker**：Set MODEL_NAME variable to the model.

- **Using** **`tensorflow_model_server`**(Single model)：
  - `--model_name`：Name your model.
  - `--base_path`：Path to your saved model.

启动官方的`half_plus_two`模型，并使用RESTful API：

```shell
tensorflow_model_server --model_base_path=/path/to/saved_model_half_plus_two_gpu --model_name=half_plus_two --rest_api_port=8501
```

那么client端只需要这么调用：

```shell
curl -d '{"instances": [1.0, 2.0, 5.0]}' -X POST http://localhost:8500/v1/models/half_plus_two:predict
```

- `-d`：即`--data`，以HTTP POST的方式传送数据；
- `-X`：即`--request`，指定命令（这里对应POST，在RESTful API中，POST）；

##### Configuration File

- User `--model_config_file` to point out the configuration file.

The Model Server configuration file provided must be an ASCII [ModelServerConfig](https://github.com/tensorflow/serving/blob/master/tensorflow_serving/config/model_server_config.proto#L76) protocol buffer. Refer to the following to understand [what an ASCII protocol buffer looks like](https://stackoverflow.com/questions/18873924/what-does-the-protobuf-text-format-look-like).

单独模型

```json
model_config_list {
  config {
    name: 'half_plus_two',
    base_path: '...',
    model_platform: 'tensorflow',
    model_version_policy: {
      all: {}
    }
  }
}
```

此时只需要：`tensorflow_model_server --model_config_file=half_plus_two.config --rest_api_port=8051`即可。

**注意！！！**

- 不能有`//`注释；
- `base_path`中不要用`$HOME, $(pwd)`这种，没用的，直接走绝对地址即可；

多模型

```json
model_config_list {
  config {
  	name: '1st_model',
  	base_path: '...'
	}
	config {
    name: '1st_model',
  	base_path: '...'
  }
}
```

#### Monitoring Config

`--monitoring_config_file` 指定对应的`MonitoringConfig`文件；

```json
prometheus_config {
  enable: true,
  path: "/monitoring/prometheus/metrics"
}
```

`tensorflow_model_server --model_config_file=half_plus_two.config --rest_api_port=8501 --monitoring_config_file=half_plus_two.monitor`

client端只需要：

`curl http://localhost:8501/monitoring/prometheus/metrics`

#### Batching Config

用于设置批处理；

- `--enable_batching`来启动
- `--batching_parameters_file`配置文件的位置

```json
max_batch_size { value: 128 }
batch_timeout_micros { value: 0 }
max_enqueued_batches { value: 1000000 }
num_batch_threads { value: 8 }
```

具体`tf serving`做`batching`的细节在[这](https://github.com/tensorflow/serving/blob/master/tensorflow_serving/batching/README.md).

- `max_batch_size`: The maximum size of any batch. 
- `batch_timeout_micros`: The maximum amount of time to wait before executing a batch (even if it hasn't reached `max_batch_size`). Used to rein in tail latency. (See `basic_batch_scheduler.h` for the exact latency contract.)
- `num_batch_threads`: The degree of parallelism, i.e. the maximum number of batches processed concurrently.
- `max_enqueued_batches`: The number of batches worth of tasks that can be enqueued to the scheduler. Used to bound queueing delay, by turning away requests that would take a long time to get to, rather than building up a large backlog.

#### Misc. Flags

- `--port`：开放8500的gRPC端口
- `--per_process_gpu_memory_fraction=0.5`：允许模型使用gpu百分之多少（这里是一半）的显存
- `--rest_api_port`：启用RESTful API，并设置其端口
- `--help`：查看更多的参数

## Thoughts

- grpc用的protobuf不适合tensor这种binary blob，因为protobuf是为小message设计的，这里有一些benchmark: [PaddlePaddle/Paddle#4610](https://github.com/PaddlePaddle/Paddle/pull/4610)。但通过`pack encoding`可以让数值型数据的field key只出现一次。
  - BTW，在gRPC中有个数据类型叫：`protobuf repeated`，相当于vector，可以用来存放N个相同类型的内容。每个repeated都会有个field tag（类型，长度）。

#### Reference:

- [PaddlePaddle的Serving调研](https://github.com/PaddlePaddle/cloud/issues/394)

