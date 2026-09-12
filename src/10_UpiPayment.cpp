// OOP: inheritance - UpiPayment is substitutable for Payment.
class UpiPayment : public Payment {
private:
    bool shouldSucceed;

public:
    UpiPayment() : Payment("UPI"), shouldSucceed(true) {}
    UpiPayment(bool shouldSucceed) : Payment("UPI"), shouldSucceed(shouldSucceed) {}

    bool pay(double amount) override {
        cout << "UPI payment of Rs." << fixed << setprecision(2) << amount;
        cout << (shouldSucceed ? " succeeded.\n" : " failed.\n");
        return shouldSucceed;
    }
};
