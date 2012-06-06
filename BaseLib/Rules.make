TOP = $(PWD)
PRE = 
CC = $(PRE)gcc
LD = $(PRE)gcc
AR = $(PRE)ar rc
RM= rm -f
AR2= ranlib

LDFLAGS = -Wl 
CFLAGS =  -O2 -DLINUX
 
DEBUG = -D__DEBUG__ 

INCLUDE = -I$(PWD)/include -I$(PWD)/include/mysql5 -I$(PWD)/include/oracle9i
LIBS = $(PWD)/../OciLib/linux

