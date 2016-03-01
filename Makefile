main: analysis.cc chamber.cc config.cc event.cc hit.cc line.cc main.cc outputroot.cc run.cc track.cc trackfinder.cc wiremap.cc xtcurve.cc 
	g++ -O2 -o main `root-config --cflags` analysis.cc chamber.cc config.cc event.cc hit.cc line.cc main.cc outputroot.cc run.cc track.cc trackfinder.cc wiremap.cc xtcurve.cc `root-config --libs` -lMinuit

clean:
	rm main *.d *.pcm *.so *~
