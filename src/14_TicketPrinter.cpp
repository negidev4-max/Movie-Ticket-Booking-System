class TicketPrinter {
private:
    void printSeatNumbers(const vector<ShowSeat*>& seats) const {
        for (size_t index = 0; index < seats.size(); ++index) {
            if (index > 0) cout << ", ";
            cout << seats[index]->getSeat().getNumber();
        }
    }

public:
    void print(const Booking& booking) const {
        cout << "\n================ TICKET ================\n";
        cout << "Booking ID : " << booking.getBookingId() << "\n";
        cout << "Customer   : " << booking.getCustomer().getName() << "\n";
        cout << "Movie      : " << booking.getShow().getMovie().getTitle() << "\n";
        cout << "Screen     : " << booking.getShow().getScreen().getScreenNumber() << "\n";
        cout << "Time       : " << booking.getShow().getStartTime() << "\n";
        cout << "Seats      : ";
        printSeatNumbers(booking.getBookedSeats());
        cout << "\nAmount     : Rs." << fixed << setprecision(2)
             << booking.getBookingAmount() << "\n";
        cout << "Payment    : " << booking.getPaymentMethod() << "\n";
        cout << "Status     : " << booking.getStatusName() << "\n";
        cout << "========================================\n";
    }
};
