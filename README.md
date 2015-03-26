# ParallelMandelbox
Repository for development of parallelized implementation of Mandelbox frame computation and video generation scripts, using OpenMP

## Instructions - building & running

**First** you will need to modify the ``convert_image_crontab`` file so that it points to your project path. Unforunately, cron execution doesn't acknowledge our environment variables and there's no other way for it to know the directory of the cron job script.

``make generate-cam-data`` to compile the cam data generator
``make cam-data`` to actually generate the cam data (uses input file cam_instructions.dat and outputs to cam_params.dat)

The format of cam_instructions.dat is as follows

  init_pos_x init_pos_y init_pos_z init_target_x init_target_x init_target_x
  delta_pos_x delta_pos_y delta_pos_z delta_target_pos_x delta_target_y delta_target_z num_frames

the first line has the initial values for camPos and camTarget
the subsequent lines consist of deltas for the the camPos and camTarget and the number of frames to apply the deltas over

### Makefile specifics

``make video``: generate a video from bmp frames
``make clean-video``: deletes video file and frames

``make all-serial``: compile all files related to serial execution
``make all-parallel``; compiles all files related to the parallel execution

*Warning: these will override any video currently generated*
``make run-all-serial``: run all files related to serial execution (i.e. generate camera instruction, generate frames, generate video)
``make everything-serial``: does make all-serial, make run-all-serial then attempts to open the video (platform specific)
``make run-all-parallel``: makes the ``videos/`` directory, starts cron job that converts BMP files to JPG (and removes the former), so that we don't accidentally crash the server by filling up the filesystem, and runs the parallel execution!
