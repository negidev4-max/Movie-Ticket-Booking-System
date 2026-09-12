enum class BookingStatus { PENDING, CONFIRMED, FAILED, CANCELLED };

class Booking {
private:
    static int nextBookingNumber;            // OOP: static member for unique IDs
    string bookingId;
    const Show* show;
    const Customer* customer;                // association: Booking does not own Customer
    vector<ShowSeat*> bookedSeats;           // aggregation: Show owns these ShowSeats
    double bookingAmount;                    // OOP: encapsulation
    BookingStatus status;
    string paymentMethod;

public:
    Booking(const Show& show, const Customer& customer,
            const vector<ShowSeat*>& bookedSeats, double bookingAmount)
        : show(&show), customer(&customer), bookedSeats(bookedSeats),
          bookingAmount(bookingAmount), status(BookingStatus::PENDING) {
        ostringstream id;
        id << "BK" << nextBookingNumber++;
        this->bookingId = id.str();
    }

    const string& getBookingId() const { return bookingId; }
    const Show& getShow() const { return *show; }
    const Customer& getCustomer() const { return *customer; }
    const vector<ShowSeat*>& getBookedSeats() const { return bookedSeats; }
    double getBookingAmount() const { return bookingAmount; }
    const string& getPaymentMethod() const { return paymentMethod; }
    BookingStatus getStatus() const { return status; }
    bool isConfirmed() const { return status == BookingStatus::CONFIRMED; }

    string getStatusName() const {
        if (status == BookingStatus::PENDING) return "PENDING";
        if (status == BookingStatus::CONFIRMED) return "CONFIRMED";
        if (status == BookingStatus::FAILED) return "FAILED";
        return "CANCELLED";
    }

    void confirm(const string& method) {
        if (status != BookingStatus::PENDING) return;
        paymentMethod = method;
        status = BookingStatus::CONFIRMED;
    }

    void markFailed() {
        if (status == BookingStatus::PENDING) status = BookingStatus::FAILED;
    }

    bool cancel() {
        if (!isConfirmed()) return false;
        status = BookingStatus::CANCELLED;
        return true;
    }
};

int Booking::nextBookingNumber = 1001;
