COMMON_MAKEFILE_DIR = ../CommonMakefile
include $(COMMON_MAKEFILE_DIR)/Makefile_front

XMLPATH= ../TinyXml
SYSLIBPATH = ../SysLib
TOOLPATH = ../Tools
COMMPATH = ../CommLib
PROTOCOLBASEPATH = ../ProtocolBase

INCS = -I$(XMLPATH) -I$(SYSLIBPATH) -I$(TOOLPATH) -I$(COMMPATH) -I$(PROTOCOLBASEPATH)

OUTPUT= $(LIB_DIR)/AppLib

#****************************************************************************
# objects 
#****************************************************************************

OBJS = UnixApp.o AppLibVer.o TaskBase.o

include $(COMMON_MAKEFILE_DIR)/Makefile_back_lib

