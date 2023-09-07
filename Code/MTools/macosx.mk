# macosx:
#   to build the .so, use "make"
#   to build the .dylib, use "make DLLSUFFIX=dylib"

MACOSX_MINOR  := $(shell sw_vers -productVersion | cut -d'.' -f2)
PLATFORM      := macosx
# FC:  do not force dynamic lib by default
#DLLSUFFIX     := dylib


# compiler
CXX        := c++
CXXFLAGS   += -O3 -pipe -Wall -W -Woverloaded-virtual

UNDEFOPT   := suppress
LD         := c++
LDFLAGS    := -shared -Wl


ifeq "$(DLLSUFFIX)" "dylib"
   SOLIBNAME  := lib$(LIBRARYNAME).dylib.$(VERSION)
   LDFLAGS    := -dynamiclib -flat_namespace -undefined $(UNDEFOPT) -current_version $(VERSION)
   ADDITIONAL_TARGETS := $(SOLIBDIR)/lib$(LIBRARYNAME).so

   $(SOLIBDIR)/lib$(LIBRARYNAME).so: $(SOLIBDIR)/lib$(LIBRARYNAME).dylib
	@ln -s lib$(LIBRARYNAME).dylib $(SOLIBDIR)/lib$(LIBRARYNAME).so
endif

# Workaround for 'cp' not providing the '--parents' option on Mac OS X
SYNC := rsync -R

