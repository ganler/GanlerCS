[TOC]

## conda & Python

```shell
# LINUX without ROOT
wget https://repo.anaconda.com/archive/Anaconda3-2019.10-Linux-x86_64.sh
bash ./Anaconda3-2019.10-Linux-x86_64.sh
echo 'export PATH=$HOME/anaconda3/bin/:$PATH'>>~/.zshrc

# Create env
conda create -n NAME python=3.7 anaconda 
```

## Config CUDA version

```shell
nvcc --version
# or
cat /usr/local/cuda/version.txt
```

## Choose a cuda dev

```shell
CUDA_VISIBLE_DEVICES=N COMMAND
```

## Auto SSH Connection

[stackoverflow](https://serverfault.com/questions/241588/how-to-automate-ssh-login-with-password)

