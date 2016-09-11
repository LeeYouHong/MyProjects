# coding=utf-8

#描述：经过分析qq号的头像地址都是:http://qlogo3.store.qq.com/qzone/[qq]/[qq]/100,比如qq号10004的头像地址为http://qlogo3.store.qq.com/qzone/10004/10004/100,不过经分析有的qq号头像显示为空,,，但有的又有，原因待研究。
#注意：因为在本人机子上测得sys.maxint=2147483647,所以qq号最好不超过这个值，对于大于这个值的qq号，有待解决方案
#时间：2014-09-05

import urllib2
import math
import sys
import string

first = string.atoi(sys.argv[1]);
last = string.atoi(sys.argv[2]);
for i in xrange(first, last):
    qq = str(i)
    url = 'http://qlogo3.store.qq.com/qzone/'+qq+'/'+qq+'/100'
    response = urllib2.urlopen(url)
    image = response.read()
    #有的qq头像得到的是一个2055大小的空白图片，所以这里过滤掉一下
    if len(image) > 2055:
        fileHandle = open(str(i)+'.jpg', 'wb')
        fileHandle.write(image)
        fileHandle.close()
