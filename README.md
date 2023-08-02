# syslog_audit
linux登录日志 提交mysql与自动和管理
# 实战项目：日志审计系统

事件类型：ssh登录：密码错误，成功登录

日志工具：rsyslog `/etc/init.d/rsyslog ` `/var/log/auth.log`

- agent ：读取日志，插入数据库
- center：
  - 审计规则
    - 直接在数据库插入，从文件导入规则
    - 审计策略
      - 登录密码错误：日志，正则。结果：登录用户名，ip，端口，时间
      - 登录成功：日志、正则，结果：登录用户名，ip，端口，时间
  - 添加监控这杯：设备名称，设备id，设备ip，设备状态（心跳包）
  - 审计并写入审计结果
  - 注册用户
- client：
  - 显示日志 分页处理
  - 登录
  - 搜索日志：
    - 日期：测试索引和无效索引，效率
    - 内容：模糊查找like，全文查找
    - 插入测试数据：用事务插入两百万条记录

