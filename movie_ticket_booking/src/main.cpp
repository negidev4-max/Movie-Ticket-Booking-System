#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Unity build: every class remains in one .cpp file; no header files are used.
#include "01_Movie.cpp"
#include "02_Seat.cpp"
#include "03_Screen.cpp"
#include "04_Cinema.cpp"
#include "05_ShowSeat.cpp"
#include "06_Show.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "10_UpiPayment.cpp"
#include "11_CardPayment.cpp"
#include "12_CashPayment.cpp"
#include "13_PriceCalculator.cpp"
#include "14_TicketPrinter.cpp"
#include "15_BookingService.cpp"

string trim(const string& text) {
    size_t first = text.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

string upper(string text) {
    transform(text.begin(), text.end(), text.begin(),
              [](unsigned char ch) { return static_cast<char>(toupper(ch)); });
    return text;
}

int readInt(const string& prompt) {
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);
        stringstream parser(line);
        int value;
        char extra;
        if ((parser >> value) && !(parser >> extra)) return value;
        cout << "Invalid input. Please enter a number.\n";
    }
}

string readRequired(const string& prompt) {
    while (true) {
        cout << prompt;
        string value;
        getline(cin, value);
        value = trim(value);
        if (!value.empty()) return value;
        cout << "Input cannot be empty.\n";
    }
}

string readPhone() {
    while (true) {
        string phone = readRequired("Phone (10 digits): ");
        bool allDigits = all_of(phone.begin(), phone.end(),
                                [](unsigned char ch) { return isdigit(ch); });
        if (phone.size() == 10 && allDigits) return phone;
        cout << "Invalid phone number. Enter exactly 10 digits.\n";
    }
}

bool readApproval() {
    while (true) {
        string answer = upper(readRequired("Approve payment? (Y/N): "));
        if (answer == "Y") return true;
        if (answer == "N") return false;
        cout << "Invalid choice. Enter Y or N.\n";
    }
}

vector<string> splitSeatNumbers(const string& line) {
    vector<string> numbers;
    string item;
    stringstream input(line);
    while (getline(input, item, ',')) {
        item = upper(trim(item));
        if (!item.empty()) numbers.push_back(item);
    }
    return numbers;
}

void addStandardSeats(Screen& screen) {
    screen.addSeat("A1", SeatType::SILVER);
    screen.addSeat("A2", SeatType::SILVER);
    screen.addSeat("A3", SeatType::SILVER);
    screen.addSeat("A4", SeatType::SILVER);
    screen.addSeat("B1", SeatType::GOLD);
    screen.addSeat("B2", SeatType::GOLD);
    screen.addSeat("B3", SeatType::GOLD);
    screen.addSeat("C1", SeatType::PLATINUM);
    screen.addSeat("C2", SeatType::PLATINUM);
}

Cinema createCinema() {
    Cinema cinema("Campus Cinema");
    Screen firstScreen(1);
    Screen secondScreen(2);
    addStandardSeats(firstScreen);
    addStandardSeats(secondScreen);
    cinema.addScreen(firstScreen);
    cinema.addScreen(secondScreen);
    return cinema;
}

void listMovies(const vector<Movie>& movies) {
    cout << "\nMovies currently playing\n";
    for (size_t index = 0; index < movies.size(); ++index) {
        cout << index + 1 << ". " << movies[index].getTitle() << " | "
             << movies[index].getLanguage() << " | "
             << movies[index].getDurationMinutes() << " minutes\n";
    }
}

Movie* chooseMovie(vector<Movie>& movies) {
    listMovies(movies);
    int choice = readInt("Choose movie number: ");
    if (choice < 1 || choice > static_cast<int>(movies.size())) {
        cout << "Invalid movie choice.\n";
        return nullptr;
    }
    return &movies[choice - 1];
}

vector<Show*> showsForMovie(vector<Show>& shows, const Movie& movie) {
    vector<Show*> matchingShows;
    for (Show& show : shows) {
        if (&show.getMovie() == &movie) matchingShows.push_back(&show);
    }
    return matchingShows;
}

void listShows(const vector<Show*>& shows) {
    cout << "\nShows\n";
    for (size_t index = 0; index < shows.size(); ++index) {
        cout << index + 1 << ". " << shows[index]->getShowId()
             << " | Screen " << shows[index]->getScreen().getScreenNumber()
             << " | " << shows[index]->getStartTime() << "\n";
    }
}

Show* chooseShow(vector<Show>& shows, Movie& movie) {
    vector<Show*> matchingShows = showsForMovie(shows, movie);
    listShows(matchingShows);
    int choice = readInt("Choose show number: ");
    if (choice < 1 || choice > static_cast<int>(matchingShows.size())) {
        cout << "Invalid show choice.\n";
        return nullptr;
    }
    return matchingShows[choice - 1];
}

Show* chooseMovieAndShow(vector<Movie>& movies, vector<Show>& shows) {
    Movie* movie = chooseMovie(movies);
    if (movie == nullptr) return nullptr;
    return chooseShow(shows, *movie);
}

unique_ptr<Payment> createPayment(int choice, bool shouldSucceed) {
    if (choice == 1) return make_unique<UpiPayment>(shouldSucceed);
    if (choice == 2) return make_unique<CardPayment>(shouldSucceed);
    return make_unique<CashPayment>(shouldSucceed);
}

int choosePaymentMethod() {
    while (true) {
        cout << "Payment methods: 1. UPI  2. Card  3. Cash\n";
        int choice = readInt("Choose payment method: ");
        if (choice >= 1 && choice <= 3) return choice;
        cout << "Invalid payment choice.\n";
    }
}

void handleListShows(vector<Movie>& movies, vector<Show>& shows) {
    Movie* movie = chooseMovie(movies);
    if (movie == nullptr) return;
    listShows(showsForMovie(shows, *movie));
}

void handleDisplaySeats(vector<Movie>& movies, vector<Show>& shows) {
    Show* show = chooseMovieAndShow(movies, shows);
    if (show != nullptr) show->displaySeatLayout();
}

void handleBooking(vector<Movie>& movies, vector<Show>& shows,
                   BookingService& bookingService) {
    Show* show = chooseMovieAndShow(movies, shows);
    if (show == nullptr) return;
    show->displaySeatLayout();
    string seatLine = readRequired("Seat numbers separated by commas: ");
    vector<string> seatNumbers = splitSeatNumbers(seatLine);
    string error;
    if (!bookingService.canBookSeats(*show, seatNumbers, error)) {
        cout << "Booking rejected: " << error << ". No seat changed.\n";
        return;
    }

    string name = readRequired("Customer name: ");
    string phone = readPhone();
    int methodChoice = choosePaymentMethod();
    bool shouldSucceed = readApproval();
    unique_ptr<Payment> payment = createPayment(methodChoice, shouldSucceed);
    Customer customer(name, phone);
    bookingService.bookSeats(*show, seatNumbers, customer, *payment);
}

void printMenu() {
    cout << "\n===== MOVIE TICKET BOOKING =====\n";
    cout << "1. List movies\n";
    cout << "2. List shows for a movie\n";
    cout << "3. Display seat availability\n";
    cout << "4. Book seats and pay\n";
    cout << "5. Print a ticket\n";
    cout << "6. Cancel a booking\n";
    cout << "0. Exit\n";
}

int main() {
    vector<Movie> movies;
    movies.reserve(2);
    movies.emplace_back("3 Idiots", "Hindi", 170);
    movies.emplace_back("Interstellar", "English", 169);

    Cinema cinema = createCinema();
    vector<Show> shows;
    shows.reserve(3);
    shows.emplace_back("SH101", movies[0], *cinema.findScreen(1), "06:00 PM");
    shows.emplace_back("SH102", movies[0], *cinema.findScreen(2), "09:00 PM");
    shows.emplace_back("SH201", movies[1], *cinema.findScreen(2), "03:00 PM");

    BookingService bookingService;
    cout << "Welcome to " << cinema.getName() << "!\n";
    while (true) {
        printMenu();
        int choice = readInt("Choose: ");
        if (choice == 0) { cout << "Thank you.\n"; break; }
        if (choice == 1) listMovies(movies);
        else if (choice == 2) handleListShows(movies, shows);
        else if (choice == 3) handleDisplaySeats(movies, shows);
        else if (choice == 4) handleBooking(movies, shows, bookingService);
        else if (choice == 5) bookingService.printTicket(readRequired("Booking ID: "));
        else if (choice == 6) bookingService.cancelBooking(readRequired("Booking ID: "));
        else cout << "Invalid menu choice. Choose 0 to 6.\n";
    }
    return 0;
}
