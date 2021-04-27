#include <iostream>
#include <Windows.h>
#include "escapi.h"

void InitEscapi(int &Cameras, SimpleCapParams &capture) {
	Cameras = setupESCAPI();
	if (Cameras == 0) {
		throw "No cameras";
	}

	capture.mWidth = 800;
	capture.mHeight = 600;
	capture.mTargetBuf = new int[800 * 600];

	if (initCapture(0, &capture) == 0) {
		throw "Could not capture";
	}
}

void SwitchTab() {
	INPUT inputs[4];
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_LWIN;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_TAB;

	inputs[2].type = INPUT_KEYBOARD;
	inputs[2].ki.wVk = VK_TAB;
	inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

	inputs[3].type = INPUT_KEYBOARD;
	inputs[3].ki.wVk = VK_LWIN;
	inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

int main() {

	union RGBint {
		int rgb;
		unsigned char c[4];
	};

	int Cameras = 0;
	SimpleCapParams capture;

	float* NewFarme = nullptr;
	float* OldFrame = nullptr;
	float* NewFilterd = nullptr;
	float* OldFilterd = nullptr;

	int frames = 0;
	float total = 0;

	NewFarme = new float[800 * 600];
	OldFrame = new float[800 * 600];
	NewFilterd = new float[800 * 600];
	OldFilterd = new float[800 * 600];

	memset(NewFarme, 0, sizeof(float) * 600 * 800);
	memset(OldFrame, 0, sizeof(float) * 600 * 800);
	memset(NewFilterd, 0, sizeof(float) * 600 * 800);
	memset(OldFilterd, 0, sizeof(float) * 600 * 800);

	InitEscapi(Cameras, capture);

	while (true) {
		doCapture(0); while (isCaptureDone(0) == 0) {}

		for (int i = 0; i < capture.mWidth; i++) {
			for (int j = 0; j < capture.mHeight; j++) {
				int index = j * capture.mWidth + i;
				RGBint colour;
				colour.rgb = capture.mTargetBuf[index];

				OldFrame[index] = NewFarme[index];
				OldFilterd[index] = NewFilterd[index];

				float lumanace = (colour.c[3] / 255.0f) * 0.3 + (colour.c[2] / 255.0f) * 0.59 + (colour.c[1] / 255.0f) * 0.8;
				NewFarme[index] = lumanace;

				NewFilterd[index] += (NewFarme[index] - NewFilterd[index]) * 0.3f;

				float diff = fabs(NewFilterd[index] - OldFilterd[index]);
				float value = diff > 0.05f ? diff : 0.0f;
				total += value;
			}
		}

		if (total > 3000.0f && frames >= 5) { SwitchTab(); std::cout << "Move "; }
		if (frames <= 5) frames++;
		total = 0;
	}
}

//#include <iostream>
//
//#define OLC_PGE_APPLICATION
//#include "olcPixelGameEngine.h"
//
//#include "escapi.h"
//
//class Engine : public olc::PixelGameEngine
//{
//public:
//	Engine()
//	{
//		sAppName = "Motion Detector";
//	}
//
//private:
//	union RGBint {
//		int rgb;
//		unsigned char c[4];
//	};
//
//	int Cameras = 0;
//	SimpleCapParams capture;
//
//	float* NewFarme = nullptr;
//	float* OldFrame = nullptr;
//	float* NewFilterd = nullptr;
//	float* OldFilterd = nullptr;
//
//	int frames = 0;
//	float total = 0;
//
//public:
//	bool OnUserCreate() override
//	{
//		Cameras = setupESCAPI();
//
//		if (Cameras == 0) {
//			std::cout << "No cameras";
//			return false;
//		}
//
//		capture.mWidth = ScreenWidth();
//		capture.mHeight = ScreenHeight();
//		capture.mTargetBuf = new int[ScreenWidth() * ScreenHeight()];
//
//		NewFarme = new float[ScreenWidth() * ScreenHeight()];
//		OldFrame = new float[ScreenWidth() * ScreenHeight()];
//		NewFilterd = new float[ScreenWidth() * ScreenHeight()];
//		OldFilterd = new float[ScreenWidth() * ScreenHeight()];
//
//		memset(NewFarme, 0, sizeof(float) * ScreenWidth() * ScreenHeight());
//		memset(OldFrame, 0, sizeof(float) * ScreenWidth() * ScreenHeight());
//		memset(NewFilterd, 0, sizeof(float) * ScreenWidth() * ScreenHeight());
//		memset(OldFilterd, 0, sizeof(float) * ScreenWidth() * ScreenHeight());
//
//		if (initCapture(0, &capture) == 0) {
//			std::cout << "Could not capture";
//			return false;
//		}
//		return true;
//	}
//
//	bool OnUserUpdate(float fElapsedTime) override
//	{
//		doCapture(0); while (isCaptureDone(0) == 0) {}
//
//		for (int i = 0; i < capture.mWidth; i++) {
//			for (int j = 0; j < capture.mHeight; j++) {
//				int index = j * capture.mWidth + i;
//				RGBint colour;
//				colour.rgb = capture.mTargetBuf[index];
//
//				OldFrame[index] = NewFarme[index];
//				OldFilterd[index] = NewFilterd[index];
//
//				float lumanace = (colour.c[3] / 255.0f) * 0.3 + (colour.c[2] / 255.0f) * 0.59 + (colour.c[1] / 255.0f) * 0.11;
//				NewFarme[index] = lumanace;
//
//				NewFilterd[index] += (NewFarme[index] - NewFilterd[index]) * 0.3f;
//
//				float diff = fabs(NewFilterd[index] - OldFilterd[index]);
//				float value = diff > 0.05f ? diff : 0.0f;
//				total += value;
//				Draw(i, j, olc::Pixel(value * 255.0f, value * 255.0f, value * 255.0f));
//			}
//		}
//
//		if (total > 1500.0f && frames >= 5)
//			std::cout << "Move";
//		if (frames <= 5) frames++;
//		total = 0;
//
//		return true;
//	}
//};
//
//int main()
//{
//	Engine engine;
//	if (engine.Construct(800, 600, 1, 1))
//		engine.Start();
//	return 0;
//}