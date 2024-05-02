
# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := Analysis

# including the module.mk file
-include $(patsubst %, %/module.mk,Analysis)
-include $(patsubst %, %/pfgct/module.mk,Analysis)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst Analysis/%,%,$(basename $(wildcard $(patsubst %,Analysis/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,Analysis/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort Analysis/ $(dir $(patsubst %,Analysis/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_Analysis := $(foreach i, $(patsubst %,Analysis/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_Analysis),)
  SRC += Analysis/dict_Analysis
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,Analysis/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_Analysis := $(foreach i, $(patsubst %,Analysis/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_Analysis := $(filter %selection.xml,$(REFLEXH_Analysis))
REFLEXH_Analysis := $(filter-out %selection.xml,$(REFLEXH_Analysis))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_Analysis),)
ifeq ($(SELECTIONXML_Analysis),)
$(error No selection.xml file specified in the REFLEXFILES of module Analysis)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module Analysis : common dictionary building is not implemented for REFLEX.)
endif
SRC += Analysis/reflexdict_Analysis
ADDDEPS += ${DEPDIR}/Analysis/reflexdict_Analysis.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,Analysis/%,$(PROGRAMS))

Analysis_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/Analysis/%.bin
	cp $^ $@

$(BINDIR)/%: Analysis/%
	cp $^ $@

$(BINDIR)/%: Analysis/bin/%
	cp $^ $@

define Analysis_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/Analysis/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(Analysis_PROGDIRS),$(eval $(call Analysis_PROGRAM_template,$(sd))))

Analysis/%SK.cc Analysis/%.hh: Analysis/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CAnalysis -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := EvtModel

# including the module.mk file
-include $(patsubst %, %/module.mk,EvtModel)
-include $(patsubst %, %/pfgct/module.mk,EvtModel)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst EvtModel/%,%,$(basename $(wildcard $(patsubst %,EvtModel/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,EvtModel/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort EvtModel/ $(dir $(patsubst %,EvtModel/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_EvtModel := $(foreach i, $(patsubst %,EvtModel/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_EvtModel),)
  SRC += EvtModel/dict_EvtModel
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,EvtModel/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_EvtModel := $(foreach i, $(patsubst %,EvtModel/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_EvtModel := $(filter %selection.xml,$(REFLEXH_EvtModel))
REFLEXH_EvtModel := $(filter-out %selection.xml,$(REFLEXH_EvtModel))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_EvtModel),)
ifeq ($(SELECTIONXML_EvtModel),)
$(error No selection.xml file specified in the REFLEXFILES of module EvtModel)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module EvtModel : common dictionary building is not implemented for REFLEX.)
endif
SRC += EvtModel/reflexdict_EvtModel
ADDDEPS += ${DEPDIR}/EvtModel/reflexdict_EvtModel.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,EvtModel/%,$(PROGRAMS))

EvtModel_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/EvtModel/%.bin
	cp $^ $@

$(BINDIR)/%: EvtModel/%
	cp $^ $@

$(BINDIR)/%: EvtModel/bin/%
	cp $^ $@

define EvtModel_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/EvtModel/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(EvtModel_PROGDIRS),$(eval $(call EvtModel_PROGRAM_template,$(sd))))

EvtModel/%SK.cc EvtModel/%.hh: EvtModel/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CEvtModel -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := EvtModelTools

# including the module.mk file
-include $(patsubst %, %/module.mk,EvtModelTools)
-include $(patsubst %, %/pfgct/module.mk,EvtModelTools)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst EvtModelTools/%,%,$(basename $(wildcard $(patsubst %,EvtModelTools/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,EvtModelTools/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort EvtModelTools/ $(dir $(patsubst %,EvtModelTools/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_EvtModelTools := $(foreach i, $(patsubst %,EvtModelTools/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_EvtModelTools),)
  SRC += EvtModelTools/dict_EvtModelTools
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,EvtModelTools/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_EvtModelTools := $(foreach i, $(patsubst %,EvtModelTools/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_EvtModelTools := $(filter %selection.xml,$(REFLEXH_EvtModelTools))
REFLEXH_EvtModelTools := $(filter-out %selection.xml,$(REFLEXH_EvtModelTools))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_EvtModelTools),)
ifeq ($(SELECTIONXML_EvtModelTools),)
$(error No selection.xml file specified in the REFLEXFILES of module EvtModelTools)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module EvtModelTools : common dictionary building is not implemented for REFLEX.)
endif
SRC += EvtModelTools/reflexdict_EvtModelTools
ADDDEPS += ${DEPDIR}/EvtModelTools/reflexdict_EvtModelTools.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,EvtModelTools/%,$(PROGRAMS))

EvtModelTools_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/EvtModelTools/%.bin
	cp $^ $@

$(BINDIR)/%: EvtModelTools/%
	cp $^ $@

$(BINDIR)/%: EvtModelTools/bin/%
	cp $^ $@

define EvtModelTools_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/EvtModelTools/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(EvtModelTools_PROGDIRS),$(eval $(call EvtModelTools_PROGRAM_template,$(sd))))

EvtModelTools/%SK.cc EvtModelTools/%.hh: EvtModelTools/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CEvtModelTools -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := Fitters

# including the module.mk file
-include $(patsubst %, %/module.mk,Fitters)
-include $(patsubst %, %/pfgct/module.mk,Fitters)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst Fitters/%,%,$(basename $(wildcard $(patsubst %,Fitters/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,Fitters/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort Fitters/ $(dir $(patsubst %,Fitters/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_Fitters := $(foreach i, $(patsubst %,Fitters/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_Fitters),)
  SRC += Fitters/dict_Fitters
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,Fitters/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_Fitters := $(foreach i, $(patsubst %,Fitters/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_Fitters := $(filter %selection.xml,$(REFLEXH_Fitters))
REFLEXH_Fitters := $(filter-out %selection.xml,$(REFLEXH_Fitters))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_Fitters),)
ifeq ($(SELECTIONXML_Fitters),)
$(error No selection.xml file specified in the REFLEXFILES of module Fitters)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module Fitters : common dictionary building is not implemented for REFLEX.)
endif
SRC += Fitters/reflexdict_Fitters
ADDDEPS += ${DEPDIR}/Fitters/reflexdict_Fitters.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,Fitters/%,$(PROGRAMS))

Fitters_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/Fitters/%.bin
	cp $^ $@

$(BINDIR)/%: Fitters/%
	cp $^ $@

$(BINDIR)/%: Fitters/bin/%
	cp $^ $@

define Fitters_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/Fitters/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(Fitters_PROGDIRS),$(eval $(call Fitters_PROGRAM_template,$(sd))))

Fitters/%SK.cc Fitters/%.hh: Fitters/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CFitters -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := Misc

# including the module.mk file
-include $(patsubst %, %/module.mk,Misc)
-include $(patsubst %, %/pfgct/module.mk,Misc)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst Misc/%,%,$(basename $(wildcard $(patsubst %,Misc/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,Misc/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort Misc/ $(dir $(patsubst %,Misc/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_Misc := $(foreach i, $(patsubst %,Misc/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_Misc),)
  SRC += Misc/dict_Misc
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,Misc/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_Misc := $(foreach i, $(patsubst %,Misc/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_Misc := $(filter %selection.xml,$(REFLEXH_Misc))
REFLEXH_Misc := $(filter-out %selection.xml,$(REFLEXH_Misc))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_Misc),)
ifeq ($(SELECTIONXML_Misc),)
$(error No selection.xml file specified in the REFLEXFILES of module Misc)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module Misc : common dictionary building is not implemented for REFLEX.)
endif
SRC += Misc/reflexdict_Misc
ADDDEPS += ${DEPDIR}/Misc/reflexdict_Misc.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,Misc/%,$(PROGRAMS))

Misc_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/Misc/%.bin
	cp $^ $@

$(BINDIR)/%: Misc/%
	cp $^ $@

$(BINDIR)/%: Misc/bin/%
	cp $^ $@

define Misc_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/Misc/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(Misc_PROGDIRS),$(eval $(call Misc_PROGRAM_template,$(sd))))

Misc/%SK.cc Misc/%.hh: Misc/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CMisc -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := MTools

# including the module.mk file
-include $(patsubst %, %/module.mk,MTools)
-include $(patsubst %, %/pfgct/module.mk,MTools)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst MTools/%,%,$(basename $(wildcard $(patsubst %,MTools/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,MTools/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort MTools/ $(dir $(patsubst %,MTools/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_MTools := $(foreach i, $(patsubst %,MTools/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_MTools),)
  SRC += MTools/dict_MTools
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,MTools/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_MTools := $(foreach i, $(patsubst %,MTools/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_MTools := $(filter %selection.xml,$(REFLEXH_MTools))
REFLEXH_MTools := $(filter-out %selection.xml,$(REFLEXH_MTools))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_MTools),)
ifeq ($(SELECTIONXML_MTools),)
$(error No selection.xml file specified in the REFLEXFILES of module MTools)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module MTools : common dictionary building is not implemented for REFLEX.)
endif
SRC += MTools/reflexdict_MTools
ADDDEPS += ${DEPDIR}/MTools/reflexdict_MTools.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,MTools/%,$(PROGRAMS))

MTools_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/MTools/%.bin
	cp $^ $@

$(BINDIR)/%: MTools/%
	cp $^ $@

$(BINDIR)/%: MTools/bin/%
	cp $^ $@

define MTools_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/MTools/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(MTools_PROGDIRS),$(eval $(call MTools_PROGRAM_template,$(sd))))

MTools/%SK.cc MTools/%.hh: MTools/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CMTools -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := PID

# including the module.mk file
-include $(patsubst %, %/module.mk,PID)
-include $(patsubst %, %/pfgct/module.mk,PID)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst PID/%,%,$(basename $(wildcard $(patsubst %,PID/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,PID/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort PID/ $(dir $(patsubst %,PID/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_PID := $(foreach i, $(patsubst %,PID/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_PID),)
  SRC += PID/dict_PID
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,PID/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_PID := $(foreach i, $(patsubst %,PID/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_PID := $(filter %selection.xml,$(REFLEXH_PID))
REFLEXH_PID := $(filter-out %selection.xml,$(REFLEXH_PID))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_PID),)
ifeq ($(SELECTIONXML_PID),)
$(error No selection.xml file specified in the REFLEXFILES of module PID)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module PID : common dictionary building is not implemented for REFLEX.)
endif
SRC += PID/reflexdict_PID
ADDDEPS += ${DEPDIR}/PID/reflexdict_PID.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,PID/%,$(PROGRAMS))

PID_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/PID/%.bin
	cp $^ $@

$(BINDIR)/%: PID/%
	cp $^ $@

$(BINDIR)/%: PID/bin/%
	cp $^ $@

define PID_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/PID/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(PID_PROGDIRS),$(eval $(call PID_PROGRAM_template,$(sd))))

PID/%SK.cc PID/%.hh: PID/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CPID -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := SampleTools

# including the module.mk file
-include $(patsubst %, %/module.mk,SampleTools)
-include $(patsubst %, %/pfgct/module.mk,SampleTools)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst SampleTools/%,%,$(basename $(wildcard $(patsubst %,SampleTools/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,SampleTools/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort SampleTools/ $(dir $(patsubst %,SampleTools/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_SampleTools := $(foreach i, $(patsubst %,SampleTools/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_SampleTools),)
  SRC += SampleTools/dict_SampleTools
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,SampleTools/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_SampleTools := $(foreach i, $(patsubst %,SampleTools/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_SampleTools := $(filter %selection.xml,$(REFLEXH_SampleTools))
REFLEXH_SampleTools := $(filter-out %selection.xml,$(REFLEXH_SampleTools))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_SampleTools),)
ifeq ($(SELECTIONXML_SampleTools),)
$(error No selection.xml file specified in the REFLEXFILES of module SampleTools)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module SampleTools : common dictionary building is not implemented for REFLEX.)
endif
SRC += SampleTools/reflexdict_SampleTools
ADDDEPS += ${DEPDIR}/SampleTools/reflexdict_SampleTools.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,SampleTools/%,$(PROGRAMS))

SampleTools_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/SampleTools/%.bin
	cp $^ $@

$(BINDIR)/%: SampleTools/%
	cp $^ $@

$(BINDIR)/%: SampleTools/bin/%
	cp $^ $@

define SampleTools_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/SampleTools/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(SampleTools_PROGDIRS),$(eval $(call SampleTools_PROGRAM_template,$(sd))))

SampleTools/%SK.cc SampleTools/%.hh: SampleTools/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CSampleTools -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := SignalModel

# including the module.mk file
-include $(patsubst %, %/module.mk,SignalModel)
-include $(patsubst %, %/pfgct/module.mk,SignalModel)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst SignalModel/%,%,$(basename $(wildcard $(patsubst %,SignalModel/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,SignalModel/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort SignalModel/ $(dir $(patsubst %,SignalModel/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_SignalModel := $(foreach i, $(patsubst %,SignalModel/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_SignalModel),)
  SRC += SignalModel/dict_SignalModel
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,SignalModel/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_SignalModel := $(foreach i, $(patsubst %,SignalModel/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_SignalModel := $(filter %selection.xml,$(REFLEXH_SignalModel))
REFLEXH_SignalModel := $(filter-out %selection.xml,$(REFLEXH_SignalModel))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_SignalModel),)
ifeq ($(SELECTIONXML_SignalModel),)
$(error No selection.xml file specified in the REFLEXFILES of module SignalModel)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module SignalModel : common dictionary building is not implemented for REFLEX.)
endif
SRC += SignalModel/reflexdict_SignalModel
ADDDEPS += ${DEPDIR}/SignalModel/reflexdict_SignalModel.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,SignalModel/%,$(PROGRAMS))

SignalModel_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/SignalModel/%.bin
	cp $^ $@

$(BINDIR)/%: SignalModel/%
	cp $^ $@

$(BINDIR)/%: SignalModel/bin/%
	cp $^ $@

define SignalModel_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/SignalModel/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(SignalModel_PROGDIRS),$(eval $(call SignalModel_PROGRAM_template,$(sd))))

SignalModel/%SK.cc SignalModel/%.hh: SignalModel/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CSignalModel -bcxx $^

endif

