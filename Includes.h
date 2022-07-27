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
#include "gridlayout.h"
#include "ConfigData.h"
#include "ButtonList.h"
#include "selector.h"
#include "tip.h"
#include "splashscreen.h"
#include "menu.h"
#include "Helper.h"
#include "option.h"
#include "ArcadeMachine.h"

#endif