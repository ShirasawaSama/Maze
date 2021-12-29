#pragma once
#include "algorithms.h"
#include <JuceHeader.h>

class Map : public juce::Component, public juce::Timer {
public:
    Map();
    ~Map() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    bool keyPressed(const juce::KeyPress& key) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void loadFromFile(std::string name);
    void saveToFile(std::string name);
    void clear();
    void clearAnimate();
    void stopAnimate();
    void startAnimate(AlgorithmResult* result);
    void syncPoi();
    bool isAnimating();
    int getMapWidth();
    int getMapHeight();
    void generateMap();
    void switchGameMode();

    bool gameMode = false;
    int setMode = 0;
    int startX = 0, startY = 0, endX = 1, endY = 1;
    int matrix[MAP_SIZE][MAP_SIZE];
private:
    int prevX = -1, prevY = -1, curAnimateX = -1, curAnimateY = -1, animateId = 0, steps = 0;
    int animateMatrix[MAP_SIZE][MAP_SIZE];
    AlgorithmResult* animate = nullptr;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Map)
};
