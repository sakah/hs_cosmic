#!/usr/bin/env ruby

require "fileutils"

$prog_path="/home/had/hideyuki/private/genfit2/KEKCC/hs_cosmic/"
$inroot_dir="/home/had/hideyuki/private/genfit2/KEKCC/root/cosmic_root/"
$outroot_dir="/home/had/hideyuki/private/genfit2/KEKCC/root/cosmic_root_out/sub/"
$prog_dir="/home/had/hideyuki/private/genfit2/KEKCC/hs_cosmic/"
$config_dir="/home/had/hideyuki/private/genfit2/KEKCC/hs_cosmic/config/"

def command(total, run_num, config_num)
   #Usage ./main <prog_path> <config.txt> <input.root> <output.root> <start_event> <last_event>
   num_each=10000
   input_root="run#{run_num}.root"
   config_path="#{$config_dir}/config#{config_num}.txt"
   0.step(total, num_each) do |iev|
      iev_str="%07d"%iev
      start_iev=iev
      last_iev=iev+num_each-1
      if last_iev > total-1
         last_iev = total - 1
      end
      output_root="run_#{run_num}_config_#{config_num}_#{iev_str}.root"
      #puts "iev #{start_iev} #{last_iev}"
      cmd="#!/bin/bash\n"
      cmd+="#{$prog_dir}/main #{$prog_path} #{config_path} #{$inroot_dir}/#{input_root} #{$outroot_dir}/#{output_root} #{start_iev} #{last_iev}\n"
      File.open("batch/run_#{run_num}_config_#{config_num}_#{iev_str}.sh", "w") do |file|
         file.write cmd
      end
      FileUtils.chmod(0755,"batch/run_#{run_num}_config_#{config_num}_#{iev_str}.sh")
   end
end

#command(39322,   16, 1);
#command(1730623, 24, 1);
#command(2114226, 27, 1);

#command(39322,   16, 2);
#command(1730623, 24, 2);
#command(2114226, 27, 2);

#command(2114226, 27, 3);

command(2114226, 27, 4);
