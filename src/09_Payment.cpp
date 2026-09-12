// OOP: abstraction - callers depend on this contract, not a payment subtype.
class Payment {
private:
    string methodName;

protected:
    Payment(const string& methodName) : methodName(methodName) {}

public:
    virtual ~Payment() = default;
    virtual bool pay(double amount) = 0;
    const string& getMethodName() const { return methodName; }
};
