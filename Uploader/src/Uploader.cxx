#include "Uploader.h"
#include "TF1.h"

Uploader::Uploader(const std::string& sampleFile, Model_ReadOutGeometry* pModel_ReadOutGeometry, Model_Electronics* pModel_Electronics, Model_Charge1D* pModel_Charge1D)
	: m_SampleFile(sampleFile), p_Model_ReadOutGeometry(pModel_ReadOutGeometry), p_Model_Electronics(pModel_Electronics), p_Model_Charge1D(pModel_Charge1D), m_PRF_exist(0)
{
	std::cout << std::endl;
	std::cout << "Data file: " << m_SampleFile << std::endl;

	p_TFile = TFile::Open(sampleFile.c_str(), "READ");
	if (!p_TFile)
	{
		std::cout << "Uploader::Uploader ERROR. Couldn't open root file: " << sampleFile << std::endl;
		abort();
	}

	p_TTree = (TTree*)p_TFile->Get("outtree");
	if (!p_TTree)
	{
		std::cout << "Uploader::Uploader ERROR. p_TTree==0" << std::endl;
		abort();
	}

	m_NberOfEvent = (int)p_TTree->GetEntries();

	TF1* pTF1 = (TF1*)p_TFile->Get("PRF_function");
	if (pTF1)
	{
		if (std::string(pTF1->GetParName(0)) == "Const" &&
			std::string(pTF1->GetParName(1)) == "a2" &&
			std::string(pTF1->GetParName(2)) == "a4" &&
			std::string(pTF1->GetParName(3)) == "b2" &&
			std::string(pTF1->GetParName(4)) == "b4")
		{
			double Norm = pTF1->GetParameter(0);
			double a2 = pTF1->GetParameter(1) / (100. * 100.);
			double a4 = pTF1->GetParameter(2) / (100. * 100. * 100. * 100.);
			double b2 = pTF1->GetParameter(3) / (100. * 100.);
			double b4 = pTF1->GetParameter(4) / (100. * 100. * 100. * 100.);

			m_PRF_exist = 1;
			m_Norm = Norm;
			m_a2 = a2;
			m_a4 = a4;
			m_b2 = b2;
			m_b4 = b4;
		}
	}
	else
	{
		std::cout << std::endl;
		std::cout << "	Found no PRF function in TFile " << std::endl;
	}
}

Uploader::~Uploader() {}

std::string Uploader::Get_SampleFile()
{
	return m_SampleFile;
}

int Uploader::Get_NberOfEvent()
{
	return m_NberOfEvent;
}

int Uploader::Get_PRF_exist()
{
	return m_PRF_exist;
}

double Uploader::Get_Norm()
{
	return m_Norm;
}

double Uploader::Get_a2()
{
	return m_a2;
}

double Uploader::Get_a4()
{
	return m_a4;
}

double Uploader::Get_b2()
{
	return m_b2;
}

double Uploader::Get_b4()
{
	return m_b4;
}

Model_ReadOutGeometry* Uploader::Get_Model_ReadOutGeometry()
{
	return p_Model_ReadOutGeometry;
}

Model_Electronics* Uploader::Get_Model_Electronics()
{
	return p_Model_Electronics;
}

Model_Charge1D* Uploader::Get_Model_Charge1D()
{
	return p_Model_Charge1D;
}
