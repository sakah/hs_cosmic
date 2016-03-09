require 'rake/clean'

SRCS =%w(cell.o chamber.o config.o configlist.o event.o hit.o inputroot.o main.o layer.o line.o range.o run.o trackfindrange.o xtcurve.o wiremap.o)
OBJS = SRCS.map do |src|
   src.gsub('.cc', '.o')
end

CLEAN.include(OBJS)
CLOBBER.include("main")

task :default => :run

desc "Build main"
task :run => OBJS do
   sh "g++ -O2 #{OBJS.join(' ')} `root-config --libs` -o main"
end

rule '.o' => '.cc' do |t|
   sh "g++ -O2 `root-config --cflags` -c #{t.source} -o #{t.name}"
end
