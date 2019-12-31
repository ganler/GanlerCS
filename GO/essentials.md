## Installation for Mac

```shell
export GOPATH=$HOME/go
export GOROOT=/usr/local/opt/go/libexec
export PATH=$PATH:$GOPATH/bin
export PATH=$PATH:$GOROOT/bin
brew install go
```

## How to write GO code

[link](https://golang.org/doc/code.html)

> Most important part:
>
> 1. All GO codes(project codes & 3rd party codes) are typically kept in **ONE SINGLE WORKSPACE**.
> 2. Workspace
>    1. `src`
>    2. `bin`(go tool builds and installs binaries to the `bin` directory)
>    3. `pkg`
> 3. If any questions:
>    1. `go help`
>    2. https://godoc.org/

```shell
go(workspace)
	- bin
	- src
			- github.com
					- ganler
							- project1
									- aaa.go
							- project2
									- bbb.go
	- pkg
```

