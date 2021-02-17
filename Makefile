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
TARGET_COMPARADOR = ./bin/comparador/$(PORT)/comparador.o


PORT_PI = pi
INCPATH_PI = -I./ -I./libs/pi
LIBS_PI =  -L./libs/pi
TARGET_PI_LEITOR = ./bin/leitor/$(PORT_PI)/leitor.o
TARGET_PI_COMPARADOR = ./bin/comparador/$(PORT_PI)/comparador.o



all : $(TARGET_COMPARADOR) $(TARGET_LEITOR)

$(TARGET_LEITOR) :
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU05 -o $(TARGET_LEITOR) ./src/leitor.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu05 -lusb -lpthread

$(TARGET_COMPARADOR) :
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU05 -o $(TARGET_COMPARADOR) ./src/comparador.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu05 -lusb -lpthread


clean :
	rm -rf $(TARGET_LEITOR) $(TARGET_COMPARADOR) $(TARGET_PI_LEITOR) $(TARGET_PI_COMPARADOR)


pi : $(TARGET_PI_LEITOR) $(TARGET_PI_COMPARADOR)

$(TARGET_PI_LEITOR) :
	$(CPP) $(CFLAGS) $(INCPATH_PI) -D_FDU05 -o $(TARGET_PI_LEITOR) ./src/leitor.cpp $(LIBS_PI) -lsgfplib -lsgnfiq -lsgfpamx -lsgfdu05 -lusb -lpthread

$(TARGET_PI_COMPARADOR) :
	$(CPP) $(CFLAGS) $(INCPATH_PI) -D_FDU05 -o $(TARGET_PI_COMPARADOR) ./src/comparador.cpp $(LIBS_PI) -lsgfplib -lsgnfiq -lsgfpamx -lsgfdu05 -lusb -lpthread


piinstall: $(TARGET_PI_LEITOR) $(TARGET_PI_COMPARADOR)
	sudo cp $(TARGET_PI_LEITOR) /bin/leitor && sudo cp $(TARGET_PI_COMPARADOR) /bin/comparador

install: $(TARGET_LEITOR) $(TARGET_COMPARADOR)
	sudo cp $(TARGET_LEITOR) /bin/leitor && sudo cp $(TARGET_COMPARADOR) /bin/comparador

uninstall: /bin/leitor /bin/comparador
	sudo rm /bin/leitor && sudo rm /bin/comparador