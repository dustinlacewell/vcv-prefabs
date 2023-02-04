#pragma once

#include <rack.hpp>

using namespace std;
using namespace rack::plugin;

#include "Rack.h"
#include "Source.hpp"

struct FileSource : Source
{
   private:
    int total = 0;

    std::string slug;
    std::string path;

    std::map<std::string, Rack> racks;
    std::function<void(Rack)> callback;

    std::string pathForTag(std::string tagName);
    std::string pathForItem(std::string tagName, std::string prefabName);

    void addItem(Rack prefab);
    int crawlTag(std::string tagName);
    Rack* read(std::string tagName, std::string prefabName);

   public:
    FileSource(std::string slug, std::string path);
    virtual ~FileSource();
    virtual json_t* readJson(std::string filePath);

    int getTotal() override;
    std::string getSlug() override;
    std::string getPath() override;
    void refresh() override;
    void setCallback(std::function<void(Rack)> newCallback) override;
};