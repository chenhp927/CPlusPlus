COMMON_MAKEFILE_DIR = ../CommonMakefile
include $(COMMON_MAKEFILE_DIR)/Makefile_front

XMLPATH = ../TinyXml
SYSLIBPATH = ../SysLib

#DEFS = -DUSE_MULTI_THREAD
#CXXFLAGS += $(DEFS)

INCS = -I$(XMLPATH) -I$(SYSLIBPATH)

OUTPUT= $(LIB_DIR)/ToolLib

#****************************************************************************
# objects 
#****************************************************************************

OBJS = ToolLibVer.o GetEthif.o ProcLock.o PthreadWrap.o 


include $(COMMON_MAKEFILE_DIR)/Makefile_back_lib


