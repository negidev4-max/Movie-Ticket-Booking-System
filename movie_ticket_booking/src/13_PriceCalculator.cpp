class PriceCalculator {
public:
    static constexpr double SILVER_PRICE = 150.0;
    static constexpr double GOLD_PRICE = 250.0;
    static constexpr double PLATINUM_PRICE = 400.0;

    double priceFor(const Seat& seat) const {
        if (seat.getType() == SeatType::SILVER) return SILVER_PRICE;
        if (seat.getType() == SeatType::GOLD) return GOLD_PRICE;
        return PLATINUM_PRICE;
    }

    double calculate(const vector<ShowSeat*>& selectedSeats) const {
        double total = 0.0;
        for (const ShowSeat* showSeat : selectedSeats) {
            total += priceFor(showSeat->getSeat());
        }
        return total;
    }
};
