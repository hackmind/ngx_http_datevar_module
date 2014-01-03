nginx日期变量模块, 实现过程参考了taobao的Tengine相关代码(http://tengine.taobao.org/).
该模块主要作用是提供了以下变量:
year: 当前年份(2位)
year2: 当前年份(4位)
month: 当前月份(01-12)
hour: 当前小时(00-23)
hour12: 当前小时(01-12)
minute: 当前分钟(00-59)
second: 当前秒数(00-59)

该模块可用于nginx access log按日期切分文件,免去使用"cron + mv + kill"的蹩脚脚本。
例如:

access_log  logs/access_$year$month$day$hour.log  main;

#记得配置open_log_cache,否则对性能影响较大
open_log_file_cache max=4 inactive=60s;


