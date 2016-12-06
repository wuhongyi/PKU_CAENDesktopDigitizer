### Makefile --- 
## 
## Description: 
## Author: Hongyi Wu(吴鸿毅)
## Email: wuhongyi@qq.com 
## Created: 二 12月  6 19:28:24 2016 (+0800)
## Last-Updated: 二 12月  6 19:46:46 2016 (+0800)
##           By: Hongyi Wu(吴鸿毅)
##     Update #: 2
## URL: http://wuhongyi.cn 

OBJ = decode
sourcefile = decode.cc r2root.hh r2root.cc

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs)

CFLAGS = -g -Wall -O2 -I$(TARTSYS)/include -L$(TARTSYS)/lib -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cc ,$(sourcefile))

clean:
	rm -f *~ *.o $(OBJ)



######################################################################
### Makefile ends here
