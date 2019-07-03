import os
import platform
import re
import globalVar as gl
from socket import gethostbyname, gethostname

class __IP2MAC:
    def __init__(self):
        self.patt_mac = re.compile('([a-f0-9]{2}[-:]){5}[a-f0-9]{2}', re.I)

    def getMac(self, ip):
        sysstr = platform.system()
        if sysstr == 'Windows':
            macaddr = self.__forWin(ip)
        elif sysstr == 'Linux':
            macaddr = self.__forLinux(ip)
        else:
            macaddr = None
        return macaddr or '00-00-00-00-00-00'

    def __forWin(self, ip):
        os.popen('ping -n 1 -w 500 {} > nul'.format(ip))
        macaddr = os.popen('arp -a {}'.format(ip))
        macaddr = self.patt_mac.search(macaddr.read())
        if macaddr:
            macaddr = macaddr.group()
        else:
            macaddr = None
        return macaddr

    def __forLinux(self, ip):
        os.popen('ping -nq -c 1 -W 500 {} > /dev/null'.format(ip))
        result = os.popen('arp -an {}'.format(ip))
        result = self.patt_mac.search(result.read())
        return result.group() if result else None

class __IP2MAC_v2:
    def __init__(self):
        # 获取本机IP地址
        self.host = gethostbyname(gethostname())
        self.currentDir = gl.get('currentDir')
    
    def getMacList(self):
        macList = []
        arpFilePath = self.currentDir+'/temp.txt'
        # 获取ARP表
        os.system('arp -a > '+arpFilePath)
        with open(arpFilePath) as fp:
            for line in fp:
                line = line.split()[:2]
                if line and line[0].startswith(self.host[:4]) and (not line[0].endswith('255')):
                    #print('\t'.join(line))
                    if line[0] != '192.168.1.1':
                        macList.append(line[1].upper())
        return macList

def mac():
    #初始化
    deviceTable = gl.get('deviceTable')
    ipPrefix = gl.get('ipPrefix')
    ipRange = gl.get('ipRange')
    
    content = ('','') #邮件正文
    macData_sensitive = deviceTable['sensitive'].keys()
    macData_insensitive = deviceTable['insensitive'].keys()

    #查询
    
    g = __IP2MAC()
    macList = []
    nFailure = 0
    for i in range(ipRange[0],ipRange[1]):
        ip = ipPrefix+'.'+str(i) #当前ip
        nloop = 10 #循环次数 
        while nloop>0: #若检测ipRange中的地址失败，则循环检测
            mac = g.getMac(ip)
            nloop -= 1
            if mac != '00-00-00-00-00-00':
                macList.append(mac.upper())
                break
        nFailure = 0 if(nloop!=0) else nFailure+1
        if nFailure>10: #如果连续10个ip查询失败则直接停止查询
            break
    

    #查询 version2
    '''
    g = __IP2MAC_v2()
    macList = g.getMacList()
    '''
    
    #返回结果
    content_sensitive = ''
    content_insensitive = ''
    for mac in macList:        
        if mac in macData_sensitive:
            content_sensitive += (mac+'  '+deviceTable['sensitive'][mac]+'\n')
        elif mac in macData_insensitive:
            content_insensitive += (mac+'  '+deviceTable['insensitive'][mac]+'\n')
        else:
            content_insensitive += (mac+'\n')

    #content = (content_sensitive, content_insensitive)
    content = content_sensitive + content_insensitive
    return content,macList
    