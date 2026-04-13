#! /usr/bin/env make

.SUFFIXES: .cpp .c .obj .res .rc

CC = cl /nologo
C++ = cl /nologo
LINK = link /nologo
RC = rc /nologo

DEBUG = /D__DEBUG
NDEBUG = /DNDEBUG

ROOT = ../../..
INC = $(ROOT)/include
LIBD = $(ROOT)/lib/
TGTDIR = $(ROOT)/release
VPATH =

DEFFINE = $(NDEBUG) /DUNICODE /D_UNICODE /D_CRT_SECURE_NO_WARNINGS /MT /EHsc /utf-8 
CFLAG = $(DEFFINE) /I. /I$(INC) /W3

USERLIBS = /LIBPATH:"$(LIBD)" libcef.lib libcef_dll_wrapper.lib
WINLIBS = user32.lib gdi32.lib version.lib
LIBS = $(USERLIBS) $(WINLIBS)
LDFLAGS = $(LIBS)

SOURCE  = zbCefHandler.cpp zbWidget.cpp zbBrowser.cpp zbcefmain.cpp
OBJECTS = zbCefHandler.obj zbWidget.obj zbBrowser.obj zbcefmain.obj

TARGET = $(TGTDIR)/Cefwarp.exe

all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(LINK) /OUT:$@ $^ $(LDFLAGS)

.c.obj:
	$(CC) /c $< $(CFLAG)
.cpp.obj:
	$(C++) /c $< $(CFLAG)
.rc.res:
	$(RC) /utf-8 /fo $<

test: all
	$(TARGET)

depend: all
	@dumpbin /nologo /dependents $(TARGET)

clean:
	$(RM) -r $(TGTDIR)/*.exe GPUCache *.res *.gbk *.o *.obj *.log $(TGTDIR)/*.log *~ .*~

zbcefmain.obj: zbcefmain.cpp
zbCefHandler.obj: zbCefHandler.cpp zbCefHandler.h zbCefApp.h
zbBrowser.obj: zbBrowser.cpp zbBrowser.h zbWidget.h
zbWidget.obj: zbWidget.cpp zbWidget.h
