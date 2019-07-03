"""全局变量管理"""
import os

def init():
    global __global_dict
    __global_dict = {}
    __load_default()

def set(key,value):
    __global_dict[key] = value

def get(key,defValue=None):
    try:
        return __global_dict[key]
    except:
        return defValue

def __load_default():
    '''初始化'''
    # 查询时间间隔
    set('QueryInterval',3) #等待时间，单位minute
    set('DurationOfSleep',int(7.5*60)) #睡眠时间

    # 在线记录文件
    currentDir = os.path.dirname(os.path.abspath(__file__))
    set('currentDir', currentDir)
    set('recordFilePath', currentDir+'/records.txt')

    #局域网搜索设置
    set('ipPrefix','192.168.1') #ip前缀
    set('ipRange',[100,150]) #查询范围

    # 邮件设置
    set('msg_from','monitor@qq.com') #发送方邮箱
    set('lic_code','dxkojqmsbasydcqj') #发送方的邮箱授权码
    set('subject','Hello') #邮件主题
    set('msg_to', 
        [   #收件人邮箱
            'xxx@qq.com',
            'yyy@163.com'
        ]
    )

    # 设备列表
    deviceTable = {
        'sensitive':{
            #'08-66-98-75-83-45':'jinpingpleWatch *',
            '11-22-33-44-55-66':'Mr A',
            '12-23-34-45-56-67':'Miss B'
        },
        'insensitive':{
            '12-34-56-78-90-AB':'Router'
        }
    }
    set('deviceTable', deviceTable)

    # 消息发送时间表(周末除外) - 时间点不在此表时使用规则表
    set('TimeTable', [910,930,950,1000])

    # 消息发送规则表
    # 其中 0表示不在，1表示在
    RuleTable = { 
        #默认规则表，基于星期进行判断
        0:{(730,1030):'0001',(1900,2330):'100'}, #周一
        1:{(730,1030):'0001',(1900,2330):'100'}, #周二
        2:{(730,1030):'0001',(1900,2330):'100'}, #周三
        3:{(730,1030):'0001',(1900,2330):'100'}, #周四
        4:{(730,1030):'0001',(1900,2330):'100'}, #周五
        5:{(730,2330):'0001',(730,2330):'100'},  #周六
        6:{(730,2330):'0001',(730,2330):'100'},  #周日
        #基于日期的规则表，当前日期不在其中时使用默认规则表
        '1022':{(830,1030):'001',(1900,2230):'100'},
        '1116':{(830,2330):'001'},
        '1123':{(830,2330):'100'}
    } #时间段左开右闭，且不能重叠
    set('RuleTable', RuleTable)
