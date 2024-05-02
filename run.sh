#! /bin/bash -f
# @(#)
#
  cd Code 
  rm bin/Analysis
  make -j18
  source setup.sh
  cd ../
  
  time Analysis
 
