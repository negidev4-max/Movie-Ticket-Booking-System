class Show {
private:
    string showId;
    const Movie* movie;                      // OOP: aggregation - borrowed Movie
    const Screen* screen;                    // OOP: aggregation - borrowed Screen
    string startTime;
    vector<ShowSeat> showSeats;              // OOP: composition - status belongs to Show

    void printRow(SeatType type, const string& label, int price) const {
        cout << left << setw(10) << label << "(Rs." << price << ")  ";
        for (const ShowSeat& showSeat : showSeats) {
            if (showSeat.getSeat().getType() != type) continue;
            string status = showSeat.isAvailable() ? "AVAILABLE" : "BOOKED";
            cout << showSeat.getSeat().getNumber() << "[" << status << "] ";
        }
        cout << "\n";
    }

public:
    Show(const string& showId, const Movie& movie,
         const Screen& screen, const string& startTime)
        : showId(showId), movie(&movie), screen(&screen), startTime(startTime) {
        for (const Seat& seat : screen.getSeats()) showSeats.emplace_back(seat);
    }

    const string& getShowId() const { return showId; }
    const Movie& getMovie() const { return *movie; }
    const Screen& getScreen() const { return *screen; }
    const string& getStartTime() const { return startTime; }

    ShowSeat* findShowSeat(const string& seatNumber) {
        for (ShowSeat& showSeat : showSeats) {
            if (showSeat.getSeat().getNumber() == seatNumber) return &showSeat;
        }
        return nullptr;
    }

    void displaySeatLayout() const {
        cout << "\nScreen " << screen->getScreenNumber() << " | " << startTime
             << " | " << movie->getTitle() << "\n";
        printRow(SeatType::SILVER, "SILVER", 150);
        printRow(SeatType::GOLD, "GOLD", 250);
        printRow(SeatType::PLATINUM, "PLATINUM", 400);
        cout << "Status legend: AVAILABLE = free, BOOKED = not selectable\n";
    }
};
