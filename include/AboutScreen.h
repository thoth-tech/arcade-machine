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
	int m_contributorsIndex;
	int m_contributorTicker;
	std::vector<std::string> m_contributors;
	std::vector<std::string> m_linesOfCode;
	std::vector<std::string> m_gitContributions;

	void readInput();
	void tick();
	void shiftTitle();
	void shiftStars();
	void render();
	color getRainbowShade(double x);
	void renderTitle();
	void renderStars();
	void renderDescription();

	void tickContributor();
	void renderContributor();
	void onExit();
	void loop();

public:
	AboutScreen();
	void main();
};

#endif