#include "SnowWidget.hpp"

bool started = false;

SnowWidget::SnowWidget()
{
    box.size = Vec(20000, 20000);
    box.pos = Vec(0, 0);

    firstOffset = APP->scene->rackScroll->getScrollOffset();

    for (int i = 0; i < numSnowflakes; i++) {
        auto flake = Snowflake();
        snowflakes.push_back(flake);
    }
}

void SnowWidget::randomizeFlake(int i)
{
    auto& flake = snowflakes[i];
    flake.x = random::uniform() * box.size.x;
    flake.y = -1;
    flake.dx = random::uniform();
    flake.dy = random::uniform();
}

void SnowWidget::step()
{
    if (!started) {
        // get system time
        lastTime = glfwGetTime();
        for (int i = 0; i < numSnowflakes; i++) {
            auto& flake = snowflakes[i];
            randomizeFlake(i);
            flake.y = random::uniform() * box.size.y;
        }
        started = true;
    }

    // get change in time
    float time = glfwGetTime();
    float deltaTime = time - lastTime;
    lastTime = time;

    for (int i = 0; i < numSnowflakes; i++) {
        auto& flake = snowflakes[i];
        flake.x += windSpeed * flake.dx * deltaTime;
        flake.y += fallSpeed * flake.dy * deltaTime;
        if (flake.y > box.size.y || (flake.x < 0 || flake.x > box.size.x)) {
            randomizeFlake(i);
        }
    }
}

void SnowWidget::draw(const DrawArgs& args)
{
    Vec offset = APP->scene->rackScroll->getScrollOffset();
    offset = firstOffset.minus(offset);

    // draw the snowflakes as boxes
    nvgBeginPath(args.vg);
    for (int i = 0; i < numSnowflakes; i++) {
        auto flake = snowflakes[i];
        nvgRect(args.vg, flake.x + offset.x, flake.y + offset.y, size, size);
    }
    nvgFillColor(args.vg, color);
    nvgFill(args.vg);
}