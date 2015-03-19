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

gprof-mandelbox: CFLAGS = -O3 -Wall -g -pg -fprofile-arcs -ftest-coverage
gprof-mandelbox: CXXFLAGS = -O3 -Wall -g -pg -fprofile-arcs -ftest-coverage
gprof-mandelbox: main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -fprofile-arcs -ftest-coverage -pg -g -o $@ $^ $(CFLAGS) $(LDFLAGS)

run-gprof-serial: mandelbox-gprof
	./mandelbox-gprof params.dat; gprof mandelbox-gprof gmon.out;

clean:
	$(RM) *.o $(PROGRAM_NAME)$(EXEEXT) *~
