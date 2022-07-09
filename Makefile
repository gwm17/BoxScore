########################################################################
#
##              --- CAEN SpA - Computing Division ---
#
##   CAENDigitizer Software Project
#
##   Created  :  October    2009      (Rel. 1.0)
#
##   Auth: A. Lucchesi
#
#########################################################################
ARCH	=	`uname -m`

OUT2     =    DetectDigitizer

CC	=	g++

COPTS	=	-fPIC -DLINUX -w

DEPLIBS	=	-lCAENDigitizer

INCLUDEDIR =	-I./src/oldCore

OBJS1	=	src/oldCore/keyb.o src/oldCore/Functions.o 
OBJS2   =   src/oldCore/keyb.o src/oldCore/DetectDigitizer.o 

INCLUDES = src/oldCore/keyb.h src/oldCore/Functions.h	

ROOTLIBS = `root-config --cflags --glibs`

#########################################################################

all	:	$(OUT2) CutsCreator BoxScore BoxScoreReader

clean	:
		/bin/rm -f $(OBJS1) $(OBJS2) $(OUT2) ./CutsCreator ./BoxScore ./BoxScoreReader

$(OUT2)	:	$(OBJS2)
		$(CC) -o $(OUT2) $(OBJS2) $(DEPLIBS)

$(OBJS2)	:	$(INCLUDES) Makefile

%.o	:	%.c
		$(CC) $(COPTS) $(INCLUDEDIR) -c -o $@ $<

CutsCreator:	$(OBJS3) src/oldCore/CutsCreator.c
		g++ -std=c++17 -pthread $^ -o $@ $(ROOTLIBS)

BoxScore	: src/oldCore/BoxScore.c src/oldCore/DigitizerClass.h src/oldCore/FileIO.h src/oldCore/GenericPlane.h src/oldCore/HelioTarget.h src/oldCore/IsoDetect.h src/oldCore/HelioArray.h src/oldCore/MCPClass.h
		g++ -std=c++17 -pthread $^ -o $@  $(DEPLIBS) $(ROOTLIBS)

BoxScoreReader: src/oldCore/BoxScoreReader.c src/oldCore/GenericPlane.h src/oldCore/HelioTarget.h src/oldCore/IsoDetect.h src/oldCore/HelioArray.h
		g++ -std=c++17 $^ -o $@ $(ROOTLIBS)

