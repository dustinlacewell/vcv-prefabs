#pragma once

#include <rack.hpp>
#include <regex>

using namespace rack;

struct SearchResult {
    bool perfectMatch;
    bool allMatched;
    int tokensMatched;
    std::map<std::string, int> matches;

    int getScore()
    {
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

    std::string query;
    std::set<std::string> tokens;

    std::function<std::set<Rack>()> candidateCallback;

    std::vector<Rack> results;

    RackSearcher()
    {
        query = "";
        tokens = {};
        candidateCallback = []() {
            return std::set<Rack>();
        };
    }

    std::set<std::string> breakTokens(std::string input)
    {
        auto newTokens = std::set<std::string>();

        // break text by whitespace (not just space, use regex)
        auto regex = std::regex("\\s+");
        auto foundTokens = std::vector<std::string>(
            std::sregex_token_iterator(input.begin(), input.end(), regex, -1), std::sregex_token_iterator());
        // lowercase all tokens
        for (auto& token : foundTokens) {
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);
            newTokens.insert(token);
        }

        return newTokens;
    }

    SearchResult matchTokens(std::string input)
    {
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

    int rateRack(Rack rack)
    {
        int rating = 0;

        // check displayName
        rating += matchTokens(rack.getDisplayName()).getScore() * 10;

        if (rack.metadata.has_value()) {
            rating += matchTokens(rack.metadata->author.name).getScore() * 3;
            rating += matchTokens(rack.metadata->content).getScore();
        }

        if (rating > 0 && rack.isValid) {
            rating += 10000;
        }

        return rating;
    }

    void setQuery(std::string query)
    {
        results.clear();

        this->query = query;
        this->tokens = this->breakTokens(query);

        auto candidates = candidateCallback();

        auto scores = std::vector<std::pair<int, Rack>>();

        for (auto candidate : candidates) {
            auto score = rateRack(candidate);
            if (score > 0) {
                scores.push_back(std::make_pair(score, candidate));
            }
        }

        std::sort(scores.begin(), scores.end(), [](std::pair<int, Rack> a, std::pair<int, Rack> b) {
            return a.first > b.first;
        });

        for (auto score : scores) {
            results.push_back(score.second);
        }
    }
};

struct SearchBox : ui::TextField {
    WeakPtr<ui::Menu> menu{nullptr};
    std::string lastText{};
    function<void(std::string)> onChange;
    std::vector<std::string> tokens;

    SearchBox(function<void(std::string)> onChange)
    {
        this->onChange = onChange;
        text = "";
        lastText = text;
        multiline = false;
        placeholder = "Search";
    }

    void step() override
    {
        if (text != lastText) {
            onChange(text);
        }
        lastText = text;
        // Keep selected
        APP->event->setSelectedWidget(this);

        TextField::step();
    }

    void draw(const DrawArgs& args) override { TextField::draw(args); }

    void setPos(Vec pos) { TextField::box.pos = pos; }

    void setExtents(Vec size) { TextField::box.size = size; }
};