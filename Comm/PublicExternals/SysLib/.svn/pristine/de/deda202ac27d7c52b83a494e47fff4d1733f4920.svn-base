COMMON_MAKEFILE_DIR = ../CommonMakefile
include $(COMMON_MAKEFILE_DIR)/Makefile_front

XMLPATH = ../TinyXml


INCS = -I$(XMLPATH)

OUTPUT= $(LIB_DIR)/SysLib

#****************************************************************************
# objects 
#****************************************************************************

OBJS = SysLibVer.o AppPath.o Buffer.o CTime.o CRC16.o LocalDeviceConf.o Logger.o \
		ObjectBase.o ProcMutex.o PthMutex.o PubFuncSystem.o Utility.o

include $(COMMON_MAKEFILE_DIR)/Makefile_back_lib


