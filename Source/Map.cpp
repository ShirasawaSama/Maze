#include "Map.h"
#include "globals.h"
#include <fstream>

int width = 16;

juce::Colour PRIMARY_COLOR = juce::Colour::fromRGB(66, 165, 245);
juce::Colour SUCCESS_COLOR = juce::Colour::fromRGB(76, 175, 80);
juce::Colour ERROR_COLOR = juce::Colour::fromRGB(239, 83, 80);
juce::Colour WARN_COLOR = juce::Colour::fromRGB(255, 152, 0);
juce::Colour WALL_COLOR = juce::Colour::fromRGB(0, 30, 60);
juce::Colour WALKED_COLOR = juce::Colour::fromRGB(141, 201, 251);
juce::Colour BORDER_COLOR = juce::Colour::fromRGB(210, 210, 210);

Map::Map() {
    memset(matrix, 0, sizeof(matrix));
    clearAnimate();
    startTimerHz(20);
}

Map::~Map() { }

void Map::loadFromFile(std::string name) {
    clear();
    std::ifstream in(name);
    auto n = 0, m = 0;
    if (n < 300 && m < 300) {
        in >> n >> m;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) in >> matrix[i][j];
            matrix[i][m] = 1;
        }
        for (int j = 0; j <= m; j++) matrix[n][j] = 1;
    }
    in.close();
    syncPoi();
    repaint();
}

int Map::getMapWidth() { return getWidth() / width; }
int Map::getMapHeight() { return getHeight() / width; }

void Map::syncPoi() {
    for (int i = 0; i < MAP_SIZE; i++) for (int j = 0; j < MAP_SIZE; j++) {
        switch (matrix[i][j]) {
        case 2:
            startX = j;
            startY = i;
            break;
        case 3:
            endX = j;
            endY = i;
            break;
        }
    }
}

void Map::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::white);
    int n = getMapHeight(), m = getMapWidth();
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
        switch (matrix[i][j]) {
        case 0:
            switch (animateMatrix[i][j]) {
            case 1:
                g.setColour(WALKED_COLOR);
                break;
            case 2:
                g.setColour(WARN_COLOR);
                break;
            default: continue;
            }
            break;
        case 1:
            g.setColour(WALL_COLOR);
            break;
        case 2:
            g.setColour(SUCCESS_COLOR);
            break;
        case 3:
            g.setColour(ERROR_COLOR);
            break;
        }
        g.fillRect(j * width, i * width, width, width);
    }
    g.setColour(BORDER_COLOR);
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
        g.drawRect(j * width, i * width, width, width);
    }
}

void Map::mouseDown(const juce::MouseEvent& e) {
    int y = e.y / width, x = e.x / width;
    prevX = x;
    prevY = y;
    switch (setMode) {
    case 0:
        if (matrix[y][x] > 1) return;
        matrix[y][x] = matrix[y][x] == 0;
        break;
    case 1:
        matrix[startY][startX] = 0;
        startX = x;
        startY = y;
        matrix[y][x] = 2;
        setMode = 0;
        break;
    case 2:
        matrix[endY][endX] = 0;
        endX = x;
        endY = y;
        matrix[y][x] = 3;
        setMode = 0;
        break;
    }
    this->repaint();
}

void Map::mouseDrag(const juce::MouseEvent& e) {
    if (setMode != 0) return;
    int y = e.y / width, x = e.x / width;
    if (prevX == x && prevY == y) return;
    prevX = x;
    prevY = y;
    if (matrix[y][x] > 1) return;
    matrix[y][x] = matrix[y][x] == 0;
    this->repaint();
}

void Map::clear() {
    for (int i = 0; i < MAP_SIZE; i++) for (int j = 0; j < MAP_SIZE; j++) {
        if (matrix[i][j] == 1) matrix[i][j] = 0;
    }
    repaint();
}

void Map::clearAnimate() { memset(animateMatrix, 0, sizeof(animateMatrix)); }

bool Map::isAnimating() { return animate != nullptr; }

void Map::resized() {
    delete animate;
    animate = nullptr;
    clearAnimate();
}

void Map::timerCallback() {
    if (animate == nullptr) return;
    if (animate->order.empty()) {
        if (curAnimateX == -1) {
            curAnimateX = endX;
            curAnimateY = endY;
        }
        if (curAnimateX == startX && curAnimateY == startY) {
            delete animate;
            animate = nullptr;
            return;
        } else {
            int x, y, preX, preY;
            std::tie(x, y, curAnimateX, curAnimateY) = animate->vis[curAnimateY][curAnimateX];
            animateMatrix[y][x] = 2;
        }
    } else {
        int x, y;
        std::tie(x, y, std::ignore, std::ignore) = animate->order.front();
        animate->order.pop();
        animateMatrix[y][x] = 1;
    }
    repaint();
}

void Map::startAnimate(AlgorithmResult* result) {
    if (animate != nullptr) return;
    clearAnimate();
    curAnimateX = -1;
    curAnimateY = -1;
    animate = result;
}
