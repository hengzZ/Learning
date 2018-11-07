SUBDIR1=./tcp/
SUBDIR2=./udp/

MAKE=make
MAKE_CLEAN=make clean

all:
	$(MAKE) -C $(SUBDIR1)
	$(MAKE) -C $(SUBDIR2)

.PHONY:clean

clean:
	$(MAKE_CLEAN) -C $(SUBDIR1)
	$(MAKE_CLEAN) -C $(SUBDIR2)

