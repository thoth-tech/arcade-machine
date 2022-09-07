#include "AboutScreen.h"
#include "Configuration.h"
#include "splashkit.h"
#include <string>
#include <cmath>

#define TITLE_FONT_SIZE 44
#define TITLE_FONT_CHAR_WIDTH 32
#define TITLE_FONT_CHAR_HEIGHT 32
#define TITLE_FONT_Y ((((ARCADE_MACHINE_RES_Y / 2) + (TITLE_FONT_CHAR_HEIGHT / 2))) / 3)

const char *title = "About The Thoth Tech Arcade Machine!";

AboutScreen::AboutScreen() {
	this->m_shouldQuit = false;
	this->m_titleX = ARCADE_MACHINE_RES_X;
	this->m_title = std::string(title);
	this->m_titleEnd = ((TITLE_FONT_CHAR_WIDTH * this->m_title.length()));
	this->m_titleEnd *= -1;
}

void AboutScreen::readInput() {
	if (quit_requested() || key_down(ESCAPE_KEY))
		this->m_shouldQuit = true;
}

void AboutScreen::tick() {
	this->shiftTitle();

	this->m_ticker++;
}

void AboutScreen::render() {
	clear_screen(COLOR_BLACK);

	this->renderTitle();

	refresh_screen();
}

void AboutScreen::shiftTitle() {
	this->m_titleX -= 6;

	if (this->m_titleX < this->m_titleEnd)
		this->m_titleX = ARCADE_MACHINE_RES_X;
}

void AboutScreen::renderTitle() {
	double x = this->m_titleX;
	static font f = font_named("upheavtt.ttf");

	for (int i=0; i<this->m_title.length(); ++i) {
		double y = TITLE_FONT_Y + (sin(x / 80) * 115);
		double fontSize = TITLE_FONT_SIZE + (sin(x / 120) * 12);
		color c;
		c.r = sin(i + 0) * 127 + ((sin(x / 80)) * 1.28);
		c.g = sin(i + 2) * 127 + ((sin(x / 80)) * 1.28);
		c.b = sin(i + 4) * 127 + ((sin(x / 80)) * 1.28);


		// Underlay.
		draw_text(
			this->m_title.substr(i, 1), 
			COLOR_GRAY,
			f,
			fontSize,
			x + 2,
			y + 2
		);	

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

void AboutScreen::loop() {
	while (! quit_requested()) {
		process_events();

		this->readInput();
		this->tick();
		this->render();

		delay(1000 / 60);
	}
}