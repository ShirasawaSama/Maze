#include "MainComponent.h"
#include "globals.h"
#include <queue>

int __btnLeft = 0;

#define ADD_BUTTON(btn, color, width, cb) btn.setColour(juce::TextButton::buttonColourId, color); \
    btn.setBounds(__btnLeft += 10, 20, width, 40); \
    __btnLeft += width; \
    addAndMakeVisible(btn); \
    btn.onClick = [this] { if (map.isAnimating()) return; cb; };

MainComponent::MainComponent() {
    setSize(16 * 50, 16 * 40);

    ADD_BUTTON(startBtn, SUCCESS_COLOR, 120, map.setMode = 1; map.clearAnimate());
    ADD_BUTTON(endBtn, ERROR_COLOR, 120, map.setMode = 2; map.clearAnimate());
    ADD_BUTTON(loadBtn, WARN_COLOR, 130, map.loadFromFile("./map.txt"));
    ADD_BUTTON(clearBtn, WARN_COLOR, 70, map.clear(); map.clearAnimate());
    ADD_BUTTON(bfsBtn, PRIMARY_COLOR, 50, map.startAnimate((new BFS(&map))->exec()));
    ADD_BUTTON(dfsBtn, PRIMARY_COLOR, 50, { /* TODO */ });
    ADD_BUTTON(aStarBtn, PRIMARY_COLOR, 50, { /* TODO */ });

    map.setTopLeftPosition(0, 80);
    map.setSize(getWidth(), getHeight() - 80);
    addAndMakeVisible(map);
}

MainComponent::~MainComponent() { }

void MainComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::white);

    g.setFont(juce::Font(16.0f));
}

void MainComponent::resized() {
    map.setSize(getWidth(), getHeight() - 80);
}