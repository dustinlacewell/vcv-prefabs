#include <rack.hpp>

#include "clients/HttpClient.hpp"
#include "clients/typedefs.hpp"

struct LibraryClient {
   protected:
    std::string username;
    std::string password;
    std::optional<std::string> token;

    Request makeRequest(std::string endpoint);

   public:
    LibraryClient(std::string username, std::string password);
    ~LibraryClient();
    bool login();
};