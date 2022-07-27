#ifndef INCLUDE_H
#define INCLUDE_H

#ifdef _WIN32
#define ARCADE_MACHINE_PATH_SEP "\\"
#else
#define ARCADE_MACHINE_PATH_SEP "/"
#endif

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
#include <Windows.h>

// Custom classes
#include "buttons.h"
#include "gridlayout.h"
#include "cdata.h"
#include "ButtonList.h"
#include "selector.h"
#include "tip.h"
#include "splashscreen.h"
#include "menu.h"
#include "helper.h"
#include "option.h"
#include "arcademachine.h"

#endif