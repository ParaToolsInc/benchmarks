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

#testList=['serial','openmp','openmp_2','tbb_rankBuffer','tbb_singleBuffer']
testList=['tbb_rankBuffer','tbb_singleBuffer']
threadCount=[1,2,4,8,10,12,14,16,18,19,20,25,30,35,40]


p=subprocess.check_output('hostname')
machine=p.split('.')[1].strip('\n')


def main():
  parser = argparse.ArgumentParser(description='Run all the psum tests on Stampede or Maverick. ')#\
#                                                'calculate averages and standard deviations.'\
#                                                ' Also, a histogram will be generated for each type and thread count.'\
#                                                '  We also will produce a line plot for the avg. per type vs. thread count')
  args = parser.parse_args()

  currentDir=os.getcwd()

  for test in testList:
    if test == 'serial':
      fileName= machine + '_' + test
      cmdLine = 'psum_' + test + ' 2>&1 >> ' + fileName
      print cmdLine
      for i in range(0,100):
        errorMessage = " Error running: " + cmdLine
        shellCommand(cmdLine,errorMessage)
    else:
      for thread in threadCount:
        fileName= machine + '_' + test + '_' + str(thread) + '.txt'
        cmdLine = 'psum_' + test + ' ' + str(thread) + ' 2>&1 >> ' + fileName
        print cmdLine
        for i in range(0,100):
          errorMessage = " Error running: " + cmdLine
          shellCommand(cmdLine,errorMessage)


if __name__ == "__main__":
   main()


