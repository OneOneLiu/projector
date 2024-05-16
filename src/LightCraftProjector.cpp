#include "LightCraftProjector.h"
#include <iostream>
#include "std_msgs/Int32.h"
namespace amnl
{
	CLightCraftProjector::CLightCraftProjector(void)
	{
		m_numLutEntries = 0;
		m_numSplashLutEntries = 0;
		m_num_pats_in_exposure = 1;
	}

	CLightCraftProjector::~CLightCraftProjector(void)
	{
		Close();
	}

	bool CLightCraftProjector::SetFrameParam(unsigned int trig, unsigned int exp)
	{
		try {
			if (IsOpen())
			{
				SetPatternPeried(trig, exp);
				return true;
			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector SetFrameParam" << std::endl;
		}
		return false;
	}
	bool CLightCraftProjector::IsOpen() { 
		try {
			return DLPC350_USB_IsConnected();
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector IsOpen" << std::endl;
		}
		return false;
	}
	bool CLightCraftProjector::Open()
	{
		try {
			if (DLPC350_USB_IsConnected())
				DLPC350_USB_Close();
			bool blOpen = (DLPC350_USB_Open() == 0) ? true : false;
			return blOpen;
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector Open" << std::endl;
		}
		return false;
	}

	void CLightCraftProjector::Close()
	{
		try {
			if (DLPC350_USB_IsConnected())
			{
				DLPC350_USB_Close();
			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector Close" << std::endl;
		}
	}

	void CLightCraftProjector::SetMode(int mode)
	{
		try {
			switch (mode)
			{
			case Video:
			{
				DLPC350_SetPowerMode(0);
				DLPC350_SetMode(0);
			}break;
			case Pattern:
			{
				DLPC350_SetPowerMode(0);
				DLPC350_SetMode(1);
			}break;
			case Standby:
			{
				DLPC350_SetPowerMode(1);
			}break;
			default:
				break;
			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector SetMode" << std::endl;
		}
	}

	void CLightCraftProjector::SetPatternPeried(unsigned int trig, unsigned int exp)
	{
		m_TrigTime = trig;
		m_ExpTime = exp;
	}

	void CLightCraftProjector::SetDisplayPattern(unsigned char number)
	{
		try {
			DLPC350_SetInputSource(2, 0);
			DLPC350_LoadImageIndex(number);
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector SetDisplayPattern" << std::endl;
		}
	}

	void CLightCraftProjector::ClearLut()
	{
		try {
			DLPC350_ClearPatLut();
			m_numSplashLutEntries = 0;
			m_numLutEntries = 0;
			for (int i = 0; i < 64; i++)
			{
				m_splashLut[i] = 0;
			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector ClearLut" << std::endl;
		}
	}

	void CLightCraftProjector::AddPatternToSequence(unsigned char Splash_index, int PatNUM, int BITDPT)
	{
		try {
			if (PatNUM == 0)
			{
				DLPC350_AddToPatLut(0, PatNUM, BITDPT, m_LedColor, 0, 1, 1, 0);
				m_splashLut[m_numSplashLutEntries++] = Splash_index;
			}
			else {
				DLPC350_AddToPatLut(0, PatNUM, BITDPT, m_LedColor, 0, 1, 0, 0);
			}
			m_numLutEntries++;
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector AddPatternToSequence" << std::endl;
		}
	}

	void CLightCraftProjector::StartSequence(char onceornot)
	{
		try {
			DLPC350_SetPatternDisplayMode(0);
			if (onceornot)
				DLPC350_SetPatternConfig(m_numLutEntries, 1, m_numLutEntries, m_numSplashLutEntries);
			else
				DLPC350_SetPatternConfig(m_numLutEntries, 0, m_numLutEntries, m_numSplashLutEntries);

			DLPC350_SetExposure_FramePeriod(m_ExpTime, m_TrigTime);
			DLPC350_SetPatternTriggerMode(1);
			DLPC350_SendPatLut();
			DLPC350_SendImageLut(&m_splashLut[0], m_numSplashLutEntries);
			DLPC350_ValidatePatLutData(&m_status);
			//_sleep(200);
			DLPC350_PatternDisplay(2);
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector StartSequence" << std::endl;
		}
	}

	void CLightCraftProjector::Stop()
	{
		try {
			if (IsOpen())
			{
				DLPC350_PatternDisplay(1);
			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector Stop" << std::endl;
		}
	}

	bool CLightCraftProjector::SetIntensity(UINT uiIntensity)
	{
		try {
			if (IsOpen())
			{
				if (uiIntensity < 0 && uiIntensity>255)
				{
					return false;
				}
				DLPC350_SetLedCurrents(uiIntensity, uiIntensity, uiIntensity);
				return true;
			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector SetIntensity" << std::endl;
		}
		return false;
	}

	void CLightCraftProjector::SystemReset()
	{
		try {
			if (IsOpen()) {
				DLPC350_SoftwareReset();
			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector SystemReset" << std::endl;
		}
	}

	void CLightCraftProjector::ProjectCrossPatternContinue()
	{
		try {
			if (IsOpen())
			{
				SetMode(1);
				ClearLut();
				AddPatternToSequence(10, 0, 8);
				StartSequence(1);
			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector ProjectCrossPatternContinue" << std::endl;
		}
	}

	void CLightCraftProjector::ProjectScanImagesOnce()
	{
		try {
			if (IsOpen())
			{
				SetMode(1);
				ClearLut();


				AddPatternToSequence(1, 0, 8);

				AddPatternToSequence(1, 1, 8);
 
				AddPatternToSequence(1, 2, 8);
 
				AddPatternToSequence(2, 0, 8);
 
				AddPatternToSequence(2, 1, 8);
 
				AddPatternToSequence(2, 2, 8);

				AddPatternToSequence(3, 0, 8);

				AddPatternToSequence(3, 1, 8);

				AddPatternToSequence(3, 2, 8);

				AddPatternToSequence(4, 0, 8);

				AddPatternToSequence(4, 1, 8);

				AddPatternToSequence(4, 2, 8);

				AddPatternToSequence(9, 0, 8);

				StartSequence(0);

			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector ProjectScanImagesOnce" << std::endl;
		}
	}

	void CLightCraftProjector::ProjectBPScanImagesOnce()
	{
		try {
			if (IsOpen())
			{
				SetMode(1);
				ClearLut();

				AddPatternToSequence(1, 0, 8);
				AddPatternToSequence(1, 1, 8);
				AddPatternToSequence(1, 2, 8);
				AddPatternToSequence(2, 0, 8);

				AddPatternToSequence(2, 1, 8);
				AddPatternToSequence(2, 2, 8);
				AddPatternToSequence(3, 0, 8);
				AddPatternToSequence(3, 1, 8);

				AddPatternToSequence(3, 2, 8);
				AddPatternToSequence(4, 0, 8);
				AddPatternToSequence(4, 1, 8);
				AddPatternToSequence(4, 2, 8);

				AddPatternToSequence(5, 0, 8);
				AddPatternToSequence(5, 1, 8);
				AddPatternToSequence(5, 2, 8);
				AddPatternToSequence(6, 0, 8);

				AddPatternToSequence(6, 1, 8);
				AddPatternToSequence(6, 2, 8);
				AddPatternToSequence(7, 0, 8);
				AddPatternToSequence(7, 1, 8);

				AddPatternToSequence(7, 2, 8);
				AddPatternToSequence(8, 0, 8);
				AddPatternToSequence(8, 1, 8);
				AddPatternToSequence(8, 2, 8);

				AddPatternToSequence(9, 0, 8);

				StartSequence(0);
			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector ProjectBPScanImagesOnce" << std::endl;
		}
	}

	void CLightCraftProjector::ProjectWhiteImageContinue()
	{
		try {
			if (IsOpen())
			{
				SetMode(1);
				ClearLut();
				AddPatternToSequence(9, 0, 8);
				
				StartSequence(1);
			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector ProjectWhiteImageContinue" << std::endl;
		}
	}

	void CLightCraftProjector::ProjectWhiteImageOnce()
	{
		try {
			if (IsOpen())
			{
				SetMode(1);
				ClearLut();
				AddPatternToSequence(9, 0, 8);
				StartSequence(0);
			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector ProjectWhiteImageOnce" << std::endl;
		}
	}
	void CLightCraftProjector::ProjectSpeckleImageContinue()
	{
		try {
			if (IsOpen())
			{
				SetMode(1);
				ClearLut();
				AddPatternToSequence(0, 0, 8);

				StartSequence(1);
			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector ProjectWhiteImageContinue" << std::endl;
		}
	}
	void CLightCraftProjector::ProjectScanImages5678Once()
	{
		try {
			if (IsOpen())
			{
				SetMode(1);
				ClearLut();

				AddPatternToSequence(5, 0, 8);
				AddPatternToSequence(5, 1, 8);
				AddPatternToSequence(5, 2, 8);
				AddPatternToSequence(6, 0, 8);

				AddPatternToSequence(6, 1, 8);
				AddPatternToSequence(6, 2, 8);
				AddPatternToSequence(7, 0, 8);
				AddPatternToSequence(7, 1, 8);

				AddPatternToSequence(7, 2, 8);
				AddPatternToSequence(8, 0, 8);
				AddPatternToSequence(8, 1, 8);
				AddPatternToSequence(8, 2, 8);

				AddPatternToSequence(9, 0, 8);

				StartSequence(0);

			}
		}
		catch (...) {
			std::cout << "Error CLightCraftProjector ProjectScanImages5678Once" << std::endl;
		}
	}
	// The following is the added callback function
	void CLightCraftProjector::commandCallback(const std_msgs::Int32::ConstPtr& msg){
		ROS_INFO("Start Projector: %d", msg->data);
		// Check if msg is equal to 1
		if (msg->data == 1) {
			// Run ProjectScanImagesOnce()
			ProjectScanImagesOnce();
		}
	}
}