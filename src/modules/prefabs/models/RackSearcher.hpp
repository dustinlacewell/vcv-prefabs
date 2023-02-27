#pragma once

#include <rack.hpp>
#include <regex>

using namespace rack;

struct SearchResult {
    bool perfectMatch;                   // haystack and needle are the same
    bool allMatched;                     // all needles were found in haystack
    int tokensMatched;                   // number of needles found in haystack
    std::map<std::string, int> matches;  // needle -> number of times found in haystack

    int getScore() {
        int score = 0;

        for (auto match : matches) {
            score += match.second;
        }

        if (allMatched) {
            score *= 2;
        }

        if (perfectMatch) {
            score *= 10;
        }

        return score;
    }
};

struct RackSearcher {

    std::string query;                                  // the user input query
    std::set<std::string> tokens;                       // the query broken into tokens
    std::function<std::set<Rack>()> candidateCallback;  // callback to get a list of racks to search
    std::vector<Rack> results;                          // the results of the search

    RackSearcher() {
        query = "";
        tokens = {};
        candidateCallback = []() {
            return std::set<Rack>();
        };
    }

    /**
     * Break an input query by tokens
     */
    std::set<std::string> breakTokens(std::string input) {
        auto newTokens = std::set<std::string>();

        // break text by whitespace
        auto regex = std::regex("\\s+");

        // tokens broken by whitespace
        auto foundTokens = std::vector<std::string>(
            std::sregex_token_iterator(input.begin(), input.end(), regex, -1), std::sregex_token_iterator());

        // lowercase all tokens
        for (auto& token : foundTokens) {
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);
            newTokens.insert(token);
        }

        return newTokens;
    }

    /**
     * Match current tokens against a haystack (usually some rack property)
     */
    SearchResult matchTokens(std::string input) {
        SearchResult result;
        result.perfectMatch = false;
        result.allMatched = false;
        result.tokensMatched = 0;

        // check if the input matches the query exactly
        if (input == query) {
            result.perfectMatch = true;
        }

        auto inputTokens = breakTokens(input);

        for (auto needle : tokens) {
            for (auto haystack : inputTokens) {
                auto pos = haystack.find(needle);
                if (pos != std::string::npos) {
                    if (result.matches.find(needle) == result.matches.end()) {
                        result.matches[needle] = 0;
                    }
                    result.matches[needle] += haystack.size() - pos;
                    result.tokensMatched++;
                }
            }
        }

        if (result.tokensMatched == tokens.size()) {
            result.allMatched = true;
        }

        return result;
    }

    /**
     * Rate a rack based on how well it matches the query
     */
    int rateRack(Rack rack) {
        int rating = 0;

        // check displayName
        rating += matchTokens(rack.getDisplayName()).getScore() * 10;

        if (rack.metadata.has_value()) {
            rating += matchTokens(rack.metadata->author.name).getScore() * 3;
            rating += matchTokens(rack.metadata->content).getScore();
        }

        // racks that match, and are valid get a bonus
        if (rating > 0 && rack.isValid) {
            rating += 10000;
        }

        return rating;
    }

    /**
     * Set the query, break it into tokens, and search for matching racks
     */
    void setQuery(std::string query) {
        results.clear();

        this->query = query;                      // update what the user typed
        this->tokens = this->breakTokens(query);  // break the query into tokens

        // get a list of racks to search
        auto candidates = candidateCallback();

        // the score of each rack
        auto scores = std::vector<std::pair<int, Rack>>();

        // push racks that match the query in some way
        for (auto candidate : candidates) {
            auto score = rateRack(candidate);
            if (score > 0) {
                scores.push_back(std::make_pair(score, candidate));
            }
        }

        // sort the racks by score
        std::sort(scores.begin(), scores.end(), [](std::pair<int, Rack> a, std::pair<int, Rack> b) {
            return a.first > b.first;
        });

        // push just the racks into the results
        for (auto score : scores) {
            results.push_back(score.second);
        }
    }
};