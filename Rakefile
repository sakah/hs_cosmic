task :all=> ["clean", "run"]
task :default => "run"

SRCS =%w(cell.o chamber.o config.o configlist.o event.o hit.o inputroot.o layer.o line.o main.o range.o run.o trackfindrange.o xtcurve.o wiremap.o)

OBJS = SRCS.map do |src|
   src.gsub('.cc', '.o')
end

task :run => OBJS do
   puts "run"
end

rule '.o' => '.cc' do |t|
   sh "g++ -O2 `root-config --cflags` -c #{t.source} -o #{t.name}"
end
