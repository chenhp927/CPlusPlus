COMMON_MAKEFILE_DIR = ../CommonMakefile
include $(COMMON_MAKEFILE_DIR)/Makefile_front

TOOLPATH = ../Tools
SYSLIBPATH = ../SysLib
PROTOCOLBASEPATH = ../ProtocolBase

#如果是linux，那么不管静态库还是动态库，都不需要链接时指定依赖的库，但是AIX必须指定
ifeq ($(IS_AIX),1)
EXTRA_LIBS	 = $(LIB_DIR)/TinyXmLib -I$(SYSLIBPATH) $(LIB_DIR)/ToolLib $(LIB_DIR)/MsgLib $(LIB_DIR)/CommLib $(LIB_DIR)/ProBaseLib 
endif

INCS = -I$(TOOLPATH) -I$(SYSLIBPATH) -I$(PROTOCOLBASEPATH)

OUTPUT= $(LIB_DIR)/ProIndLib

#****************************************************************************
# objects 
#****************************************************************************

OBJS = ProtocolIndication.o ProtocolSignal.o ProtocolMeasure.o ProtocolIndicationLibVer.o ProtocolDevState.o

include $(COMMON_MAKEFILE_DIR)/Makefile_back_lib
