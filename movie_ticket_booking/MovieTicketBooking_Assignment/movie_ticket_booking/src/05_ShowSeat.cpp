enum class SeatStatus { AVAILABLE, BOOKED };

class ShowSeat {
private:
    const Seat* seat;
    SeatStatus seatStatus;                   // OOP: encapsulation

public:
    ShowSeat(const Seat& seat) : seat(&seat), seatStatus(SeatStatus::AVAILABLE) {}

    const Seat& getSeat() const { return *seat; }
    SeatStatus getStatus() const { return seatStatus; }
    bool isAvailable() const { return seatStatus == SeatStatus::AVAILABLE; }

    bool bookSeat() {
        if (!isAvailable()) return false;
        seatStatus = SeatStatus::BOOKED;      // controlled state change
        return true;
    }

    void releaseSeat() {
        seatStatus = SeatStatus::AVAILABLE;  // controlled state change
    }
};
