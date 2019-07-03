import time

#时间
def getFullTimeString(): #解析时间 201810202258
    now = time.time()
    timeArray = time.localtime(now)
    timeString = time.strftime("%Y%m%d%H%M",timeArray)
    return timeString

def getFullTimeString_w(): #解析时间 201810202258
    now = time.time()
    timeArray = time.localtime(now)
    timeString = time.strftime("%Y%m%d%H%M%w",timeArray)
    return timeString

def getDayOfWeek():
    now = time.time()
    timeArray = time.localtime(now)
    return timeArray.tm_wday

def get(timeFormate):
    now = time.time()
    timeArray = time.localtime(now)
    timeString = time.strftime(timeFormate,timeArray)
    return timeString

def sleep(seconds):
    time.sleep(seconds)

def sleepForMinutes(minutes):
    time.sleep(minutes*60)
