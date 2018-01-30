COMMON_MAKEFILE_DIR = ../CommonMakefile
include $(COMMON_MAKEFILE_DIR)/Makefile_front

SYSLIBPATH = ../SysLib
COMMLIBPATH = ../CommLib
XMLPATH = ../TinyXml
TOOLPATH = ../Tools
PROTOCOLBASEPATH = ../ProtocolBase
PROTOCOLALARMPATH = ../ProtocolAlarm
PROTOCOLINDPATH = ../ProtocolIndication

#如果是linux，那么不管静态库还是动态库，都不需要链接时指定依赖的库，但是AIX必须指定
ifeq ($(IS_AIX),1)
EXTRA_LIBS	 = $(LIB_DIR)/TinyXmLib -I$(SYSLIBPATH) $(LIB_DIR)/ToolLib\
			   $(LIB_DIR)/MsgLib $(LIB_DIR)/ProBaseLib\
			   $(LIB_DIR)/ProAlmLib $(LIB_DIR)/ProIndLib $(LIB_DIR)/CommLib
endif
			   
INCS = -I$(SYSLIBPATH) -I$(COMMLIBPATH) -I$(XMLPATH) -I$(TOOLPATH) -I$(PROTOCOLBASEPATH)\
	   -I$(PROTOCOLALARMPATH) -I$(PROTOCOLINDPATH)

OUTPUT= $(LIB_DIR)/ProMainLib

#****************************************************************************
# objects 
#****************************************************************************

OBJS = ProtocolMaintenContent.o ProtocolMaintenLibVer.o\
	   ProtocolMaintenSysInfo.o ProtocolMaintenComm.o ProtocolSysUpdate.o

include $(COMMON_MAKEFILE_DIR)/Makefile_back_lib
