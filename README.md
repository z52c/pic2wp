##pic2wp概览

 - 将图片站(你懂得)的文章搬运到wordpress站点中
 - wordpress中安装Featured Image From URL插件作为特色图插件
 - 图片站目前支持t66y.com（图片区）,meitulu.com
 - 图床目前支持s6tu.com,s2tu.com,pic303.com(目前有问题)，sm.ms
 - 支持socks5代理

##类
###图源-picsource
图源可以是链接或者是文件列表，根据是否上传到图床在mainWindow中调用图床类

###图床-tuchuang
图床接收一个文件列表以及指定图床类型，上传到指定图床，返回一个图片链接列表

###wordpress-wpcontent
支持登录，获取分类列表，发表文章
发表：接收标题、图片链接列表、分类


##其他
后期要添加其他图源或者图床，只需按照其中一个类的signal写好新的类添加到picsource或者tuchuang中即可
pic303中问题未修改
图床貌似有每日上传数量限制，但是一般够用
meitulu有防盗链，所以一定要勾选上传到图床
t66y一般用的是图床外链，可以直接用，t66y使用的部分图床可能已经被墙
