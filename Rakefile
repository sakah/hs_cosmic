require 'rake/clean'

#SRCS =%w(cell.cc chamber.cc config.cc configlist.cc event.cc hit.cc inputroot.cc main.cc layer.cc line.cc range.cc run.cc trackfindrange.cc xtcurve.cc wiremap.cc)
SRCS =%w(cell.cc chamber.cc config.cc configlist.cc event.cc hit.cc inputroot.cc main.cc layer.cc line.cc run.cc trackfindrange.cc xtcurve.cc wiremap.cc)
OBJS = SRCS.map do |src|
   src.gsub('.cc', '.o')
end
headers=SRCS.map do |src|
   src.gsub('.cc', '.h')
end
headers.delete('main.h')
headers.push("range.h")
HEADERS=headers

obj=OBJS.dup
CLEAN.include(obj.push("mydict.cc", "mydict.o", "mydict_rdict.pcm", "libmylib.so", "main"))

task :default => :main

desc "Build main"
task :main => OBJS do |t|
   sh "g++ -O2 #{t.prerequisites.join(' ')} `root-config --libs` -o main"
end

rule '.o' => '.cc' do |t|
   sh "g++ -O2 `root-config --cflags` -c #{t.source} -o #{t.name}"
end

file "mydict.cc" => OBJS do |t|
   #puts "--- #{HEADERS} ----"
   sh "rootcint -f mydict.cc -c -p #{HEADERS.join(' ')} Linkdef.h"
end

OBJS2=OBJS
OBJS2.delete("main.o")
OBJS2.push("mydict.cc")
desc "Build shared library for CINT"
task :lib => OBJS2 do |t|
   #puts "--- #{OBJS2} ----"
   sh "g++ --shared -O2 -o libmylib.so #{t.prerequisites.join(' ')} `root-config --cflags` `root-config --libs`"
end

