class BookingService {
private:
    vector<unique_ptr<Customer>> customers;
    vector<unique_ptr<Booking>> bookings;    // composition: service owns Booking records
    PriceCalculator priceCalculator;
    TicketPrinter ticketPrinter;

    bool collectSeats(Show& show, const vector<string>& seatNumbers,
                      vector<ShowSeat*>& result, string& error) const {
        if (seatNumbers.empty()) {
            error = "Select at least one seat.";
            return false;
        }
        set<string> seen;
        for (const string& number : seatNumbers) {
            ShowSeat* seat = show.findShowSeat(number);
            if (seat == nullptr) { error = "Invalid seat number: " + number; return false; }
            if (!seat->isAvailable()) { error = "Seat " + number + " is already BOOKED"; return false; }
            if (!seen.insert(number).second) { error = "Duplicate seat number: " + number; return false; }
            result.push_back(seat);
        }
        return true;
    }

    void reserveSeats(const vector<ShowSeat*>& seats) const {
        for (ShowSeat* seat : seats) seat->bookSeat();
    }

    void releaseSeats(const vector<ShowSeat*>& seats) const {
        for (ShowSeat* seat : seats) seat->releaseSeat();
    }

    Booking* createPendingBooking(Show& show, const Customer& customer,
                                  const vector<ShowSeat*>& seats, double total) {
        customers.push_back(make_unique<Customer>(customer));
        auto pending = make_unique<Booking>(show, *customers.back(), seats, total);
        Booking* booking = pending.get();
        bookings.push_back(move(pending));
        return booking;
    }

    void failBooking(Booking& booking, const vector<ShowSeat*>& seats) const {
        releaseSeats(seats);
        booking.markFailed();
        cout << "Booking " << booking.getBookingId()
             << " is FAILED. Reserved seats were released.\n";
    }

    void confirmBooking(Booking& booking, const Payment& payment) const {
        booking.confirm(payment.getMethodName());
        cout << "Booking " << booking.getBookingId() << " is CONFIRMED.\n";
        ticketPrinter.print(booking);
    }

public:
    bool canBookSeats(Show& show, const vector<string>& seatNumbers,
                      string& error) const {
        vector<ShowSeat*> checkedSeats;
        return collectSeats(show, seatNumbers, checkedSeats, error);
    }

    // Association: a Customer asks BookingService to perform this operation.
    Booking* bookSeats(Show& show, const vector<string>& seatNumbers,
                       const Customer& customer, Payment& payment) {
        vector<ShowSeat*> selectedSeats;
        string error;
        if (!collectSeats(show, seatNumbers, selectedSeats, error)) {
            cout << "Booking rejected: " << error << ". No seat changed.\n";
            return nullptr;
        }

        double total = priceCalculator.calculate(selectedSeats);
        Booking* booking = createPendingBooking(show, customer, selectedSeats, total);
        reserveSeats(selectedSeats);
        cout << "Booking total: Rs." << fixed << setprecision(2) << total << "\n";
        bool paid = payment.pay(total);       // OOP: runtime polymorphism
        if (!paid) {
            failBooking(*booking, selectedSeats);
            return booking;
        }
        confirmBooking(*booking, payment);
        return booking;
    }

    Booking* findBooking(const string& bookingId) const {
        for (const unique_ptr<Booking>& booking : bookings) {
            if (booking->getBookingId() == bookingId) return booking.get();
        }
        return nullptr;
    }

    void printTicket(const string& bookingId) const {
        Booking* booking = findBooking(bookingId);
        if (booking == nullptr) { cout << "Booking ID not found.\n"; return; }
        if (!booking->isConfirmed()) {
            cout << "Ticket is available only for a CONFIRMED booking. Current status: "
                 << booking->getStatusName() << ".\n";
            return;
        }
        ticketPrinter.print(*booking);
    }

    void cancelBooking(const string& bookingId) {
        Booking* booking = findBooking(bookingId);
        if (booking == nullptr) { cout << "Booking ID not found.\n"; return; }
        if (!booking->cancel()) {
            cout << "Only a CONFIRMED booking can be cancelled. Current status: "
                 << booking->getStatusName() << ".\n";
            return;
        }
        releaseSeats(booking->getBookedSeats());
        cout << "Booking " << bookingId
             << " is CANCELLED. Its seats are AVAILABLE again.\n";
    }
};
