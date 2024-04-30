
# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := Analysis_JF

# including the module.mk file
-include $(patsubst %, %/module.mk,Analysis_JF)
-include $(patsubst %, %/pfgct/module.mk,Analysis_JF)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst Analysis_JF/%,%,$(basename $(wildcard $(patsubst %,Analysis_JF/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,Analysis_JF/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort Analysis_JF/ $(dir $(patsubst %,Analysis_JF/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_Analysis_JF := $(foreach i, $(patsubst %,Analysis_JF/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_Analysis_JF),)
  SRC += Analysis_JF/dict_Analysis_JF
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,Analysis_JF/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_Analysis_JF := $(foreach i, $(patsubst %,Analysis_JF/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_Analysis_JF := $(filter %selection.xml,$(REFLEXH_Analysis_JF))
REFLEXH_Analysis_JF := $(filter-out %selection.xml,$(REFLEXH_Analysis_JF))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_Analysis_JF),)
ifeq ($(SELECTIONXML_Analysis_JF),)
$(error No selection.xml file specified in the REFLEXFILES of module Analysis_JF)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module Analysis_JF : common dictionary building is not implemented for REFLEX.)
endif
SRC += Analysis_JF/reflexdict_Analysis_JF
ADDDEPS += ${DEPDIR}/Analysis_JF/reflexdict_Analysis_JF.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,Analysis_JF/%,$(PROGRAMS))

Analysis_JF_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/Analysis_JF/%.bin
	cp $^ $@

$(BINDIR)/%: Analysis_JF/%
	cp $^ $@

$(BINDIR)/%: Analysis_JF/bin/%
	cp $^ $@

define Analysis_JF_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/Analysis_JF/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(Analysis_JF_PROGDIRS),$(eval $(call Analysis_JF_PROGRAM_template,$(sd))))

Analysis_JF/%SK.cc Analysis_JF/%.hh: Analysis_JF/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CAnalysis_JF -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := AnalysisTools

# including the module.mk file
-include $(patsubst %, %/module.mk,AnalysisTools)
-include $(patsubst %, %/pfgct/module.mk,AnalysisTools)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst AnalysisTools/%,%,$(basename $(wildcard $(patsubst %,AnalysisTools/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,AnalysisTools/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort AnalysisTools/ $(dir $(patsubst %,AnalysisTools/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_AnalysisTools := $(foreach i, $(patsubst %,AnalysisTools/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_AnalysisTools),)
  SRC += AnalysisTools/dict_AnalysisTools
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,AnalysisTools/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_AnalysisTools := $(foreach i, $(patsubst %,AnalysisTools/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_AnalysisTools := $(filter %selection.xml,$(REFLEXH_AnalysisTools))
REFLEXH_AnalysisTools := $(filter-out %selection.xml,$(REFLEXH_AnalysisTools))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_AnalysisTools),)
ifeq ($(SELECTIONXML_AnalysisTools),)
$(error No selection.xml file specified in the REFLEXFILES of module AnalysisTools)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module AnalysisTools : common dictionary building is not implemented for REFLEX.)
endif
SRC += AnalysisTools/reflexdict_AnalysisTools
ADDDEPS += ${DEPDIR}/AnalysisTools/reflexdict_AnalysisTools.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,AnalysisTools/%,$(PROGRAMS))

AnalysisTools_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/AnalysisTools/%.bin
	cp $^ $@

$(BINDIR)/%: AnalysisTools/%
	cp $^ $@

$(BINDIR)/%: AnalysisTools/bin/%
	cp $^ $@

define AnalysisTools_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/AnalysisTools/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(AnalysisTools_PROGDIRS),$(eval $(call AnalysisTools_PROGRAM_template,$(sd))))

AnalysisTools/%SK.cc AnalysisTools/%.hh: AnalysisTools/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CAnalysisTools -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := AOBt

# including the module.mk file
-include $(patsubst %, %/module.mk,AOBt)
-include $(patsubst %, %/pfgct/module.mk,AOBt)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst AOBt/%,%,$(basename $(wildcard $(patsubst %,AOBt/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,AOBt/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort AOBt/ $(dir $(patsubst %,AOBt/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_AOBt := $(foreach i, $(patsubst %,AOBt/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_AOBt),)
  SRC += AOBt/dict_AOBt
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,AOBt/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_AOBt := $(foreach i, $(patsubst %,AOBt/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_AOBt := $(filter %selection.xml,$(REFLEXH_AOBt))
REFLEXH_AOBt := $(filter-out %selection.xml,$(REFLEXH_AOBt))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_AOBt),)
ifeq ($(SELECTIONXML_AOBt),)
$(error No selection.xml file specified in the REFLEXFILES of module AOBt)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module AOBt : common dictionary building is not implemented for REFLEX.)
endif
SRC += AOBt/reflexdict_AOBt
ADDDEPS += ${DEPDIR}/AOBt/reflexdict_AOBt.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,AOBt/%,$(PROGRAMS))

AOBt_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/AOBt/%.bin
	cp $^ $@

$(BINDIR)/%: AOBt/%
	cp $^ $@

$(BINDIR)/%: AOBt/bin/%
	cp $^ $@

define AOBt_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/AOBt/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(AOBt_PROGDIRS),$(eval $(call AOBt_PROGRAM_template,$(sd))))

AOBt/%SK.cc AOBt/%.hh: AOBt/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CAOBt -bcxx $^

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

THISMODULE := Fitters_Cluster

# including the module.mk file
-include $(patsubst %, %/module.mk,Fitters_Cluster)
-include $(patsubst %, %/pfgct/module.mk,Fitters_Cluster)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst Fitters_Cluster/%,%,$(basename $(wildcard $(patsubst %,Fitters_Cluster/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,Fitters_Cluster/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort Fitters_Cluster/ $(dir $(patsubst %,Fitters_Cluster/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_Fitters_Cluster := $(foreach i, $(patsubst %,Fitters_Cluster/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_Fitters_Cluster),)
  SRC += Fitters_Cluster/dict_Fitters_Cluster
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,Fitters_Cluster/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_Fitters_Cluster := $(foreach i, $(patsubst %,Fitters_Cluster/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_Fitters_Cluster := $(filter %selection.xml,$(REFLEXH_Fitters_Cluster))
REFLEXH_Fitters_Cluster := $(filter-out %selection.xml,$(REFLEXH_Fitters_Cluster))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_Fitters_Cluster),)
ifeq ($(SELECTIONXML_Fitters_Cluster),)
$(error No selection.xml file specified in the REFLEXFILES of module Fitters_Cluster)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module Fitters_Cluster : common dictionary building is not implemented for REFLEX.)
endif
SRC += Fitters_Cluster/reflexdict_Fitters_Cluster
ADDDEPS += ${DEPDIR}/Fitters_Cluster/reflexdict_Fitters_Cluster.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,Fitters_Cluster/%,$(PROGRAMS))

Fitters_Cluster_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/Fitters_Cluster/%.bin
	cp $^ $@

$(BINDIR)/%: Fitters_Cluster/%
	cp $^ $@

$(BINDIR)/%: Fitters_Cluster/bin/%
	cp $^ $@

define Fitters_Cluster_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/Fitters_Cluster/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(Fitters_Cluster_PROGDIRS),$(eval $(call Fitters_Cluster_PROGRAM_template,$(sd))))

Fitters_Cluster/%SK.cc Fitters_Cluster/%.hh: Fitters_Cluster/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CFitters_Cluster -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := Fitters_Sample

# including the module.mk file
-include $(patsubst %, %/module.mk,Fitters_Sample)
-include $(patsubst %, %/pfgct/module.mk,Fitters_Sample)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst Fitters_Sample/%,%,$(basename $(wildcard $(patsubst %,Fitters_Sample/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,Fitters_Sample/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort Fitters_Sample/ $(dir $(patsubst %,Fitters_Sample/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_Fitters_Sample := $(foreach i, $(patsubst %,Fitters_Sample/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_Fitters_Sample),)
  SRC += Fitters_Sample/dict_Fitters_Sample
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,Fitters_Sample/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_Fitters_Sample := $(foreach i, $(patsubst %,Fitters_Sample/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_Fitters_Sample := $(filter %selection.xml,$(REFLEXH_Fitters_Sample))
REFLEXH_Fitters_Sample := $(filter-out %selection.xml,$(REFLEXH_Fitters_Sample))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_Fitters_Sample),)
ifeq ($(SELECTIONXML_Fitters_Sample),)
$(error No selection.xml file specified in the REFLEXFILES of module Fitters_Sample)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module Fitters_Sample : common dictionary building is not implemented for REFLEX.)
endif
SRC += Fitters_Sample/reflexdict_Fitters_Sample
ADDDEPS += ${DEPDIR}/Fitters_Sample/reflexdict_Fitters_Sample.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,Fitters_Sample/%,$(PROGRAMS))

Fitters_Sample_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/Fitters_Sample/%.bin
	cp $^ $@

$(BINDIR)/%: Fitters_Sample/%
	cp $^ $@

$(BINDIR)/%: Fitters_Sample/bin/%
	cp $^ $@

define Fitters_Sample_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/Fitters_Sample/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(Fitters_Sample_PROGDIRS),$(eval $(call Fitters_Sample_PROGRAM_template,$(sd))))

Fitters_Sample/%SK.cc Fitters_Sample/%.hh: Fitters_Sample/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CFitters_Sample -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := Fitters_Track

# including the module.mk file
-include $(patsubst %, %/module.mk,Fitters_Track)
-include $(patsubst %, %/pfgct/module.mk,Fitters_Track)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst Fitters_Track/%,%,$(basename $(wildcard $(patsubst %,Fitters_Track/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,Fitters_Track/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort Fitters_Track/ $(dir $(patsubst %,Fitters_Track/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_Fitters_Track := $(foreach i, $(patsubst %,Fitters_Track/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_Fitters_Track),)
  SRC += Fitters_Track/dict_Fitters_Track
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,Fitters_Track/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_Fitters_Track := $(foreach i, $(patsubst %,Fitters_Track/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_Fitters_Track := $(filter %selection.xml,$(REFLEXH_Fitters_Track))
REFLEXH_Fitters_Track := $(filter-out %selection.xml,$(REFLEXH_Fitters_Track))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_Fitters_Track),)
ifeq ($(SELECTIONXML_Fitters_Track),)
$(error No selection.xml file specified in the REFLEXFILES of module Fitters_Track)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module Fitters_Track : common dictionary building is not implemented for REFLEX.)
endif
SRC += Fitters_Track/reflexdict_Fitters_Track
ADDDEPS += ${DEPDIR}/Fitters_Track/reflexdict_Fitters_Track.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,Fitters_Track/%,$(PROGRAMS))

Fitters_Track_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/Fitters_Track/%.bin
	cp $^ $@

$(BINDIR)/%: Fitters_Track/%
	cp $^ $@

$(BINDIR)/%: Fitters_Track/bin/%
	cp $^ $@

define Fitters_Track_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/Fitters_Track/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(Fitters_Track_PROGDIRS),$(eval $(call Fitters_Track_PROGRAM_template,$(sd))))

Fitters_Track/%SK.cc Fitters_Track/%.hh: Fitters_Track/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CFitters_Track -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := GenerateShape

# including the module.mk file
-include $(patsubst %, %/module.mk,GenerateShape)
-include $(patsubst %, %/pfgct/module.mk,GenerateShape)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst GenerateShape/%,%,$(basename $(wildcard $(patsubst %,GenerateShape/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,GenerateShape/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort GenerateShape/ $(dir $(patsubst %,GenerateShape/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_GenerateShape := $(foreach i, $(patsubst %,GenerateShape/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_GenerateShape),)
  SRC += GenerateShape/dict_GenerateShape
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,GenerateShape/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_GenerateShape := $(foreach i, $(patsubst %,GenerateShape/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_GenerateShape := $(filter %selection.xml,$(REFLEXH_GenerateShape))
REFLEXH_GenerateShape := $(filter-out %selection.xml,$(REFLEXH_GenerateShape))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_GenerateShape),)
ifeq ($(SELECTIONXML_GenerateShape),)
$(error No selection.xml file specified in the REFLEXFILES of module GenerateShape)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module GenerateShape : common dictionary building is not implemented for REFLEX.)
endif
SRC += GenerateShape/reflexdict_GenerateShape
ADDDEPS += ${DEPDIR}/GenerateShape/reflexdict_GenerateShape.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,GenerateShape/%,$(PROGRAMS))

GenerateShape_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/GenerateShape/%.bin
	cp $^ $@

$(BINDIR)/%: GenerateShape/%
	cp $^ $@

$(BINDIR)/%: GenerateShape/bin/%
	cp $^ $@

define GenerateShape_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/GenerateShape/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(GenerateShape_PROGDIRS),$(eval $(call GenerateShape_PROGRAM_template,$(sd))))

GenerateShape/%SK.cc GenerateShape/%.hh: GenerateShape/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CGenerateShape -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := JeanFrancois

# including the module.mk file
-include $(patsubst %, %/module.mk,JeanFrancois)
-include $(patsubst %, %/pfgct/module.mk,JeanFrancois)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst JeanFrancois/%,%,$(basename $(wildcard $(patsubst %,JeanFrancois/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,JeanFrancois/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort JeanFrancois/ $(dir $(patsubst %,JeanFrancois/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_JeanFrancois := $(foreach i, $(patsubst %,JeanFrancois/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_JeanFrancois),)
  SRC += JeanFrancois/dict_JeanFrancois
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,JeanFrancois/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_JeanFrancois := $(foreach i, $(patsubst %,JeanFrancois/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_JeanFrancois := $(filter %selection.xml,$(REFLEXH_JeanFrancois))
REFLEXH_JeanFrancois := $(filter-out %selection.xml,$(REFLEXH_JeanFrancois))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_JeanFrancois),)
ifeq ($(SELECTIONXML_JeanFrancois),)
$(error No selection.xml file specified in the REFLEXFILES of module JeanFrancois)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module JeanFrancois : common dictionary building is not implemented for REFLEX.)
endif
SRC += JeanFrancois/reflexdict_JeanFrancois
ADDDEPS += ${DEPDIR}/JeanFrancois/reflexdict_JeanFrancois.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,JeanFrancois/%,$(PROGRAMS))

JeanFrancois_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/JeanFrancois/%.bin
	cp $^ $@

$(BINDIR)/%: JeanFrancois/%
	cp $^ $@

$(BINDIR)/%: JeanFrancois/bin/%
	cp $^ $@

define JeanFrancois_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/JeanFrancois/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(JeanFrancois_PROGDIRS),$(eval $(call JeanFrancois_PROGRAM_template,$(sd))))

JeanFrancois/%SK.cc JeanFrancois/%.hh: JeanFrancois/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CJeanFrancois -bcxx $^

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

THISMODULE := Procedures

# including the module.mk file
-include $(patsubst %, %/module.mk,Procedures)
-include $(patsubst %, %/pfgct/module.mk,Procedures)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst Procedures/%,%,$(basename $(wildcard $(patsubst %,Procedures/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,Procedures/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort Procedures/ $(dir $(patsubst %,Procedures/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_Procedures := $(foreach i, $(patsubst %,Procedures/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_Procedures),)
  SRC += Procedures/dict_Procedures
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,Procedures/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_Procedures := $(foreach i, $(patsubst %,Procedures/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_Procedures := $(filter %selection.xml,$(REFLEXH_Procedures))
REFLEXH_Procedures := $(filter-out %selection.xml,$(REFLEXH_Procedures))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_Procedures),)
ifeq ($(SELECTIONXML_Procedures),)
$(error No selection.xml file specified in the REFLEXFILES of module Procedures)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module Procedures : common dictionary building is not implemented for REFLEX.)
endif
SRC += Procedures/reflexdict_Procedures
ADDDEPS += ${DEPDIR}/Procedures/reflexdict_Procedures.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,Procedures/%,$(PROGRAMS))

Procedures_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/Procedures/%.bin
	cp $^ $@

$(BINDIR)/%: Procedures/%
	cp $^ $@

$(BINDIR)/%: Procedures/bin/%
	cp $^ $@

define Procedures_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/Procedures/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(Procedures_PROGDIRS),$(eval $(call Procedures_PROGRAM_template,$(sd))))

Procedures/%SK.cc Procedures/%.hh: Procedures/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CProcedures -bcxx $^

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


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := SignalShape

# including the module.mk file
-include $(patsubst %, %/module.mk,SignalShape)
-include $(patsubst %, %/pfgct/module.mk,SignalShape)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst SignalShape/%,%,$(basename $(wildcard $(patsubst %,SignalShape/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,SignalShape/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort SignalShape/ $(dir $(patsubst %,SignalShape/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_SignalShape := $(foreach i, $(patsubst %,SignalShape/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_SignalShape),)
  SRC += SignalShape/dict_SignalShape
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,SignalShape/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_SignalShape := $(foreach i, $(patsubst %,SignalShape/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_SignalShape := $(filter %selection.xml,$(REFLEXH_SignalShape))
REFLEXH_SignalShape := $(filter-out %selection.xml,$(REFLEXH_SignalShape))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_SignalShape),)
ifeq ($(SELECTIONXML_SignalShape),)
$(error No selection.xml file specified in the REFLEXFILES of module SignalShape)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module SignalShape : common dictionary building is not implemented for REFLEX.)
endif
SRC += SignalShape/reflexdict_SignalShape
ADDDEPS += ${DEPDIR}/SignalShape/reflexdict_SignalShape.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,SignalShape/%,$(PROGRAMS))

SignalShape_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/SignalShape/%.bin
	cp $^ $@

$(BINDIR)/%: SignalShape/%
	cp $^ $@

$(BINDIR)/%: SignalShape/bin/%
	cp $^ $@

define SignalShape_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/SignalShape/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(SignalShape_PROGDIRS),$(eval $(call SignalShape_PROGRAM_template,$(sd))))

SignalShape/%SK.cc SignalShape/%.hh: SignalShape/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CSignalShape -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := Simulation

# including the module.mk file
-include $(patsubst %, %/module.mk,Simulation)
-include $(patsubst %, %/pfgct/module.mk,Simulation)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst Simulation/%,%,$(basename $(wildcard $(patsubst %,Simulation/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,Simulation/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort Simulation/ $(dir $(patsubst %,Simulation/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_Simulation := $(foreach i, $(patsubst %,Simulation/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_Simulation),)
  SRC += Simulation/dict_Simulation
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,Simulation/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_Simulation := $(foreach i, $(patsubst %,Simulation/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_Simulation := $(filter %selection.xml,$(REFLEXH_Simulation))
REFLEXH_Simulation := $(filter-out %selection.xml,$(REFLEXH_Simulation))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_Simulation),)
ifeq ($(SELECTIONXML_Simulation),)
$(error No selection.xml file specified in the REFLEXFILES of module Simulation)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module Simulation : common dictionary building is not implemented for REFLEX.)
endif
SRC += Simulation/reflexdict_Simulation
ADDDEPS += ${DEPDIR}/Simulation/reflexdict_Simulation.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,Simulation/%,$(PROGRAMS))

Simulation_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/Simulation/%.bin
	cp $^ $@

$(BINDIR)/%: Simulation/%
	cp $^ $@

$(BINDIR)/%: Simulation/bin/%
	cp $^ $@

define Simulation_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/Simulation/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(Simulation_PROGDIRS),$(eval $(call Simulation_PROGRAM_template,$(sd))))

Simulation/%SK.cc Simulation/%.hh: Simulation/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CSimulation -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := Studies

# including the module.mk file
-include $(patsubst %, %/module.mk,Studies)
-include $(patsubst %, %/pfgct/module.mk,Studies)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst Studies/%,%,$(basename $(wildcard $(patsubst %,Studies/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,Studies/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort Studies/ $(dir $(patsubst %,Studies/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_Studies := $(foreach i, $(patsubst %,Studies/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_Studies),)
  SRC += Studies/dict_Studies
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,Studies/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_Studies := $(foreach i, $(patsubst %,Studies/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_Studies := $(filter %selection.xml,$(REFLEXH_Studies))
REFLEXH_Studies := $(filter-out %selection.xml,$(REFLEXH_Studies))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_Studies),)
ifeq ($(SELECTIONXML_Studies),)
$(error No selection.xml file specified in the REFLEXFILES of module Studies)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module Studies : common dictionary building is not implemented for REFLEX.)
endif
SRC += Studies/reflexdict_Studies
ADDDEPS += ${DEPDIR}/Studies/reflexdict_Studies.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,Studies/%,$(PROGRAMS))

Studies_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/Studies/%.bin
	cp $^ $@

$(BINDIR)/%: Studies/%
	cp $^ $@

$(BINDIR)/%: Studies/bin/%
	cp $^ $@

define Studies_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/Studies/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(Studies_PROGDIRS),$(eval $(call Studies_PROGRAM_template,$(sd))))

Studies/%SK.cc Studies/%.hh: Studies/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CStudies -bcxx $^

endif


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := Tristan_DESY21

# including the module.mk file
-include $(patsubst %, %/module.mk,Tristan_DESY21)
-include $(patsubst %, %/pfgct/module.mk,Tristan_DESY21)

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := $(patsubst Tristan_DESY21/%,%,$(basename $(wildcard $(patsubst %,Tristan_DESY21/%,$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,Tristan_DESY21/%,$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := $(sort Tristan_DESY21/ $(dir $(patsubst %,Tristan_DESY21/%,$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += $(patsubst %, -I%, $(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq ($(BUILDCOMMONDICT),no)
  DICTH_Tristan_DESY21 := $(foreach i, $(patsubst %,Tristan_DESY21/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
  # if dict header files exist, append to variable SRC
  ifneq ($(DICTH_Tristan_DESY21),)
  SRC += Tristan_DESY21/dict_Tristan_DESY21
  endif
else
  DICTHEADERS += $(foreach i, $(patsubst %,Tristan_DESY21/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_Tristan_DESY21 := $(foreach i, $(patsubst %,Tristan_DESY21/%,$(REFLEXFILES)), $(wildcard $(i).h) $(wildcard $(i).hh) $(wildcard $(i)))
SELECTIONXML_Tristan_DESY21 := $(filter %selection.xml,$(REFLEXH_Tristan_DESY21))
REFLEXH_Tristan_DESY21 := $(filter-out %selection.xml,$(REFLEXH_Tristan_DESY21))
# if reflex header files exist, append to variable SRC
ifneq ($(REFLEXH_Tristan_DESY21),)
ifeq ($(SELECTIONXML_Tristan_DESY21),)
$(error No selection.xml file specified in the REFLEXFILES of module Tristan_DESY21)
endif
ifneq ($(BUILDCOMMONDICT),no)
$(error Module Tristan_DESY21 : common dictionary building is not implemented for REFLEX.)
endif
SRC += Tristan_DESY21/reflexdict_Tristan_DESY21
ADDDEPS += ${DEPDIR}/Tristan_DESY21/reflexdict_Tristan_DESY21.gendict.d
endif

PROG += $(patsubst %,$(BINDIR)/%,$(notdir $(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,Tristan_DESY21/%,$(PROGRAMS))

Tristan_DESY21_PROGDIRS := $(sort $(dir $(PROGRAMS)))

# a couple of rules to copy executable files correctly
$(BINDIR)/%: ${OBJDIR}/Tristan_DESY21/%.bin
	cp $^ $@

$(BINDIR)/%: Tristan_DESY21/%
	cp $^ $@

$(BINDIR)/%: Tristan_DESY21/bin/%
	cp $^ $@

define Tristan_DESY21_PROGRAM_template
$(BINDIR)/%: ${OBJDIR}/Tristan_DESY21/$(1)/%.bin
	cp $$^ $$@
endef

$(foreach sd,$(Tristan_DESY21_PROGDIRS),$(eval $(call Tristan_DESY21_PROGRAM_template,$(sd))))

Tristan_DESY21/%SK.cc Tristan_DESY21/%.hh: Tristan_DESY21/idl/%.idl
	omniidl $(OMNICORBAFLAGS) -CTristan_DESY21 -bcxx $^

endif

