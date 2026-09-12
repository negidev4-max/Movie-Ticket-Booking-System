enum class SeatType { SILVER, GOLD, PLATINUM };

class Seat {
private:
    string number;
    SeatType type;

public:
    Seat(const string& number, SeatType type) {
        this->number = number;               // OOP: this keyword
        this->type = type;
    }

    const string& getNumber() const { return number; }
    SeatType getType() const { return type; }

    string getTypeName() const {
        if (type == SeatType::SILVER) return "SILVER";
        if (type == SeatType::GOLD) return "GOLD";
        return "PLATINUM";
    }
};
