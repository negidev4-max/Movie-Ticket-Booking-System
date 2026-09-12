# Assignment 1 - Movie Ticket Booking System

**Course:** B.Tech CSE, Semester 5  
**Subject:** System Design (TCS-504)  
**Student name:** Abhishek Negi  
**Roll number:** ____________________

## A. Requirement analysis

### Functional requirements

| ID | Testable requirement |
|---|---|
| FR1 | **List movies:** When the user selects the movie-list option, the system shall display every currently playing movie with its title, language, and duration. With the seeded data, both *3 Idiots* and *Interstellar* must appear. |
| FR2 | **List shows:** Given a valid chosen movie, the system shall display every show for that movie, and each row shall contain the show ID, screen number, and start time. A movie with no shows shall produce an empty list rather than shows for another movie. |
| FR3 | **Seat availability:** Given a valid show, the system shall display every show seat under SILVER, GOLD, or PLATINUM and label each one exactly AVAILABLE or BOOKED. Viewing the layout shall not change any status. |
| FR4 | **Book seats:** A customer may select one or more seat numbers for one show. If any selected number is invalid, duplicated, or already BOOKED, the whole request shall be rejected and no selected seat shall change state. A booking is confirmed only after payment succeeds. |
| FR5 | **Calculate price:** The system shall calculate the total as `150 x SILVER count + 250 x GOLD count + 400 x PLATINUM count`. For A1 (SILVER) and B1 (GOLD), the displayed total must be Rs.400.00. |
| FR6 | **Payment:** Each booking attempt shall use exactly one of UPI, Card, or Cash. If `pay(total)` returns false, the booking status shall become FAILED, every temporarily reserved seat shall return to AVAILABLE, and no ticket shall be printed. |
| FR7 | **Print ticket:** For a CONFIRMED booking ID, the system shall print the booking ID, customer name, movie title, screen number, start time, all seat numbers, total amount, payment method, and CONFIRMED status. It shall refuse to issue a ticket for FAILED or CANCELLED records. |
| FR8 | **Cancel booking:** Given an existing CONFIRMED booking ID, the system shall change its status to CANCELLED and release all its seats to AVAILABLE. A missing ID or a non-confirmed booking shall produce a clear message and shall not modify any seat. |

### Non-functional requirements

| ID | Testable requirement |
|---|---|
| NFR1 | **Modularity:** Each named class shall have one responsibility and one `.cpp` file. The build shall use no custom header file. This can be checked from `src/` and by compiling only `main.cpp`. |
| NFR2 | **Extensibility:** A new payment type shall be implementable as a new subclass of `Payment`; `BookingService::bookSeats` shall continue to accept it through `Payment&` and shall not require edits. |
| NFR3 | **Robust input handling:** Non-numeric input, out-of-range menu/movie/show/payment choices, empty values, invalid phone numbers, and invalid seat numbers shall display a message and shall not terminate the process abnormally. |
| NFR4 | **Portability and responsiveness:** The source shall compile as standard C++17 with warnings enabled and use no platform-specific API. With the small in-memory seeded data, each menu operation shall complete interactively without deliberate delay. |

## B. Noun-verb analysis

### Marked problem statement

Legend: `[N: ...]` is a noun/candidate object; `[V: ...]` is a verb/candidate responsibility.

> A `[N: customer]` should be able to `[V: see]` which `[N: movies]` are playing, `[V: pick]` a `[N: show]`, `[V: see]` which `[N: seats]` are free, `[V: book]` seats, `[V: pay]`, `[V: get]` a `[N: ticket]`, and `[V: cancel]` a `[N: booking]` in one `[N: cinema]`. Each show has a `[N: screen]`, `[N: start time]`, and show-specific `[N: seat status]`.

### Nouns: keep or reject

| Noun found | Keep as class? | Reason |
|---|---:|---|
| Movie | Yes | It has identity and its own title, language, and duration. |
| Seat | Yes | It represents one physical numbered seat and its type. |
| Screen | Yes | It is an auditorium with a number and owns physical seats. |
| Cinema | Yes | It represents the single theatre and owns its screens. |
| Show | Yes | One movie-screen-time screening has its own identity. |
| ShowSeat | Yes | Availability belongs to a seat *for one show*, not to the physical chair. |
| Customer | Yes | Name and phone identify the person making the booking. |
| Booking | Yes | It records the show, customer, selected seats, amount, and status. |
| Payment | Yes, abstract | It is the common payment contract used polymorphically. |
| UPI/Card/Cash payment | Yes | Each provides a different implementation of the payment contract. |
| Price calculator | Yes | It isolates the one reason to change pricing rules. |
| Ticket printer | Yes | It isolates ticket formatting/output. |
| Booking service | Yes | It coordinates validation, reservation, payment, confirmation, and release. |
| Seat layout | No | It is a read-only view produced by `Show::displaySeatLayout`, not an object with independent identity or lifetime. |
| Start time | No | A string value is enough; it has no behaviour in the stated scope. |
| Language/duration/status | No | These are attributes/enumerated values of other classes. |
| Menu | No separate class | The suggested C++ structure places console input in `main.cpp`; it is presentation logic, not a domain entity. |

### Verbs mapped to methods

| Verb | Responsible class and method |
|---|---|
| list/see movies | `main.cpp::listMovies()` reads `Movie` data |
| list/pick shows | `main.cpp::showsForMovie()` and `chooseShow()` |
| display seat layout | `Show::displaySeatLayout()` |
| check seat | `Show::findShowSeat()`, `ShowSeat::isAvailable()` |
| book/reserve | `BookingService::bookSeats()`, `ShowSeat::bookSeat()` |
| calculate price | `PriceCalculator::calculate()` |
| pay | `Payment::pay()` implemented by each payment subtype |
| confirm/fail/cancel | `Booking::confirm()`, `markFailed()`, `cancel()` |
| print ticket | `TicketPrinter::print()` |
| release seat | `ShowSeat::releaseSeat()` coordinated by `BookingService` |

### Required classes: knows, does, must not do

| Class | What it knows | What it does | What it must NOT do |
|---|---|---|---|
| Movie | title, language, duration | exposes movie details | know shows, seats, payments, or bookings |
| Seat | physical number and type | exposes number/type | store BOOKED state (that differs by show) |
| Screen | screen number and owned seats | adds/exposes physical seats | schedule movies or process bookings |
| Cinema | cinema name and owned screens | adds/finds screens | calculate prices or accept payment |
| Show | show ID, borrowed movie/screen, time, owned show seats | finds and displays show seats | charge the customer or print tickets |
| ShowSeat | physical seat reference and private show status | validates booking and releases status | know customer/payment/booking totals |
| Customer | name and phone | exposes customer details | reserve seats or own the cinema |
| Booking | ID, show/customer references, selected show seats, private amount/status | controls booking status | calculate prices, execute payments, or print itself |
| Payment (abstract) | payment method name | declares `pay(amount)` contract | reserve seats or decide booking status |
| UpiPayment | simulated UPI outcome | performs UPI `pay` implementation | handle Card/Cash behaviour |
| CardPayment | simulated Card outcome | performs Card `pay` implementation | handle UPI/Cash behaviour |
| CashPayment | simulated Cash outcome | performs Cash `pay` implementation | handle UPI/Card behaviour |
| PriceCalculator | three named price constants | totals selected seats | book seats, take input, or print tickets |
| TicketPrinter | booking data passed to it | formats/prints one ticket | alter a booking or seat status |
| BookingService | retained customers/bookings and helper services | orchestrates the end-to-end flow | contain concrete UPI/Card/Cash selection logic |

## C. Relationship table with lifetime justification

| Pair | Choice | Lifetime-test justification |
|---|---|---|
| Cinema - Screen | Composition | `Cinema` stores `Screen` values. Destroying the cinema destroys those auditorium objects; they are not shared with another cinema. |
| Screen - Seat | Composition | `Screen` stores its physical `Seat` values. Destroying a screen destroys that screen's seat collection. |
| Show - Movie | Aggregation | `Show` borrows a movie pointer. Deleting one show does not delete the movie because the same movie may have other shows. |
| Show - Screen | Aggregation | `Show` borrows a screen pointer. Cancelling/destroying a show does not destroy the auditorium, which can host other shows. |
| Show - ShowSeat | Composition | `Show` creates and stores its `ShowSeat` values. When the show is destroyed, its show-specific availability states have no meaning and die with it. |
| Booking - Customer | Association | A booking points to a customer record retained by `BookingService`. Deleting a booking does not delete the customer; neither is a physical part of the other. |
| Booking - ShowSeat | Aggregation | A booking keeps non-owning pointers to selected `ShowSeat` objects. Cancelling/destroying it releases but does not destroy them; their owner is `Show`. |
| Booking - Payment | Association | A payment object is used for the transaction and is not owned by the booking. The booking retains only the successful method label, so either object can end independently. |
| Payment - UpiPayment | Inheritance | This is an is-a relationship, not whole-part lifetime ownership. `UpiPayment` implements the abstract `Payment` contract and can be used through `Payment&`. |
| BookingService - Booking | Composition | `BookingService` owns booking records through `unique_ptr`. Destroying the service destroys its in-memory booking records. |

## D. Class diagram

The complete diagram is in [`../diagrams/class_diagram.mmd`](../diagrams/class_diagram.mmd). It includes all required classes, three class compartments (name, attributes, methods), `-/#/+` visibility, an abstract `Payment`, multiplicities, filled-diamond composition, hollow-diamond aggregation, associations, and hollow-triangle inheritance.

## E. Booking/payment sequence diagram

The sequence diagram is in [`../diagrams/booking_sequence.mmd`](../diagrams/booking_sequence.mmd). It follows the required “customer books A1 and pays by UPI” case. It contains all eight named lifelines, activation bars, synchronous calls, dashed return messages, and `<<create>>` messages for `Booking` and `UpiPayment`. A note records the required failed-payment rollback.

## F. Modular code and demo run

### File arrangement

| Files | Purpose |
|---|---|
| `01_Movie.cpp` to `08_Booking.cpp` | Core entity/value classes |
| `09_Payment.cpp` | Abstract payment contract |
| `10_UpiPayment.cpp` to `12_CashPayment.cpp` | One concrete payment class per file |
| `13_PriceCalculator.cpp` | Pricing only |
| `14_TicketPrinter.cpp` | Ticket formatting/printing only |
| `15_BookingService.cpp` | Flow orchestration and rollback |
| `main.cpp` | Seed data, validated console input, menu, and unity-build includes |

Build command:

```text
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o movie_booking.exe
```

The clean build completed without warnings. The captured demonstration is in [`../demo/demo_run.txt`](../demo/demo_run.txt), and the exact input is in [`../demo/demo_input.txt`](../demo/demo_input.txt). It shows an Rs.400.00 UPI booking for A1+B1, ticket BK1001, atomic rejection of A1+A3 because A1 is already booked, Card failure with release, cancellation with release, invalid menu input, and invalid seat rejection.

### OOP concepts present in code

| Concept | Where it appears |
|---|---|
| Encapsulation | `ShowSeat::seatStatus` and `Booking::bookingAmount/status` are private and changed only by controlled methods. |
| Abstraction | `Payment` declares pure virtual `pay(double)`. |
| Inheritance | `UpiPayment`, `CardPayment`, and `CashPayment` publicly inherit `Payment`. |
| Runtime polymorphism | `BookingService` receives `Payment&` and calls `payment.pay(total)`. |
| Compile-time polymorphism | `Screen::addSeat` and the payment constructors are overloaded. |
| Static member | `Booking::nextBookingNumber` generates BK1001, BK1002, and so on. |
| `this` keyword | Constructors such as `Movie` and `Seat` use `this` to distinguish members. |
| Composition | `Cinema` owns screens, `Screen` owns seats, `Show` owns show seats, and `BookingService` owns records. |
| Aggregation | `Show` borrows movie/screen objects and `Booking` borrows selected show seats. |
| Association | A customer interacts with `BookingService`; the service accepts customer and payment objects without those becoming part of one another. |

## G. SOLID mapping and one deliberate omission

| Principle | Application in this design |
|---|---|
| **S - Single Responsibility** | `PriceCalculator` changes for pricing rules, `TicketPrinter` changes for ticket formatting, payment subclasses change for payment behaviour, and `Booking` remains a record with controlled status. |
| **O - Open/Closed** | A `NetBankingPayment : public Payment` class can be added and passed to `BookingService` without changing the booking algorithm. Only the presentation-layer menu/factory would expose the new choice. |
| **L - Liskov Substitution** | UPI, Card, and Cash all work through the same `Payment&` contract and return success/failure from `pay(amount)` without requiring subtype-only setup calls. |
| **I - Interface Segregation** | `Payment` exposes only what every payment type needs: `pay` and a method name. It does not force Cash to implement irrelevant card/UPI operations. |
| **D - Dependency Inversion** | `BookingService::bookSeats` receives a `Payment&`; it does not construct or switch on concrete payment classes. |

### One thing deliberately not implemented

`Payment` deliberately has **no `refund()` method**. The eight-feature scope says cancellation must release seats but does not specify financial refund rules, and Cash/UPI/Card refunds would not share identical behaviour. Adding a forced refund operation now would widen the assignment scope and violate interface segregation. The current cancellation therefore changes the booking status and seat availability only.

## Verification conclusion

The program compiles under C++17 with warnings enabled. The repeatable demo verifies the successful path and all four required edge cases. Detailed before/after evidence is recorded in [`../tests/EDGE_CASE_VERIFICATION.md`](../tests/EDGE_CASE_VERIFICATION.md).
