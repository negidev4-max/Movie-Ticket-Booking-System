class Screen {
private:
    int screenNumber;
    vector<Seat> seats;                      // OOP: composition - Screen owns Seats

public:
    Screen(int screenNumber) {
        this->screenNumber = screenNumber;
    }

    int getScreenNumber() const { return screenNumber; }
    const vector<Seat>& getSeats() const { return seats; }

    void addSeat(const Seat& seat) { seats.push_back(seat); }

    // OOP: compile-time polymorphism through an overloaded method.
    void addSeat(const string& number, SeatType type) {
        seats.emplace_back(number, type);
    }
};
