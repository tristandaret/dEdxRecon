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

#include "PRF_param.h"
#include "DoTracksReconstruction.h"
#include "JFL_Do_ClusterFit.h"
#include "TheFitterTrack.h"
#include "EvtModelTools_Histos.h"
#include "EvtModelTools_TD_Selections.h"
#include "GiveMe_Uploader.h"
#include "JFL_Selector.h"

ClassImp(Reconstruction::TPad)
ClassImp(Reconstruction::TCluster);
ClassImp(Reconstruction::TModule)
ClassImp(Reconstruction::TEvent)

Reconstruction::dEdx::dEdx(){
}

Reconstruction::dEdx::~dEdx(){
}

void Reconstruction::dEdx::Reconstruction(){
	TH1F *ph1f_ratio =  					new TH1F("ph1f_ratio", "Ratio;ratio;count", 150, 0, 2);
	TH1F *ph1f_ncross = 					new TH1F("ph1f_ncross", "Number of crossed pads;Number of crossed pads;count", 45, 0, 44);
	TH1F *ph1f_nclusters = 					new TH1F("ph1f_nclusters", "Number of clusters;Number of clusters;count", 45, 0, 44);
	TH1F *ph1f_angle = 						new TH1F("ph1f_angle", "Angle;Angle;count", 181, -90, 90);
	TH1F *ph1f_padlength = 					new TH1F("ph1f_padlength", "Pad length;Pad length;count", 100, 0, 16);
	TH1F *ph1f_clusterlength = 				new TH1F("ph1f_clusterlength", "Cluster length;Cluster length;count", 100, 0, 16);
	TH1F *ph1f_gaincorr = 					new TH1F("ph1f_gaincorr", "Gain correction;Gain correction;count", 100, 0, 2);
	TH2F *ph2f_ratiozcalczfile = 			new TH2F("ph2f_ratiozcalczfile", "Ratio zcalc vs zfile;ratio zfile;ratio zcalc", 80, 0, 2.1, 80, 0, 2.1);
	TH2F *ph2f_lutvspadlength = 			new TH2F("ph2f_lutvspadlength", "LUT vs pad length;LUT;Pad length", 100, 0, 16, 100, 0, 2.1);
	TH1F *ph1f_wfcorr = 					new TH1F("ph1f_wfcorr", "WF correction;WF correction;count", 100, 0, 2);
	TH2F *ph2f_AmaxvsLength = 				new TH2F("h2f_AmaxvsLength", "ADC_{max} VS length in pad (before length cut);Length in pad (mm);ADC_{max}", 80, -0.1, 16, 80, 0, 4100);
	std::string OUTDirName =                outDir + tag + "/";
	MakeMyDir(OUTDirName);
	std::string OUTDIR_Display =            OUTDirName + "Evt_Display/";
	MakeMyDir(OUTDIR_Display);
	std::string OUTDIR_WF_Display =         OUTDirName + "WF_Display/";
	MakeMyDir(OUTDIR_WF_Display);

	// Redirect Output
	std::string     logs =        OUTDirName + "2_" + tag + "_dEdx_XP.log";
	std::cout <<    "logs: " << logs       << std::endl;
	std::cout <<    std::setprecision(2)  << std::fixed;
	std::cout <<    std::endl;
	std::streambuf* coutbuf =                std::cout.rdbuf();  // Save old buf
	std::ofstream   OUT_DataFile(logs.c_str() );    // Set output file
	std::cout.      rdbuf(OUT_DataFile.rdbuf());                // Redirect to output file

	std::cout << "dataFile         : " << dataFile        << std::endl;
	std::cout << "OUTDirName       : " << OUTDirName      << std::endl;
	std::cout << "tag              : " << tag             << std::endl;
	std::cout << "comment          : " << comment         << std::endl;
	std::cout << "selectionSet     : " << selectionSet    << std::endl;
	std::cout << "prtcle           : " << prtcle          << std::endl;
	std::cout << "driftMethod      : " << driftMethod     << std::endl;
	std::cout << "WF updated       : " << WFupdated       << std::endl;
	std::cout << "Gain correction  : " << fcorrectGain << std::endl;
	std::cout <<                                             std::endl;

	// Get ERAM ID
	std::vector<int> fERAMs_iD;
	std::vector<int> fERAMs_pos;
	if(tag.find("DESY") != std::string::npos)         {fERAMs_iD.push_back(1);  fERAMs_pos.push_back(12);}
	if(dataFile.find("ERAM18") != std::string::npos)  {fERAMs_iD.push_back(18); fERAMs_pos.push_back(18);} // take 14 in position #18 because 18 is not mounted but they are similar
	if(dataFile.find("All_ERAMS") != std::string::npos){fERAMs_iD =  {7, 1, 23, 2, 16, 15, 10, 12}; fERAMs_pos =  {26, 7, 12, 10, 7, 17, 19, 13, 14};} // 12 replace with 11 in pos #14

	float costheta =     1;
	int theta_arr[3] =   {-45, -20, 20};
	for (int theta_file : theta_arr) if(tag.find("theta") != std::string::npos and tag.find(std::to_string(theta_file)) != std::string::npos) costheta =  fabs(cos((float)theta_file/180*M_PI));
	Reconstruction::ERAMMaps *pERAMMaps =  new Reconstruction::ERAMMaps();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Output
	TFile *pFile_dEdx =    new TFile((OUTDirName + "2_" + tag + "_dEdx" + comment + ".root").c_str(), "RECREATE");
	TTree *pTree_dEdx =    new TTree("dEdx_tree", "dEdx TTree");
	Reconstruction::TEvent *p_teventRef =  nullptr;
	pTree_dEdx->Branch("eventBranch", "TEvent", &p_teventRef);
	// Selection stage
	JFL_Selector aJFL_Selector(selectionSet);
	int NEvent =  p_uploader->Get_NberOfEvent();
	std::cout << "Number of entries :" << NEvent << std::endl;

	Init_selection(selectionSet, aJFL_Selector, tag, p_uploader, moduleCase, 0);
		
	aJFL_Selector.Tell_Selection();

	std::cout << "drift distance : " << driftDist       << " mm"        << std::endl;
	std::cout << "Peaking time   : " << PT              << " ns"        << std::endl;
	std::cout << "Time bin length: " << TB              << " ns"        << std::endl;
	std::cout << "PT/TB =           " << PT/TB           << " time bins" << std::endl;
	std::cout << "drift method   : " << driftMethod                     << std::endl;
	std::cout << "minimal length : " << fminLength*1e3  << " mm"        << std::endl;
	std::cout <<                                                           std::endl;

	// Track fit initializations
	TF1 *pcorrFunctionWF =              			corr_func(dataFile, tag, WFupdated);
	float fAref =                        			pcorrFunctionWF->Eval(XPADLENGTH);

	TheFitterTrack aTheFitterTrack("Minuit", fnParamsTrack);
	PRF_param aPRF_param;
	TF1 *ptf1PRF =                      			new TF1("ptf1PRF",aPRF_param, -2.5*1.128, 2.5*1.128, 5);
	ptf1PRF->                     					SetParameters(p_uploader->Get_Norm(), p_uploader->Get_a2(), p_uploader->Get_a4(), p_uploader->Get_b2(), p_uploader->Get_b4());
	int fcounterFail =                 				0;
	int fcounterFit =                  				0;

	std::vector<float> v_dx;
	std::vector<float> v_dE;
	std::vector<float> v_dEdxXP;
	std::vector<float> v_dEdxWF;
	std::vector<double> v_erams;
	TH1F *ph1f_WF =  new TH1F("ph1f_WF", "<dE/dx> with WF;<dE/dx> (ADC count);Number of events", 90, 0, 1800);
	TH1F *ph1f_XP =  new TH1F("ph1f_XP", "<dE/dx> with XP;<dE/dx> (ADC count);Number of events", 90, 0, 1800);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Compute dE/dx
	aJFL_Selector.                					Reset_StatCounters();
	std::cout << "Processing events:" << std::endl;
	for (int iEvent =  0; iEvent < NEvent; iEvent++){
		if(iEvent % 500 ==  0 or iEvent ==  NEvent-1) std::cout << iEvent << "/" << NEvent << std::endl;

		Event* pEvent =                     		p_uploader->GiveMe_Event(iEvent, moduleCase, 0, 0);
		if (!pEvent)                				continue;

		aJFL_Selector.              				ApplySelection(pEvent);
		if (pEvent->IsValid() != 1) 				continue;

		Reconstruction::TEvent *p_tevent =  		new Reconstruction::TEvent();
		p_tevent->eventNbr = 			 			iEvent;

		p_tevent->numberOfModules =           		pEvent->Get_NberOfModule();

		float fmodID;
		// Check if there is a track in 4 ERAMs that are aligned
		for (int iMod =  0; iMod < p_tevent->numberOfModules; iMod++){
			fmodID =                  				pEvent->Get_Module_InArray(iMod)->Get_ModuleNber();
			if (pEvent->Validity_ForThisModule(fmodID)!=0) v_erams.push_back(fERAMs_iD[fmodID]);
		} 

		// int nber =  2;
		// if(dataFile.find("All_ERAMS") != std::string::npos){
		//   // 1 ERAM
		//   if(fnERAMs ==  1 and!is_in(v_erams, nber)){
		//     delete pEvent;
		//     continue;
		//   }
		//   // 2 ERAMs
		//   if(fnERAMs ==  2 and !(is_in(v_erams, 7) and is_in(v_erams, 1)) and !(is_in(v_erams, 16) and is_in(v_erams, 10))){
		//     delete pEvent;
		//     continue;
		//   }
		//   // 4 ERAMs
		//   if(fnERAMs ==  4
		//     and !(is_in(v_erams,  7) and is_in(v_erams,  1) and is_in(v_erams, 23) and is_in(v_erams,  2))
		//     and !(is_in(v_erams, 16) and is_in(v_erams, 15) and is_in(v_erams, 10) and is_in(v_erams, 12))){
		//     delete pEvent;
		//     continue;
		//   }
		// }

			for (int iMod =  0; iMod < p_tevent->numberOfModules; iMod++){
			Module* pModule =          				pEvent->Get_Module_InArray(iMod);
			fmodID =                  				pModule->Get_ModuleNber();
			if (pEvent->Validity_ForThisModule(fmodID) ==  0) continue;

			// if(dataFile.find("All_ERAMS") != std::string::npos){
			//   if(fnERAMs ==  1 and Gainmaps[fmodID]->Get_iD()[fmodID] != nber) continue;
			//   if(fnERAMs ==  2 and Gainmaps[fmodID]->Get_iD()[fmodID] != 7 and Gainmaps[fmodID]->Get_iD()[fmodID] != 1 and Gainmaps[fmodID]->Get_iD()[fmodID] != 16 and Gainmaps[fmodID]->Get_iD()[fmodID] != 10) continue; // 2 ERAMs
			// }

			int NC =                   				pModule->Get_NberOfCluster();
			if(NC ==  0) 							continue;

			Reconstruction::TModule *p_tmodule =  	new Reconstruction::TModule();
			p_tmodule->position =         			iMod;
			p_tmodule->ID =               			fmodID;

			// Track fitting
			if(tag.find("diag") == std::string::npos){
				TheFitterCluster_PV0_PV1 aTheFitterCluster_PV0_PV1("Minuit");
				Do_ClusterFit_PV0_PV1_Event(pEvent, fmodID, ptf1PRF, fcounterFit, fcounterFail, aTheFitterCluster_PV0_PV1);
			}
			else{
				TheFitterCluster_PV0_Diag aTheFitterCluster_PV0_Diag("Minuit");
				Do_ClusterFit_PV0_Diag_Event(-(M_PI_2-(PHIMAX*M_PI/180)), pEvent, fmodID, ptf1PRF, fcounterFit, fcounterFail, aTheFitterCluster_PV0_Diag);
			}
			TheFitterTrack              aTheFitterTrack("Minuit", fnParamsTrack);
			DoTracksReconstruction_Event(aTheFitterTrack, pEvent, fmodID, fnParamsTrack);

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
				Cluster* pCluster =            		pModule->Get_Cluster(iC);
				Reconstruction::TCluster *p_tcluster = new Reconstruction::TCluster();
				p_tcluster->ph1f_WF_cluster->SetName(Form("ph1f_WF_cluster_%d_%d_%d", iEvent, iMod, iC));

				// Loop On Pads
				int NPads =                   		 pCluster->Get_NberOfPads();
				for(int iP =  0; iP < NPads; iP ++){
					const Pad* pPad =            pCluster->Get_Pad(iP);
					Reconstruction::TPad * p_tpad = new Reconstruction::TPad();
					p_tpad->ix =                    pPad->Get_iX();
					p_tpad->iy =                    pPad->Get_iY();
					p_tpad->ph1f_WF_pad =           GiveMe_WaveFormDisplay(pPad, "main");
					p_tpad->RC =                    pERAMMaps->RC(fERAMs_pos[iMod], pPad->Get_iX(), pPad->Get_iY());
					if(fcorrectGain){
						p_tpad->gain =              pERAMMaps->Gain(fERAMs_pos[iMod], pPad->Get_iX(), pPad->Get_iY());
						p_tpad->GainCorrection =    AVG_GAIN/p_tpad->gain;
						p_tpad->ADC =               p_tpad->GainCorrection*pPad->Get_AMax();
						p_tpad->ph1f_WF_pad->       Scale(p_tpad->GainCorrection);
						ph1f_gaincorr->             Fill(p_tpad->GainCorrection);
					}
					else p_tpad->ADC =              pPad->Get_AMax();
					p_tcluster->ph1f_WF_cluster->  	Add(p_tpad->ph1f_WF_pad);
					
					// Track computations (impact parameter in m, angle in degrees, length in pad in m)
					local_params(pPad, pTrack, p_tpad->d, p_tpad->dd, p_tpad->phi, p_tpad->length);
					p_tpad->d *= 					1000; // in mm
					p_tpad->dd *= 					1000; // in mm
					p_tpad->length /= 				costheta;
					p_tcluster->length += 			p_tpad->length;
					if(p_tpad->length*costheta <= fminLength){
						p_tcluster->				v_pads.push_back(p_tpad);	
						continue;
					}

					// Compute Z
					p_tpad->TMax =              	pPad->Get_TMax();
					if      (PT ==  412 and TB ==  50){ p_tpad->T0 =  45; p_tpad->driftDistance =  3.90*(p_tpad->TMax-p_tpad->T0); } // 45 =  37(time shift) +  8(PT) from David
					else if (PT ==  412 and TB ==  40){ p_tpad->T0 =  56; p_tpad->driftDistance =  3.12*(p_tpad->TMax-p_tpad->T0); } // 56 =  46(time shift) + 10(PT)
					else if (PT ==  200 and TB ==  50){ p_tpad->T0 =  39; p_tpad->driftDistance =  3.90*(p_tpad->TMax-p_tpad->T0); } // 39 =  35(time shift) +  4(PT) own computation
					else if (PT ==  200 and TB ==  40){ p_tpad->T0 =  48; p_tpad->driftDistance =  3.12*(p_tpad->TMax-p_tpad->T0); } // 48 =  44(time shift) +  5(PT)
					
					p_tpad->ratioDrift =        	p_lut->getRatio(fabs(p_tpad->phi), fabs(p_tpad->d), p_tpad->RC, p_tpad->driftDistance);
					p_tpad->ratioFile =         	p_lut->getRatio(fabs(p_tpad->phi), fabs(p_tpad->d), p_tpad->RC, driftDist);
					ph2f_ratiozcalczfile->      	Fill(p_tpad->ratioFile, p_tpad->ratioDrift);

					if(driftMethod ==  "zcalc") p_tpad->ratio = p_tpad->ratioDrift;
					if(driftMethod ==  "zfile") p_tpad->ratio = p_tpad->ratioFile;

					v_dE.                   		push_back(p_tpad->ADC*p_tpad->ratio);
					v_dx.                   		push_back(p_tpad->length);
					v_dEdxXP.               		push_back(p_tpad->ADC*p_tpad->ratio/(p_tpad->length*100)); // m to cm

					p_tevent->						NCrossedPads++;
					p_tcluster->v_pads.				push_back(p_tpad);
					ph1f_ratio->           			Fill(p_tpad->ratio);
					ph1f_padlength->        		Fill(p_tpad->length*1000);
					ph1f_angle->           			Fill(p_tpad->phi);
					ph2f_lutvspadlength->   		Fill(p_tpad->length*1000, p_tpad->ratio);
					ph2f_AmaxvsLength->      		Fill(p_tpad->length*1000, p_tpad->ADC);
				} // Pads

				// WF method
				p_tcluster->ratioCorr =             fAref / pcorrFunctionWF->Eval(p_tcluster->length*1000);
				ph1f_wfcorr->                       Fill(p_tcluster->ratioCorr);
				if(tag.find("diag") != std::string::npos) v_dEdxWF.push_back(p_tcluster->ph1f_WF_cluster->GetMaximum()*p_tcluster->ratioCorr/XPADLENGTH*10);
				else v_dEdxWF.						push_back(p_tcluster->ph1f_WF_cluster->GetMaximum()/(p_tcluster->length*100));
				p_tevent->							NClusters++;
				p_tmodule->v_clusters.				push_back(p_tcluster);
				ph1f_clusterlength->        		Fill(p_tcluster->length*1000);
			} // Clusters

			p_tevent->v_modules.					push_back(p_tmodule);
		} // Modules

		if(p_tevent->NCrossedPads > 0){
			// WF
			p_tevent->dEdxWF =                  	ComputedEdxWF(v_dEdxWF, p_tevent->NClusters);
			ph1f_WF->                       		Fill(p_tevent->dEdxWF);

			// XP
			p_tevent->dEdxXP =                  	ComputedEdxXP(v_dEdxXP, v_dE, v_dx, p_tevent->NCrossedPads);
			ph1f_XP->                       		Fill(p_tevent->dEdxXP);
		}

		ph1f_nclusters->                Fill(p_tevent->NClusters);
		ph1f_ncross->                   Fill(p_tevent->NCrossedPads);
		p_teventRef =  				p_tevent;
		pTree_dEdx->                Fill();
		v_erams.                    clear();
		v_dE.                       clear();
		v_dx.                       clear();
		v_dEdxXP.                   clear();
		v_dEdxWF.                   clear();
		delete pEvent;
		delete p_tevent;
	} // Events

	aJFL_Selector.PrintStat();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Fitting //
	std::cout << "Fitting: Started... "; 
	TF1 *ptf1_WF				 = Fit1Gauss(ph1f_WF, 2);
	ptf1_WF->					SetNameTitle("ptf1_WF", "ptf1_WF");

	TF1 *ptf1_XP				 = Fit1Gauss(ph1f_XP, 2);
	ptf1_XP->					SetNameTitle("ptf1_XP", "ptf1_XP");
	std::cout << "done!" << std::endl; 

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Methods
	pFile_dEdx->                  cd();
	ptf1_WF->                     Write();
	ptf1_XP->                     Write();
	ph1f_WF->                     Write();
	ph1f_XP->                     Write();
	ph1f_ratio->                  Write();
	ph1f_ncross->                 Write();
	ph1f_nclusters->              Write();
	ph1f_angle->                  Write();
	ph1f_padlength->              Write();
	ph1f_clusterlength->          Write();
	ph1f_gaincorr->               Write();
	ph2f_ratiozcalczfile->        Write();
	ph2f_lutvspadlength->         Write();
	ph1f_wfcorr->                 Write();
	ph2f_AmaxvsLength->           Write();
	pFile_dEdx->                  Close();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Deleting
	delete pFile_dEdx;
	delete pcorrFunctionWF;
	delete pERAMMaps;
	delete ptf1PRF;
	std::cout.rdbuf(coutbuf); // Reset to standard output
}



float Reconstruction::dEdx::ComputedEdxWF(std::vector<float> v_dEdxWF, const int& NClusters){
    float NClustersTrunc =                         int(floor(NClusters*falpha));
    std::sort(v_dEdxWF.begin(), v_dEdxWF.end());
    return std::accumulate(v_dEdxWF.begin(), v_dEdxWF.begin() + NClustersTrunc, 0.) / NClustersTrunc;
}




float Reconstruction::dEdx::ComputedEdxXP(const std::vector<float>& v_dEdx, const std::vector<float>& v_dE, const std::vector<float>& v_dx, const int& nCrossedPads) {
    /// dEdx =  sum(dE)/Sum(dx) and not average(dE/dx) of each pad = > less sensitive to statistical fluctuations
    float NCrossedTrunc =                         int(floor(nCrossedPads*falpha));
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


Reconstruction::TPad::~TPad(){
	delete ph1f_WF_pad;
}


Reconstruction::TCluster::~TCluster(){
	delete ph1f_WF_cluster;
	for (auto p_tpad : v_pads) delete p_tpad;
	v_pads.clear();
}


Reconstruction::TModule::~TModule(){
	delete Track;
	for (auto p_tcluster : v_clusters) delete p_tcluster;
	v_clusters.clear();
}

Reconstruction::TEvent::TEvent(){
}
Reconstruction::TEvent::~TEvent(){
	for (auto p_tmodule : v_modules) delete p_tmodule;
	v_modules.clear();
}

void Reconstruction::TEvent::Clear(){
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