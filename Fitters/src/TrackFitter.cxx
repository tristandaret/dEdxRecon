#include "TrackFitter.h"
#include "TFitter.h"

//--------------------------------------------------------------------//
/** This class is needed to run Minuit */
class StaticTrackFitter {
public: 
	/** Constructor */
	StaticTrackFitter();
	virtual ~StaticTrackFitter( );
	/** Set */
	static void Set(TrackFitter* pTrackFitter); 
	/** Pointer */
	static TrackFitter* p_TrackFitter;
};

TrackFitter* StaticTrackFitter::p_TrackFitter = NULL;

StaticTrackFitter::StaticTrackFitter() {}
StaticTrackFitter::~StaticTrackFitter() {}
void StaticTrackFitter::Set(TrackFitter* pTrackFitter) { p_TrackFitter = pTrackFitter; }

void TrackFitterFunction(int& nDim, double* gout, double& result, double par[], int flg) 
{
	result = StaticTrackFitter::p_TrackFitter->Chi2(par);
}

//--------------------------------------------------------------------//

TrackFitter::TrackFitter(
				const std::string& FitterName,
				const int& NberOfParameters 
)
{
	int ierr; 
	double arg[1];

	m_FitterName = FitterName;
	m_NberOfParameters = NberOfParameters;
	
	p_TVirtualFitter = 0;
	
	delete p_TVirtualFitter;
	TVirtualFitter::SetDefaultFitter(m_FitterName.c_str());
	p_TVirtualFitter = TVirtualFitter::Fitter(0, 500);

	// Set function pointer
	StaticTrackFitter::Set(this);
	p_TVirtualFitter->SetFCN(TrackFitterFunction);

	p_TVirtualFitter->ExecuteCommand("clear", arg, 0);

	// Set print level to -1 to suppress all messages
	arg[0] = -1;
	ierr = p_TVirtualFitter->ExecuteCommand("SET PRINT LEVEL", arg, 1);

	// Set other parameters
	arg[0] = 0;
	ierr = p_TVirtualFitter->ExecuteCommand("SET NOW", arg, 1);
	
	arg[0] = 1.e-9;
	ierr = p_TVirtualFitter->ExecuteCommand("SET EPS", arg, 1);
	
	ierr = p_TVirtualFitter->ExecuteCommand("SET NOG", arg, 0);

	arg[0] = 2;
	ierr = p_TVirtualFitter->ExecuteCommand("SET STRATEGY", arg, 1);
}

TrackFitter::~TrackFitter() {}

void TrackFitter::Set_Track (Track* pTrack)
{
	p_Track		= pTrack;
	p_Track->SetNberOfParameters(m_NberOfParameters);
}

int TrackFitter::DoMinimisation()
{
	int ierReturned = p_Track->SetParameter(p_TVirtualFitter);

//	
	double arg[10];
	
	arg[0] = 10000.; // max number of calls
	arg[1] = 1.; // tolerance on minimum (1. corresponds to 1.d-3)

//
	int ierMIGRAD_N = p_TVirtualFitter->ExecuteCommand("MIGRAD",arg ,2);
	
	if (ierMIGRAD_N==0){

	int ierMINOS_N = p_TVirtualFitter->ExecuteCommand("MINOS",arg ,1);
	if (ierMINOS_N!=0) std::cout << " TrackFitter::DoMinimisation() ierMINOS_N	" << ierMINOS_N << std::endl;

	p_Track->SetResults(p_TVirtualFitter);
	
	ierReturned = 0;
	
	}else{
	
	std::cout << std::endl;
	std::cout << " TrackFitter::DoMinimisation() normal fitting failed "
				<< "->ierMIGRAD_N " << ierMIGRAD_N	
				<< std::endl;

	int ierMIGRAD_R1	= p_TVirtualFitter->ExecuteCommand("MINMIZE",arg ,2);
		ierMIGRAD_R1	= p_TVirtualFitter->ExecuteCommand("MIGRAD",arg ,2); 

	if (ierMIGRAD_R1==0){

		std::cout << " TrackFitter::DoMinimisation() Rescue 1 succeeded "
				<< std::endl;

		int ierMINOS_R1 = p_TVirtualFitter->ExecuteCommand("MINOS",arg ,1);
		if (ierMINOS_R1!=0) std::cout << " TrackFitter::DoMinimisation() ->->ierMINOS_R1	" << ierMINOS_R1 << std::endl;

		p_Track->SetResults(p_TVirtualFitter);

		ierReturned = 0;
	
	}else{

		std::cout << " TrackFitter::DoMinimisation() Rescue 1 failed "
				<< "->ierMIGRAD_R1 " << ierMIGRAD_R1	
				<< std::endl;

		int ierMIGRAD_R2	= p_TVirtualFitter->ExecuteCommand("SIMPLEX",arg ,2);
			ierMIGRAD_R2	= p_TVirtualFitter->ExecuteCommand("MIGRAD",arg ,2); 

		if (ierMIGRAD_R2==0){

		std::cout << " TrackFitter::DoMinimisation() Rescue 2 succeeded "
					<< std::endl;

		int ierMINOS_R2 = p_TVirtualFitter->ExecuteCommand("MINOS",arg ,1);
		if (ierMINOS_R2!=0) std::cout << " TrackFitter::DoMinimisation() ->->->ierMINOS_R2	" << ierMINOS_R2 << std::endl;

		p_Track->SetResults(p_TVirtualFitter);

		ierReturned = 0;
	
		}else{

		ierReturned = ierMIGRAD_R2;

		std::cout << " TrackFitter::DoMinimisation() Rescue 2 failed "
					<< "->ierMIGRAD_R2 " << ierMIGRAD_R2	
					<< std::endl;
		std::cout << " TrackFitter::DoMinimisation() All Rescues failed : abort "
					<< std::endl;
		
		int NClusters = p_Track->Get_NberOfCluster();
		for (int iC = 0; iC< NClusters; iC++){
			const Cluster* pCluster = p_Track->Get_Cluster(iC);
			std::cout
			<< " Y "	<< std::setw(16) << std::setprecision(3) <<	pCluster->Get_YTrack()	* 1.E3
			<< " +/- " << std::setw(16) << std::setprecision(3) <<	pCluster->Get_eYTrack() * 1.E3
			<< std::endl;
		}
		
		abort();

		}

	}
	}

//	ier = p_TVirtualFitter->ExecuteCommand("MIGRAD",arg ,2);
//	ier = p_TVirtualFitter->ExecuteCommand("MINOS",arg ,1);
// 
//	ier = p_TVirtualFitter->ExecuteCommand("MIGRAD",arg ,2);
//	ier = p_TVirtualFitter->ExecuteCommand("MINOS",arg ,1);
// 
//	ier = p_TVirtualFitter->ExecuteCommand("MIGRAD",arg ,2);
//	ier = p_TVirtualFitter->ExecuteCommand("MINOS",arg ,1);

//	p_Track->SetResults(p_TVirtualFitter);

//	int STATUS = (((TFitter*)p_TVirtualFitter)->GetMinuit())->GetStatus();
// 
//	std::cout 
//	<< "TrackFitter::DoMinimisation()"	
//	<< " STATUS " << STATUS << std::endl;
	

	return ierReturned;
		
}

double TrackFitter::Chi2(double par[]) { return p_Track->Chi2(par); }
