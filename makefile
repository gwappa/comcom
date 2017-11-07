VPATH+=include:src

TARGET=comcom
SEMIT=dummydevice
MAIN=main.o
OBJS=iobase.o iodevice.o arduinodriver.o dummydriver.o session.o
CXX=g++
CXXFLAGS=-Wall -I./include

all: $(TARGET)
device: $(SEMIT)

$(TARGET): $(MAIN) $(OBJS) $(SEMIT)
	${CXX} ${CXXFLAGS} $(MAIN) $(OBJS) -o $@

$(SEMIT): dummymain.o iobase.o
	${CXX} ${CXXFLAGS} $+ -o $@

*.o: %.o: %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

$(OBJS): %.o: %.h
dummymain.o: iobase.h
session.o: iobase.h iodevice.h outputformatter.h
dummydriver.o: iobase.h iodevice.h
arduinodriver.o: iodevice.h
iodevice.o:
iobase.o:

clean:
	rm -f $(MAIN) $(OBJS) dummymain.o

distclean: clean
	rm -f $(TARGET) $(SEMIT)
