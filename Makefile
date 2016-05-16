main: main.cc  chamber.cc event.cc hit.cc line.cc outputroot.cc run.cc track.cc trackfinder.cc wiremap.cc xtcurve.cc 
	g++ -O2 -o main `root-config --cflags` main.cc  chamber.cc event.cc hit.cc line.cc outputroot.cc run.cc track.cc trackfinder.cc wiremap.cc xtcurve.cc `root-config --libs` -lMinuit

clean:
	rm main *.d *.pcm *.so *~