#!/bin/bash
# Cron job which runs every minute (this frequency is specified in the crontab)
# 1) Create /videos/temp folder
# 2) Move all bmps from /videos to /videos/temp
# 3) Convert bmps to jpgs
# 4) Delete bmps from /videos/temp
# 5) Move jpgs back to /videos
# 6) Delete the /videos/temp folder

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
export LD_LIBRARY_PATH=/u50/skeeteel/local_installs/usr/local/lib
BIN_PATH=/u50/skeeteel/local_installs/usr/local/bin

(mkdir $DIR/videos/temp; mv $DIR/videos/*.bmp $DIR/videos/temp; $BIN_PATH/mogrify -format jpg -quality 87 $DIR/videos/temp/*.bmp; rm $DIR/videos/temp/*.bmp; mv $DIR/videos/temp/* $DIR/videos; rm -rf $DIR/videos/temp;) &
