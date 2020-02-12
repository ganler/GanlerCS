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

- Inference with REST


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

## Quick Start

[link](https://github.com/tensorflow/serving)

## Thoughts

- grpc用的protobuf不适合tensor这种binary blob，因为protobuf是为小message设计的，这里有一些benchmark: [PaddlePaddle/Paddle#4610](https://github.com/PaddlePaddle/Paddle/pull/4610)。但通过`pack encoding`可以让数值型数据的field key只出现一次。
  - BTW，在gRPC中有个数据类型叫：`protobuf repeated`，相当于vector，可以用来存放N个相同类型的内容。每个repeated都会有个field tag（类型，长度）。

#### Reference:

- [PaddlePaddle的Serving调研](https://github.com/PaddlePaddle/cloud/issues/394)

