# SoundSystemUdp_Reciver
数字会议系统语音文件 第三方主机存储demo


需要使用到 windows   C++  自带的socket  Lib
默认 组播地址224.1.1.11  端口号 1000


1、音频数据包通过WIFI 的RJ45 口进行数据上发：
  A、音频采样率为48K；
  B、 每包数据大小为324 字节。 前4 个字节指示为话筒的ID 号。
  每2 个字节为话筒的 ID 号，重复一遍发送，即 XXYYXXYY， 如
  000600 06 为表示ID 号为6 的单元。
  剩余320 个字节， 包含160 个采样点， 即， 每2 个字节为一个采
  样点，打包时高字节在前、低字节在后；
  C、组播地址为224.1.1.11；
  
  
2、 数据包发送是在”WIFI”口，需要配合”ETHERNET”口发出的话筒
  开/关 MIC 命令来判断当前数据包为哪个 ID 号的话筒数据（ TCP 连
  接，端口号为10166， IP 地址为192.168.10.100）；
  
![image]https://github.com/ZhangSanFengByGit/image_folder/blob/master/protocal.png)
  
