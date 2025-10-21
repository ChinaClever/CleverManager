minigw编译方法：VS编译方法请参考VS目录下的readMe

在编译libcurl时，可以使用其lib下的makefile文件直接编译，但是必须先配置好MinGW

先将其添加到环境变量中，然后调用Make编译即可。

配置环境变量的参数是 MINGW_HOME和path，如果你不知道，请百度一下，方法和配置Java等都一样的：

1.新建 MINGW_HOME 值为C:\Qt\Qt5.5.1\Tools\mingw492_32 （你的安装路径）

2.path变量添加“;%MINGW_HOME%\bin;”

假如你的文件在D:\workspace\curl-7.32.0\lib下，那么cd过去，然后使用下面的命令编译即可：

mingw32-make -f Makefile.m32

然后等待编译完成。 编译完成之后，我们可以在curl的lib目录下看到几个文件：libcurl.a、libcurl.dll和libcurldll.a三个文件，这就是我们需要的链接库了。

然后将curl下的include文件夹复制到当前工程目录下。
接下来，在当前工程目录下新建一个lib文件夹，将编译好的libcurl.a、libcurl.dll和libcurldll.a三个文件放进来。

完成之后，将libcurl.dll放到qt的安装目录的bin目录下,测试发现，如果不进行这个操作，会有可以编译但不能运行的问题。

最后一个配置。打开libcurl_download_html.pro文件，在TEMPLATE = app下添加一行代码：
LIBS+="./../lib/libcurl.dll"