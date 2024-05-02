AOBt/dict_AOBt.cc: $(DICTH_AOBt)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
AOBt/reflexdict_AOBt.hh: $(REFLEXH_AOBt)
	@echo "Generating reflex header $@..."
	@echo "#ifndef AOBT_REFLEXDICT_AOBT_HH" > $@
	@echo "#define AOBT_REFLEXDICT_AOBT_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
AOBt/reflexdict_AOBt.cc: AOBt/reflexdict_AOBt.hh $(SELECTIONXML_AOBt) ${DEPDIR}/AOBt/reflexdict_AOBt.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_AOBt) -o $@ $(filter -I% -D%,$(CXXFLAGS))
EvtModel/dict_EvtModel.cc: $(DICTH_EvtModel)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
EvtModel/reflexdict_EvtModel.hh: $(REFLEXH_EvtModel)
	@echo "Generating reflex header $@..."
	@echo "#ifndef EVTMODEL_REFLEXDICT_EVTMODEL_HH" > $@
	@echo "#define EVTMODEL_REFLEXDICT_EVTMODEL_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
EvtModel/reflexdict_EvtModel.cc: EvtModel/reflexdict_EvtModel.hh $(SELECTIONXML_EvtModel) ${DEPDIR}/EvtModel/reflexdict_EvtModel.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_EvtModel) -o $@ $(filter -I% -D%,$(CXXFLAGS))
EvtModelTools/dict_EvtModelTools.cc: $(DICTH_EvtModelTools)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
EvtModelTools/reflexdict_EvtModelTools.hh: $(REFLEXH_EvtModelTools)
	@echo "Generating reflex header $@..."
	@echo "#ifndef EVTMODELTOOLS_REFLEXDICT_EVTMODELTOOLS_HH" > $@
	@echo "#define EVTMODELTOOLS_REFLEXDICT_EVTMODELTOOLS_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
EvtModelTools/reflexdict_EvtModelTools.cc: EvtModelTools/reflexdict_EvtModelTools.hh $(SELECTIONXML_EvtModelTools) ${DEPDIR}/EvtModelTools/reflexdict_EvtModelTools.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_EvtModelTools) -o $@ $(filter -I% -D%,$(CXXFLAGS))
Fitters/dict_Fitters.cc: $(DICTH_Fitters)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
Fitters/reflexdict_Fitters.hh: $(REFLEXH_Fitters)
	@echo "Generating reflex header $@..."
	@echo "#ifndef FITTERS_REFLEXDICT_FITTERS_HH" > $@
	@echo "#define FITTERS_REFLEXDICT_FITTERS_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
Fitters/reflexdict_Fitters.cc: Fitters/reflexdict_Fitters.hh $(SELECTIONXML_Fitters) ${DEPDIR}/Fitters/reflexdict_Fitters.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_Fitters) -o $@ $(filter -I% -D%,$(CXXFLAGS))
Misc/dict_Misc.cc: $(DICTH_Misc)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
Misc/reflexdict_Misc.hh: $(REFLEXH_Misc)
	@echo "Generating reflex header $@..."
	@echo "#ifndef MISC_REFLEXDICT_MISC_HH" > $@
	@echo "#define MISC_REFLEXDICT_MISC_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
Misc/reflexdict_Misc.cc: Misc/reflexdict_Misc.hh $(SELECTIONXML_Misc) ${DEPDIR}/Misc/reflexdict_Misc.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_Misc) -o $@ $(filter -I% -D%,$(CXXFLAGS))
MTools/dict_MTools.cc: $(DICTH_MTools)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
MTools/reflexdict_MTools.hh: $(REFLEXH_MTools)
	@echo "Generating reflex header $@..."
	@echo "#ifndef MTOOLS_REFLEXDICT_MTOOLS_HH" > $@
	@echo "#define MTOOLS_REFLEXDICT_MTOOLS_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
MTools/reflexdict_MTools.cc: MTools/reflexdict_MTools.hh $(SELECTIONXML_MTools) ${DEPDIR}/MTools/reflexdict_MTools.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_MTools) -o $@ $(filter -I% -D%,$(CXXFLAGS))
PID/dict_PID.cc: $(DICTH_PID)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
PID/reflexdict_PID.hh: $(REFLEXH_PID)
	@echo "Generating reflex header $@..."
	@echo "#ifndef PID_REFLEXDICT_PID_HH" > $@
	@echo "#define PID_REFLEXDICT_PID_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
PID/reflexdict_PID.cc: PID/reflexdict_PID.hh $(SELECTIONXML_PID) ${DEPDIR}/PID/reflexdict_PID.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_PID) -o $@ $(filter -I% -D%,$(CXXFLAGS))
SampleTools/dict_SampleTools.cc: $(DICTH_SampleTools)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
SampleTools/reflexdict_SampleTools.hh: $(REFLEXH_SampleTools)
	@echo "Generating reflex header $@..."
	@echo "#ifndef SAMPLETOOLS_REFLEXDICT_SAMPLETOOLS_HH" > $@
	@echo "#define SAMPLETOOLS_REFLEXDICT_SAMPLETOOLS_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
SampleTools/reflexdict_SampleTools.cc: SampleTools/reflexdict_SampleTools.hh $(SELECTIONXML_SampleTools) ${DEPDIR}/SampleTools/reflexdict_SampleTools.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_SampleTools) -o $@ $(filter -I% -D%,$(CXXFLAGS))
SignalModel/dict_SignalModel.cc: $(DICTH_SignalModel)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
SignalModel/reflexdict_SignalModel.hh: $(REFLEXH_SignalModel)
	@echo "Generating reflex header $@..."
	@echo "#ifndef SIGNALMODEL_REFLEXDICT_SIGNALMODEL_HH" > $@
	@echo "#define SIGNALMODEL_REFLEXDICT_SIGNALMODEL_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
SignalModel/reflexdict_SignalModel.cc: SignalModel/reflexdict_SignalModel.hh $(SELECTIONXML_SignalModel) ${DEPDIR}/SignalModel/reflexdict_SignalModel.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_SignalModel) -o $@ $(filter -I% -D%,$(CXXFLAGS))
