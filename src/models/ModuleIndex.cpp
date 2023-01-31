#include "ModuleIndex.hpp"
#include <plugin.hpp>
#include <tag.hpp>

struct TokenMatch
{
    int length;
    float multiplier;
    bool nameMatched;
    bool brandMatched;
    bool tagMatched;

    int getScore() { return length * ((nameMatched ? 10 : 0) + (brandMatched ? 10 : 0) + (tagMatched ? 0 : 0)); }
};

struct Result
{
    Model* model;
    std::map<std::string, TokenMatch> matches;

    int getScore()
    {
        int score = 0;
        for (auto& [token, match] : matches) {
            score += match.getScore();
        }
        return score;
    }
};

void ModuleIndex::search(std::string searchString)
{
    results.clear();

    auto tokens = std::vector<std::string>();

    auto matches = std::map<Model*, Result>();

    auto getResult = [&matches](Model* model) {
        if (matches.find(model) == matches.end()) {
            return Result{model, {}};
        }
        return matches[model];
    };

    auto getTokenMatch = [&getResult](Model* model, std::string token) {
        Result result = getResult(model);
        if (result.matches.find(token) == result.matches.end()) {
            int length = token.length();
            return TokenMatch{length, false, false, false};
        }
        return result.matches[token];
    };

    auto setTokenMatch = [&getResult, &matches](Model* model, std::string token, TokenMatch match) {
        Result result = getResult(model);
        result.matches[token] = match;
        matches[model] = result;
    };

    auto addNameMatch = [&getTokenMatch, &setTokenMatch](Model* model, std::string token, float multiplier = 1.0) {
        auto match = getTokenMatch(model, token);
        match.nameMatched = true;
        match.multiplier = multiplier;
        setTokenMatch(model, token, match);
    };

    auto addBrandMatch = [&getTokenMatch, &setTokenMatch](Model* model, std::string token, float multiplier = 1.0) {
        auto match = getTokenMatch(model, token);
        match.brandMatched = true;
        match.multiplier = multiplier;
        setTokenMatch(model, token, match);
    };

    auto addTagMatch = [&getTokenMatch, &setTokenMatch](Model* model, std::string token, float multiplier = 1.0) {
        auto match = getTokenMatch(model, token);
        match.tagMatched = true;
        match.multiplier = multiplier;
        setTokenMatch(model, token, match);
    };

    // Split the search string into tokens
    std::string t;
    std::istringstream tokenStream(searchString);
    while (std::getline(tokenStream, t, ' ')) {
        tokens.push_back(t);
    }

    // Search every module in every plugin
    for (auto& plugin : rack::plugin::plugins) {
        for (auto& model : plugin->models) {
            // Search every token in the module's name
            for (auto& token : tokens) {
                bool token_found = false;
                auto token_lower = token;
                std::transform(token_lower.begin(), token_lower.end(), token_lower.begin(), ::tolower);

                auto name_lower = model->name;
                std::transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);
                int namePos = name_lower.find(token_lower);
                if (namePos != std::string::npos) {
                    addNameMatch(model, token_lower, namePos == 0 ? 2 : 1);
                }

                // Search every token in the module's tags
                for (auto& tagId : model->tagIds) {
                    bool shouldBreak = false;
                    for (const std::string& tagAlias : tag::tagAliases[tagId]) {
                        auto tag_lower = tagAlias;
                        std::transform(tag_lower.begin(), tag_lower.end(), tag_lower.begin(), ::tolower);
                        int tagPos = tag_lower.find(token_lower);
                        if (tagPos != std::string::npos) {
                            addTagMatch(model, token_lower);
                        }
                    }
                }

                // Search every token in the module's brand
                auto brand_lower = model->plugin->brand;
                std::transform(brand_lower.begin(), brand_lower.end(), brand_lower.begin(), ::tolower);
                int brandPos = brand_lower.find(token_lower);
                if (brandPos != std::string::npos) {
                    addBrandMatch(model, token_lower, brandPos == 0 ? 2 : 1);
                }
            }
        }
    }

    std::vector<Result> _preResults;
    for (auto& pair : matches) {
        _preResults.push_back(pair.second);
    }

    std::sort(_preResults.begin(), _preResults.end(), [this](Result a, Result b) {
        if (module->moduleSorter.sortType > SortType::USAGE) {
            if (a.model->isFavorite() && !b.model->isFavorite()) {
                return true;
            }
            if (!a.model->isFavorite() && b.model->isFavorite()) {
                return false;
            }
        }
        return a.getScore() > b.getScore();
    });

    for (auto& result : _preResults) {
        results.push_back(result.model);
    }
}