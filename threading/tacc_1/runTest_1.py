#! /usr/bin/env python

#####
## loop over executables, loop over number of threads
## stdout to file name <machine>_<test>_N.txt
## run in idev
import subprocess
import sys,os,getopt,argparse,math


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


p=subprocess.check_output('hostname')
machine=p.split('.')[1]


def main():
  parser = argparse.ArgumentParser(description='Run all the psum tests on Stampede or Maverick. '\
                                                'calculate averages and standard deviations.'\
                                                ' Also, a histogram will be generated for each type and thread count.'\
                                                '  We also will produce a line plot for the avg. per type vs. thread count')
  args = parser.parse_args()

  currentDir=os.getcwd()

  for test in testList:
    print test


if __name__ == "__main__":
   main()


