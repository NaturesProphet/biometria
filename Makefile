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

PORT = linux# Mude para "pi" sem aspas para buildar a versão raspberry
CFLAGS = -DLINUX3
CPP=g++
INCPATH = -I./ -I./libs/linux# Mude para ./libs/pi para a versão raspberry
LIBS =  -L./libs/linux


TARGET = ./bin/$(PORT)/leitor.o


all : $(TARGET) ./src/leitor.cpp


$(TARGET) :
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU05 -o $(TARGET) ./src/leitor.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu05 -lusb -lpthread 



clean :
	rm -rf *.raw $(TARGET) core

run: $(TARGET)
	$(TARGET)