#!/usr/bin/python

import sys
import os

if len(sys.argv) != 2:
    n = sys.argv[0]
    print 'usage: ' + sys.argv[0] + ' basename'
    print '   or  python ' + sys.argv[0] + ' basename'
    sys.exit()

basename = sys.argv[1] 
#basecmd = "raspistill -w 1296 -h 972 -q 100 -n -th none -t 1 -v -x testID="
basecmd =  "raspistill -w  648 -h 486 -q 100 -n -th none -t 1 -v -x testID="
count = 1

def acquirePicture(cmd):
    global count
    fname = "%02d.jpg" % count
    idname = "%02d " % count
    count = count + 1
    print basecmd + idname + cmd + " --output " + basename + fname
    os.system(basecmd + idname + cmd + " --output " + basename + fname)
    os.system("sleep 2")

acquirePicture("")

acquirePicture("-awb off -ISO   0")
acquirePicture("-awb off -ISO 100")
acquirePicture("-awb off -ISO 200")
acquirePicture("-awb off -ISO 400")
acquirePicture("-awb off -ISO 800")

acquirePicture("-awb off -ISO 100 -ev -10")
acquirePicture("-awb off -ISO 100 -ev  -8")
acquirePicture("-awb off -ISO 100 -ev  -6")
acquirePicture("-awb off -ISO 100 -ev  -4")
acquirePicture("-awb off -ISO 100 -ev  -2")
acquirePicture("-awb off -ISO 100 -ev  -0")
acquirePicture("-awb off -ISO 100 -ev   2")
acquirePicture("-awb off -ISO 100 -ev   4")
acquirePicture("-awb off -ISO 100 -ev   6")
acquirePicture("-awb off -ISO 100 -ev   8")
acquirePicture("-awb off -ISO 100 -ev  10")
