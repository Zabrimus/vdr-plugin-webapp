#include <regex>
#include <sstream>
#include <iostream>
#include <set>
#include "m3upipe.h"
#include "convm3u.h"

// all desired values
std::regex regs[] = {
        std::regex(".*(tvg-name)=\"(.*?)\".*"),
        std::regex(".*(tvg-id)=\"(.*?)\".*"),
        std::regex(".*(tvg-logo)=\"(.*?)\".*"),
        std::regex(".*(group-title)=\"(.*?)\".*"),
        std::regex(".*(m3u-title)=\"(.*?)\".*"),
        std::regex(".*(m3u-url)=\"(.*?)\".*")
};


ConvM3U::ConvM3U(std::string inputFileName, std::string outputFileName) : outputFileName(outputFileName), inputFileName(inputFileName) {
}

ConvM3U::~ConvM3U() {
}

void ConvM3U::convert() {
    FILE* input = fopen(inputFileName.c_str(), "rb");
    if (input == nullptr) {
        printf("Unable to open input file %s\n", inputFileName.c_str());
        exit(1);
    }

    FILE* output = fopen(outputFileName.c_str(), "wb");
    if (output == nullptr) {
        printf("Unable to open output file %s\n", outputFileName.c_str());
        exit(1);
    }

    printf("Process m3u file...\n");

    m3uContent = processM3U(input);
    fclose(input);

    printf("Extract data...\n");

    std::set<std::string> groups;
    std::vector<m3uEntry> entries;

    std::istringstream inputStream{m3uContent};
    std::string line;
    while(std::getline(inputStream, line)) {
        m3uEntry entry;

        // extract interesting values per line
        for (const auto &r: regs) {
            std::smatch match;
            if (std::regex_search(line, match, r)) {
                std::string key = match[1].str();
                std::string value = match[2].str();

                if (key == "tvg-name") {
                    entry.tvgName = value;
                } else if (key == "tvg-id") {
                    entry.tvgId = value;
                } else if (key == "tvg-logo") {
                    entry.tvgLogo = value;
                } else if (key == "group-title") {
                    entry.groupTitle = value;
                } else if (key == "m3u-title") {
                    entry.m3uTitle = value;
                } else if (key == "m3u-url") {
                    entry.m3uUrl = value;
                }
            }
        }

        if (entry.groupTitle.empty()) {
            entry.groupTitle = "unknown";
        }

        if (entry.m3uTitle.empty() && entry.tvgName.empty()) {
            // ignore this entry
            continue;
        }

        groups.insert(entry.groupTitle);

        entries.push_back(entry);
    }

    printf("Create Menu items...\n");
    std::string menuItems = createMenuItems(groups);

    std::string completeJson = createCompleteJson(groups, entries, menuItems);

    // write to file
    fwrite(completeJson.c_str(), completeJson.length(), 1, output );
    fclose(output);
}

std::string ConvM3U::createMenuItems(const std::set<std::string>& groups) {
    std::string result;

    long unsigned index = 1;
    for(auto f : groups) {
        result += "{ \"title\":\"" + f + "\", \"app\":0, \"submenu\":" + std::to_string(index++) + "}";
        if (index <= groups.size()) {
            result += ",\n";
        } else {
            result += "\n";
        }
    }

    return result;
}

std::string ConvM3U::createChannelItems(const std::vector<m3uEntry>& m3u, const std::string& group) {
    std::string result;

    long unsigned index = 1;
    for(auto f : m3u) {
        long unsigned count = countGroups(m3u, f.groupTitle);

        if (f.groupTitle == group) {
            result += createChannelItem(f);
            if (index < count) {
                if (!result.empty()) {
                    result += ",\n";
                }
            } else {
                result += "\n";
            }
            index++;
        }
    }

    return result;
}

std::string ConvM3U::createChannelItem(const m3uEntry& entry) {
    std::string result;

    result += "{ ";

    // title
    result += "\"title\":\"";
    if (!entry.tvgName.empty()) {
        result += entry.tvgName;
    } else if (!entry.m3uTitle.empty()) {
        result += entry.m3uTitle;
    } else {
        // neither title not tvg-name, ignore this entry
        return std::string();
    }
    result += "\",";

    // url
    result += "\"url\":\"" + entry.m3uUrl + "\",";

    // img
    result += "\"img\":\"";
    if (!entry.tvgLogo.empty()) {
        result += entry.tvgLogo;
    } else {
        result += "icons/icons_1x1_catchup.png";
    }
    result += "\",";

    // app
    result += "\"app\": 6,";

    // desc
    result += "\"desc\":\"" + entry.groupTitle + ", " + entry.m3uTitle + "\",";

    // not official (xmltv id)
    result += "\"xmltvid\":\"" + entry.tvgId + "\"";

    result += "}";

    return result;
}

std::string ConvM3U::createCompleteJson(const std::set<std::string>& groups, const std::vector<m3uEntry>& m3u, const std::string& menuItems) {
    std::string result;

    // header
    result += std::string("{") + "\"menus\": [ { \"center\": 1, \"title\": \"Main\", \"items\": [";
    result += menuItems;
    result += "] },";

    long unsigned index = 1;
    for(auto f : groups) {
        result += "{ \"center\": 0, \"title\": \"" + f + "\",";
        result += "\"items\": [" + createChannelItems(m3u, f) + "]}";

        if (index < groups.size()) {
            result += ",\n";
        } else {
            result += "\n";
        }

        index++;
    }

    // footer
    result += "], \"icondelay\": \"10\" }\n";

    return result;
}

unsigned long ConvM3U::countGroups(const std::vector<m3uEntry>& m3u, const std::string& group) {
    unsigned long result = 0;

    for (auto e : m3u) {
        if (e.groupTitle == group) {
            result++;
        }
    }

    return result;
}