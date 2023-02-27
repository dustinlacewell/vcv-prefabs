#include "SimpleQuantity.hpp"

void SimpleQuantity::setValue(float newValue)
{
    this->value = clamp(rounded ? std::round(newValue) : newValue, minValue, maxValue);
}

float SimpleQuantity::getValue()
{
    return value;
}

float SimpleQuantity::getDefaultValue()
{
    return defaultValue;
}

float SimpleQuantity::getMinValue()
{
    return minValue;
}

float SimpleQuantity::getMaxValue()
{
    return maxValue;
}

std::string SimpleQuantity::getLabel()
{
    return label;
}

std::string SimpleQuantity::getUnit()
{
    return unit;
}

void SimpleQuantity::reset()
{
    if (resetCallback)
        value = resetCallback();
    else
        value = defaultValue;
}

void SimpleQuantity::randomize()
{
    if (randomCallback)
        value = randomCallback();
    else
        value = random::uniform() * (maxValue - minValue) + minValue;
}

json_t* SimpleQuantity::toJson()
{
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "value", json_real(value));
    return rootJ;
}

void SimpleQuantity::fromJson(json_t* rootJ)
{
    json_t* valueJ = json_object_get(rootJ, "value");
    if (valueJ)
        value = json_number_value(valueJ);
}