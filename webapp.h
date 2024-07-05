/*
 * webapp.h: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#pragma once

#include <vdr/plugin.h>

#define MINI_CASE_SENSITIVE
#include "mini/ini.h"

static const char *VERSION = "0.0.1";
static const char *DESCRIPTION = "Start specific Apps or load URLs using the web plugin";
static const char *MAINMENUENTRY = "Webapp";
static char* MAINMENUENTRYALT = nullptr;

class cPluginWebapp : public cPlugin {
        private:
        mINI::INIStructure configuration;

        public:
        cPluginWebapp();
        ~cPluginWebapp() override;
        const char *Version() override { return VERSION; }
        const char *Description() override;
        const char *CommandLineHelp() override;
        bool ProcessArgs(int argc, char *argv[]) override;
        bool Initialize() override;
        bool Start() override;
        void Stop() override;
        void Housekeeping() override;
        void MainThreadHook() override;
        cString Active() override;
        time_t WakeupTime() override;
        const char *MainMenuEntry() override { return MAINMENUENTRYALT != nullptr ? MAINMENUENTRYALT : MAINMENUENTRY; }
        cOsdObject *MainMenuAction() override;
        cMenuSetupPage *SetupMenu() override;
        bool SetupParse(const char *Name, const char *Value) override;
        bool Service(const char *Id, void *Data) override;
        const char **SVDRPHelpPages() override;
        cString SVDRPCommand(const char *Command, const char *Option, int &ReplyCode) override;
};
