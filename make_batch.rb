#!/usr/bin/env ruby

require "fileutils"

$prog_dir="/home/had/hideyuki/private/genfit2/KEKCC/hs_cosmic/"
$inroot_dir="/home/had/hideyuki/private/genfit2/KEKCC/root/cdcKEK16/input"
$outroot_dir="/home/had/hideyuki/private/genfit2/KEKCC/root/cdcKEK16/output"

def command(total, run_num)
   #Usage ./main <input.root> <output.root> <start_event> <last_event>
   num_each=5000
   input_root="#{$inroot_dir}/run%d.root"%run_num
   0.step(total, num_each) do |iev|
      iev_str="%07d"%iev
      start_iev=iev
      last_iev=iev+num_each-1
      if last_iev > total-1
         last_iev = total - 1
      end
      output_root="#{$outroot_dir}/run%d_%s.root" % [run_num, iev_str]
      cmd="#!/bin/bash\n"
      cmd+="#{$prog_dir}/main #{input_root} #{output_root} #{start_iev} #{last_iev}\n"
      batch_file = "batch/run_#{run_num}_start_iev_#{iev_str}.sh"
      File.open(batch_file, "w") do |file|
         file.write cmd
      end
      FileUtils.chmod(0755,batch_file)
   end
end

#command(39322,   16, 1);
#command(1730623, 24, 1);
#command(2114226, 27, 1);

#command(39322,   16, 2);
#command(1730623, 24, 2);
#command(2114226, 27, 2);

#command(2114226, 27, 3);

#command(2114226, 27, 4);
#command(2114226, 27, 5);
command(1066760, 1024)
