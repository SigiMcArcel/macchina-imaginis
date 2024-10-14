DESTDIR=
prefix=/usr
bindir=/usr/lib


INCLUDES = -I. -I/.
CXXFLAGS = -fpic -Wall -Wl,--no-undefined
LD_FLAGS = -lmiutils -lmimodules -lmicomponents -lmisound -lasound -lsndfile -lmidriver -lmitasks

SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXEOUT=miquadratmachine


.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<
$(EXEOUT):$(OBJECTS)
	$(CXX) $(LD_FLAGS) $(OBJECTS) -o $(EXEOUT)
clean:
	$(RM) $(EXEOUT) *.o

install:
	install -D -d $(DESTDIR)/${bindir}
	install -D -m 0755 $(EXEOUT) $(DESTDIR)/${bindir}/$(EXEOUT) 

	