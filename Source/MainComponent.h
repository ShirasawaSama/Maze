#pragma once

#include "Map.h"
#include <JuceHeader.h>

class MainComponent : public juce::Component {
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void modifierKeysChanged(const juce::ModifierKeys& modifiers) override;
private:
    bool isShiftDown = false;
    Map map;
    juce::TextButton startBtn{ "SET START POINT" }, endBtn{ "SET END POINT" },
        clearBtn{ "CLEAR" }, loadBtn{ "LOAD" }, saveBtn{ "SAVE" }, generateBtn{ "GENERATE" }, stopBtn{ "STOP" },
        bfsBtn{ "BFS" }, dfsBtn{ "DFS" }, aStarBtn{ "A*" }, gameModeBtn{ "GAME MODE" }, githubBtn{ "GITHUB" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
