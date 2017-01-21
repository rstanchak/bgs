CXXFLAGS=-ggdb `pkg-config opencv --cflags` 
LDFLAGS=`pkg-config opencv --libs` -lhdf5

all: bgsubtract write_mog_images

write_mog_images: write_mog_images.o cvhdf5.o
	g++ $(LDFLAGS) $^ -o $@
