""" 
局域网设备查询与通知系统 v1.7

功能说明：
- 可以实现局域网设备在线情况的查询，并通过邮件通知
- 基本原理为，
  搜索局域网内的设定网段的ip，通过ARP协议获取mac地址，
  经过是否通知的判断后，通过QQ邮箱向用户发送设备在线情况
"""
import mailSender
import macQueryServer
import sendManager
import timeString
import globalVar as gl

if __name__ =='__main__':
    #变量初始化
    gl.init()
    QueryInterval = gl.get('QueryInterval')
    DurationOfSleep = gl.get('DurationOfSleep')

    # 循环检测
    sendflag_last = False
    MessageList = ['']
    while True:
        # 查询局域网内的设备
        content,macList = macQueryServer.mac()

        # 邮件正文
        hourAndMin_now = timeString.get('%H%M') #当前时间
        fullMessage = hourAndMin_now + ':\n' + content
        print(fullMessage)

        # 基于判断准则获取是否发送的标志
        sendFlag_now,record = sendManager.flag(macList,hourAndMin_now,sendflag_last)
        sendflag_last = sendFlag_now

        #邮件正文记录表
        MessageList[-1] = record
        MessageList.append(fullMessage)
        if len(MessageList) > 7: #n-2条历史记录
            MessageList.pop(0)

        # 发送消息（时间早于7点半或晚于23点半都不发送）
        if sendFlag_now and 730 < int(hourAndMin_now) < 2330: 
            mailSender.sending(MessageList)
        
        # 等待更新
        if int(hourAndMin_now) > 2330:
            timeString.sleepForMinutes(DurationOfSleep) #夜间睡眠
        else:
            timeString.sleepForMinutes(QueryInterval) #正常等待
    