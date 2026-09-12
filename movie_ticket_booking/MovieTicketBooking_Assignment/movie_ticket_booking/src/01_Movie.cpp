class Movie {
private:
    string title;
    string language;
    int durationMinutes;

public:
    Movie(const string& title, const string& language, int durationMinutes) {
        this->title = title;                 // OOP: this keyword
        this->language = language;
        this->durationMinutes = durationMinutes;
    }

    const string& getTitle() const { return title; }
    const string& getLanguage() const { return language; }
    int getDurationMinutes() const { return durationMinutes; }
};
