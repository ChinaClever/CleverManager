minigw���뷽����VS���뷽����ο�VSĿ¼�µ�readMe

�ڱ���libcurlʱ������ʹ����lib�µ�makefile�ļ�ֱ�ӱ��룬���Ǳ��������ú�MinGW

�Ƚ�����ӵ����������У�Ȼ�����Make���뼴�ɡ�

���û��������Ĳ����� MINGW_HOME��path������㲻֪������ٶ�һ�£�����������Java�ȶ�һ���ģ�

1.�½� MINGW_HOME ֵΪC:\Qt\Qt5.5.1\Tools\mingw492_32 ����İ�װ·����

2.path������ӡ�;%MINGW_HOME%\bin;��

��������ļ���D:\workspace\curl-7.32.0\lib�£���ôcd��ȥ��Ȼ��ʹ�������������뼴�ɣ�

mingw32-make -f Makefile.m32

Ȼ��ȴ�������ɡ� �������֮�����ǿ�����curl��libĿ¼�¿��������ļ���libcurl.a��libcurl.dll��libcurldll.a�����ļ��������������Ҫ�����ӿ��ˡ�

Ȼ��curl�µ�include�ļ��и��Ƶ���ǰ����Ŀ¼�¡�
���������ڵ�ǰ����Ŀ¼���½�һ��lib�ļ��У�������õ�libcurl.a��libcurl.dll��libcurldll.a�����ļ��Ž�����

���֮�󣬽�libcurl.dll�ŵ�qt�İ�װĿ¼��binĿ¼��,���Է��֣����������������������п��Ա��뵫�������е����⡣

���һ�����á���libcurl_download_html.pro�ļ�����TEMPLATE = app�����һ�д��룺
LIBS+="./../lib/libcurl.dll"