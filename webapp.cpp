/*
 * webapp.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include <getopt.h>
#include "webapp.h"
#include "osdwebappmenu.h"

std::string configFile;
std::string relativeConfigDir;

cPluginWebapp::cPluginWebapp() {
    // Initialize any member variables here.
    // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
    // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
}

cPluginWebapp::~cPluginWebapp() {
    // Clean up after yourself!
}

const char *cPluginWebapp::CommandLineHelp() {
    return "  -c file,     --config=file configuration file\n"
           "  -d directory --config-dir  path to the configuration directory.\n"
           "                             All configuration files are searched "
           "                             relativ to this directory."
           "  -n,          --name        Menu entry name\n";}

bool cPluginWebapp::ProcessArgs(int argc, char *argv[]) {
    static struct option long_options[] = {
            {"config",       required_argument, nullptr, 'c'},
            {"--name",       optional_argument, nullptr, 'n'},
            {"--config-dir", required_argument, nullptr, 'd'},
            {nullptr}
    };

    int c, option_index = 0;
    while ((c = getopt_long(argc, argv, "c:n:d:", long_options, &option_index)) != -1) {
        if (c == 'c') {
            configFile = std::string(optarg);
        } else if (c == 'n') {
            MAINMENUENTRYALT = strdup(optarg);
        } else if (c == 'd') {
            relativeConfigDir = std::string(optarg);
        }
    }

    // read the configuration ini file
    std::string filename;

    if (relativeConfigDir.empty()) {
        filename = configFile;
    } else {
        if (endsWith(relativeConfigDir, "/")) {
            filename = relativeConfigDir + configFile;
        } else {
            filename = relativeConfigDir + "/" + configFile;
        }
    }

    mINI::INIFile file(filename);
    bool result = file.read(configuration);

    if (!result) {
        esyslog("Unable to read configuration file: %s", optarg);
        return false;
    }

    return true;
}

bool cPluginWebapp::Initialize() {
    return true;
}

bool cPluginWebapp::Start() {
    // Start any background activities the plugin shall perform.
    return true;
}

void cPluginWebapp::Stop() {
    // Stop any background activities the plugin is performing.
}

void cPluginWebapp::Housekeeping() {
    // Perform any cleanup or other regular tasks.
}

void cPluginWebapp::MainThreadHook() {
    // Perform actions in the context of the main program thread.
    // WARNING: Use with great care - see PLUGINS.html!
}

cString cPluginWebapp::Active() {
    // Return a message string if shutdown should be postponed
    return nullptr;
}

time_t cPluginWebapp::WakeupTime() {
    // Return custom wakeup time for shutdown script
    return 0;
}

cOsdObject *cPluginWebapp::MainMenuAction() {
    return new cOsdWebAppMenu("Verfügbare Apps", configuration, relativeConfigDir);
}

cMenuSetupPage *cPluginWebapp::SetupMenu() {
    // Return a setup menu in case the plugin supports one.
    return nullptr;
}

bool cPluginWebapp::SetupParse(const char *Name, const char *Value) {
    // Parse your own setup parameters and store their values.
    return false;
}

bool cPluginWebapp::Service(const char *Id, void *Data) {
    // Handle custom service requests from other plugins
    return false;
}

const char **cPluginWebapp::SVDRPHelpPages() {
    // Return help text for SVDRP commands this plugin implements
    return nullptr;
}

cString cPluginWebapp::SVDRPCommand(const char *Command, const char *Option, int &ReplyCode) {
// Process SVDRP commands this plugin implements
return nullptr;
}

const char *cPluginWebapp::Description() { return DESCRIPTION; }

VDRPLUGINCREATOR(cPluginWebapp); // Don't touch this!
