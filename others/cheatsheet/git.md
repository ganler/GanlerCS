## Submodules

```shell
# Add submodule
	# .gitmodules will be generated.
git submodule add https://github.com/someone/project

# Download with submodules:
git submodule update --init --recursive

# Update the urls of submodules
	# 1. 更新.gitsubmodule中对应submodule的条目URL
	# 2. 更新 .git/config 中对应submodule的条目的URL
# 3.
git submodule sync
```

