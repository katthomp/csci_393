CXX = cc
CFLAGS = -g -Wall 

all: myShell

.c.o:
	$(CXX) -c $(CFLAGS) $<

clean:
	rm -f *.o *.dev

spotless: clean
	rm -f testDevice testMaster myShell

TEST_DEV_OBJS = master_block.o block_device.o dylanBitSet.o test_master_block.o
SHELL_OBJS = SPOILER_my_shell.o block_device.o master_block.o dylanBitSet.o

testMaster: master_block.o block_device.o dylanBitSet.o SPOILER_my_shell.o
	$(CXX) -o testMaster master_block.o block_device.o dylanBitSet.o SPOILER_my_shell.o -lreadline 

testDevice: $(TEST_DEV_OBJS)
	$(CXX) -o testDevice $(TEST_DEV_OBJS)

myShell: $(SHELL_OBJS)
	$(CXX) -o myShell $(SHELL_OBJS) -lreadline 

test: all
	./testMaster