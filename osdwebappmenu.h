//
// Created by rh on 25.06.24.
//

#pragma once

#include <vdr/osd.h>
#include <vdr/menu.h>

#define MINI_CASE_SENSITIVE
#include "mini/ini.h"

inline bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
}

class cOsdWebAppMenu : public cOsdMenu {
private:
    mINI::INIStructure& configuration;
    std::vector<std::string> sections;

    std::string configDir;

    void processSection(std::string& section);

public:
    cOsdWebAppMenu(const char *title,  mINI::INIStructure& configuration, std::string relativeConfigDir);
    ~cOsdWebAppMenu();

    eOSState ProcessKey(eKeys key) override;
};

class cOsdWebAppMenuItem : public cOsdItem {
private:

public:
    explicit cOsdWebAppMenuItem(std::string string);
    ~cOsdWebAppMenuItem() override;
};

