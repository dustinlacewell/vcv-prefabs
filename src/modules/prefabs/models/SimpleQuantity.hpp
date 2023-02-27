#pragma once

#include <rack.hpp>

using namespace rack;

struct SimpleQuantity : Quantity
{
    bool rounded = false;
    float value = 0.0f;
    float minValue = 0.0f;
    float maxValue = 1.0f;
    float defaultValue = minValue;
    std::string label = "Value";
    std::string unit = "";
    std::function<float()> resetCallback;
    std::function<float()> randomCallback;

    void setValue(float newValue) override;

    float getValue() override;

    float getDefaultValue() override;

    float getMinValue() override;

    float getMaxValue() override;

    std::string getLabel() override;

    std::string getUnit() override;

    void reset() override;

    void randomize() override;

    json_t* toJson();
    void fromJson(json_t* rootJ);
};