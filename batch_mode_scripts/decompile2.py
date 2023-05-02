#!/usr/bin/python2.7
import os

from subprocess import Popen, PIPE

#get total files
#get total success 
#total fail

output1 = "decompiled"
osFolder = os.path.join(".", output1)
dsFolder = os.path.join(".", "os")
decompiler = os.path.join(".", "sofos.exe")
counter = 0
for root, dirs, files in os.walk(dsFolder):
    for name in files:
        if name.endswith('.os'):
            convertMe = os.path.join(root,name)
            osPath = os.path.join(output1,root[5:])
            osPathFull = os.path.join(output1,root[5:],name)
            if os.path.isdir(osPath) == False:
                os.makedirs(osPath)
            arg1 = str(convertMe[2:])
            dsFname = osPathFull[:-2] + "ds"
            arg2 = str(dsFname)
            #hello = subprocess.call([decompiler,arg1,arg2])
            # print 'DEBUG: ' + decompiler + ' ' + arg1 +  ' ' + arg2
            p = Popen([decompiler, arg1, arg2], stdin=PIPE, stdout=PIPE, stderr=PIPE, shell=False)
            lines = p.stdout.readlines()
            output, err = p.communicate(b"input data that is passed to assubprocess' stdin")
            src = p.returncode
            
            out_filesize = os.stat(dsFname).st_size
            print dsFname + ':' + str(out_filesize)
            # error
            if src != 0 or out_filesize == 0:
                counter += 1
                print "*********************"
                codemsg = "Returncode = " + str(src)
                print codemsg
                msg = "File: " + name
                print msg
                for x in lines:
                    print x
                print "*********************"
                print "~~~~~~~~~~~~~~~~~~~~~"


byeworld = "Errors: " + str(counter)
print byeworld