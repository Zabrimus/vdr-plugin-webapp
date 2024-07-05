//
// Created by rh on 25.06.24.
//

#pragma once

#include <vdr/osd.h>
#include <vdr/menu.h>

#define MINI_CASE_SENSITIVE
#include "mini/ini.h"

class cOsdWebAppMenu : public cOsdMenu {
private:
    mINI::INIStructure& configuration;
    std::vector<std::string> sections;

    void processSection(std::string& section);

public:
    cOsdWebAppMenu(const char *title,  mINI::INIStructure& configuration);
    ~cOsdWebAppMenu();

    eOSState ProcessKey(eKeys key) override;
};

class cOsdWebAppMenuItem : public cOsdItem {
private:

public:
    explicit cOsdWebAppMenuItem(std::string string);
    ~cOsdWebAppMenuItem() override;
};

