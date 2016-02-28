#!/usr/bin/env ruby

require "fileutils"

$inroot_dir="/home/had/hideyuki/private/genfit2/KEKCC/root/cosmic_root/"
$outroot_dir="/home/had/hideyuki/private/genfit2/KEKCC/root/cosmic_root_out/freet0/sub/"
$prog_dir="/home/had/hideyuki/private/genfit2/KEKCC/hs_cosmic/"

def command(total, input_root, output_root, fit_func_name)
   #Usage ./main <input.root> <output.root> <start_event> <last_event> <fit_func_name>
   num_each=10000
   0.step(total, num_each) do |iev|
      start_iev=iev
      last_iev=iev+num_each-1
      if last_iev > total-1
         last_iev = total - 1
      end
      #puts "iev #{start_iev} #{last_iev}"
      cmd="#!/bin/bash\n"
      cmd+="#{$prog_dir}/main #{$inroot_dir}/#{input_root} #{$outroot_dir}/#{output_root} #{start_iev} #{last_iev} #{fit_func_name}\n"
      File.open("batch/run#{iev}.sh", "w") do |file|
         file.write cmd
      end
      FileUtils.chmod(0755,"batch/run#{iev}.sh")
   end
end

command(39322,   "run16.root", "run16.root", "FIT_FUNC_TYPE_FREE_T0")
command(1730623, "run24.root", "run24.root", "FIT_FUNC_TYPE_FREE_T0")
command(2114226, "run27.root", "run27.root", "FIT_FUNC_TYPE_FREE_T0")
