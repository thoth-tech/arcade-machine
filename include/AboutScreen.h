#ifndef ARCADE_MACHINE_ABOUT_SCREEN_H
#define ARCADE_MACHINE_ABOUT_SCREEN_H
#include <string>
#include <vector>
#include "splashkit.h"

struct s_star {
	double x;
	double y;
	double distance;
	color c;
};

class AboutScreen {
private:
	bool m_shouldQuit;
	unsigned long long m_ticker;
	std::string m_title;
	double m_titleX;
	double m_titleEnd;
	std::vector<struct s_star> m_stars;

	void readInput();
	void tick();
	void shiftTitle();
	void shiftStars();
	void render();
	void renderTitle();
	void renderStars();

public:
	AboutScreen();
	void loop();
};

#endif