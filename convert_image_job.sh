#!/bin/bash
# Cron wrapper to call the convert images script

# Initialisation
if [ "$1" == "" ]; then
exit 1
fi

interval=$1

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
export LD_LIBRARY_PATH=/u50/skeeteel/local_installs/usr/local/lib
BIN_PATH=/u50/skeeteel/local_installs/usr/local/bin

next=$(date +%M | awk -v interval=$interval '{ print int($0 * 60 % interval) }')

if [ $next -gt 0 ]; then
next=$(echo $next | awk -v interval=$interval '{print interval - $0}')
fi


for i in $(seq $next $interval 59)
do
(sleep $i; $BIN_PATH/mogrify -format jpg -quality 80 $DIR/videos/*.bmp; rm $DIR/videos/*.bmp;) &
done
