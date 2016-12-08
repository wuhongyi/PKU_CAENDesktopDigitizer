### Makefile --- 
## 
## Description: 
## Author: Hongyi Wu(吴鸿毅)
## Email: wuhongyi@qq.com 
## Created: 四 12月  8 19:23:25 2016 (+0800)
## Last-Updated: 四 12月  8 19:25:08 2016 (+0800)
##           By: Hongyi Wu(吴鸿毅)
##     Update #: 1
## URL: http://wuhongyi.cn 

OBJ = offline
sourcefile = main.cc offline.hh offline.cc

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
