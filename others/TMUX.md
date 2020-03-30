# TMUX

https://gist.github.com/ryerh/14b7c24dfd623ef8edc7

## CMDs

```shell
tmux new -s py # 建立名为py的session
```

## 快捷键

- `ctr b` + `d` 退出session
- `ctr b` + `%` 左右分屏
- `ctr b` + `"` 上下分屏
- `ctr b` + `c` 新建窗口
- `ctr b` + `n` 下一个会话 next
- `ctr b` + `p` 上一个会话 previous
- `ctr b` + `x` 取消当前的pane

### Resize

https://michaelsoolee.com/resize-tmux-panes/

### Cp

1、 C-b [ 进入复制模式
2、 参考上表移动鼠标到要复制的区域，移动鼠标时可用vim的搜索功能"/","?"
3、 安空格键开始选择复制区域
4、   选择完成后安enter键退出
5、 C-b ] 粘贴