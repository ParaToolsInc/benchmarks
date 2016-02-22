#! /usr/bin/env python

#####
## loop over executables, loop over number of threads
## stdout to file name <test>_N.txt
import subprocess
import os,sys,getopt

def shellCommand(command,errorMessage):
#command initiated where this script is ran
  try:
    print command
    subprocess.check_call(command, stderr=subprocess.STDOUT, shell=True)
  except :
    print errorMessage
    pass
  return

testList=['serial','openmp','openmp_2','tbb_rankBuffer','tbb_singleBuffer']
threadCount=[1,2,4,8,10,12,14,16,18,19,20,25,30,35,40]

