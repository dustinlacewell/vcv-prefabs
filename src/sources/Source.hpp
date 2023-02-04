#pragma once

#include <rack.hpp>

using namespace std;
using namespace rack::plugin;

#include "Rack.h"

struct Source
{
    virtual int getTotal() = 0;
    virtual std::string getSlug() = 0;
    virtual std::string getPath() = 0;

    virtual void refresh() = 0;
    virtual void setCallback(std::function<void(Rack)> newCallback) = 0;
};