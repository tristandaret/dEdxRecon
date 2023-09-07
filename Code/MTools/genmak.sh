#!/bin/bash

#
# This script is called from the main Makefile to generate makefile
# bits for each of the modules (those of the subdirectories that
# contain a config.mk file). Those are the files gen.mk and dictgen.mk
# (rules for the ROOT dictionary generation).
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
# Author: P.F. Giraud
#

modules=$*

function makegen
{
    mod=$1
    mkdir -p .gen
    cat >> .gen/gen.mk <<EOF

# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=
REFLEXFILES :=
USEWILDCARD := 
PROGRAMS :=
SKIP_THISMODULE :=

THISMODULE := ${mod}

# including the module.mk file
-include \$(patsubst %, %/module.mk,${mod})
-include \$(patsubst %, %/pfgct/module.mk,${mod})

ifndef SKIP_THISMODULE

ifdef USEWILDCARD
  FILES := \$(patsubst ${mod}/%,%,\$(basename \$(wildcard \$(patsubst %,${mod}/%,\$(FILES)))))
endif

# appending the values found in FILES to the variable SRC
SRC += \$(patsubst %,${mod}/%,\$(FILES))

# all subdirectories for this given set of FILES
SUBDIRS := \$(sort ${mod}/ \$(dir \$(patsubst %,${mod}/%,\$(FILES))))

# put these subdirectories in the INCLUDEFLAGS
INCLUDEFLAGS += \$(patsubst %, -I%, \$(SUBDIRS))

## appending the values found in DICTFILES to DICTHEADERS or DICTH_modulename
ifeq (\$(BUILDCOMMONDICT),no)
  DICTH_${mod} := \$(foreach i, \$(patsubst %,${mod}/%,\$(DICTFILES)), \$(wildcard \$(i).h) \$(wildcard \$(i).hh) \$(wildcard \$(i)))
  # if dict header files exist, append to variable SRC
  ifneq (\$(DICTH_${mod}),)
  SRC += ${mod}/dict_${mod}
  endif
else
  DICTHEADERS += \$(foreach i, \$(patsubst %,${mod}/%,\$(DICTFILES)), \$(wildcard \$(i).h) \$(wildcard \$(i).hh) \$(wildcard \$(i)))
endif

# appending the values found in REFLEXFILES to REFLEXH_modulename
REFLEXH_${mod} := \$(foreach i, \$(patsubst %,${mod}/%,\$(REFLEXFILES)), \$(wildcard \$(i).h) \$(wildcard \$(i).hh) \$(wildcard \$(i)))
SELECTIONXML_${mod} := \$(filter %selection.xml,\$(REFLEXH_${mod}))
REFLEXH_${mod} := \$(filter-out %selection.xml,\$(REFLEXH_${mod}))
# if reflex header files exist, append to variable SRC
ifneq (\$(REFLEXH_${mod}),)
ifeq (\$(SELECTIONXML_${mod}),)
\$(error No selection.xml file specified in the REFLEXFILES of module ${mod})
endif
ifneq (\$(BUILDCOMMONDICT),no)
\$(error Module ${mod} : common dictionary building is not implemented for REFLEX.)
endif
SRC += ${mod}/reflexdict_${mod}
ADDDEPS += \${DEPDIR}/${mod}/reflexdict_${mod}.gendict.d
endif

PROG += \$(patsubst %,\$(BINDIR)/%,\$(notdir \$(PROGRAMS)))

# appending the values found in FILES to the variable SRC
PROGSRC += \$(patsubst %,${mod}/%,\$(PROGRAMS))

${mod}_PROGDIRS := \$(sort \$(dir \$(PROGRAMS)))

# a couple of rules to copy executable files correctly
\$(BINDIR)/%: \${OBJDIR}/${mod}/%.bin
	cp \$^ \$@

\$(BINDIR)/%: ${mod}/%
	cp \$^ \$@

\$(BINDIR)/%: ${mod}/bin/%
	cp \$^ \$@

define ${mod}_PROGRAM_template
\$(BINDIR)/%: \${OBJDIR}/${mod}/\$(1)/%.bin
	cp \$\$^ \$\$@
endef

\$(foreach sd,\$(${mod}_PROGDIRS),\$(eval \$(call ${mod}_PROGRAM_template,\$(sd))))

${mod}/%SK.cc ${mod}/%.hh: ${mod}/idl/%.idl
	omniidl \$(OMNICORBAFLAGS) -C${mod} -bcxx \$^

endif

EOF
}

function makedictgen
{
    mod=$1
    include_guard=`echo ${mod}/reflexdict_${mod}.hh | tr '[a-z]/. ' '[A-Z]___'`
    mkdir -p .gen
    cat >> .gen/dictgen.mk <<EOF
${mod}/dict_${mod}.cc: \$(DICTH_${mod})
	@echo "Generating dictionary \$@..."
	@\$(ROOTCINT) -f \$@ -c -p -multiDict -s \$(SOLIBD) \$(filter -I% -D%,\$(CXXFLAGS)) \$^
${mod}/reflexdict_${mod}.hh: \$(REFLEXH_${mod})
	@echo "Generating reflex header \$@..."
	@echo "#ifndef ${include_guard}" > \$@
	@echo "#define ${include_guard}" >> \$@
	@for i in \$^; do echo '#include "'\$\${i}'"' >> \$@; done
	@echo "#endif" >> \$@
${mod}/reflexdict_${mod}.cc: ${mod}/reflexdict_${mod}.hh \$(SELECTIONXML_${mod}) \${DEPDIR}/${mod}/reflexdict_${mod}.gendict.d
	@echo "Generating reflex dictionary \$@..."
	@\$(GENREFLEX) \$< --fail_on_warnings --quiet --multiDict -l \$(SOLIBD) -s \$(SELECTIONXML_${mod}) -o \$@ \$(filter -I% -D%,\$(CXXFLAGS))
EOF
}

rm -f .gen/gen.mk .gen/dictgen.mk

for i in $modules; do
    makegen $i
    makedictgen $i
done
