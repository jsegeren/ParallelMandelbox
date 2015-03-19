# ParallelMandelbox
Repository for development of parallelized implementation of Mandelbox frame computation and video generation scripts, using OpenMP

INSTRUCTIONS
============

"make generate-cam_-ata" to compile the cam data generator
"make cam-data" to actually generate the cam data (uses input file cam_instructions.dat and outputs to cam_params.dat)

the format of cam_instructions.dat is as follows

init_pos_x init_pos_y init_pos_z init_target_x init_target_x init_target_x
delta_pos_x delta_pos_y delta_pos_z delta_target_pos_x delta_target_y delta_target_z num_frames

the first line has the initial values for camPos and camTarget
the subsequent lines consist of deltas for the the camPos and camTarget and the number of frames to apply the deltas over

makefile tips:

you probably need to 'mkdir videos' before running the code

make video: generate a video from bmp frames
make clean-video: deletes video file and frames

make all-serial: compile all files related to serial execution

//warning these will override any video currently generated
make run-all-serial: run all files related to serial execution (i.e. generate camera instruction, generate frames, generate video)
make everything-serial: does make all-serial, make run-all-serial then attempts to open the video (platform specific)
