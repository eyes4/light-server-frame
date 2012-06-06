# makefile for BaseLib
include Rules.make

NOW=`date +\"%Y-%m-%d %H:%M:%S\"`
VERSION = "$Revision$"
TAG = ""
RELDATE = ""


UTIL = common.cpp strutil.cpp memqueue.cpp
DATABASE = CDBSrcBase.cpp CDBSrcOracle.cpp CDBSrcMySql.cpp CDBConnPool.cpp
NETWORK = linux_socket.cpp
THREAD = linux_semaphore.cpp linux_timedsem.cpp linux_thread.cpp CThreadPool.cpp


SRCS = $(UTIL:%.cpp=util/%.cpp) $(DATABASE:%.cpp=database/%.cpp) \
			 $(NETWORK:%.cpp=network/%.cpp) $(THREAD:%.cpp=thread/%.cpp)
OBJS = $(SRCS:%.cpp=%.o)

libmylib.a: $(OBJS)
	$(RM) libmylib.a
	$(AR) libmylib.a  $(OBJS)
	$(AR2) libmylib.a
	
$(SRCS):
#$(SRCS:%.cpp=%.h)
	@touch $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) $(DEBUG) -c -o $(?:%.cpp=%.o) $? 
	
clean:
	find . -name '*.o' -exec rm -rf {} \;
	rm -rf *.o *.bin *.gdb
	rm -rf libmylib.a

