#!/usr/bin/python

import sys
import os

if len(sys.argv) != 2:
    n = sys.argv[0]
    print 'usage: ' + sys.argv[0] + ' basename'
    print '   or  python ' + sys.argv[0] + ' basename'
    sys.exit()

basename = sys.argv[1] 
basecmd = "raspistill --width 1296 --height 972 --quality 100 --exif testID="
count = 1

def acquirePicture(cmd):
    global count
    fname = "%02d.jpg" % count
    idname = "%02d " % count
    count = count + 1
    print basecmd + idname + cmd + " --output " + basename + fname
    #os.system(basecmd + idname + cmd + " --output " + basename + fname)
    #os.system("sleep 1")

acquirePicture("--ISO 0")
acquirePicture("--ISO 100")
acquirePicture("--ISO 200")
acquirePicture("--ISO 400")
acquirePicture("--ISO 800")

acquirePicture("--ISO 0 --exposure fixedfps")
acquirePicture("--ISO 100 --exposure fixedfps")
acquirePicture("--ISO 200 --exposure fixedfps")
acquirePicture("--ISO 400 --exposure fixedfps")
acquirePicture("--ISO 800 --exposure fixedfps")

acquirePicture("--ISO 100 --exposure fixedfps --ev -10")
acquirePicture("--ISO 100 --exposure fixedfps --ev -8")
acquirePicture("--ISO 100 --exposure fixedfps --ev -6")
acquirePicture("--ISO 100 --exposure fixedfps --ev -4")
acquirePicture("--ISO 100 --exposure fixedfps --ev -2")
acquirePicture("--ISO 100 --exposure fixedfps --ev -0")
acquirePicture("--ISO 100 --exposure fixedfps --ev 2")
acquirePicture("--ISO 100 --exposure fixedfps --ev 4")
acquirePicture("--ISO 100 --exposure fixedfps --ev 6")
acquirePicture("--ISO 100 --exposure fixedfps --ev 8")
acquirePicture("--ISO 100 --exposure fixedfps --ev 10")
