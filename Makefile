#*************************************************************
#*
#* Author :      AUIM
#* Description : Modulo leitor de impressão digital
#* Copyright(c): 2021 AUIM, All rights reserved
#*
#* ======================================================
#* 11/02/2021  Mateus Garcia   initial release
#*
#*************************************************************

PORT = linux
CFLAGS = -DLINUX3
CPP=g++
INCPATH = -I./ -I./libs/linux
LIBS =  -L./libs/linux
TARGET = ./bin/$(PORT)/leitor.o


PORT_PI = pi
INCPATH_PI = -I./ -I./libs/pi
LIBS_PI =  -L./libs/pi
TARGET_PI = ./bin/$(PORT_PI)/leitor.o



all : $(TARGET) ./src/leitor.cpp
$(TARGET) :
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU05 -o $(TARGET) ./src/leitor.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu05 -lusb -lpthread 

clean :
	rm -rf *.raw $(TARGET) $(TARGET_PI)


pi : $(TARGET_PI)
$(TARGET_PI) :
	$(CPP) $(CFLAGS) $(INCPATH_PI) -D_FDU05 -o $(TARGET_PI) ./src/leitor.cpp $(LIBS_PI) -lsgfplib -lsgnfiq -lsgfpamx -lsgfdu05 -lusb -lpthread 



run : $(TARGET)
	$(TARGET)
