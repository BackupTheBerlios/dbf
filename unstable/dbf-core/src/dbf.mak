# Microsoft Developer Studio Generated NMAKE File, Based on dbf.dsp
!IF "$(CFG)" == ""
CFG=dbf - Win32 Debug
!MESSAGE No configuration specified. Defaulting to dbf - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dbf - Win32 Release" && "$(CFG)" != "dbf - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dbf.mak" CFG="dbf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dbf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "dbf - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dbf - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\dbf.exe"


CLEAN :
	-@erase "$(INTDIR)\an_string.obj"
	-@erase "$(INTDIR)\codepages.obj"
	-@erase "$(INTDIR)\congraph.obj"
	-@erase "$(INTDIR)\csv.obj"
	-@erase "$(INTDIR)\dbf.obj"
	-@erase "$(INTDIR)\dbfversion.obj"
	-@erase "$(INTDIR)\endian.obj"
	-@erase "$(INTDIR)\iodbf.obj"
	-@erase "$(INTDIR)\sql.obj"
	-@erase "$(INTDIR)\statistic.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\dbf.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\dbf.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dbf.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\dbf.pdb" /machine:I386 /out:"$(OUTDIR)\dbf.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dbf.obj" \
	"$(INTDIR)\statistic.obj" \
	"$(INTDIR)\codepages.obj" \
	"$(INTDIR)\congraph.obj" \
	"$(INTDIR)\csv.obj" \
	"$(INTDIR)\dbfversion.obj" \
	"$(INTDIR)\endian.obj" \
	"$(INTDIR)\iodbf.obj" \
	"$(INTDIR)\sql.obj" \
	"$(INTDIR)\an_string.obj"

"$(OUTDIR)\dbf.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dbf - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\dbf.exe"


CLEAN :
	-@erase "$(INTDIR)\an_string.obj"
	-@erase "$(INTDIR)\codepages.obj"
	-@erase "$(INTDIR)\congraph.obj"
	-@erase "$(INTDIR)\csv.obj"
	-@erase "$(INTDIR)\dbf.obj"
	-@erase "$(INTDIR)\dbfversion.obj"
	-@erase "$(INTDIR)\endian.obj"
	-@erase "$(INTDIR)\iodbf.obj"
	-@erase "$(INTDIR)\sql.obj"
	-@erase "$(INTDIR)\statistic.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\dbf.exe"
	-@erase "$(OUTDIR)\dbf.ilk"
	-@erase "$(OUTDIR)\dbf.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\dbf.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dbf.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\dbf.pdb" /debug /machine:I386 /out:"$(OUTDIR)\dbf.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\dbf.obj" \
	"$(INTDIR)\statistic.obj" \
	"$(INTDIR)\codepages.obj" \
	"$(INTDIR)\congraph.obj" \
	"$(INTDIR)\csv.obj" \
	"$(INTDIR)\dbfversion.obj" \
	"$(INTDIR)\endian.obj" \
	"$(INTDIR)\iodbf.obj" \
	"$(INTDIR)\sql.obj" \
	"$(INTDIR)\an_string.obj"

"$(OUTDIR)\dbf.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("dbf.dep")
!INCLUDE "dbf.dep"
!ELSE 
!MESSAGE Warning: cannot find "dbf.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "dbf - Win32 Release" || "$(CFG)" == "dbf - Win32 Debug"
SOURCE=.\an_string.c

"$(INTDIR)\an_string.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\codepages.c

"$(INTDIR)\codepages.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\congraph.c

"$(INTDIR)\congraph.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\csv.c

"$(INTDIR)\csv.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dbf.c

"$(INTDIR)\dbf.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dbfversion.c

"$(INTDIR)\dbfversion.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\endian.c

"$(INTDIR)\endian.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\iodbf.c

"$(INTDIR)\iodbf.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sql.c

"$(INTDIR)\sql.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\statistic.c

"$(INTDIR)\statistic.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

