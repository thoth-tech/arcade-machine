#ifndef INCLUDE_H
#define INCLUDE_H

// Includes
#include <stdlib.h>
#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <experimental/filesystem>
#include <regex>
#include "splashkit.h"
#include <string>
#include <chrono>
//#include <json/json.h>

#ifdef _WIN32
#include <Windows.h>
#endif

// Custom classes
#include "Button.h"
#include "GridLayout.h"
#include "ConfigData.h"
#include "ButtonList.h"
#include "Selector.h"
#include "Tip.h"
#include "Splashscreen.h"
#include "Menu.h"
#include "Helper.h"
#include "Option.h"
#include "ArcadeMachine.h"

#endif