class Customer {
private:
    string name;
    string phone;

public:
    Customer(const string& name, const string& phone) {
        this->name = name;
        this->phone = phone;
    }

    const string& getName() const { return name; }
    const string& getPhone() const { return phone; }
};
