COMMON_MAKEFILE_DIR = ../CommonMakefile
include $(COMMON_MAKEFILE_DIR)/Makefile_front

SYSLIBPATH = ../SysLib
TOOLPATH = ../Tools

#如果是linux，那么不管静态库还是动态库，都不需要链接时指定依赖的库，但是AIX必须指定
ifeq ($(IS_AIX),1)
EXTRA_LIBS	 = $(LIB_DIR)/TinyXmLib $(LIB_DIR)/ToolLib $(LIB_DIR)/MsgLib $(LIB_DIR)/CommLib $(LIB_DIR)/ProBaseLib 
endif

INCS = -I$(SYSLIBPATH) -I$(TOOLPATH)

OUTPUT= $(LIB_DIR)/ProBaseLib 

#****************************************************************************
# objects 
#****************************************************************************
OBJS = ProtocolTime.o ProtocolHead.o \
	   MyUuid.o ProtocolBaseLibVer.o

include $(COMMON_MAKEFILE_DIR)/Makefile_back_lib
