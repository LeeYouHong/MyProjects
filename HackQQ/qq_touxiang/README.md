#下载所有qq头像

偶然发现qq头像的地址为*http://qlogo3.store.qq.com/qzone/[qq]/[qq]/100*,比如qq号10004的头像地址为*http://qlogo3.store.qq.com/qzone/10004/10004/100*,于是决定写个爬虫去下载大量qq头像玩玩。并不是所有的qq头像都可以下载到，原因有待研究。

### Python版本
使用方法：
下载[qq_touxiang.py](/qq_touxiang/qq_touxiang.py)
命令格式：`python qq_touxiang.py 起始qq 终点qq `

比如下载qq号从10000到20000之间的头像,使用`python qq_touxiang.py 10000 20000`


*注意：*在本人的环境中sys.maxint=2147483647,所以最大qq号不能超过这个值，否则会报错。

### JavaScript版本
下载[qq_touxiang.html](/qq_touxiang/qq_touxiang.html)，并用浏览器打开。
