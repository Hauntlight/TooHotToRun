#pragma once
#include <Windows.h>
#include <playsoundapi.h>
#include <string>

#pragma comment(lib, "winmm.lib")

//Wrapper della funzione PlaySound

class SoundManager {
public:

	bool esplosioneInCorso = false;

	void playMusicLoop(std::string s) {
		std::wstring stemp = std::wstring(s.begin(), s.end());
		PlaySound(stemp.c_str(), 0, SND_ASYNC | SND_LOOP);
	}

	void playMusic(std::string s) {
		if (esplosioneInCorso == false) {
			std::wstring stemp = std::wstring(s.begin(), s.end());
			PlaySound(stemp.c_str(), 0, SND_ASYNC);
			esplosioneInCorso = true;
		}

	}

	void stopMusic() {
		PlaySound(NULL, 0, 0);
	}


};