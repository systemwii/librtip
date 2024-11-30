# see the readme for instructions: https://github.com/systemwii/make

# --- target ---
TARGET		:=	rtip
TYPE		:=  a+h
PLATFORM	:=	wii
BUILD		:=	_
CACHE		:=	_/cache

# --- sources ---
SRCS		 =	src
SRCEXTS		 =	.c
BINS		 =  
BINEXTS		 =  
LIBS		 =	-logc
LIBDIRSBNDLE =	
LIBDIRSLOOSE =	
INCLUDES	 =	

# --- flags ---
CFLAGS		 =	-save-temps -g -O2 -Wall -Wno-unused-variable $(MACHDEP)
CXXFLAGS	 =	$(CFLAGS)
ASFLAGS		 =	-D_LANGUAGE_ASSEMBLY
LDFLAGS		 =	-g $(MACHDEP) -Wl,-Map,$(CACHE)/$(notdir $@).map
ARFLAGS		 =	rcs

# --- runs the templates ---
$(if $(findstring /,$(DEVKITPPC)),,$(error DEVKITPPC not set; run: export DEVKITPPC=<path to>devkitPPC))
RULESDIR	:=	lib/make/rules
include $(RULESDIR)/main.mk
