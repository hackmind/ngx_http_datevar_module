<p>
nginx日期变量模块, 实现过程参考了taobao的Tengine相关代码(http://tengine.taobao.org/).<br/>
该模块主要作用是提供了以下变量: <br/>

year: 当前年份(2位) <br/>
year2: 当前年份(4位) <br/>
month: 当前月份(01-12) <br/>
hour: 当前小时(00-23) <br/>
hour12: 当前小时(01-12) <br/>
minute: 当前分钟(00-59) <br/>
second: 当前秒数(00-59) <br/>

</p>

<p>
该模块可用于nginx access log按日期切分文件,免去使用"cron + mv + kill"的蹩脚脚本。<br/>

例如: <br/>

access_log  logs/access_$year$month$day$hour.log  main; <br/>
open_log_file_cache max=4 inactive=60s; <br/>

注意，记得配置open_log_cache,否则对性能影响较大 <br/>


