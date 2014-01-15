#!/usr/bin/python

import sys
import os

def touch(fname, times=None):
    with file(fname, 'a'):
        os.utime(fname, times)
        
if len(sys.argv) != 2:
    n = sys.argv[0]
    print 'usage: ' + sys.argv[0] + ' filename'
    print '   or  python ' + sys.argv[0] + ' filename'
    sys.exit()

filename = sys.argv[1] 
lockdir  = "/tmp"
lockfile = lockdir + "/videocapture.lck"

touch(lockfile)

CMD  = "raspivid "
CMD += "--width 1280 "
CMD += "--height 720 "
CMD += "--bitrate 5000000 "
CMD += "--framerate 25 "
CMD += "--timeout 1 "
CMD += "-t 900000 "
CMD += "--output "
CMD += filename

os.system(CMD)
os.system("sleep 2")

os.remove(lockfile)
