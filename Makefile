main: cell.o chamber.o config.o configlist.o event.o hit.o inputroot.o layer.o line.o main.o range.o run.o trackfindrange.o xtcurve.o wiremap.o
	g++ -O2 -o main cell.o chamber.o config.o configlist.o event.o hit.o inputroot.o layer.o line.o main.o range.o run.o trackfindrange.o xtcurve.o wiremap.o  `root-config --libs` 

cell.o: cell.cc cell.h
	g++ -O2 -o cell.o -c `root-config --cflags` cell.cc

chamber.o: chamber.cc chamber.h
	g++ -O2 -o chamber.o -c `root-config --cflags` chamber.cc

config.o: config.cc config.h
	g++ -O2 -o config.o -c `root-config --cflags` config.cc

configlist.o: configlist.cc configlist.h
	g++ -O2 -o configlist.o -c `root-config --cflags` configlist.cc

event.o: event.cc event.h
	g++ -O2 -o event.o -c `root-config --cflags` event.cc

hit.o: hit.cc hit.h
	g++ -O2 -o hit.o -c `root-config --cflags` hit.cc

layer.o: layer.cc layer.h
	g++ -O2 -o layer.o -c `root-config --cflags` layer.cc

line.o: line.cc line.h
	g++ -O2 -o line.o -c `root-config --cflags` line.cc

main.o: main.cc
	g++ -O2 -o main.o -c `root-config --cflags` main.cc

range.o: range.cc range.h
	g++ -O2 -o range.o -c `root-config --cflags` range.cc

run.o: run.cc run.h
	g++ -O2 -o run.o -c `root-config --cflags` run.cc

trackfindrange.o: trackfindrange.cc trackfindrange.h
	g++ -O2 -o trackfindrange.o -c `root-config --cflags` trackfindrange.cc

xtcurve.o: xtcurve.cc xtcurve.h
	g++ -O2 -o xtcurve.o -c `root-config --cflags` xtcurve.cc

wiremap.o: wiremap.cc wiremap.h
	g++ -O2 -o wiremap.o -c `root-config --cflags` wiremap.cc



clean:
	rm main *.d *.pcm *.so *~
