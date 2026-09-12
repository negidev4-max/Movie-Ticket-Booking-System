# Required edge-case verification

Build used:

```text
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o movie_booking.exe
```

The test process used `demo/demo_input.txt` in a fresh process. All status was in memory, so the starting state was deterministic.

| Required edge case | Test action | Evidence expected in `demo_run.txt` | State result |
|---|---|---|---|
| 1. Already BOOKED seat | Confirm A1+B1 as BK1001, then request A1+A3 together. | `Booking rejected: Seat A1 is already BOOKED. No seat changed.` | A1 and B1 remain BOOKED, while the otherwise free A3 remains AVAILABLE; no partial reservation, payment, or new booking occurs. |
| 2. Failed payment | Request A2, choose Card, answer N. | `Card payment ... failed.` followed by `Booking BK1002 is FAILED. Reserved seats were released.` | Later layout shows A2 AVAILABLE; failed payment prints no ticket. |
| 3. Cancel booking | Cancel confirmed BK1001 and display the same show. | `Booking BK1001 is CANCELLED. Its seats are AVAILABLE again.` | Later layout shows both A1 and B1 AVAILABLE. |
| 4. Invalid seat/menu choice | Enter menu choice 99; later request Z9. | `Invalid menu choice. Choose 0 to 6.` and `Booking rejected: Invalid seat number: Z9 No seat changed.` | Program continues without a crash; all valid seat statuses remain unchanged. |

Additional validation covered by the program includes numeric parsing, range checks for movie/show/payment choices, non-empty text, duplicate seat numbers, and a ten-digit phone number.
