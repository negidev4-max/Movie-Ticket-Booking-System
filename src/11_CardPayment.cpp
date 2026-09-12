// OOP: inheritance - CardPayment is substitutable for Payment.
class CardPayment : public Payment {
private:
    bool shouldSucceed;

public:
    CardPayment() : Payment("CARD"), shouldSucceed(true) {}
    CardPayment(bool shouldSucceed) : Payment("CARD"), shouldSucceed(shouldSucceed) {}

    bool pay(double amount) override {
        cout << "Card payment of Rs." << fixed << setprecision(2) << amount;
        cout << (shouldSucceed ? " succeeded.\n" : " failed.\n");
        return shouldSucceed;
    }
};
