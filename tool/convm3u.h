#pragma once

#include <string>
#include <vector>
#include <set>

struct m3uEntry {
    std::string tvgName;
    std::string tvgId;
    std::string tvgLogo;
    std::string groupTitle;
    std::string m3uTitle;
    std::string m3uUrl;
};

class ConvM3U {
public:
    ConvM3U(std::string inputFileName, std::string outputFileName);
    ~ConvM3U();

    void convert();

private:
    std::string createMenuItems(const std::set<std::string>& groups);
    std::string createChannelItems(const std::vector<m3uEntry>& m3u, const std::string& group);
    std::string createChannelItem(const m3uEntry& entry);

    std::string createCompleteJson(const std::set<std::string>& groups, const std::vector<m3uEntry>& m3u, const std::string& menuItems);
    unsigned long countGroups(const std::vector<m3uEntry>& m3u, const std::string& group);

private:
    std::string m3uContent;
    std::string outputFileName;
    std::string inputFileName;
};