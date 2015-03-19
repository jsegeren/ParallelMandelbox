LDFLAGS = -lm
CFLAGS= -O3 -Wall
CXXFLAGS= -O3 -Wall
CC=g++
RM=rm

PROGRAM_NAME= mandelbox

$(PROGRAM_NAME): main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

run-serial: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)$(EXEXT) params.dat

generate-cam-data: generate_cam_data.o
	$(CC) -o generate_cam_data $^ $(CFLAGS) $(LDFLAGS)

cam-data: generate-cam-data
	./generate_cam_data

video:
	ffmpeg -i 'videos/f%03d.bmp' -r 30 -y videos/out.mkv

clean-video:
	rm -f videos/*

all-serial: $(PROGRAM_NAME) generate-cam-data

run-all-serial: cam-data clean-video run-serial video
	open videos/out.mkv

everything-serial: all-serial run-all-serial

clean:
	$(RM) *.o $(PROGRAM_NAME)$(EXEEXT) *~ generate_cam_data
