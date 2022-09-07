#ifndef ARCADE_MACHINE_ABOUT_SCREEN_H
#define ARCADE_MACHINE_ABOUT_SCREEN_H
#include <string>

class AboutScreen {
private:
	bool m_shouldQuit;
	unsigned long long m_ticker;
	std::string m_title;
	double m_titleX;
	double m_titleEnd;

	void readInput();
	void tick();
	void shiftTitle();
	void render();
	void renderTitle();

public:
	AboutScreen();
	void loop();
};

#endif