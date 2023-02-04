#pragma once

#include <rack.hpp>

using namespace std;
using namespace rack::plugin;

#include "Rack.h"
#include "Source.hpp"
#include "efsw/efsw.h"
#include "efsw/efsw.hpp"

class UpdateListener : public efsw::FileWatchListener
{
   public:
    std::function<void(std::string, std::string)> callback;

    void handleFileAction(efsw::WatchID watchid,
        const std::string& dir,
        const std::string& filename,
        efsw::Action action,
        std::string oldFilename) override;
};

struct FileSource : Source
{
    int readRack(std::string tagName, std::string prefabName);

   private:
    efsw_watcher watcher;

    std::string slug;
    std::string path;

    std::map<std::string, Rack> racks;

    std::string pathForTag(std::string tagName);
    std::string pathForItem(std::string tagName, std::string prefabName);

    void addItem(Rack prefab);
    int crawlTag(std::string tagName);
    Rack* read(std::string tagName, std::string prefabName);

   public:
    std::function<void(Rack)> callback;

    FileSource(std::string slug, std::string path);
    virtual ~FileSource();
    virtual json_t* readJson(std::string filePath);

    int getTotal() override;
    std::string getSlug() override;
    std::string getPath() override;
    void refresh() override;
    void setCallback(std::function<void(Rack)> newCallback) override;
};