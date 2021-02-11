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
TARGET_LEITOR = ./bin/leitor/$(PORT)/leitor.o
TARGET_COMPARADOR = ./bin/comparador/$(PORT)/leitor.o


PORT_PI = pi
INCPATH_PI = -I./ -I./libs/pi
LIBS_PI =  -L./libs/pi
TARGET_PI_LEITOR = ./bin/leitor/$(PORT_PI)/leitor.o
TARGET_PI_COMPARADOR = ./bin/comparador/$(PORT_PI)/leitor.o



all : $(TARGET_LEITOR) ./src/leitor.cpp
$(TARGET_LEITOR) :
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU05 -o $(TARGET_LEITOR) ./src/leitor.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu05 -lusb -lpthread

clean :
	rm -rf *.raw $(TARGET_LEITOR) $(TARGET_PI_LEITOR)


pi : $(TARGET_PI_LEITOR)
$(TARGET_PI_LEITOR) :
	$(CPP) $(CFLAGS) $(INCPATH_PI) -D_FDU05 -o $(TARGET_PI_LEITOR) ./src/leitor.cpp $(LIBS_PI) -lsgfplib -lsgnfiq -lsgfpamx -lsgfdu05 -lusb -lpthread



run : $(TARGET_LEITOR)
	$(TARGET_LEITOR)
