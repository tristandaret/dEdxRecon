#include "dEdx.h"
#include "LUTs.h"
#include "ReconTools.h"
#include "Variables.h"

#include "Util.h"

#include <cmath>
#include <numeric>
#include <fstream>
#include <string>
#include <random>

#include "PRFParameters.h"
#include "TrackRecon.h"
#include "ClusterFitter.h"
#include "TrackFitter.h"
#include "Displays.h"
#include "GiveMe_Uploader.h"
#include "Selector.h"

ClassImp(Reconstruction::RecoPad)
ClassImp(Reconstruction::RecoCluster);
ClassImp(Reconstruction::RecoModule)
ClassImp(Reconstruction::RecoEvent)

Reconstruction::dEdx::dEdx(){
}

Reconstruction::dEdx::~dEdx(){
}

void Reconstruction::dEdx::Reconstruction(){

	// Make directories
	std::string OUTDirName =                outDir + tag + "/";
	MakeMyDir(OUTDirName);
	std::string OUTDIR_Display =            OUTDirName + "Evt_Display/";
	MakeMyDir(OUTDIR_Display);
	std::string OUTDIR_WF_Display =         OUTDirName + "WF_Display/";
	MakeMyDir(OUTDIR_WF_Display);

	// Redirect Output
	std::string     logs = OUTDirName + "dEdx_" + tag + ".log";
	std::cout <<    "logs: " << logs		<< std::endl;
	std::cout <<    std::setprecision(2)	<< std::fixed;
	std::cout <<    std::endl;
	std::streambuf* coutbuf = std::cout.rdbuf();	// Save old buf
	std::ofstream   OUT_DataFile(logs.c_str() );	// Set output file
	std::cout.      rdbuf(OUT_DataFile.rdbuf());	// Redirect to output file

	// Log info
	std::cout << "dataFile         : " << dataFile      << std::endl;
	std::cout << "OUTDirName       : " << OUTDirName    << std::endl;
	std::cout << "tag              : " << tag           << std::endl;
	std::cout << "comment          : " << comment       << std::endl;
	std::cout << "selectionSet     : " << selectionSet  << std::endl;
	std::cout << "prtcle           : " << prtcle        << std::endl;
	std::cout << "driftMethod      : " << driftMethod	<< std::endl;
	std::cout << "WF updated       : " << WFupdated		<< std::endl;
	std::cout << "Gain correction  : " << fcorrectGain	<< std::endl;
	std::cout << "RC correction    : " << fcorrectRC	<< std::endl;
	std::cout <<                                           std::endl;

	// Get ERAM maps
	std::vector<int> fERAMs_iD;
	std::vector<int> fERAMs_pos;
	if(tag.find("DESY") != std::string::npos)         {fERAMs_iD.push_back(1);  fERAMs_pos.push_back(12);}
	if(dataFile.find("ERAM18") != std::string::npos)  {fERAMs_iD.push_back(18); fERAMs_pos.push_back(33);}
	if(dataFile.find("All_ERAMS") != std::string::npos){fERAMs_iD =  {7, 1, 23, 2, 16, 15, 10, 12}; fERAMs_pos =  {26, 12, 10, 7, 17, 19, 13, 32};}
	Reconstruction::ERAMMaps *pERAMMaps =  new Reconstruction::ERAMMaps();

	// Diagonal clustering?
	bool diag = false;
	if(tag.find("diag") != std::string::npos) diag = true;

	// Handle theta case
	float costheta =     1;
	int theta_arr[3] =   {-45, -20, 20};
	for (int theta_file : theta_arr) if(tag.find("theta") != std::string::npos and tag.find(std::to_string(theta_file)) != std::string::npos) costheta =  fabs(cos((float)theta_file/180*M_PI));


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Output
	TFile *pFile_dEdx =    						new TFile(outFile_dEdx.c_str(), "RECREATE");
	TTree *pTree_dEdx =    						new TTree("dEdx_tree", "dEdx TTree");
	Reconstruction::RecoEvent *p_tevent =  		new Reconstruction::RecoEvent();
	pTree_dEdx->								Branch("event_branch", "Reconstruction::RecoEvent", &p_tevent);

	// Selection stage
	Selector aSelector(selectionSet);
	int NEvent =  								p_uploader->Get_NberOfEvent();
	Init_selection(selectionSet, aSelector, tag, p_uploader, moduleCase, 0);
	aSelector.Tell_Selection();

	// Log info
	std::cout << "Number of entries :" 	<< NEvent << std::endl;
	std::cout << "drift distance:    " 	<< driftDist       << " mm"        << std::endl;
	std::cout << "Peaking time:      " 	<< PT              << " ns"        << std::endl;
	std::cout << "Time bin length:   " 	<< TB              << " ns"        << std::endl;
	std::cout << "PT/TB =            " 	<< PT/TB           << " time bins" << std::endl;
	std::cout << "drift method:      " 	<< driftMethod                     << std::endl;
	std::cout << "minimal length:    " 	<< fminLength*1e3  << " mm"        << std::endl;
	std::cout <<                                                              std::endl;

	// Correction function for WF
	TF1 *pcorrFunctionWF =              			corr_func(dataFile, tag, WFupdated);
	float fAref =                        			pcorrFunctionWF->Eval(XPADLENGTH);

	// Track fit initializations
	TrackFitter aTrackFitter("Minuit", fnParamsTrack);
	PRFParameters aPRFParameters;
	TF1 *ptf1PRF =                      			new TF1("ptf1PRF",aPRFParameters, -2.5*1.128, 2.5*1.128, 5);
	ptf1PRF->                     					SetParameters(p_uploader->Get_Norm(), p_uploader->Get_a2(), p_uploader->Get_a4(), p_uploader->Get_b2(), p_uploader->Get_b4());
	int fcounterFail =                 				0;
	int fcounterFit =                  				0;

	// Initialization of analysis vectors and histograms
	// Modules
	std::vector<float> 	v_mod_dx;
	std::vector<float> 	v_mod_dE;
	std::vector<float> 	v_mod_dEdxXP;
	std::vector<float> 	v_mod_dEdxWF;
	// Events
	std::vector<float> 	v_evt_dx;
	std::vector<float> 	v_evt_dE;
	std::vector<float> 	v_evt_dEdxXP;
	std::vector<float> 	v_evt_dEdxWF;
	std::vector<double> v_erams;
	// Waveforms
	std::vector<int>   	waveform_cluster(510, 0);
	std::vector<int>   	waveform_pad(510, 0);
	// dE/dx quick access histograms
	TH1F *ph1f_WF =  								new TH1F("ph1f_WF", "<dE/dx> with WF;<dE/dx> (ADC count);Number of events", 100, 0, 1300);
	TH1F *ph1f_XP =  								new TH1F("ph1f_XP", "<dE/dx> with XP;<dE/dx> (ADC count);Number of events", 100, 0, 1300);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Compute dE/dx
	aSelector.                						Reset_StatCounters();
	std::cout << "Processing events:" << std::endl;


	// Event loop
	for (int iEvent =  0; iEvent < NEvent; iEvent++){
		if(iEvent % 1000 ==  0 or iEvent ==  NEvent-1) std::cout << iEvent << "/" << NEvent << std::endl;

		Event* pEvent =                     		p_uploader->GiveMe_Event(iEvent, moduleCase, 0, 0);
		if (!pEvent)                				continue;

		aSelector.              					ApplySelection(pEvent);
		if (pEvent->IsValid() != 1) 				continue;

		p_tevent->eventNbr = 			 			iEvent;
		p_tevent->numberOfModules =           		pEvent->Get_NberOfModule();

		float fmodID;


		// Module loop
		for (int iMod =  0; iMod < p_tevent->numberOfModules; iMod++){
			Module* pModule =          				pEvent->Get_Module_InArray(iMod);
			fmodID =                  				pModule->Get_ModuleNber();
			if (pEvent->Validity_ForThisModule(fmodID) ==  0) continue;

			int NC =                   				pModule->Get_NberOfCluster();
			if(NC ==  0) 							continue;

			Reconstruction::RecoModule *p_tmodule =  	new Reconstruction::RecoModule();
			p_tmodule->position =         			fmodID;
			p_tmodule->ID =               			fERAMs_iD[fmodID];

			// Track fitting
			if(!diag){
				ClusterFitter_Horizontal aClusterFitter_Horizontal("Minuit");
				ClusterFit_Horizontal_Event(pEvent, fmodID, ptf1PRF, aClusterFitter_Horizontal);
			}
			else{
				ClusterFitter_Diagonal aClusterFitter_Diagonal("Minuit");
				ClusterFit_Diagonal_Event(-(M_PI_2-(PHIMAX*M_PI/180)), pEvent, fmodID, ptf1PRF, fcounterFit, fcounterFail, aClusterFitter_Diagonal);
			}
			TrackFitter aTrackFitter("Minuit", fnParamsTrack);
			TrackRecon_Event(aTrackFitter, pEvent, fmodID, fnParamsTrack);

			// Track details
			const Track* pTrack =      				pEvent->GiveMe_Track_ForThisModule(fmodID);
			p_tmodule->phi =              			std::atan(pTrack->Get_ParameterValue(1))/M_PI*180;
			p_tmodule->Track->						SetParameters(pTrack->Get_ParameterValue(0), pTrack->Get_ParameterValue(1));
			Double_t parErrors[2] = 				{pTrack->Get_ParameterError(0), pTrack->Get_ParameterError(1)};
			p_tmodule->Track->						SetParErrors(parErrors);
			if(fnParamsTrack ==  3){
				p_tmodule->Track->					SetParameter(2, pTrack->Get_ParameterValue(2));
				p_tmodule->Track->					SetParError(2, pTrack->Get_ParameterError(2));
			}


			// Loop On Clusters
			for (int iC =  0; iC < NC; iC++){
				std::fill(waveform_cluster.begin(), waveform_cluster.end(), 0); // reset waveform
				Cluster* pCluster =            		pModule->Get_Cluster(iC);
				Reconstruction::RecoCluster *p_tcluster = new Reconstruction::RecoCluster();
				float fGainCorrectionLead = 0;


				// Loop On Pads
				int NPads =                   		pCluster->Get_NberOfPads();
				for(int iP =  0; iP < NPads; iP ++){
					std::fill(waveform_pad.begin(), waveform_pad.end(), 0); // reset waveform
					const Pad* pPad =            	pCluster->Get_Pad(iP);
					Reconstruction::RecoPad * p_tpad = new Reconstruction::RecoPad();
					waveform_pad = 					pPad->Get_vADC();
					p_tpad->ix =                    pPad->Get_iX();
					p_tpad->iy =                    pPad->Get_iY();

					// RC correction
					if(fcorrectRC) p_tpad->RC =		pERAMMaps->RC(fERAMs_pos[fmodID], p_tpad->ix, p_tpad->iy);
					else           p_tpad->RC =		120;

					// Gain correction
					if(fcorrectGain){
						p_tpad->gain =              pERAMMaps->Gain(fERAMs_pos[fmodID], p_tpad->ix, p_tpad->iy);
						p_tpad->GainCorrection =    AVG_GAIN/p_tpad->gain;
						p_tpad->ADCmax =            p_tpad->GainCorrection*pPad->Get_AMax();
						for(int i=0;i<510;i++) waveform_pad[i] = round(waveform_pad[i] * p_tpad->GainCorrection);
						if(iP == 0) fGainCorrectionLead = p_tpad->GainCorrection;		
					}
					else p_tpad->ADCmax =              pPad->Get_AMax();
					for(int i=0;i<510;i++) waveform_cluster[i] += waveform_pad[i];
					
					// Track computations
					local_params(pPad, pTrack, p_tpad->d, p_tpad->dd, p_tpad->phi, p_tpad->length); // impact parameter and length in pad in m, angle in degrees
					p_tpad->d *= 					1000; // in mm
					p_tpad->dd *= 					1000; // in mm
					p_tpad->length /= 				costheta;
					p_tcluster->length += 			p_tpad->length;

					// Minimum length in pad cutoff to avoid fluctuations from small segments
					if(p_tpad->length*costheta <= fminLength){ // cutoff defined in the ERAM's plane -> remove theta dependence
						p_tcluster->				v_pads.push_back(p_tpad);	
						continue;
					}

					// Compute drift distance for different time bin sizes and peaking times
					p_tpad->TMax =              	pPad->Get_TMax();
					if      (PT ==  412 and TB ==  50){ p_tpad->T0 =  45; p_tpad->driftDistance =  3.90*(p_tpad->TMax-p_tpad->T0); } // 45 =  37(time shift) +  8(PT) from David Attié
					else if (PT ==  412 and TB ==  40){ p_tpad->T0 =  56; p_tpad->driftDistance =  3.12*(p_tpad->TMax-p_tpad->T0); } // 56 =  46(time shift) + 10(PT)
					else if (PT ==  200 and TB ==  50){ p_tpad->T0 =  39; p_tpad->driftDistance =  3.90*(p_tpad->TMax-p_tpad->T0); } // 39 =  35(time shift) +  4(PT) own computation
					else if (PT ==  200 and TB ==  40){ p_tpad->T0 =  48; p_tpad->driftDistance =  3.12*(p_tpad->TMax-p_tpad->T0); } // 48 =  44(time shift) +  5(PT)
					
					// Get the LUT ratio 
					p_tpad->ratioDrift =        	p_lut->getRatio(fabs(p_tpad->phi), fabs(p_tpad->d), p_tpad->RC, p_tpad->driftDistance);
					p_tpad->ratioFile =         	p_lut->getRatio(fabs(p_tpad->phi), fabs(p_tpad->d), p_tpad->RC, driftDist);

					if(driftMethod ==  "zcalc") p_tpad->ratio = p_tpad->ratioDrift;
					if(driftMethod ==  "zfile") p_tpad->ratio = p_tpad->ratioFile;

					p_tpad->charge =             	p_tpad->ADCmax*p_tpad->ratio;
					p_tpad->dEdxXP =             	p_tpad->charge/p_tpad->length/100; // m to cm
					v_mod_dE.                   	push_back(p_tpad->charge);
					v_mod_dx.                   	push_back(p_tpad->length);
					v_mod_dEdxXP.               	push_back(p_tpad->dEdxXP);

					p_tcluster->v_pads.				push_back(p_tpad);
					p_tmodule->NCrossedPads++;
					p_tmodule->lengthXP += 			p_tpad->length;

				} // Pads

				// // Gain correction for the whole cluster (doesn't work really well)
				// if(fcorrectGain) for(int i=0;i<510;i++) waveform_cluster[i] *= fGainCorrectionLead;

				// WF correction function
				p_tcluster->charge =           		*std::max_element(waveform_cluster.begin(), waveform_cluster.end());
				p_tcluster->ratioCorr =             fAref / pcorrFunctionWF->Eval(p_tcluster->length*1000);
				if(diag) p_tcluster->dEdxWF = 		p_tcluster->charge*p_tcluster->ratioCorr/XPADLENGTH*10;
				else p_tcluster->dEdxWF = 			p_tcluster->charge/(p_tcluster->length*100);
				v_mod_dEdxWF.						push_back(p_tcluster->dEdxWF);

				// Fill cluster information
				p_tmodule->v_clusters.				push_back(p_tcluster);
				p_tmodule->NClusters++;
				p_tmodule->lengthWF += 				p_tcluster->length;
			} // Clusters

			// Compute dE/dx for the module
			p_tmodule->dEdxWF =                 	ComputedEdxWF(v_mod_dEdxWF, p_tmodule->NClusters, falpha);
			p_tmodule->dEdxXP =                 	ComputedEdxXP(v_mod_dEdxXP, v_mod_dE, v_mod_dx, p_tmodule->NCrossedPads, falpha);
			p_tmodule->dEdxWFnoTrunc =          	ComputedEdxWF(v_mod_dEdxWF, p_tmodule->NClusters, 1);
			p_tmodule->dEdxXPnoTrunc =          	ComputedEdxXP(v_mod_dEdxXP, v_mod_dE, v_mod_dx, p_tmodule->NCrossedPads, 1);

			// Fill module information into the event class
			p_tevent->v_modules.					push_back(p_tmodule);
			p_tevent->NCrossedPads += 				p_tmodule->NCrossedPads;
			p_tevent->NClusters += 					p_tmodule->NClusters;
			p_tevent->lengthXP += 					p_tmodule->lengthXP;
			p_tevent->lengthWF += 					p_tmodule->lengthWF;
			v_evt_dE.                       		insert(v_evt_dE.end(), v_mod_dE.begin(), v_mod_dE.end());
			v_evt_dx.                       		insert(v_evt_dx.end(), v_mod_dx.begin(), v_mod_dx.end());
			v_evt_dEdxXP.                   		insert(v_evt_dEdxXP.end(), v_mod_dEdxXP.begin(), v_mod_dEdxXP.end());
			v_evt_dEdxWF.                   		insert(v_evt_dEdxWF.end(), v_mod_dEdxWF.begin(), v_mod_dEdxWF.end());

			// clear module vectors
			v_mod_dE.                       		clear();
			v_mod_dx.                       		clear();
			v_mod_dEdxXP.                   		clear();
			v_mod_dEdxWF.                   		clear();
		} // Modules

		// Compute dE/dx for the event
		p_tevent->dEdxWF =                  		ComputedEdxWF(v_evt_dEdxWF, p_tevent->NClusters, falpha);
		p_tevent->dEdxXP =                  		ComputedEdxXP(v_evt_dEdxXP, v_evt_dE, v_evt_dx, p_tevent->NCrossedPads, falpha);
		p_tevent->dEdxWFnoTrunc =           		ComputedEdxWF(v_evt_dEdxWF, p_tevent->NClusters, 1);
		p_tevent->dEdxXPnoTrunc =           		ComputedEdxXP(v_evt_dEdxXP, v_evt_dE, v_evt_dx, p_tevent->NCrossedPads, 1);

		// Make the quick access histograms
		ph1f_WF->                       			Fill(p_tevent->dEdxWF);
		ph1f_XP->                       			Fill(p_tevent->dEdxXP);

		// Fill the tree
		pTree_dEdx->                				Fill();

		// Clear event vectors
		v_erams.                    				clear();
		v_evt_dE.                       			clear();
		v_evt_dx.                       			clear();
		v_evt_dEdxXP.                   			clear();
		v_evt_dEdxWF.                   			clear();
		p_tevent->									Clear();
		delete 										pEvent;
	} // Events

	aSelector.PrintStat();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Historam fitting
	std::cout << "Fitting dE/dx histograms... "; 
	TF1 *ptf1_WF = 									Fit1Gauss(ph1f_WF, 2);
	TF1 *ptf1_XP = 									Fit1Gauss(ph1f_XP, 2);
	ptf1_WF->										SetNameTitle("ptf1_WF", "ptf1_WF");
	ptf1_XP->										SetNameTitle("ptf1_XP", "ptf1_XP");
	std::cout << "done!" << std::endl; 

	// Save histograms and fits
	ptf1_WF->                     					Write();
	ptf1_XP->                     					Write();
	ph1f_WF->                     					Write();
	ph1f_XP->                     					Write();
	pTree_dEdx->    								Write();
	pFile_dEdx->                  					Close();

	// Deleting
	delete 											pFile_dEdx;
	delete 											pcorrFunctionWF;
	delete 											pERAMMaps;
	delete 											ptf1PRF;
	std::cout.rdbuf(coutbuf); // Reset to standard output
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* dE/dx RECONSTRUCTION ALGORITHMS */

float Reconstruction::dEdx::ComputedEdxWF(std::vector<float> v_dEdxWF, const int& NClusters, const float& alpha){
    float NClustersTrunc =                         int(floor(NClusters*alpha));
    std::sort(v_dEdxWF.begin(), v_dEdxWF.end());
    return std::accumulate(v_dEdxWF.begin(), v_dEdxWF.begin() + NClustersTrunc, 0.) / NClustersTrunc;
}



float Reconstruction::dEdx::ComputedEdxXP(const std::vector<float>& v_dEdx, const std::vector<float>& v_dE, const std::vector<float>& v_dx, const int& nCrossedPads, const float& alpha){
    /// dEdx =  sum(dE)/Sum(dx) and not average(dE/dx) of each pad = > less sensitive to statistical fluctuations
    float NCrossedTrunc =                         int(floor(nCrossedPads*alpha));
	float DE = 0, DX = 0;

    // Few steps to order v_dE & v_dx similarly to v_dEdx
    std::vector<int> indices(v_dEdx.size());
    std::iota(indices.begin(), indices.end(), 0); // make list from 0 to v_dEdx.size()

    std::sort(indices.begin(), indices.end(), [&](int i, int j){ // sorting wrt v_dEdx
        return v_dEdx[i] < v_dEdx[j];
    });
    for (int i =  0; i < NCrossedTrunc; ++i) {
        DE +=  v_dE[indices[i]];
        DX +=  v_dx[indices[i]]*100; // m->cm normalization
    }
    return DE/DX;
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* ANALYSIS CLASSES DEFINITION */

Reconstruction::RecoPad::~RecoPad(){
	// v_waveform.clear();
}


Reconstruction::RecoCluster::~RecoCluster(){
	// v_waveform.clear();
	for (auto p_tpad : v_pads) delete p_tpad;
	v_pads.clear();
}


Reconstruction::RecoModule::~RecoModule(){
	delete Track;
	for (auto p_tcluster : v_clusters) delete p_tcluster;
	v_clusters.clear();
}

Reconstruction::RecoEvent::RecoEvent(){
}
Reconstruction::RecoEvent::~RecoEvent(){
	for (auto p_tmodule : v_modules) delete p_tmodule;
	v_modules.clear();
}

void Reconstruction::RecoEvent::Clear(){
	for (auto p_tmodule : v_modules) delete p_tmodule;
	v_modules.clear();
	eventNbr =  0;
	NCrossedPads =  0;
	NClusters =  0;
	dEdxXP =  0;
	dEdxWF =  0;
	lengthXP =  0;
	lengthWF =  0;
	numberOfModules =  0;
}