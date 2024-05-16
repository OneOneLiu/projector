#ifndef LIGHTCRAFTPROJECTOR_H
#define LIGHTCRAFTPROJECTOR_H

//#include "API.h"
//#include "usb.h"
#include "dlpc350_common.h"
#include "dlpc350_api.h"
#include "dlpc350_usb.h"
#include <stdlib.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"

typedef unsigned int UINT;

namespace amnl
{
	enum Mode
	{
		Video, Pattern, Standby
	};

	class CLightCraftProjector
	{
	public:
		CLightCraftProjector(void);
		~CLightCraftProjector(void);

	public:
		bool SetFrameParam(unsigned int exp, unsigned int trig);
		bool SetIntensity(UINT uiIntensity);
		void SystemReset();
		void ProjectCrossPatternContinue();
		void ProjectScanImagesOnce();
		void ProjectBPScanImagesOnce();
		void ProjectWhiteImageContinue();
		void ProjectWhiteImageOnce();

		void ProjectSpeckleImageContinue();//ͶӰɢ��
		void ProjectScanImages5678Once();	//ͶӰ5678����
		void commandCallback(const std_msgs::Int32::ConstPtr& msg);

		void Stop();
		bool IsOpen();
		void SetLedColor(int color) {
			m_LedColor = color;
		}
	public:
		bool Open();
		void Close();

		void SetDisplayPattern(unsigned char number); //����ͶӰͼ�������ţ�
		void SetMode(int mode); //���ù���ģʽ��ͶӰͼ������Ƶ��������
		void AddPatternToSequence(unsigned char Splash_index, int PatNUM, int BITDPT); //����ͶӰ����
		void SetPatternPeried(unsigned int trig, unsigned int exp); //����ͶӰ��������������ͶӰʱ�䣩
		void StartSequence(char onceornot); //��ʼ����ͶӰ
		void ClearLut(); //��������

	public:
		//bool m_bOpen;
		unsigned char m_splashLut[64];
		unsigned int m_status;
		unsigned int m_TrigTime = 1000000;
		unsigned int m_ExpTime = 1000000;
		int m_numLutEntries;
		int m_numSplashLutEntries;
		int m_num_pats_in_exposure;
		/*
		Choose the LEDs that are on : b0 = Red, b1 = Green, b2 = Blue
			* 0 = No LED(Pass Through)
			* 1 = Red
			* 2 = Green
			* 3 = Yellow(Green + Red)
			* 4 = Blue
			* 5 = Magenta(Blue + Red)
			* 6 = Cyan(Blue + Green)
			* 7 = White(Red + Blue + Green)
		*/
		int m_LedColor = 4;
	};

	typedef CLightCraftProjector CProjectorT;
}
#endif