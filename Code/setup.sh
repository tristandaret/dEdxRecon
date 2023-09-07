export LD_LIBRARY_PATH=`pwd`/lib:${LD_LIBRARY_PATH}
export PATH=`pwd`/bin:${PATH}
if [ `uname` = "Darwin" ]; then
   export DYLD_LIBRARY_PATH=`pwd`/lib:${DYLD_LIBRARY_PATH}
fi

