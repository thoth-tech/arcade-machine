#include "AboutScreen.h"
#include "Configuration.h"
#include "splashkit.h"
#include <string>
#include <cmath>
#include <vector>
#include <fstream>

#define TITLE_FONT_SIZE 38
#define TITLE_FONT_CHAR_WIDTH 32
#define TITLE_FONT_CHAR_HEIGHT 32
#define TITLE_FONT_Y ((((ARCADE_MACHINE_RES_Y / 2) + (TITLE_FONT_CHAR_HEIGHT / 2))) / 3)
#define STAR_COUNT 1024
#define DISTANCE_SHIFT 10

#define CONTRIBUTION_TIME (60 * 3)
#define CONTRIBUTION_FADE_TIME 30

static const char *title = "About The Thoth Tech Arcade Machine!";
static const char *description[] =  {
	"This arcade machine has been created",
	"by students undertaking capstone units",
	"in the School of Information Technology",
	"as a platform to designed to showcase",
	"games built by students using SplashKit.",
	"",
	"Lines of code"
};

static const std::string createdBy = "Created by";

AboutScreen::AboutScreen() {
	this->m_shouldQuit = false;
	this->m_titleX = ARCADE_MACHINE_RES_X;
	this->m_title = std::string(title);
	this->m_titleEnd = ((TITLE_FONT_CHAR_WIDTH * this->m_title.length()));
	this->m_titleEnd *= -1;
	this->m_stars = std::vector<struct s_star>();
	this->m_contributorsIndex = 0;
	this->m_contributorTicker = 0;
	this->m_ticker = 0;

	for (int i=0; i<STAR_COUNT; ++i) {
		struct s_star star;
		star.x = (rand() % (ARCADE_MACHINE_RES_X - 0 + 1)  + 0);
		star.y = (rand() % (ARCADE_MACHINE_RES_Y - 0 + 1) + 0);
		star.distance = (rand() % (DISTANCE_SHIFT - 1 + 1) + 1);

		double brightness = (rand() % (80 - 40) + 40);
		brightness /= 100;

		star.c.a = brightness;
		star.c.r = 1;
		star.c.g = 1;
		star.c.b = 1;

		this->m_stars.push_back(star);
	}

	this->m_contributors = std::vector<std::string>();
	std::ifstream contributors("stats" ARCADE_MACHINE_PATH_SEP "contributors.txt");
	std::string line;
	while (std::getline(contributors, line)) {
		if (line.length() > 0)
			this->m_contributors.push_back(line);
	}
	contributors.close();

	this->m_linesOfCode = std::vector<std::string>();
	std::ifstream linesOfCode("stats" ARCADE_MACHINE_PATH_SEP "lines-of-code.txt");
	while (std::getline(linesOfCode, line)) {
		if (line.length() > 0)
			this->m_linesOfCode.push_back(line);
	}

	this->m_gitContributions = std::vector<std::string>();
	std::ifstream contributions("stats" ARCADE_MACHINE_PATH_SEP "git.txt");
	while (std::getline(contributions, line)) {
		if (line.length() > 0)
			this->m_gitContributions.push_back(line);
	}

}

void AboutScreen::onExit() {
	// if (music_playing())
	// 	stop_music();
}

void AboutScreen::readInput() {
	if (quit_requested() || key_down(ESCAPE_KEY))
		this->m_shouldQuit = true;
}

void AboutScreen::tick() {
	this->shiftTitle();
	this->shiftStars();
	this->tickContributor();

	// Every 1/4 second.
	// if (this->m_ticker % 15 == 0) {
	// 	if (! music_playing())
	// 		play_music("music_about");
	// }

	this->m_ticker++;
}

void AboutScreen::render() {
	clear_screen(COLOR_BLACK);

	this->renderStars();
	this->renderTitle();
	this->renderDescription();
	this->renderContributor();

	refresh_screen();
}

void AboutScreen::shiftTitle() {
	this->m_titleX -= 6;

	if (this->m_titleX < this->m_titleEnd)
		this->m_titleX = ARCADE_MACHINE_RES_X;
}

color AboutScreen::getRainbowShade(double x) {
	color c;
	double rd = (double)ARCADE_MACHINE_RES_X / 16;
	double gd = (double)ARCADE_MACHINE_RES_X / 10;
	double bd = (double)ARCADE_MACHINE_RES_X / 6;
	double p = sin(x / rd) * 0.5;
	double g = sin(x / gd) * 0.5;
	double b = sin(x / bd) * 0.5;

	c.r = 0.5 + p;
	c.g = 0.5 + g;
	c.b = 0.5 + b;

	return c;
}

void AboutScreen::renderTitle() {
	double x = this->m_titleX;

	for (int i=0; i<this->m_title.length(); ++i) {
		double y = TITLE_FONT_Y + (sin(x / 80) * 115);
		double fontSize = TITLE_FONT_SIZE + (sin(x / 120) * 8);

		color c = this->getRainbowShade(x);

		draw_text(
			this->m_title.substr(i, 1), 
			c,
			"font_about",
			fontSize,
			x,
			y
		);	

		x += TITLE_FONT_CHAR_WIDTH;
	}
}

void AboutScreen::shiftStars() {
	for (int i=0; i<this->m_stars.size(); ++i) {
		this->m_stars[i].x += this->m_stars[i].distance;
		if (this->m_stars[i].x > ARCADE_MACHINE_RES_X)
			this->m_stars[i].x = -10;
	}
}

void AboutScreen::renderStars() {
	for (auto star : this->m_stars)
		fill_rectangle(star.c, star.x, star.y, star.distance / 2, star.distance / 4);
}

void AboutScreen::renderDescription() {
	double offset = sin(((double)this->m_ticker / 16)) * 12;
	double offsetX = sin((double)this->m_ticker / 24) * 19;
	double y = 480 + offset;
	double x = 140 + offsetX;
	int maxIOffset = 0;
	for (int i=0; i<sizeof(description) / sizeof(description[0]); ++i) {
		maxIOffset = (i * 32);
		draw_text(description[i], COLOR_WHITE, "font_about", 18, x, y + maxIOffset);
	}

	y = y + maxIOffset + 32;
	for (int i=0; i<this->m_linesOfCode.size(); ++i) {
		maxIOffset = (i * 32);
		draw_text(this->m_linesOfCode[i], COLOR_WHITE, "font_about", 18, x, y + (maxIOffset));
	}

	y = y + maxIOffset + 64;
	draw_text("Contributions", COLOR_WHITE, "font_about", 18, x, y);
	y += 32;

	for (int i=0; i<this->m_gitContributions.size(); ++i) {
		maxIOffset = (i * 32);
		draw_text(this->m_gitContributions[i], COLOR_WHITE, "font_about", 18, x, y + (maxIOffset));
	}

}

void AboutScreen::loop() {
	while (! this->m_shouldQuit) {
		process_events();

		this->readInput();
		this->tick();
		this->render();

		delay(1000 / 60);
	}

}


void AboutScreen::main() {
	// Clear music and start the about screen music.
	// stop_music();
	// play_music("music_about");

	this->loop();

	// Tidy up once the loop is done.
	this->onExit();
}

void AboutScreen::tickContributor() {
	this->m_contributorTicker++;
	if (this->m_contributorTicker >= CONTRIBUTION_TIME) {
		this->m_contributorTicker = 0;
		this->m_contributorsIndex = (this->m_contributorsIndex + 1) % this->m_contributors.size();
	}
}

void AboutScreen::renderContributor() {
	int r = this->m_contributorTicker % CONTRIBUTION_TIME;
	double ratio = 1;
	double fontSize = 32;
	double fontRatio = 1;

	if (r < CONTRIBUTION_FADE_TIME) {
		ratio = r / (double)CONTRIBUTION_FADE_TIME;
		fontRatio = 1 + ((1 - ratio) * 4);
	} else if ((CONTRIBUTION_TIME - r) < CONTRIBUTION_FADE_TIME) {
		ratio = (CONTRIBUTION_TIME - r) / (double)CONTRIBUTION_FADE_TIME;
		fontRatio = ratio;
	}

	fontSize = fontSize * fontRatio;
	if (ratio > 1.0)
		ratio = 1.0;

	color c;
	c.r = ratio;
	c.g = ratio;
	c.b = ratio;


	draw_text(this->m_contributors[this->m_contributorsIndex], c, "font_about", fontSize, 1100, 600);

	double x = 0;
	double y = 0;
	fontSize = 24;
	for (int i=0; i<createdBy.length(); ++i) {
		y = sin((this->m_ticker + (i * 4)) / (double)16) * (double)9;
		x = sin(this->m_ticker / (double) 50) * (double)225;
		double actualX = 1300 + x + (i * 28);

		fontRatio = sin((actualX + 190) / (double)80) * (double)1.5;
		if (fontRatio < 1)
			fontRatio = (double)1;

		color c = this->getRainbowShade(actualX - 350);
		draw_text(createdBy.substr(i, 1), c, "font_about", (double)24 * fontRatio, actualX, (double)500 + y);
	}
}