#pragma once

#include <rack.hpp>

using namespace std;
using namespace rack::plugin;

#include "Rack.h"
#include "Source.hpp"
#include "efsw/efsw.h"
#include "efsw/efsw.hpp"

class UpdateListener : public efsw::FileWatchListener {
   public:
    std::function<void(std::string, std::string)> callback;

    void handleFileAction(efsw::WatchID watchid,
        const std::string& dir,
        const std::string& filename,
        efsw::Action action,
        std::string oldFilename) override;
};

struct FileSource : Source {

   protected:
    efsw_watcher watcher;

    std::string slug;
    std::string path;

    std::map<std::string, Rack> racks;

    int crawlRoot(std::string root);
    virtual Rack* read(std::string filename);

   private:
    //    std::string pathForTag(std::string tagName);
    //    std::string pathForItem(std::string tagName, std::string prefabName);

    void addItem(Rack prefab);

   public:
    std::function<void(Rack)> callback;

    FileSource(std::string slug, std::string path);
    virtual ~FileSource();
    int readRack(std::string filename);
    virtual json_t* readJson(std::string filePath);
    virtual std::vector<std::string> filterFiles(std::vector<std::string>& files);

    int getTotal() override;
    std::string getSlug() override;
    std::string getPath() override;
    virtual void load() override;
    void setCallback(std::function<void(Rack)> newCallback) override;
};