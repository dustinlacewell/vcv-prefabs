#include <rack.hpp>

#include "clients/HttpClient.hpp"
#include "clients/typedefs.hpp"

struct LibraryClient {
   protected:
    Request makeRequest(std::string endpoint);

   public:
    //    bool login();
    bool addModule(std::string pluginSlug, std::string moduleSlug);
    bool removeModule(std::string pluginSlug, std::string moduleSlug);
    bool subscribe(std::string pluginSlug);
    bool unsubscribe(std::string pluginSlug);
    void checkUpdates();
};