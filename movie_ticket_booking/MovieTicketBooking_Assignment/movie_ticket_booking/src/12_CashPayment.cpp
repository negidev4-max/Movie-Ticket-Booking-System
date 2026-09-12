// OOP: inheritance - CashPayment is substitutable for Payment.
class CashPayment : public Payment {
private:
    bool shouldSucceed;

public:
    CashPayment() : Payment("CASH"), shouldSucceed(true) {}
    CashPayment(bool shouldSucceed) : Payment("CASH"), shouldSucceed(shouldSucceed) {}

    bool pay(double amount) override {
        cout << "Cash payment of Rs." << fixed << setprecision(2) << amount;
        cout << (shouldSucceed ? " received.\n" : " was not received.\n");
        return shouldSucceed;
    }
};
