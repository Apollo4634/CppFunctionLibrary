''' 
判断是否发送邮件
该判断取决于当前时间、当前设备在线情况、上一时刻的设备在线情况
'''
import os
import timeString
import globalVar as gl

###########################################
#文件读写
def readRecordsFile(macTxtPath):
    timeString_old = ''
    onlineRecords = []
    if os.path.exists(macTxtPath): #判断文件是否存在
        with open(macTxtPath,'r') as rFile: #若存在则进行读取
            contents = rFile.read()
            if len(contents) > 0:
                if contents[-1] != '\n': #若只有时间戳则最后的字符为换行符
                    onlineRecords = contents.split('\n')
                    timeString_old = onlineRecords.pop(0)
                    if onlineRecords[-1]=='\n' or onlineRecords[-1]=='':
                        del onlineRecords[-1]
    return timeString_old,onlineRecords

def writeRecordsFile(macTxtPath,hourAndMinute_now,online_now,onlineRecords=[]):
    timeString_now = timeString.get('%Y%m%d')
    if len(onlineRecords)==0: 
        with open(macTxtPath,'w') as rFile: #重新记录
            rFile.write(timeString_now+'\n') #年月日
            rFile.write(hourAndMinute_now+' '+str(online_now)) #当前状态
    else: 
        with open(macTxtPath,'w') as rFile: #追加记录
            rFile.write(timeString_now+'\n')
            for record in onlineRecords: #存储之前的状态
                rFile.write(record+'\n')
            rFile.write(hourAndMinute_now+' '+str(online_now))

###########################################
#判断敏感人物是否在当前mac列表中
def inTheList(macData_sensitive,macList): 
    inFlag = 0
    for mac in macData_sensitive:
        if mac in macList:
            inFlag = 1
            break
    return inFlag

def convertRecord(record):
    hourAndMinute = ''
    online = ''
    if record != '':
        tmp = record.split(' ')
        if len(tmp) == 2:
            hourAndMinute,online = tmp
    return int(hourAndMinute),online
    
###########################################
#无当日mac记录时的默认操作
def actionWithoutRecords():
    return False #暂无

#有当日mac记录时的操作
def actionWithRecords(hourAndMinute_now,online_now,onlineRecords,RuleTable):
    #获取判断规则
    timeString_now = timeString.get('%m%d') #当前的月份与日期
    if timeString_now in RuleTable.keys(): 
        rule = RuleTable[timeString_now] #使用基于日期的规则表
    else:
        w = timeString.getDayOfWeek() #星期
        rule = RuleTable[w] #使用默认的规则
    
    #根据规则进行判断
    template = '' #匹配模板
    for timeInterval in rule.keys():
        if timeInterval[0] <= hourAndMinute_now < timeInterval[1]:
            template = rule[timeInterval]
            break

    sendFlag = False
    if template == '':
        return sendFlag
    else:
        tempLen = len(template) #模板的长度
        recordsLen = len(onlineRecords)
        if template[-1] == str(online_now): #若当前状态符合要求
            if tempLen-1 <= recordsLen:
                #转换
                '''
                template = template[:-1]
            
                '''

                onlineString = '' #0和1表示的一组字符串，表示在线历史记录
                for i in range(recordsLen):
                    hourAndMinute,online = convertRecord(onlineRecords[-1-i])
                    if timeInterval[0] < hourAndMinute < timeInterval[1]:
                        onlineString = online + onlineString
                    else:
                        break
                    if tempLen-1 == len(onlineString):
                        if template[:tempLen-1] == onlineString:
                            sendFlag = True
                            break
                if tempLen-1 <= len(onlineString):
                    if template[:tempLen-1] == onlineString[-(tempLen-1):]:
                        sendFlag = True
    return sendFlag

###########################################
# 主函数
def flag(macList_now,hourAndMinute_now,sendflag_last):
    #参数初始化
    QueryInterval = gl.get('QueryInterval')
    deviceTable = gl.get('deviceTable')
    TimeTable = gl.get('TimeTable')
    RuleTable = gl.get('RuleTable')
    recordFilePath = gl.get('recordFilePath')
    sendFlag = False #输出：是否执行发送操作

    #开始运行
    macData_sensitive = deviceTable['sensitive'].keys()
    isOnline_now = inTheList(macData_sensitive, macList_now) #当前是否在

    #hourAndMinute_now = timeString.get('%H%M') #当前的小时和分钟
    hm_now = int(hourAndMinute_now)
    timeString_now = timeString.get('%Y%m%d') #当前的年月日
    timeString_old,onlineRecords = readRecordsFile(recordFilePath) #在线记录和时间戳

    #基于时间表的发送判断
    if len(TimeTable)>0 and timeString.getDayOfWeek() not in [5,6]:
        recentMomentList = [hm_now-time for time in TimeTable if hm_now>=time]
        if len(recentMomentList) != 0:
            recentMoment = min(recentMomentList)
            #若上一次没有发送，且时间间隔（minute）小于查询间隔
            if not sendflag_last and recentMoment <= QueryInterval: 
                sendFlag = True

    #基于规则表的发送判断
    if not sendFlag:
        if len(onlineRecords)==0: #上一次的records.txt为空
            sendFlag = actionWithoutRecords()
        else: #上一次的records.txt不为空
            if timeString_now != timeString_old: #不是同一天
                sendFlag = actionWithoutRecords()
            else:
                #若记录和当前值属于同一天，则判断敏感人物的在线情况
                sendFlag = actionWithRecords(hm_now,isOnline_now,onlineRecords,RuleTable)

    #更新records.txt
    if len(onlineRecords)==0 or (timeString_now!=timeString_old): #records.txt为空或者上次的不是同一天
        writeRecordsFile(recordFilePath,hourAndMinute_now,isOnline_now) #重新记录
    else: #上一次的records.txt不为空
        writeRecordsFile(recordFilePath,hourAndMinute_now,isOnline_now,onlineRecords) #更新记录

    record = hourAndMinute_now+'  '+str(isOnline_now)+'\n'
    return sendFlag,record
