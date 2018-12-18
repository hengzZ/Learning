TARGET = demo
CC = g++
CFLAGS = -I/usr/local/include/opencv \
		 -I/usr/local/include/opencv2
LIBS = -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_superres \
	-lopencv_videostab -lopencv_calib3d -lopencv_features2d \
	-lopencv_objdetect -lopencv_highgui -lopencv_videoio \
	-lopencv_photo -lopencv_imgcodecs -lopencv_video \
	-lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core   

ODIR = obj

_OBJ = demo.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(TARGET) $(ODIR)/*.o
