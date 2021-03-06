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
juce::Colour SECONDARY_COLOR = juce::Colour::fromRGB(186, 104, 200);

Map::Map() {
    memset(matrix, 0, sizeof(matrix));
    matrix[0][0] = 2;
    matrix[1][1] = 3;
    clearAnimate();
    startTimerHz(60);
    setWantsKeyboardFocus(true);
}

Map::~Map() { }

void Map::loadFromFile(std::string name) {
    memset(matrix, 0, sizeof(matrix));
    clearAnimate();
    std::ifstream in(name);
    auto n = 0, m = 0;
    in >> n >> m;
    if (n < MAP_SIZE && m < MAP_SIZE && n > 0 && m > 0) {
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

void Map::saveToFile(std::string name) {
    std::ofstream out(name);
    auto n = getMapHeight(), m = getMapWidth();
    out << n << " " << m << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) out << matrix[i][j] << " ";
        out << std::endl;
    }
    out.flush();
    out.close();
}

int Map::getMapWidth() { return std::min(getWidth() / width, MAP_SIZE); }
int Map::getMapHeight() { return std::min(getHeight() / width, MAP_SIZE); }

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
    g.setFont(juce::Font(10.0f));
    g.fillAll(juce::Colours::white);
    int n = getMapHeight(), m = getMapWidth();
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
        switch (matrix[i][j]) {
        case 0:
            if (animateMatrix[i][j] == 1) {
                g.setColour(WALKED_COLOR);
                break;
            } else if (animateMatrix[i][j] > 1) {
                g.setColour(WARN_COLOR);
                g.fillRect(j * width, i * width, width, width);
                g.setColour(juce::Colours::white);
                g.drawText(std::to_string(animateMatrix[i][j] - 1), j * width, i * width, width, width, juce::Justification::centred);
            }
            continue;
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
    if (gameMode) return;
    int y = e.y / width, x = e.x / width;
    if (y < 0 || x < 0 || y >= MAP_SIZE || x >= MAP_SIZE) return;
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
    if (gameMode || setMode != 0) return;
    int y = e.y / width, x = e.x / width;
    if ((prevX == x && prevY == y) || y < 0 || x < 0 || y >= MAP_SIZE || x >= MAP_SIZE) return;
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

bool Map::isAnimating() { return animate != nullptr || !walls.empty(); }

void Map::resized() {
    stopAnimate();
    clearAnimate();
}

void Map::timerCallback() {
    if (!walls.empty()) {
        int x, y, dir;
        bool flag = true;
        int n = getMapHeight(), m = getMapWidth();
        while (flag && !walls.empty()) {
            int randWall = rand() % walls.size();
            std::tie(x, y, dir) = walls[randWall];
            int nextX = x, nextY = y;
            switch (dir) {
            case 0: nextY--; break;
            case 1: nextX++; break;
            case 2: nextY++; break;
            case 3: nextX--;
            }
            if (matrix[nextY][nextX] == 1) {
                matrix[y][x] = matrix[nextY][nextX] = 0;
                if (nextY >= 2 && matrix[nextY - 1][nextX] == 1) walls.emplace_back(std::make_tuple(nextX, nextY - 1, 0));
                if (nextX + 1 < m && matrix[nextY][nextX + 1] == 1) walls.emplace_back(std::make_tuple(nextX + 1, nextY, 1));
                if (nextY + 1 < n && matrix[nextY + 1][nextX] == 1) walls.emplace_back(std::make_tuple(nextX, nextY + 1, 2));
                if (nextX >= 2 && matrix[nextY][nextX - 1] == 1) walls.emplace_back(std::make_tuple(nextX - 1, nextY, 3));
                flag = fastGenerate;
            }
            walls.erase(walls.begin() + randWall);
        }
        if (walls.empty()) do {
            matrix[startY = rand() % n][startX = rand() % m] = 2;
            matrix[endY = rand() % n][endX = rand() % m] = 3;
        } while (startY == endY && startX == endX);
        repaint();
        return;
    }
    if (animate == nullptr) return;
    if (animate->order.empty()) {
        if (curAnimateX == -1) {
            curAnimateX = endX;
            curAnimateY = endY;
            animateId = 0;
        }
        if (curAnimateX == startX && curAnimateY == startY) {
            stopAnimate();
            return;
        } else {
            int x, y;
            std::tie(x, y, curAnimateX, curAnimateY) = animate->vis[curAnimateY][curAnimateX];
            if (x == curAnimateX && y == curAnimateY) {
                stopAnimate();
                return;
            }
            animateMatrix[y][x] = ++animateId;
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

void Map::stopAnimate() {
    delete animate;
    animate = nullptr;
}

void Map::generateMap() {
    memset(matrix, 0, sizeof(matrix));
    clearAnimate();
    srand((unsigned) time(NULL));
    int n = getMapHeight(), m = getMapWidth();
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) matrix[i][j] = 1;
    matrix[0][0] = 0;
    walls.emplace_back(std::make_tuple(1, 0, 1));
    walls.emplace_back(std::make_tuple(0, 1, 2));
}

void Map::switchGameMode() {
    gameMode = !gameMode;
    if (gameMode) {
        grabKeyboardFocus();
        getParentComponent()->getParentComponent()->setName("Maze (Game Mode, Steps: 0)");
        juce::AlertWindow::showOkCancelBox(juce::AlertWindow::InfoIcon, "Maze:", "Game Start!", "", "", nullptr, nullptr);
        grabKeyboardFocus();
    } else {
        juce::AlertWindow::showOkCancelBox(juce::AlertWindow::InfoIcon, "Maze:", "Game Over!", "", "", nullptr, nullptr);
        getParentComponent()->getParentComponent()->setName("Maze");
    }
}

bool Map::keyPressed(const juce::KeyPress& key) {
    if (!gameMode) return false;
    if (key.getKeyCode() == juce::KeyPress::downKey) {
        if (startY >= getMapHeight() - 1 || matrix[startY + 1][startX] != 0) return false;
        matrix[startY++][startX] = 0;
    } else if (key.getKeyCode() == juce::KeyPress::upKey) {
        if (startY < 1 || matrix[startY - 1][startX] != 0) return false;
        matrix[startY--][startX] = 0;
    } else if (key.getKeyCode() == juce::KeyPress::leftKey) {
        if (startX < 1 || matrix[startY][startX - 1] != 0) return false;
        matrix[startY][startX--] = 0;
    } else if(key.getKeyCode() == juce::KeyPress::rightKey) {
        if (startX >= getMapWidth() - 1 || matrix[startY][startX + 1] != 0) return false;
        matrix[startY][startX++] = 0;
    }
    matrix[startY][startX] = 2;
    getParentComponent()->getParentComponent()->setName("Maze (Game Mode, Steps: " + std::to_string(++steps) + ")");
    if (std::abs(startY - endY) + std::abs(startX - endX) <= 1) {
        gameMode = false;
        juce::AlertWindow::showOkCancelBox(juce::AlertWindow::InfoIcon, "Maze:", "Game Over! (Steps: " + std::to_string(steps) + ")", "", "", nullptr, nullptr);
        getParentComponent()->getParentComponent()->setName("Maze");
    }
    repaint();
    return false;
}
