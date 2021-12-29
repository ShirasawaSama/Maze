#pragma once

#include "Map.h"
#include <JuceHeader.h>

class MainComponent : public juce::Component {
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    Map map;
    juce::TextButton startBtn{ "SET START POINT" }, endBtn{ "SET END POINT" },
        clearBtn{ "CLEAR" }, loadBtn{ "LOAD FROM FILE" }, generateBtn{ "GENERATE" },
        bfsBtn{ "BFS" }, dfsBtn{ "DFS" }, aStarBtn{ "A*" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
