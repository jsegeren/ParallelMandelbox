
LDFLAGS = -lm -fopenmp
CFLAGS= -O3 -Wall -fopenmp 
CXXFLAGS= -O3 -Wall -fopenmp
CC=mpicxx
CXX=mpicxx
RM=rm

PROGRAM_NAME= mandelbox
PROGRAM_NAME_PARA= mandelbox_para

all: mandelbox mandelbox_para generate-cam-data

$(PROGRAM_NAME): main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(PROGRAM_NAME_PARA): main_para.o print.o timing.o savebmp.o getparams_para.o 3d_para.o getcolor.o distance_est_para.o mandelboxde_para.o raymarching_para.o renderer_para.o init3D_para.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

run-serial: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)$(EXEXT) params.dat

run-parallel: $(PROGRAM_NAME_PARA)
	./$(PROGRAM_NAME_PARA)$(EXEXT) params.dat

generate-cam-data: generate_cam_data.o
	$(CC) -o generate_cam_data $^ $(CFLAGS) $(LDFLAGS)

cam-data: generate-cam-data
	./generate_cam_data

video:
	ffmpeg -i 'videos/f%03d.jpg' -r 30 -y videos/out.mkv

clean-video:
	rm -rf videos/*

all-serial: $(PROGRAM_NAME) generate-cam-data; mkdir videos;

run-all-serial: cam-data clean-video run-serial video
	open videos/out.mkv

everything-serial: all-serial run-all-serial

all-parallel: $(PROGRAM_NAME_PARA) generate-cam-data

run-all-parallel: cam-data clean-video 
	mkdir -p ./videos
	@echo "Starting cron cleanup crew..."
	crontab convert_image_crontab;
	@echo "Starting program..."	
	mpirun -np 13 ./$(PROGRAM_NAME_PARA)$(EXEXT) params.dat;
	@echo "Program complete! Telling the cleaners to go home..."
	crontab -r;
	@echo "Done!"

#MAKE WITH GPROF PROFILING ENABLED
gprof-mandelbox: CFLAGS = -O3 -Wall -g -pg -fprofile-arcs -ftest-coverage
gprof-mandelbox: CXXFLAGS = -O3 -Wall -g -pg -fprofile-arcs -ftest-coverage
gprof-mandelbox: main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -fprofile-arcs -ftest-coverage -pg -g -o $@ $^ $(CFLAGS) $(LDFLAGS)

run-gprof-serial: mandelbox-gprof
	./mandelbox-gprof params.dat; gprof mandelbox-gprof gmon.out;

clean:
	$(RM) *.o $(PROGRAM_NAME)$(EXEEXT) *~ generate_cam_data
