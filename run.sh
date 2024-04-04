#! /bin/bash -f
# @(#)
#
  cd Code 
  rm bin/AOBt_App
  make -j18
  source setup.sh
  cd ../
  
  time AOBt_App
 
