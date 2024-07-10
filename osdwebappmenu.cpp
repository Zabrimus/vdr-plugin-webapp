#include <iostream>
#include <vdr/plugin.h>
#include "osdwebappmenu.h"
#include "service/web_service.h"

cOsdWebAppMenu::cOsdWebAppMenu(const char *title, mINI::INIStructure& configuration, std::string relativeConfigDir) : cOsdMenu(title), configuration(configuration), configDir(relativeConfigDir) {
    for (auto const& it : configuration) {
        auto const& section = it.first;

        sections.push_back(section);

        Add( new cOsdWebAppMenuItem(section));
    }
}

cOsdWebAppMenu::~cOsdWebAppMenu() {

}

eOSState cOsdWebAppMenu::ProcessKey(eKeys key) {
    eOSState state = cOsdMenu::ProcessKey(key);

    if (state == osUnknown) {
        switch (key) {
            case kOk:
                processSection(sections[Current()]);
                return osEnd;

            default:
                break;
        }
    }
    return state;

}

void cOsdWebAppMenu::processSection(std::string &section) {
    dsyslog("Process Section %s\n", section.c_str());

    std::string appType = configuration[section]["app"];
    const char *m3uList;
    std::string url;
    std::string userAgent;
    std::string referrer;
    std::string cookie;

    auto p = cPluginManager::GetPlugin("web");
    if (!p) {
        esyslog("Plugin web cannot be found. Abort.");
        return;
    }

    if (appType == "main") {
        // create service object
        WebApp_Main_v1_0 sc;
        sc.section = section;
        sc.userAgent = configuration[section]["userAgent"];
        sc.referrer = configuration[section]["referrer"];
        sc.cookie = configuration[section]["cookie"];;

        // call service
        p->Service("WebApp-Main-v1.0", &sc);
    } else if (appType == "m3u") {
        // read the m3u file
        m3uList = configuration[section]["list"].c_str();

        // read content
        std::string filename;
        if (configDir.empty()) {
            filename = m3uList;
        } else {
            if (endsWith(configDir, "/")) {
                filename = configDir + m3uList;
            } else {
                filename = configDir + "/" + m3uList;
            }
        }

        std::ifstream ifs(filename);
        std::string content(std::istreambuf_iterator<char>{ifs}, {});

        // create service object
        WebApp_M3U_v1_0 sc;
        sc.section = section;
        sc.m3uContent = content;
        sc.userAgent = configuration[section]["userAgent"];
        sc.referrer = configuration[section]["referrer"];
        sc.cookie = configuration[section]["cookie"];;

        // call service
        p->Service("WebApp-M3U-v1.0", &sc);
    } else if (appType == "url") {
        // get url
        url = configuration[section]["url"];

        // create service object
        WebApp_Url_v1_0 sc;
        sc.section = section;
        sc.url = url;
        sc.userAgent = configuration[section]["userAgent"];
        sc.referrer = configuration[section]["referrer"];
        sc.cookie = configuration[section]["cookie"];;

        // call service
        p->Service("WebApp-Url-v1.0", &sc);
    } else {
        esyslog("[vdrwebapp] Section %s contains unknown app '%s'", section.c_str(), appType.c_str());
        return;
    }
}

cOsdWebAppMenuItem::cOsdWebAppMenuItem(std::string string) {
    SetText(string.c_str(), true);
}

cOsdWebAppMenuItem::~cOsdWebAppMenuItem() {

}
