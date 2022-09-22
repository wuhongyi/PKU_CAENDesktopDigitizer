OBJ = pku

CXX=g++ #-std=c++11  
CXXFLAGS:=-fno-stack-protector -g -Wall -lrt -lfftw3  -I. 
ROOTLIBS = $(shell root-config --libs) -lGui
ROOTINC = -I$(shell root-config --cflags) 
CXXFLAGS+=$(ROOTINC)
LIBS:=-lCAENComm -lCAENDigitizer



INCLUDES:=Analysis.hh  DT_PHA.hh  DT_Standard.hh  MainFrame.hh    PrintRegisters.hh Board.hh     DT_PSD.hh  Global.hh       ParSetTable.hh

OBJECTS:=Analysis.o  DT_PHA.o  DT_Standard.o  MainFrame.o    PrintRegisters.o Board.o     DT_PSD.o  Global.o  ParSetTable.o main.o MainDict.o

SRCS:=Analysis.cc  DT_PHA.cc  DT_Standard.cc  MainFrame.cc    PrintRegisters.cc Board.cc     DT_PSD.cc  Global.cc  ParSetTable.cc main.cc MainDict.cc

all: $(OBJ)


$(OBJ): $(SRCS) $(OBJECTS) $(INCLUDES)
	$(CXX) -o  $@  $(OBJECTS) $(CXXFLAGS) $(LIBS) $(ROOTLIBS)

%.o: %.cxx 
	$(CXX) $(CXXFLAGS) -c $< 
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< 
MainDict.cc:MainFrame.hh MainLinkdef.h
	rootcint -f $@ -c $^


clean: 
	rm -f *.o *~ $(OBJ) *Dict*

very-clean:
	rm -f $(OBJ) *.o *~

.PHONY: clean very-clean
