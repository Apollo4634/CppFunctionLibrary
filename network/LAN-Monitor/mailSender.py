#coding=utf-8
import smtplib
from email.mime.text import MIMEText
import globalVar as gl

def __sending_to_a_receiver(msg_from,lic_code,msg_to,subject,content):
    '''
    msg_from = 'xxx@qq.com' #发送方邮箱
    passwd = 'dxdujqjvkasybcdj' #发送方邮箱的授权码
    msg_to = 'xxx@163.com' #收件人邮箱
    subject = 'subject' #主题     
    content = 'content' #正文
    '''
    msg = MIMEText(content)
    msg['Subject'] = subject
    msg['From'] = msg_from
    msg['To'] = msg_to
    try:
        s = smtplib.SMTP_SSL("smtp.qq.com",465) #邮件服务器及端口号
        s.login(msg_from, lic_code)
        s.sendmail(msg_from, msg_to, msg.as_string())
        #print("发送成功")
        return True 
    except:
        #print("发送失败")
        return False 
    finally:
        s.quit()

def __getContent(message):
    #获取邮件正文
    content = ''
    if isinstance(message,list):
        content += message[-1] + '\n'
        for msg in message[:-2]:
            content += msg
    else:
        content = message
    return content

def sending(message):
    #初始化
    msg_from = gl.get('msg_from')
    lic_code = gl.get('lic_code')
    msg_to = gl.get('msg_to')
    subject = gl.get('subject')
    content = __getContent(message)

    #开始发送
    _msg_to = [recevier for recevier in msg_to if recevier != '']
    for recevier in _msg_to:
        success = __sending_to_a_receiver(msg_from,lic_code,recevier,subject,content)
        if not success:
            print(recevier+': 邮件发送失败')
