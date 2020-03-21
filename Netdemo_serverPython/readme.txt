这是以太网络控制的上位机实现部分。
该程序可以实现：
1.网络摄像机的实时预览（该版本为SDK版本），实时鱼眼矫正（可选），抓图保存
2.与下位机进行TCP/UDP(可选)通讯，并实时绘制下位机上传的数据流
3.文本窗展示并记录交互信息。并可完成停起，复制，建立并保存至txt文件等。
4.发送指令至下位机。

运行注意：
安装相关的python依赖库后（即Netdemo.py中所有需要import的库），在python3环境下运行Netdemo.py。
请至hikvision.com下载SDK开发包CH-HCNetSDKV6.1.4.6_build20191220_Win64，解压文件。
并将CH-HCNetSDKV6.1.4.6_build20191220_Win64\lib添加到系统路径。
dist文件夹下有可执行.exe文件，但仍建议在python环境下运行.py文件。.exe程序存在容易断连，执行效率慢等问题。
