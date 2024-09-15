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

	// Redirect Output
	std::cout <<		std::setprecision(2) << std::fixed;
	std::cout <<		std::endl;
	std::streambuf* 	coutbuf = std::cout.rdbuf();		// Save old buf
	std::ofstream		OUT_DataFile(log_file.c_str() );	// Set output file
	std::cout.			rdbuf(OUT_DataFile.rdbuf());		// Redirect to output file

	// Get ERAM maps
	if(tag.find("DESY") != std::string::npos)						{fERAMs_iD.push_back(1);					fERAMs_pos.push_back(12);}
	if(v_datafiles.back().find("ERAM18") != std::string::npos)		{fERAMs_iD.push_back(18);					fERAMs_pos.push_back(33);}
	if(v_datafiles.back().find("All_ERAMS") != std::string::npos)	{fERAMs_iD = {7, 1, 23, 2, 16, 15, 10, 12};	fERAMs_pos = {26, 12, 10, 7, 17, 19, 13, 32};}
	pERAMMaps =									new Reconstruction::ERAMMaps();

	// Diagonal clustering?
	diag = false;
	if(tag.find("diag") != std::string::npos) diag = true;

	// Handle theta case
	costheta =									1;
	v_theta = 									{-20, 20, 45};
	for (int theta_file : v_theta) if(tag.find("theta") != std::string::npos and tag.find(std::to_string(theta_file)) != std::string::npos) costheta = fabs(cos((float)theta_file/180*M_PI));


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Output
	fpFile_dEdx =								new TFile(rootout_file.c_str(), "RECREATE");
	fpTree_dEdx =								new TTree("dEdx_tree", "dEdx TTree");
	p_recoevent =								new Reconstruction::RecoEvent();
	fpTree_dEdx->								Branch("event_branch", "Reconstruction::RecoEvent", &p_recoevent);

	// Selection stage
	Selector aSelector(selectionSet);
	NEvents =									p_uploader->Get_NberOfEvent();
	Init_selection(selectionSet, aSelector, tag, p_uploader, moduleCase, 0);
	aSelector.									Tell_Selection();

	// Log info
	std::cout << "dataFile:			  " << v_datafiles.back()				<<	std::endl;
	std::cout << "drawout_scanpath:   " << drawout_scanpath					<<	std::endl;
	std::cout << "root output file:   " << rootout_file						<<	std::endl;
	std::cout << "tag:                " << tag								<<	std::endl;
	std::cout << "comment:            " << comment							<<	std::endl;
	std::cout << "selectionSet:       " << selectionSet						<<	std::endl;
	std::cout << "drift method:       " << fcorrectDrift					<<	std::endl;
	std::cout << "WF    correction:   " << fcorrectWF						<<	std::endl;
	std::cout << "Gain  correction:   " << fcorrectGain						<<	std::endl;
	std::cout << "RC    correction:   " << fcorrectRC						<<	std::endl;
	std::cout << "Save only selected: " << fsaveSelectOnly					<<	std::endl;
	std::cout <<																std::endl;
	std::cout << "Number of entries:  "	<< NEvents 							<<	std::endl;
	std::cout << "drift               "	<< driftDist		<< " mm"		<<	std::endl;
	std::cout << "Peaking time:       "	<< PT				<< " ns"		<<	std::endl;
	std::cout << "Time bin length     "	<< TB				<< " ns"		<<	std::endl;
	std::cout << "PT/TB =             "	<< PT/TB			<< " time bins"	<<	std::endl;
	std::cout << "drift method:       "	<< fcorrectDrift					<<	std::endl;
	std::cout << "minimal length      "	<< fminLength*1e3	<< " mm"		<<	std::endl;
	std::cout <<																std::endl;

	// Correction function for WF
	pcorrFunctionWF =							corr_func(v_datafiles.back(), tag, fcorrectWF);
	fAref =										pcorrFunctionWF->Eval(XPADLENGTH);

	// Track fit initializations
	TrackFitter aTrackFitter("Minuit", fnParamsTrack);
	PRFParameters aPRFParameters;
	ptf1PRF =									new TF1("ptf1PRF",aPRFParameters, -2.5*1.128, 2.5*1.128, 5);
	ptf1PRF->									SetParameters(p_uploader->Get_Norm(), p_uploader->Get_a2(), p_uploader->Get_a4(), p_uploader->Get_b2(), p_uploader->Get_b4());
	fcounterFail =								0;
	fcounterFit =								0;

	// Initialization of analysis vectors and histograms
	waveform_cluster.assign(510, 0);
	waveform_pad.assign(510, 0);
	// dE/dx quick access histograms
	int xmax =									1300;
	if(tag.find("Theta") != std::string::npos) xmax = 2000;
	ph1f_WF =									new TH1F("ph1f_WF", "<dE/dx> with WF;<dE/dx> (ADC count);Number of events", 100, 0, xmax);
	ph1f_XP =									new TH1F("ph1f_XP", "<dE/dx> with XP;<dE/dx> (ADC count);Number of events", 100, 0, xmax);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Compute dE/dx
	aSelector.										Reset_StatCounters();
	std::cout << "Processing events:" << std::endl;

	// Event loop
	for (iEvent = 0; iEvent < NEvents; iEvent++){
		if(iEvent % 1000 ==	0 or iEvent ==	NEvents-1) std::cout << iEvent << "/" << NEvents << std::endl;

		pEvent =									p_uploader->GiveMe_Event(iEvent, moduleCase, 0, 0);
		if (!pEvent)								continue;

		p_recoevent->eventNbr =						iEvent;
		NMod =										pEvent->Get_NberOfModule();
		p_recoevent->numberOfModules =				NMod;

		aSelector.									ApplySelection(pEvent);
		if (pEvent->IsValid() == 0){
			if(!fsaveSelectOnly){
				for (iMod = 0; iMod < NMod; iMod++){
					pModule =							pEvent->Get_Module_InArray(iMod);
					fmodID =							pModule->Get_ModuleNber();
					DiscardedModule();
				}
				fpTree_dEdx->								Fill();
			}
			p_recoevent->								Clear();
			delete										pEvent;
			continue;
		}

		p_recoevent->selected = 					true;

		// Module loop
		for (iMod =	0; iMod < NMod; iMod++){
			pModule =								pEvent->Get_Module_InArray(iMod);
			fmodID =								pModule->Get_ModuleNber();
			if (pEvent->Validity_ForThisModule(fmodID) == 0){
				if(!fsaveSelectOnly) DiscardedModule();
				continue;
			}

			NClusters =								pModule->Get_NberOfCluster();

			p_recomodule =							new Reconstruction::RecoModule();
			p_recomodule->position =				fmodID;
			p_recomodule->ID =						fERAMs_iD[fmodID];
			p_recomodule->selected = 				true;

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
			pTrack =								pEvent->GiveMe_Track_ForThisModule(fmodID);
			p_recomodule->phi =						std::atan(pTrack->Get_ParameterValue(1))/M_PI*180;
			p_recomodule->Track->					SetParameters(pTrack->Get_ParameterValue(0), pTrack->Get_ParameterValue(1));
			Double_t parErrors[2] =					{pTrack->Get_ParameterError(0), pTrack->Get_ParameterError(1)};
			p_recomodule->Track->					SetParErrors(parErrors);
			if(fnParamsTrack ==	3){
				p_recomodule->Track->				SetParameter(2, pTrack->Get_ParameterValue(2));
				p_recomodule->Track->				SetParError(2, pTrack->Get_ParameterError(2));
			}


			// Loop On Clusters
			for (iC = 0; iC < NClusters; iC++){
				std::fill(waveform_cluster.begin(), waveform_cluster.end(), 0); // reset waveform
				pCluster =							pModule->Get_Cluster(iC);
				p_recocluster = 					new Reconstruction::RecoCluster();
				p_recocluster->ADCmax_base =		pCluster->Get_Acluster();
				p_recocluster->ALead_base = 		pCluster->Get_AMaxLeading();
				p_recocluster->TLead =				pCluster->Get_TMaxLeading();


				// Loop On Pads
				int NPads =							pCluster->Get_NberOfPads();
				p_recocluster->NPads =				NPads;
				for(iP = 0; iP < NPads; iP ++){
					std::fill(waveform_pad.begin(), waveform_pad.end(), 0); // reset waveform
					pPad =							pCluster->Get_Pad(iP);
					p_recopad = 					new Reconstruction::RecoPad();
					if(iP == 0) p_recopad->leading = true;
					waveform_pad =					pPad->Get_vADC();
					p_recopad->ix =					pPad->Get_iX();
					p_recopad->iy =					pPad->Get_iY();

					// RC correction
					switch(fcorrectRC){
						case 0: // no RC correction
							p_recopad->RC =	170;
							break;
						case 1: // pad per pad RC correction
							p_recopad->RC =	pERAMMaps->RC(fERAMs_pos[fmodID], p_recopad->ix, p_recopad->iy);
							break;
						case 2: // ERAM per ERAM correction
							p_recopad->RC =	pERAMMaps->MeanRC(fERAMs_pos[fmodID]);
							break;
					}

					// Gain correction
					switch(fcorrectGain){
						case 0: // no gain correction
							p_recopad->GainCorrection = 1.0;
							break;
						case 1: // pad per pad gain correction
							p_recopad->gain =			pERAMMaps->Gain(fERAMs_pos[fmodID], p_recopad->ix, p_recopad->iy);
							p_recopad->GainCorrection = AVG_GAIN / p_recopad->gain;
							for (int i = 0; i < 510; i++) waveform_pad[i] = round(waveform_pad[i] * p_recopad->GainCorrection);
							if (iP == 0) p_recocluster->ALead_GCorr = p_recopad->GainCorrection;
							break;
						case 2: // ERAM per ERAM correction
							p_recopad->GainCorrection = AVG_GAIN / pERAMMaps->MeanGain(fERAMs_pos[fmodID]);
							for (int i = 0; i < 510; i++) waveform_pad[i] = round(waveform_pad[i] * p_recopad->GainCorrection);
							if (iP == 0) p_recocluster->ALead_GCorr = p_recopad->GainCorrection;
							break;
					}
					p_recopad->ADCmax_base =			pPad->Get_AMax();
					p_recopad->ADCmax = 				p_recopad->GainCorrection * pPad->Get_AMax();
					for(int i=0;i<510;i++) waveform_cluster[i] += waveform_pad[i];
					
					// Track computations
					local_params(pPad, pTrack, p_recopad->d, p_recopad->dd, p_recopad->phi, p_recopad->length); // impact parameter and length in pad in m, angle in degrees
					p_recopad->d *=					1000; // in mm
					p_recopad->dd *=				1000; // in mm
					p_recopad->length /=			costheta;
					p_recocluster->length +=		p_recopad->length;

					// Minimum length in pad cutoff to avoid fluctuations from small segments
					if(p_recopad->length*costheta <= fminLength){ // cutoff defined in the ERAM's plane -> remove theta dependence
						p_recocluster->				v_pads.push_back(p_recopad);	
						continue;
					}

					// Compute drift distance for different time bin sizes and peaking times
					p_recopad->TMax =					pPad->Get_TMax();
					if		(PT ==	412 and TB ==	50){ p_recopad->T0 =	45; p_recopad->driftDistance =	3.90*(p_recopad->TMax-p_recopad->T0); } // 45 =	37(time shift) +	8(PT) from David Attié
					else if (PT ==	412 and TB ==	40){ p_recopad->T0 =	56; p_recopad->driftDistance =	3.12*(p_recopad->TMax-p_recopad->T0); } // 56 =	46(time shift) + 10(PT)
					else if (PT ==	200 and TB ==	50){ p_recopad->T0 =	39; p_recopad->driftDistance =	3.90*(p_recopad->TMax-p_recopad->T0); } // 39 =	35(time shift) +	4(PT) own computation
					else if (PT ==	200 and TB ==	40){ p_recopad->T0 =	48; p_recopad->driftDistance =	3.12*(p_recopad->TMax-p_recopad->T0); } // 48 =	44(time shift) +	5(PT)
					
					// Get the LUT ratio 
					p_recopad->ratioDrift =			p_lut->getRatio(fabs(p_recopad->phi), fabs(p_recopad->d), p_recopad->RC, p_recopad->driftDistance);
					p_recopad->ratioFile =			p_lut->getRatio(fabs(p_recopad->phi), fabs(p_recopad->d), p_recopad->RC, driftDist);

					if(fcorrectDrift ==	1) p_recopad->ratio = p_recopad->ratioDrift;
					else					p_recopad->ratio = p_recopad->ratioFile;

					p_recopad->charge =				p_recopad->ADCmax*p_recopad->ratio;
					p_recopad->dEdxXP =				p_recopad->charge/p_recopad->length/100; // m to cm
					v_mod_dE.						push_back(p_recopad->charge);
					v_mod_dx.						push_back(p_recopad->length);
					v_mod_dEdxXP.					push_back(p_recopad->dEdxXP);

					p_recocluster->v_pads.			push_back(p_recopad);
					p_recomodule->NCrossedPads++;
					p_recomodule->lengthXP +=		p_recopad->length;

				} // Pads

				// // Gain correction for the whole cluster (doesn't work really well)
				// if(fcorrectGain == 1) for(int i=0;i<510;i++) waveform_cluster[i] *= p_recocluster->ALead_GCorr;

				// WF correction function
				p_recocluster->charge =				*std::max_element(waveform_cluster.begin(), waveform_cluster.end());
				p_recocluster->ratioCorr =			fAref / pcorrFunctionWF->Eval(p_recocluster->length*1000);
				if(diag) p_recocluster->dEdxWF =	p_recocluster->charge*p_recocluster->ratioCorr/XPADLENGTH*10;
				else p_recocluster->dEdxWF =		p_recocluster->charge/(p_recocluster->length*100);
				v_mod_dEdxWF.						push_back(p_recocluster->dEdxWF);

				// Fill cluster information
				p_recomodule->v_clusters.			push_back(p_recocluster);
				p_recomodule->NClusters++;
				p_recomodule->lengthWF +=			p_recocluster->length;
				p_recomodule->NPads +=				p_recocluster->NPads;
				
			} // Clusters

			// Fill module class attributes
			p_recomodule->avg_pad_mult =			(float)p_recomodule->NPads/p_recomodule->NClusters;

			// Compute dE/dx for the module
			p_recomodule->dEdxWF =					ComputedEdxWF(v_mod_dEdxWF, p_recomodule->NClusters, falpha);
			p_recomodule->dEdxXP =					ComputedEdxXP(v_mod_dEdxXP, v_mod_dE, v_mod_dx, p_recomodule->NCrossedPads, falpha);
			p_recomodule->dEdxWFnoTrunc =			ComputedEdxWF(v_mod_dEdxWF, p_recomodule->NClusters, 1);
			p_recomodule->dEdxXPnoTrunc =			ComputedEdxXP(v_mod_dEdxXP, v_mod_dE, v_mod_dx, p_recomodule->NCrossedPads, 1);

			// Fill module information into the event class
			p_recoevent->v_modules.					push_back(p_recomodule);
			p_recoevent->NCrossedPads +=			p_recomodule->NCrossedPads;
			p_recoevent->NPads +=					p_recomodule->NPads;
			p_recoevent->NClusters +=				p_recomodule->NClusters;
			p_recoevent->lengthXP +=				p_recomodule->lengthXP;
			p_recoevent->lengthWF +=				p_recomodule->lengthWF;
			v_evt_dE.								insert(v_evt_dE.end(), v_mod_dE.begin(), v_mod_dE.end());
			v_evt_dx.								insert(v_evt_dx.end(), v_mod_dx.begin(), v_mod_dx.end());
			v_evt_dEdxXP.							insert(v_evt_dEdxXP.end(), v_mod_dEdxXP.begin(), v_mod_dEdxXP.end());
			v_evt_dEdxWF.							insert(v_evt_dEdxWF.end(), v_mod_dEdxWF.begin(), v_mod_dEdxWF.end());

			// clear module vectors
			v_mod_dE.								clear();
			v_mod_dx.								clear();
			v_mod_dEdxXP.							clear();
			v_mod_dEdxWF.							clear();
		} // Modules

		// Fill event class attributes
		p_recoevent->avg_pad_mult =					(float)p_recoevent->NPads/p_recoevent->NClusters;

		// Compute dE/dx for the event
		p_recoevent->dEdxWF =						ComputedEdxWF(v_evt_dEdxWF, p_recoevent->NClusters, falpha);
		p_recoevent->dEdxXP =						ComputedEdxXP(v_evt_dEdxXP, v_evt_dE, v_evt_dx, p_recoevent->NCrossedPads, falpha);
		p_recoevent->dEdxWFnoTrunc =				ComputedEdxWF(v_evt_dEdxWF, p_recoevent->NClusters, 1);
		p_recoevent->dEdxXPnoTrunc =				ComputedEdxXP(v_evt_dEdxXP, v_evt_dE, v_evt_dx, p_recoevent->NCrossedPads, 1);

		// Make the quick access histograms
		ph1f_WF->									Fill(p_recoevent->dEdxWF);
		ph1f_XP->									Fill(p_recoevent->dEdxXP);

		// Fill the tree
		fpTree_dEdx->								Fill();

		// Clear event vectors
		v_evt_dE.									clear();
		v_evt_dx.									clear();
		v_evt_dEdxXP.								clear();
		v_evt_dEdxWF.								clear();
		p_recoevent->								Clear();
		delete										pEvent;
	} // Events

	aSelector.PrintStat();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// dE/dx histograms fitting
	std::cout << "Fitting dE/dx histograms... "; 
	Fit1Gauss(ph1f_WF, 2);
	Fit1Gauss(ph1f_XP, 2);
	std::cout << "done!" << std::endl; 

	// // Save
	ph1f_WF->										Write();
	ph1f_XP->										Write();
	fpTree_dEdx->									Write();
	fpFile_dEdx->									Close();

	// Deleting
	delete											fpFile_dEdx;
	delete											pcorrFunctionWF;
	delete											pERAMMaps;
	delete											ptf1PRF;
	std::cout.										rdbuf(coutbuf); // Reset to standard output
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* dE/dx RECONSTRUCTION ALGORITHMS */

float Reconstruction::dEdx::ComputedEdxWF(std::vector<float> v_dEdxWF, const int& NClusters, const float& alpha){
	float NClustersTrunc =						int(floor(NClusters*alpha));
	std::sort(v_dEdxWF.begin(), v_dEdxWF.end());
	return std::accumulate(v_dEdxWF.begin(), v_dEdxWF.begin() + NClustersTrunc, 0.) / NClustersTrunc;
}



float Reconstruction::dEdx::ComputedEdxXP(const std::vector<float>& v_dEdx, const std::vector<float>& v_dE, const std::vector<float>& v_dx, const int& nCrossedPads, const float& alpha){
	/// dEdx =	sum(dE)/Sum(dx) and not average(dE/dx) of each pad = > less sensitive to statistical fluctuations
	float NCrossedTrunc =						int(floor(nCrossedPads*alpha));
	float DE = 0, DX = 0;

	// Few steps to order v_dE & v_dx similarly to v_dEdx
	std::vector<int> indices(v_dEdx.size());
	std::iota(indices.begin(), indices.end(), 0); // make list from 0 to v_dEdx.size()

	std::sort(indices.begin(), indices.end(), [&](int i, int j){ // sorting wrt v_dEdx
		return v_dEdx[i] < v_dEdx[j];
	});
	for (int i =	0; i < NCrossedTrunc; ++i) {
		DE +=	v_dE[indices[i]];
		DX +=	v_dx[indices[i]]*100; // m->cm normalization
	}
	return DE/DX;
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Events discarded by selection steps
void Reconstruction::dEdx::DiscardedModule(){
	NClusters =								pModule->Get_NberOfCluster();

	p_recomodule =							new Reconstruction::RecoModule();
	p_recomodule->position =				fmodID;
	p_recomodule->ID =						fERAMs_iD[fmodID];

	// Loop On Clusters
	for (iC = 0; iC < NClusters; iC++){
		pCluster =							pModule->Get_Cluster(iC);
		p_recocluster = 					new Reconstruction::RecoCluster();
		p_recocluster->ADCmax_base =		pCluster->Get_Acluster();
		p_recocluster->ALead_base = 		pCluster->Get_AMaxLeading();
		p_recocluster->TLead =				pCluster->Get_TMaxLeading();


		// Loop On Pads
		int NPads =							pCluster->Get_NberOfPads();
		p_recocluster->NPads =				NPads;
		for(iP = 0; iP < NPads; iP ++){
			pPad =							pCluster->Get_Pad(iP);
			p_recopad = 					new Reconstruction::RecoPad();
			if(iP == 0) p_recopad->leading = true;
			waveform_pad =					pPad->Get_vADC();
			p_recopad->ix =					pPad->Get_iX();
			p_recopad->iy =					pPad->Get_iY();
			p_recopad->ADCmax_base =		pPad->Get_AMax();
			p_recopad->TMax =					pPad->Get_TMax();

			p_recocluster->v_pads.			push_back(p_recopad);
			p_recomodule->NCrossedPads++;

		} // Pads

		// Fill cluster information
		p_recomodule->v_clusters.			push_back(p_recocluster);
		p_recomodule->NClusters++;
		p_recomodule->NPads +=				p_recocluster->NPads;
		
	} // Clusters

	// Fill module class attributes
	p_recomodule->avg_pad_mult =			(float)p_recomodule->NPads/p_recomodule->NClusters;

	// Fill module information into the event class
	p_recoevent->v_modules.					push_back(p_recomodule);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* ANALYSIS CLASSES DEFINITION */

Reconstruction::RecoPad::~RecoPad(){
	// nothing to clear
}


Reconstruction::RecoCluster::~RecoCluster(){
	for (auto p_recopad : v_pads) delete p_recopad;
	v_pads.clear();
}


Reconstruction::RecoModule::~RecoModule(){
	delete Track;
	for (auto p_recocluster : v_clusters) delete p_recocluster;
	v_clusters.clear();
}

Reconstruction::RecoEvent::RecoEvent(){
}
Reconstruction::RecoEvent::~RecoEvent(){
	for (auto p_recomodule : v_modules) delete p_recomodule;
	v_modules.clear();
}

void Reconstruction::RecoEvent::Clear(){
	for (auto p_recomodule : v_modules) delete p_recomodule;
	v_modules.clear();
	selected =			false;
	eventNbr =			0;
	dEdxXP =			0;
	dEdxWF =			0;
	dEdxXPnoTrunc =		0;
	dEdxWFnoTrunc =		0;
	NCrossedPads =		0;
	NPads =				0;
	NClusters =			0;
	lengthXP =			0;
	lengthWF =			0;
	numberOfModules =	0;
	avg_pad_mult =		0;
}