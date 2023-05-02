import os
import subprocess

output = "decompiled"
osFolder = os.path.join(".", output)
dsFolder = os.path.join(".", "os")
decompiler = os.path.join(".", "sofos.exe")
never = True
counter = 0
for root, dirs, files in os.walk(dsFolder):
    for name in files:
        if "teleport" in name:
            print('DEBUG: ' + decompiler + ' ' + arg1 +  ' ' + arg2)
            # subprocess.call([decompiler,"teleport.os"])
        #elif not never:
        
        convertMe = os.path.join(root,name)
        osPath = os.path.join(output,root[5:])
        osPathFull = os.path.join(output,root[5:],name)
        if os.path.isdir(osPath) == False:
         os.makedirs(osPath)
        arg1 = str(convertMe[2:])
        dsFname = osPathFull[:-2] + "ds"
        arg2 = str(dsFname)
        counter+=1
        #print('DEBUG: ' + decompiler + ' ' + arg1 +  ' ' + arg2)
        #print('Counter = ' + str(counter))
        subprocess.call([decompiler,arg1,arg2])

    #os/dictator/grp2.ds
    #os/first_tsr1/remove.ds
    #os/pop/esquina3.ds
    #os/track/un.ds
