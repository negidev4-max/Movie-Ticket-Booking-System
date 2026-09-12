# Movie Ticket Booking System

This is a menu-driven C++17 learning project for one cinema. It implements only the eight assignment features: list movies, list shows, display show-specific seat status, book seats atomically, calculate type-based prices, pay by UPI/Card/Cash, print a ticket, and cancel a booking while releasing its seats.

## Build and run

Requirements: a C++ compiler with C++17 support (for example MinGW g++ on Windows).

From this project directory:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic src\main.cpp -o movie_booking.exe
.\movie_booking.exe
```

On Linux or macOS:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o movie_booking
./movie_booking
```

### Why only `main.cpp` is compiled

The course requires one class per file and no header files. Standard separate compilation normally needs headers, so this project uses a small *unity build*: `main.cpp` includes the class `.cpp` files once, in dependency order. Each class is still defined in exactly one numbered `.cpp` file. Compile only `main.cpp`; compiling every `.cpp` separately would define the classes twice.

## Project contents

- `src/` - 15 single-class files plus `main.cpp`
- `diagrams/class_diagram.mmd` - complete Mermaid class diagram
- `diagrams/booking_sequence.mmd` - booking and UPI payment sequence diagram
- `report/Assignment_Report.md` - submission items A-G
- `demo/demo_input.txt` - repeatable input that exercises the normal flow and edge cases
- `demo/demo_run.txt` - captured output from that input
- `tests/EDGE_CASE_VERIFICATION.md` - evidence for the four required edge cases
- `movie_booking.exe` - the verified Windows build (source remains the authoritative deliverable)

## Seed data and console conventions

The program starts with two movies, two screens, and three shows. Every show starts with its own AVAILABLE status for seats A1-A4 (SILVER), B1-B3 (GOLD), and C1-C2 (PLATINUM). Prices are Rs.150, Rs.250, and Rs.400 respectively. `Rs.` is used instead of the rupee glyph so the output remains readable on older Windows terminals.

Payment is simulated locally. Answer `Y` to approve it or `N` to deliberately exercise the failure path. No real payment or personal data is transmitted or stored after the process ends.

## Reading order

Start with the [assignment report](report/Assignment_Report.md), then open the two Mermaid files in any Mermaid-compatible preview. Build the code with the command above and compare a run with `demo/demo_run.txt`.

This implementation was independently structured from the supplied assignment requirements and has no external repository dependency. Replace the student-detail placeholders in the report before submission and be prepared to explain the design and code in your own words.
