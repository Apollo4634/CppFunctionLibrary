import os
import shutil
import glob

'''
将txt记录的dll文件复制到对应的文件夹中
配合depends.exe工具，可以将exe依赖的dll收集起来
'''

def clearFolder(folder):
    for i in os.listdir(folder):
        filePath = os.path.join(folder,i)
        if os.path.isfile(filePath):
            os.remove(filePath)


def get_actual_filename(name):
    name = "%s[%s]" % (name[:-1], name[-1])
    return glob.glob(name)[0]


if __name__ == '__main__':
    dllFile = 'D:\\Workspace\\Python\\MyTest\\2019\\recheck_dll\\recheck_dll.txt' 
    prefix = "c:\\windows\\system32\\"
    newFolder32 = 'D:\\Workspace\\Python\\MyTest\\2019\\recheck_dll\\dll32\\'
    newFolder64 = 'D:\\Workspace\\Python\\MyTest\\2019\\recheck_dll\\dll\\'

    clearFolder(newFolder32)
    clearFolder(newFolder64)

    with open(dllFile, 'r') as f:
        lines = f.readlines()
        for line in lines:
            line = line.strip('\n')
            line = line.strip(' ')
            dotPos = line.rfind('.')
            if line == '' or dotPos < 0:
                continue

            fileFormat = line[dotPos+1:]
            fileFormat = fileFormat.lower() #文件格式
            if fileFormat == 'dll':
                oldFile = ''
                if os.path.exists(line):
                    oldFile = get_actual_filename(line)
                else:
                    print('>> '+line)
                    continue

                slashPos = oldFile.rfind('\\')
                fileName = oldFile[slashPos+1:dotPos] #文件名

                newFolder = newFolder32 if prefix in oldFile else newFolder64
                newFile = newFolder + fileName + '.' + fileFormat
                for i in range(99): #可能存在同名文件
                    if not os.path.exists(newFile):
                        break
                    newFile = newFolder + fileName + '_' + str(i+2).zfill(3) +'.' + fileFormat

                #复制文件
                try:
                    shutil.copyfile(oldFile,newFile)
                except:
                    print('>> ' + oldFile)