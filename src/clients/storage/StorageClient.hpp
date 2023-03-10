#include <rack.hpp>

#include "clients/HttpClient.hpp"
#include "clients/typedefs.hpp"
#include "responses/PatchInfoResponse.hpp"
#include "responses/SearchResponse.hpp"

struct StorageClient {
   protected:
    std::string username;
    std::string password;
    std::optional<std::string> token;

    Request makeRequest(std::string endpoint);

   public:
    StorageClient(std::string username, std::string password);
    ~StorageClient();
    bool login();
    std::vector<PatchResult> fetchUserPatches(int authorId, int count = 0);
    std::optional<NewPatchInfo> fetchPatchInfo(int patchId);
    bool downloadPatch(std::string url, std::string path) const;
};