#!/bin/sh

for file in batch/*
do
echo "bsub -q s $file"
bsub -o /dev/null -q s $file
done
