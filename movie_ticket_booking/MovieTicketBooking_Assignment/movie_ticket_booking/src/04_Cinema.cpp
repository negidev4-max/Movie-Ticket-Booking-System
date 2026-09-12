class Cinema {
private:
    string name;
    vector<Screen> screens;                  // OOP: composition - Cinema owns Screens

public:
    Cinema(const string& name) { this->name = name; }

    const string& getName() const { return name; }
    const vector<Screen>& getScreens() const { return screens; }

    void addScreen(const Screen& screen) { screens.push_back(screen); }

    Screen* findScreen(int screenNumber) {
        for (Screen& screen : screens) {
            if (screen.getScreenNumber() == screenNumber) return &screen;
        }
        return nullptr;
    }
};
