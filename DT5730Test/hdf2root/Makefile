# ROOT flags
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)

# HDF5 FLAGS
HDF5CFLAGS    = -I/usr/include
HDF5LIBS      = -L/usr/lib64 -lhdf5 -lhdf5_cpp -lhdf5_hl_cpp -lhdf5_hl

# c++-compiler
CXX           = g++ -std=c++11 
CXXFLAGS      += -fPIC $(ROOTCFLAGS) $(HDF5CFLAGS) -I.

LIBS          = $(ROOTLIBS) $(HDF5LIBS) 

EXES = hdf2root

all: bin 

bin: $(addsuffix .e,$(EXES))

%.e: %.cc
	@echo "Linking $@"
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@ -L. $(LIBS)
	@ln -sf $@ $(subst .e,,$@)

clean:
	@rm -f *~
	@for file in $(EXES); do (rm -f $$file.e); done
	@for file in $(EXES); do (rm -f $$file); done
