from subprocess import *
import time
import os

filenames = ["videos/f{0:04d}.bmp".format(i) for i in range(0,7435)]

for filename in filenames:
    success = False

    while not success:
        success = True
        try:
            check_call(["ls", filename], stdout=open(os.devnull, 'w'), stderr=STDOUT)
        except CalledProcessError:
             success = False
        time.sleep(1);

    os.system("LD_LIBRARY_PATH=/u50/skeeteel/local_installs/usr/local/lib /u50/skeeteel/local_installs/usr/local/bin/mogrify -format jpg -quality 87 " + filename)
    check_call(["rm", filename], stdout=open(os.devnull, 'w'), stderr=STDOUT)
    print "converted " + filename
