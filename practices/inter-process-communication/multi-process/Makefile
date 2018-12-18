TARGET=demo
CXX=g++
DSRC=./
DOBJ=obj

CXX_FLAGS= -O3 -Wall -std=c++11 \
	   -I/usr/local/include
CXX_LIBS=\
	 -L/usr/local/lib \
	 -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_core \
	 -lpthread


SRCS=$(wildcard $(DSRC)/*.cpp)
OBJS=$(patsubst %.cpp, $(DOBJ)/%.o, $(notdir $(SRCS)))


all: dir $(TARGET)


$(TARGET):$(OBJS)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(CXX_LIBS)

$(DOBJ)/%.o:$(DSRC)/%.cpp
	$(CXX) -c -o $@ $< $(CXX_FLAGS)

dir:
	mkdir -p $(DOBJ)


.PHONY:clean

clean:
	rm -rf $(TARGET) $(DOBJ)
