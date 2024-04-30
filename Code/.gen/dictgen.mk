Analysis_JF/dict_Analysis_JF.cc: $(DICTH_Analysis_JF)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
Analysis_JF/reflexdict_Analysis_JF.hh: $(REFLEXH_Analysis_JF)
	@echo "Generating reflex header $@..."
	@echo "#ifndef ANALYSIS_JF_REFLEXDICT_ANALYSIS_JF_HH" > $@
	@echo "#define ANALYSIS_JF_REFLEXDICT_ANALYSIS_JF_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
Analysis_JF/reflexdict_Analysis_JF.cc: Analysis_JF/reflexdict_Analysis_JF.hh $(SELECTIONXML_Analysis_JF) ${DEPDIR}/Analysis_JF/reflexdict_Analysis_JF.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_Analysis_JF) -o $@ $(filter -I% -D%,$(CXXFLAGS))
AnalysisTools/dict_AnalysisTools.cc: $(DICTH_AnalysisTools)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
AnalysisTools/reflexdict_AnalysisTools.hh: $(REFLEXH_AnalysisTools)
	@echo "Generating reflex header $@..."
	@echo "#ifndef ANALYSISTOOLS_REFLEXDICT_ANALYSISTOOLS_HH" > $@
	@echo "#define ANALYSISTOOLS_REFLEXDICT_ANALYSISTOOLS_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
AnalysisTools/reflexdict_AnalysisTools.cc: AnalysisTools/reflexdict_AnalysisTools.hh $(SELECTIONXML_AnalysisTools) ${DEPDIR}/AnalysisTools/reflexdict_AnalysisTools.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_AnalysisTools) -o $@ $(filter -I% -D%,$(CXXFLAGS))
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
Fitters_Cluster/dict_Fitters_Cluster.cc: $(DICTH_Fitters_Cluster)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
Fitters_Cluster/reflexdict_Fitters_Cluster.hh: $(REFLEXH_Fitters_Cluster)
	@echo "Generating reflex header $@..."
	@echo "#ifndef FITTERS_CLUSTER_REFLEXDICT_FITTERS_CLUSTER_HH" > $@
	@echo "#define FITTERS_CLUSTER_REFLEXDICT_FITTERS_CLUSTER_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
Fitters_Cluster/reflexdict_Fitters_Cluster.cc: Fitters_Cluster/reflexdict_Fitters_Cluster.hh $(SELECTIONXML_Fitters_Cluster) ${DEPDIR}/Fitters_Cluster/reflexdict_Fitters_Cluster.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_Fitters_Cluster) -o $@ $(filter -I% -D%,$(CXXFLAGS))
Fitters_Sample/dict_Fitters_Sample.cc: $(DICTH_Fitters_Sample)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
Fitters_Sample/reflexdict_Fitters_Sample.hh: $(REFLEXH_Fitters_Sample)
	@echo "Generating reflex header $@..."
	@echo "#ifndef FITTERS_SAMPLE_REFLEXDICT_FITTERS_SAMPLE_HH" > $@
	@echo "#define FITTERS_SAMPLE_REFLEXDICT_FITTERS_SAMPLE_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
Fitters_Sample/reflexdict_Fitters_Sample.cc: Fitters_Sample/reflexdict_Fitters_Sample.hh $(SELECTIONXML_Fitters_Sample) ${DEPDIR}/Fitters_Sample/reflexdict_Fitters_Sample.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_Fitters_Sample) -o $@ $(filter -I% -D%,$(CXXFLAGS))
Fitters_Track/dict_Fitters_Track.cc: $(DICTH_Fitters_Track)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
Fitters_Track/reflexdict_Fitters_Track.hh: $(REFLEXH_Fitters_Track)
	@echo "Generating reflex header $@..."
	@echo "#ifndef FITTERS_TRACK_REFLEXDICT_FITTERS_TRACK_HH" > $@
	@echo "#define FITTERS_TRACK_REFLEXDICT_FITTERS_TRACK_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
Fitters_Track/reflexdict_Fitters_Track.cc: Fitters_Track/reflexdict_Fitters_Track.hh $(SELECTIONXML_Fitters_Track) ${DEPDIR}/Fitters_Track/reflexdict_Fitters_Track.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_Fitters_Track) -o $@ $(filter -I% -D%,$(CXXFLAGS))
GenerateShape/dict_GenerateShape.cc: $(DICTH_GenerateShape)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
GenerateShape/reflexdict_GenerateShape.hh: $(REFLEXH_GenerateShape)
	@echo "Generating reflex header $@..."
	@echo "#ifndef GENERATESHAPE_REFLEXDICT_GENERATESHAPE_HH" > $@
	@echo "#define GENERATESHAPE_REFLEXDICT_GENERATESHAPE_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
GenerateShape/reflexdict_GenerateShape.cc: GenerateShape/reflexdict_GenerateShape.hh $(SELECTIONXML_GenerateShape) ${DEPDIR}/GenerateShape/reflexdict_GenerateShape.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_GenerateShape) -o $@ $(filter -I% -D%,$(CXXFLAGS))
JeanFrancois/dict_JeanFrancois.cc: $(DICTH_JeanFrancois)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
JeanFrancois/reflexdict_JeanFrancois.hh: $(REFLEXH_JeanFrancois)
	@echo "Generating reflex header $@..."
	@echo "#ifndef JEANFRANCOIS_REFLEXDICT_JEANFRANCOIS_HH" > $@
	@echo "#define JEANFRANCOIS_REFLEXDICT_JEANFRANCOIS_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
JeanFrancois/reflexdict_JeanFrancois.cc: JeanFrancois/reflexdict_JeanFrancois.hh $(SELECTIONXML_JeanFrancois) ${DEPDIR}/JeanFrancois/reflexdict_JeanFrancois.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_JeanFrancois) -o $@ $(filter -I% -D%,$(CXXFLAGS))
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
Procedures/dict_Procedures.cc: $(DICTH_Procedures)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
Procedures/reflexdict_Procedures.hh: $(REFLEXH_Procedures)
	@echo "Generating reflex header $@..."
	@echo "#ifndef PROCEDURES_REFLEXDICT_PROCEDURES_HH" > $@
	@echo "#define PROCEDURES_REFLEXDICT_PROCEDURES_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
Procedures/reflexdict_Procedures.cc: Procedures/reflexdict_Procedures.hh $(SELECTIONXML_Procedures) ${DEPDIR}/Procedures/reflexdict_Procedures.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_Procedures) -o $@ $(filter -I% -D%,$(CXXFLAGS))
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
SignalShape/dict_SignalShape.cc: $(DICTH_SignalShape)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
SignalShape/reflexdict_SignalShape.hh: $(REFLEXH_SignalShape)
	@echo "Generating reflex header $@..."
	@echo "#ifndef SIGNALSHAPE_REFLEXDICT_SIGNALSHAPE_HH" > $@
	@echo "#define SIGNALSHAPE_REFLEXDICT_SIGNALSHAPE_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
SignalShape/reflexdict_SignalShape.cc: SignalShape/reflexdict_SignalShape.hh $(SELECTIONXML_SignalShape) ${DEPDIR}/SignalShape/reflexdict_SignalShape.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_SignalShape) -o $@ $(filter -I% -D%,$(CXXFLAGS))
Simulation/dict_Simulation.cc: $(DICTH_Simulation)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
Simulation/reflexdict_Simulation.hh: $(REFLEXH_Simulation)
	@echo "Generating reflex header $@..."
	@echo "#ifndef SIMULATION_REFLEXDICT_SIMULATION_HH" > $@
	@echo "#define SIMULATION_REFLEXDICT_SIMULATION_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
Simulation/reflexdict_Simulation.cc: Simulation/reflexdict_Simulation.hh $(SELECTIONXML_Simulation) ${DEPDIR}/Simulation/reflexdict_Simulation.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_Simulation) -o $@ $(filter -I% -D%,$(CXXFLAGS))
Studies/dict_Studies.cc: $(DICTH_Studies)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
Studies/reflexdict_Studies.hh: $(REFLEXH_Studies)
	@echo "Generating reflex header $@..."
	@echo "#ifndef STUDIES_REFLEXDICT_STUDIES_HH" > $@
	@echo "#define STUDIES_REFLEXDICT_STUDIES_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
Studies/reflexdict_Studies.cc: Studies/reflexdict_Studies.hh $(SELECTIONXML_Studies) ${DEPDIR}/Studies/reflexdict_Studies.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_Studies) -o $@ $(filter -I% -D%,$(CXXFLAGS))
Tristan_DESY21/dict_Tristan_DESY21.cc: $(DICTH_Tristan_DESY21)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c -p -multiDict -s $(SOLIBD) $(filter -I% -D%,$(CXXFLAGS)) $^
Tristan_DESY21/reflexdict_Tristan_DESY21.hh: $(REFLEXH_Tristan_DESY21)
	@echo "Generating reflex header $@..."
	@echo "#ifndef TRISTAN_DESY21_REFLEXDICT_TRISTAN_DESY21_HH" > $@
	@echo "#define TRISTAN_DESY21_REFLEXDICT_TRISTAN_DESY21_HH" >> $@
	@for i in $^; do echo '#include "'$${i}'"' >> $@; done
	@echo "#endif" >> $@
Tristan_DESY21/reflexdict_Tristan_DESY21.cc: Tristan_DESY21/reflexdict_Tristan_DESY21.hh $(SELECTIONXML_Tristan_DESY21) ${DEPDIR}/Tristan_DESY21/reflexdict_Tristan_DESY21.gendict.d
	@echo "Generating reflex dictionary $@..."
	@$(GENREFLEX) $< --fail_on_warnings --quiet --multiDict -l $(SOLIBD) -s $(SELECTIONXML_Tristan_DESY21) -o $@ $(filter -I% -D%,$(CXXFLAGS))
