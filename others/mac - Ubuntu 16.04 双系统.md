# mac搭建[Ubuntu16.04 双系统]

>  **机器**
>
> - macOS High Sierra: 10.13.1 (17B1003)
>
> - 一个空u盘



## 1. 适当清理存储空间

> 界面主页左上角 -> 关于本机 -> 存储空间 -> 管理
>
> 进入该界面后可以人工删除一些没用的大文件，以及清空垃圾篓也能释放存储空间。



## 2. 下载桌面镜像

> [点击下载](http://mirrors.aliyun.com/ubuntu-releases/16.04/ubuntu-16.04.5-desktop-amd64.iso)



## 3. 下载并安装Etcher

> [点击下载](https://github-production-release-asset-2e65be.s3.amazonaws.com/45055693/43492694-493f-11e8-96b7-5486965222dd?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAIWNJYAX4CSVEH53A%2F20180923%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20180923T014715Z&X-Amz-Expires=300&X-Amz-Signature=51df4ae8689fbce9db5f2b297730acc52e19945dd085c572730126fe49ef3f03&X-Amz-SignedHeaders=host&actor_id=38074777&response-content-disposition=attachment%3B%20filename%3DEtcher-1.4.4.dmg&response-content-type=application%2Foctet-stream)



## 4. 使用disk utility(磁盘工具)格式化驱动器

> 该工具Mac系统自带。

> - 插入无文件的U盘；
> - 打开磁盘工具；
> - U盘在`外部`（External）处，选中自己的U盘，点击`抹掉`，设置：
>   - `Name` - 自定义
>   - `Format`(格式) - `MS-DOS(FAT)`
> - 点击`抹掉`；
> - 记住右下角`设备`处显示的名字。（比如`disk2`）



## 5. 使用Etcher

> - 打开软件
> - [Selecte image]：选择刚刚下载的镜像iso
> - [Select drive]：选择刚刚格式化的u盘
> - [Flash]
>
> ![img](https://s1.ax1x.com/2018/09/23/iuGuY4.png)



## 6. 安装rEFInd

> 1.下载[rEFInd的二进制包](http://sourceforge.net/projects/refind/files/0.11.2/refind-bin-0.11.2.zip/download)。
>
> 2.解压缩下载的文件。
>
> 3.打开终端。
>
> 4.将“refind-installer”文件拖到“终端”图标上以运行脚本。(先别这么干)
>
> > 在继续或从恢复分区安装rEFInd之前，可能需要禁用系统完整性保护（SIP）。为此：
> >
> > - 重启（先关机，在按power键开机）。
> > - 当启动屏幕出现时，按住`Command+ R`直到屏幕上出现Apple徽标（其实出现后继续按几秒成功率更大）。
> > - 完成加载并进入恢复模式后，单击“实用程序 - >终端”。
> > - 在“终端”窗口中，键入`csrutil disable`并按Enter键。
> > - 重启
> >
> > （之后再重复上述步骤即可）



## 7. 分区

> - 回到磁盘工具；
> - 点击`分区`（partition）；
> - 点击饼状图下面的`+`号，蓝色部分表示的是分区的大小（比如15~20GB即可）；
> - 格式设置为`MS-DOS(FAT)`，命名随意；
> - 点击应用（可能需要点时间）。

配置如下图：

![img](https://s1.ax1x.com/2018/09/23/iul3a4.png)





## 8. 重启Mac

> - 通过按上下左右键选择那个【企鹅】的图标（就是linux）。
>
> - 选择intall Ubuntu



## 9. 之后的步骤

> 网上有很多详细的教程，我就不制造重复垃圾了：
>
> [ref链接](https://blog.csdn.net/fesdgasdgasdg/article/details/54183577)
>
> > 只需看中间安装Ubuntu的部分即可。



## 10. 之后的事情

> - 首先，以后启动是默认到Ubuntu的界面，如果要回到mac，就先关机然后开机的时候按住`option`键，即可。
> - 刚装好的Ubuntu还功能不全，建议下载一些应用（比如搜狗拼音等）。
> - 刚装好的时候，可能mac的屏幕很暗，直接在设置里调节屏幕亮度即可。
> - 可能一开始你会对Ubuntu系统下触控板的体验感到难过，你可以去网上查找`fusuma`的教程，让触控板的体验更加靠近mac。
> - 什么？你连主题都想和mac差不多？ok，可以参考[这篇文章](https://blog.csdn.net/ty_393148439/article/details/76539701)。 