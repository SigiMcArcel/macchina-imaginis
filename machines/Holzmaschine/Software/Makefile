

DESTDIR=
LIBDIR=/usr/lib
INCLUDEDIR=/usr/include

INCLUDES = -I. -I/.
CXX_FLAGS = -fpic -Wall
LD_FLAGS = -lmiutils -lmimodules -lmicomponents -lmisound -lasound -lsndfile

SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXEOUT=miwoodenmachine


.cpp.o:
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $<
$(EXEOUT):$(OBJECTS)
	$(CXX) $(LD_FLAGS) $(OBJECTS) -o $(EXEOUT)
clean:
	$(RM) $(EXEOUT) *.o
	