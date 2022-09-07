#include "AboutScreen.h"
#include "Configuration.h"
#include "splashkit.h"
#include <string>
#include <cmath>
#include <vector>

#define TITLE_FONT_SIZE 44
#define TITLE_FONT_CHAR_WIDTH 32
#define TITLE_FONT_CHAR_HEIGHT 32
#define TITLE_FONT_Y ((((ARCADE_MACHINE_RES_Y / 2) + (TITLE_FONT_CHAR_HEIGHT / 2))) / 3)
#define STAR_COUNT 1024
#define DISTANCE_SHIFT 10

#define CONTRIBUTION_TIME (60 * 10)

static const char *title = "About The Thoth Tech Arcade Machine!";

static const char *description[] =  {
	"This arcade machine has been created",
	"by students undertaking capstone units",
	"in the School of Information Technology",
	"as a platform to designed to showcase",
	"games built by students using SplashKit."
};

static const char *contributors[] = {
	"127  Anthony George",
    "78  Richard Denton",
    "76  lachfoy",
    "67  Riley Dellios",
    "36  Sarah Gosling",
    "16  Delcari",
    "10  zbrydon",
    "8  studioant",
    "5  quoch",
    "4  huydnnk97",
    "2  Morgaine",
    "1  sarahgos"
};

static font f = font_named("edunline.ttf");
static font fontDescription = font_named("PressStart2P.ttf");

AboutScreen::AboutScreen() {
	this->m_shouldQuit = false;
	this->m_titleX = ARCADE_MACHINE_RES_X;
	this->m_title = std::string(title);
	this->m_titleEnd = ((TITLE_FONT_CHAR_WIDTH * this->m_title.length()));
	this->m_titleEnd *= -1;
	this->m_stars = std::vector<struct s_star>();
	this->m_contributorsIndex = 0;
	this->m_contributorTicker = 0;

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

}

void AboutScreen::readInput() {
	if (quit_requested() || key_down(ESCAPE_KEY))
		this->m_shouldQuit = true;
}

void AboutScreen::tick() {
	this->shiftTitle();
	this->shiftStars();
	this->tickContributor();

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

void AboutScreen::renderTitle() {
	double x = this->m_titleX;

	for (int i=0; i<this->m_title.length(); ++i) {
		double y = TITLE_FONT_Y + (sin(x / 80) * 115);
		double fontSize = TITLE_FONT_SIZE + (sin(x / 120) * 12);

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

		draw_text(
			this->m_title.substr(i, 1), 
			c,
			f,
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
	double offset = sin(((double)this->m_ticker / 16)) * 10;
	double offsetX = sin((double)this->m_ticker / 32) * 6;
	double y = 520 + offset;
	double x = 50 + offsetX;
	for (int i=0; i<sizeof(description) / sizeof(description[0]); ++i) {
		draw_text(description[i], COLOR_WHITE, fontDescription, 16, x, y + (i * 32));
	}
}

void AboutScreen::loop() {
	while (! quit_requested()) {
		process_events();

		this->readInput();
		this->tick();
		this->render();

		delay(1000 / 60);
	}
}


void AboutScreen::tickContributor() {
	this->m_contributorTicker++;
	if (this->m_contributorTicker >= CONTRIBUTION_TIME) {
		this->m_contributorTicker = 0;
		this->m_contributorsIndex = (this->m_contributorsIndex + 1) % (sizeof(contributors) / sizeof(contributors[0]));
	}
}

void AboutScreen::renderContributor() {
	color c;
	draw_text(contributors[this->m_contributorsIndex], COLOR_WHITE, fontDescription, 24, 400, 400);
}