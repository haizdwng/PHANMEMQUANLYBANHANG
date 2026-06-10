#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <conio.h>
using namespace std;

string formatDouble(double value, int precision = 2) {
  ostringstream oss;
  oss << fixed << setprecision(precision) << value;
  return oss.str();
}

int parseInt(const string& s, int defaultValue = 0) {
  stringstream ss(s);
  int v = defaultValue;
  ss >> v;
  return v;
}

double parseDouble(const string& s, double defaultValue = 0.0) {
  stringstream ss(s);
  double v = defaultValue;
  ss >> v;
  return v;
}

vector<string> split(const string& s, char delim) {
  vector<string> parts;
  string item;
  stringstream ss(s);
  while (getline(ss, item, delim)) parts.push_back(item);
  return parts;
}

string toLower(string s) {
  for (size_t i = 0; i < s.size(); ++i) s[i] = static_cast<char>(tolower(static_cast<unsigned char>(s[i])));
  return s;
}

string nowDateTime() {
  time_t t = time(nullptr);
  tm* tmPtr = localtime(&t);
  char buf[64];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tmPtr);
  return string(buf);
}

string inputLine(const string& prompt) {
  cout << prompt;
  string s;
  getline(cin, s);
  return s;
}

int inputInt(const string& prompt, int minValue, int maxValue) {
  while (true) {
    string s = inputLine(prompt);
    stringstream ss(s);
    int v = 0;
    if (ss >> v && v >= minValue && v <= maxValue) return v;
    cout << "Gia tri khong hop le. Hay thu lai.\n";
  }
}

double inputDouble(const string& prompt, double minValue) {
  while (true) {
    string s = inputLine(prompt);
    stringstream ss(s);
    double v = 0.0;
    if (ss >> v && v >= minValue) return v;
    cout << "Gia tri khong hop le. Hay thu lai.\n";
  }
}

string inputPassword(const string& prompt) {
  cout << prompt;
  string password;
  char ch;
  while (true) {
    ch = getch();
    if (ch == '\r') {
      break;
    } else if (ch == '\b') {
      if (!password.empty()) {
        cout << "\b \b";
        password.erase(password.size() - 1);
      }
    } else {
      password += ch;
      cout << '*';
    }
  }

  cout << endl;
  return password;
}

void printSeparator(const vector<int>& widths) {
  cout << "+";
  for (size_t i = 0; i < widths.size(); ++i) cout << string(widths[i], '-') << "+";
  cout << "\n";
}

void printRow(const vector<string>& cols, const vector<int>& widths) {
  cout << "|";
  for (size_t i = 0; i < cols.size(); ++i) cout << " " << left << setw(widths[i] - 1) << cols[i] << "|";
  cout << "\n";
}

vector<int> computeWidths(const vector<string>& headers, const vector<vector<string>>& rows) {
  vector<int> widths(headers.size(), 0);
  for (size_t i = 0; i < headers.size(); ++i) {
    int maxLen = static_cast<int>(headers[i].size());
    for (size_t r = 0; r < rows.size(); ++r) if (i < rows[r].size()) {
      int len = static_cast<int>(rows[r][i].size());
      if (len > maxLen) maxLen = len;
    }
    widths[i] = maxLen + 2;
  }
  return widths;
}

class User {
  protected:
    string username;
    string password;
    string email;
    string fullName;
    string phoneNumber;
  public:
    User() {}
    User(const string& u, const string& p, const string& e, const string& f, const string& phone) : username(u), password(p), email(e), fullName(f), phoneNumber(phone) {}
    virtual ~User() {}
    const string& getUsername() const { return username; }
    bool checkLogin(const string& u, const string& p) const { return username == u && password == p; }
    void changePassword(const string& newPass) { password = newPass; }
    virtual string getRole() const = 0;
    virtual string serialize() const = 0;
};

class Customer : public User {
  private:
    string shippingAddress;
    string customerType;
    int loyaltyPoints;
  public:
    Customer() : loyaltyPoints(0) {}
    Customer(const string& u, const string& p, const string& e, const string& f, const string& phone, const string& addr, const string& type, int points) : User(u, p, e, f, phone), shippingAddress(addr), customerType(type), loyaltyPoints(points) {}
    string getRole() const override { return "CUSTOMER"; }
    const string& getAddress() const { return shippingAddress; }
    const string& getType() const { return customerType; }
    int getPoints() const { return loyaltyPoints; }
    void addPoints(int p) { loyaltyPoints += p; }
    string serialize() const override {
      return "CUSTOMER|" + username + "|" + password + "|" + email + "|" + fullName + "|" + phoneNumber + "|" + shippingAddress + "|" + customerType + "|" + to_string(loyaltyPoints);
    }
};

class Admin : public User {
  private:
    int adminLevel;
    string lastLogin;
  public:
    Admin() : adminLevel(1) {}
    Admin(const string& u, const string& p, const string& e, const string& f, const string& phone, int level, const string& last) : User(u, p, e, f, phone), adminLevel(level), lastLogin(last) {}
    string getRole() const override { return "ADMIN"; }
    int getLevel() const { return adminLevel; }
    const string& getLastLogin() const { return lastLogin; }
    void updateLastLogin() { lastLogin = nowDateTime(); }
    string serialize() const override {
      return "ADMIN|" + username + "|" + password + "|" + email + "|" + fullName + "|" + phoneNumber + "|" + to_string(adminLevel) + "|" + lastLogin;
    }
};

class Product {
  protected:
    string productID;
    string productName;
    double basePrice;
    int stockQuantity;
    string manufacturer;
  public:
    Product() : basePrice(0.0), stockQuantity(0) {}
    Product(const string& id, const string& name, double price, int stock, const string& maker) : productID(id), productName(name), basePrice(price), stockQuantity(stock), manufacturer(maker) {}
    virtual ~Product() {}
    const string& getID() const { return productID; }
    const string& getName() const { return productName; }
    double getBasePrice() const { return basePrice; }
    int getStock() const { return stockQuantity; }
    void setStock(int qty) { stockQuantity = qty; }
    void reduceStock(int qty) { stockQuantity = max(0, stockQuantity - qty); }
    const string& getManufacturer() const { return manufacturer; }
    virtual double calculateFinalPrice() const = 0;
    virtual string getType() const = 0;
    virtual string getDetail1() const = 0;
    virtual string getDetail2() const = 0;
    virtual string getDetail3() const = 0;
    virtual string serialize() const = 0;
};

class Electronic : public Product {
  private:
    int warrantyPeriod;
    double powerWatt;
    double voltage;
  public:
    Electronic() : warrantyPeriod(0), powerWatt(0.0), voltage(0.0) {}
    Electronic(const string& id, const string& name, double price, int stock, const string& maker, int warranty, double power, double volt) : Product(id, name, price, stock, maker), warrantyPeriod(warranty), powerWatt(power), voltage(volt) {}
    double calculateFinalPrice() const override {
      double price = basePrice;
      if (warrantyPeriod > 12) price += basePrice * 0.05;
      price *= 1.1;
      return price;
    }
    string getType() const override { return "Electronic"; }
    string getDetail1() const override { return to_string(warrantyPeriod); }
    string getDetail2() const override { return formatDouble(powerWatt, 0); }
    string getDetail3() const override { return formatDouble(voltage, 0); }
    string serialize() const override {
      return "Electronic|" + productID + "|" + productName + "|" + formatDouble(basePrice) + "|" + to_string(stockQuantity) + "|" + manufacturer + "|" + to_string(warrantyPeriod) + "|" + formatDouble(powerWatt) + "|" + formatDouble(voltage);
    }
};

class Clothing : public Product {
  private:
    string size;
    string material;
    string color;
  public:
    Clothing() {}
    Clothing(const string& id, const string& name, double price, int stock, const string& maker, const string& s, const string& m, const string& c) : Product(id, name, price, stock, maker), size(s), material(m), color(c) {}

    double calculateFinalPrice() const override {
      double price = basePrice * 1.1;
      if (toLower(material) == "cotton") price *= 0.9;
      return price;
    }
    string getType() const override { return "Clothing"; }
    string getDetail1() const override { return size; }
    string getDetail2() const override { return material; }
    string getDetail3() const override { return color; }
    string serialize() const override {
      return "Clothing|" + productID + "|" + productName + "|" + formatDouble(basePrice) + "|" + to_string(stockQuantity) + "|" + manufacturer + "|" + size + "|" + material + "|" + color;
    }
};

struct CartItem {
  string productID;
  int quantity;
};

struct Cart {
  string customerID;
  vector<CartItem> items;
};

struct OrderItem {
  string productID;
  string productName;
  int quantity;
  double unitPrice;
  double finalPrice;
};

class Order {
  public:
    string orderID;
    string customerID;
    string orderDate;
    string status;
    string paymentMethod;
    double totalAmount;
    vector<OrderItem> items;
    string serialize() const {
      string data = orderID + "|" + customerID + "|" + orderDate + "|" + status + "|" + paymentMethod + "|" + formatDouble(totalAmount) + "|";
      for (size_t i = 0; i < items.size(); ++i) {
        const OrderItem& it = items[i];
        data += it.productID + ":" + it.productName + ":" + to_string(it.quantity) + ":" + formatDouble(it.unitPrice) + ":" + formatDouble(it.finalPrice);
        if (i + 1 < items.size()) data += ",";
      }
      return data;
    }
};

Product* findProductByID(vector<Product*>& products, const string& id) {
  for (size_t i = 0; i < products.size(); ++i) if (products[i]->getID() == id) return products[i];
  return nullptr;
}

User* findUserByUsername(vector<User*>& users, const string& username) {
  for (size_t i = 0; i < users.size(); ++i) if (users[i]->getUsername() == username) return users[i];
  return nullptr;
}

Cart* findCartByCustomer(vector<Cart>& carts, const string& customerID) {
  for (size_t i = 0; i < carts.size(); ++i) if (carts[i].customerID == customerID) return &carts[i];
  return nullptr;
}

void loadUsers(vector<User*>& users) {
  ifstream in("users.txt");
  if (!in) {
    users.push_back(new Admin("admin", "admin", "admin@gmail.com", "Nguyen Van A", "0000000000", 1, nowDateTime()));
    users.push_back(new Customer("customer", "customer", "customer@gmail.com", "Nguyen Van B", "0000000000", "Nghe An", "Thanh vien", 0));
    return;
  }
  string line;
  while (getline(in, line)) {
    if (line.empty()) continue;
    vector<string> parts = split(line, '|');
    if (parts.size() < 6) continue;
    string role = parts[0];
    if (role == "ADMIN" && parts.size() >= 8) users.push_back(new Admin(parts[1], parts[2], parts[3], parts[4], parts[5], parseInt(parts[6]), parts[7]));
    else if (role == "CUSTOMER" && parts.size() >= 9) users.push_back(new Customer(parts[1], parts[2], parts[3], parts[4], parts[5], parts[6], parts[7], parseInt(parts[8])));
  }
}

void saveUsers(const vector<User*>& users) {
  ofstream out("users.txt");
  for (size_t i = 0; i < users.size(); ++i) out << users[i]->serialize() << "\n";
}

void loadProducts(vector<Product*>& products) {
  ifstream in("products.txt");
  if (!in) {
    products.push_back(new Electronic("E001", "MacBook Pro M5 Max", 89990000, 50, "Apple", 12, 140, 20));
    products.push_back(new Electronic("E002", "Dell XPS 17", 55990000, 40, "Dell", 24, 130, 19));
    products.push_back(new Electronic("E003", "Sony WH-1000XM6", 10990000, 120, "Sony", 18, 30, 5));
    products.push_back(new Electronic("E004", "LG OLED C5 65 Inch", 45990000, 25, "LG", 36, 250, 220));
    products.push_back(new Electronic("E005", "Asus ROG Strix G18", 68990000, 35, "Asus", 24, 280, 20));
    products.push_back(new Clothing("C001", "Ao hoodie", 450000, 30, "Nike", "L", "polyester", "den"));
    products.push_back(new Clothing("C002", "Quan jean", 550000, 25, "Levis", "32", "jean", "xanh"));
    products.push_back(new Clothing("C003", "Ao so mi", 320000, 40, "Routine", "M", "cotton", "trang"));
    products.push_back(new Clothing("C004", "Quan short", 250000, 35, "Adidas", "L", "kaki", "xam"));
    products.push_back(new Clothing("C005", "Ao khoac bomber", 780000, 20, "Zara", "XL", "du", "reu"));
    return;
  }
  string line;
  while (getline(in, line)) {
    if (line.empty()) continue;
    vector<string> parts = split(line, '|');
    if (parts.size() < 9) continue;
    string type = parts[0];
    if (type == "Electronic") products.push_back(new Electronic(parts[1], parts[2], parseDouble(parts[3]), parseInt(parts[4]), parts[5], parseInt(parts[6]), parseDouble(parts[7]), parseDouble(parts[8])));
    else if (type == "Clothing") products.push_back(new Clothing(parts[1], parts[2], parseDouble(parts[3]), parseInt(parts[4]), parts[5], parts[6], parts[7], parts[8]));
  }
}

void saveProducts(const vector<Product*>& products) {
  ofstream out("products.txt");
  for (size_t i = 0; i < products.size(); ++i) out << products[i]->serialize() << "\n";
}

void loadCarts(vector<Cart>& carts) {
  ifstream in("carts.txt");
  if (!in) return;
  string line;
  while (getline(in, line)) {
    if (line.empty()) continue;
    vector<string> parts = split(line, '|');
    if (parts.size() < 2) continue;
    Cart cart;
    cart.customerID = parts[0];
    vector<string> items = split(parts[1], ',');
    for (size_t i = 0; i < items.size(); ++i) {
      vector<string> kv = split(items[i], ':');
      if (kv.size() == 2) {
        CartItem ci;
        ci.productID = kv[0];
        ci.quantity = parseInt(kv[1]);
        cart.items.push_back(ci);
      }
    }
    carts.push_back(cart);
  }
}

void saveCarts(const vector<Cart>& carts) {
  ofstream out("carts.txt");
  for (size_t i = 0; i < carts.size(); ++i) {
    out << carts[i].customerID << "|";
    for (size_t j = 0; j < carts[i].items.size(); ++j) {
      out << carts[i].items[j].productID << ":" << carts[i].items[j].quantity;
      if (j + 1 < carts[i].items.size()) out << ",";
    }
    out << "\n";
  }
}

void loadOrders(vector<Order>& orders) {
  ifstream in("orders.txt");
  if (!in) return;
  string line;
  while (getline(in, line)) {
    if (line.empty()) continue;
    vector<string> parts = split(line, '|');
    if (parts.size() < 7) continue;
    Order order;
    order.orderID = parts[0];
    order.customerID = parts[1];
    order.orderDate = parts[2];
    order.status = parts[3];
    order.paymentMethod = parts[4];
    order.totalAmount = parseDouble(parts[5]);
    vector<string> items = split(parts[6], ',');
    for (size_t i = 0; i < items.size(); ++i) {
      vector<string> fields = split(items[i], ':');
      if (fields.size() >= 5) {
        OrderItem it;
        it.productID = fields[0];
        it.productName = fields[1];
        it.quantity = parseInt(fields[2]);
        it.unitPrice = parseDouble(fields[3]);
        it.finalPrice = parseDouble(fields[4]);
        order.items.push_back(it);
      }
    }
    orders.push_back(order);
  }
}

void saveOrders(const vector<Order>& orders) {
  ofstream out("orders.txt");
  for (size_t i = 0; i < orders.size(); ++i) out << orders[i].serialize() << "\n";
}

void printProductsTable(const vector<Product*>& products) {
  vector<string> headers = { "ID", "Ten", "Loai", "Gia goc", "Gia sau thue", "Ton", "NSX", "CT1", "CT2", "CT3" };
  vector<vector<string>> rows;
  for (size_t i = 0; i < products.size(); ++i) {
    vector<string> row;
    row.push_back(products[i]->getID());
    row.push_back(products[i]->getName());
    row.push_back(products[i]->getType() == "Electronic" ? "Dien tu" : "Thoi trang");
    row.push_back(formatDouble(products[i]->getBasePrice()));
    row.push_back(formatDouble(products[i]->calculateFinalPrice()));
    row.push_back(to_string(products[i]->getStock()));
    row.push_back(products[i]->getManufacturer());
    row.push_back(products[i]->getDetail1());
    row.push_back(products[i]->getDetail2());
    row.push_back(products[i]->getDetail3());
    rows.push_back(row);
  }
  vector<int> widths = computeWidths(headers, rows);
  printSeparator(widths);
  printRow(headers, widths);
  printSeparator(widths);
  for (size_t i = 0; i < rows.size(); ++i) printRow(rows[i], widths);
  printSeparator(widths);
}

void printOrdersTable(const vector<Order>& orders) {
  vector<string> headers = { "Ma don", "Khach", "Ngay", "Trang thai", "Thanh toan", "Tong tien" };
  vector<vector<string>> rows;
  for (size_t i = 0; i < orders.size(); ++i) {
    vector<string> row;
    row.push_back(orders[i].orderID);
    row.push_back(orders[i].customerID);
    row.push_back(orders[i].orderDate);
    row.push_back(orders[i].status);
    row.push_back(orders[i].paymentMethod);
    row.push_back(formatDouble(orders[i].totalAmount));
    rows.push_back(row);
  }
  vector<int> widths = computeWidths(headers, rows);
  printSeparator(widths);
  printRow(headers, widths);
  printSeparator(widths);
  for (size_t i = 0; i < rows.size(); ++i) printRow(rows[i], widths);
  printSeparator(widths);
}

void printCartTable(const vector<OrderItem>& items) {
  vector<string> headers = { "ID", "Ten", "So luong", "Don gia", "Thanh tien" };
  vector<vector<string>> rows;
  for (size_t i = 0; i < items.size(); ++i) {
    vector<string> row;
    row.push_back(items[i].productID);
    row.push_back(items[i].productName);
    row.push_back(to_string(items[i].quantity));
    row.push_back(formatDouble(items[i].unitPrice));
    row.push_back(formatDouble(items[i].finalPrice));
    rows.push_back(row);
  }
  vector<int> widths = computeWidths(headers, rows);
  printSeparator(widths);
  printRow(headers, widths);
  printSeparator(widths);
  for (size_t i = 0; i < rows.size(); ++i) printRow(rows[i], widths);
  printSeparator(widths);
}

int nextOrderNumber(const vector<Order>& orders) {
  int maxNum = 0;
  for (size_t i = 0; i < orders.size(); ++i) {
    string id = orders[i].orderID;
    if (id.size() > 3 && id.substr(0, 3) == "ORD") {
      int num = parseInt(id.substr(3));
      if (num > maxNum) maxNum = num;
    }
  }
  return maxNum + 1;
}

void addProduct(vector<Product*>& products) {
  cout << "Chon loai san pham:\n";
  cout << "1. Dien tu\n";
  cout << "2. Thoi trang\n";
  int type = inputInt("Nhap lua chon: ", 1, 2);
  string id = inputLine("Ma san pham: ");
  if (findProductByID(products, id)) {
    cout << "Ma san pham da ton tai.\n";
    return;
  }
  string name = inputLine("Ten san pham: ");
  double price = inputDouble("Gia goc: ", 0);
  int stock = inputInt("So luong ton: ", 0, 1000000);
  string maker = inputLine("Nha san xuat: ");
  if (type == 1) {
    int warranty = inputInt("Bao hanh (thang): ", 0, 120);
    double power = inputDouble("Cong suat (W): ", 0);
    double volt = inputDouble("Dien ap (V): ", 0);
    products.push_back(new Electronic(id, name, price, stock, maker, warranty, power, volt));
  } else {
    string size = inputLine("Size (S/M/L/XL): ");
    string material = inputLine("Chat lieu: ");
    string color = inputLine("Mau sac: ");
    products.push_back(new Clothing(id, name, price, stock, maker, size, material, color));
  }
  cout << "Da them san pham.\n";
}

void updateInventory(vector<Product*>& products) {
  string id = inputLine("Nhap ma san pham can cap nhat: ");
  Product* p = findProductByID(products, id);
  if (!p) {
    cout << "Khong tim thay san pham.\n";
    return;
  }
  int stock = inputInt("Nhap so luong ton kho moi: ", 0, 1000000);
  p->setStock(stock);
  cout << "Da cap nhat ton kho.\n";
}

void searchProducts(const vector<Product*>& products) {
  string keyword = toLower(inputLine("Nhap tu khoa: "));
  vector<Product*> result;
  for (size_t i = 0; i < products.size(); ++i) {
    string name = toLower(products[i]->getName());
    if (name.find(keyword) != string::npos) result.push_back(products[i]);
  }
  if (result.empty()) {
    cout << "Khong tim thay san pham.\n";
    return;
  }
  printProductsTable(result);
}

void addToCart(vector<Cart>& carts, const string& customerID, vector<Product*>& products) {
  string id = inputLine("Nhap ma san pham: ");
  Product* p = findProductByID(products, id);
  if (!p) {
    cout << "Khong tim thay san pham.\n";
    return;
  }
  if (p->getStock() == 0) {
    cout << "San pham da het hang.\n";
    return;
  }
  int qty = inputInt("Nhap so luong: ", 1, p->getStock());
  Cart* cart = findCartByCustomer(carts, customerID);
  if (!cart) {
    Cart newCart;
    newCart.customerID = customerID;
    carts.push_back(newCart);
    cart = &carts.back();
  }
  bool found = false;
  for (size_t i = 0; i < cart->items.size(); ++i) {
    if (cart->items[i].productID == id) {
      cart->items[i].quantity += qty;
      found = true;
      break;
    }
  }
  if (!found) {
    CartItem item;
    item.productID = id;
    item.quantity = qty;
    cart->items.push_back(item);
  }
  cout << "Da them vao gio hang.\n";
}

vector<OrderItem> buildCartItems(const Cart& cart, const vector<Product*>& products) {
  vector<OrderItem> items;
  for (size_t i = 0; i < cart.items.size(); ++i) {
    Product* p = findProductByID(const_cast<vector<Product*>&>(products), cart.items[i].productID);
    if (!p) continue;
    OrderItem it;
    it.productID = p->getID();
    it.productName = p->getName();
    it.quantity = cart.items[i].quantity;
    it.unitPrice = p->calculateFinalPrice();
    it.finalPrice = it.unitPrice * it.quantity;
    items.push_back(it);
  }
  return items;
}

void viewCart(const vector<Cart>& carts, const string& customerID, const vector<Product*>& products) {
  const Cart* cart = nullptr;
  for (size_t i = 0; i < carts.size(); ++i) {
    if (carts[i].customerID == customerID) {
      cart = &carts[i];
      break;
    }
  }
  if (!cart || cart->items.empty()) {
    cout << "Gio hang rong.\n";
    return;
  }
  vector<OrderItem> items = buildCartItems(*cart, products);
  printCartTable(items);
}

void checkout(vector<Cart>& carts, vector<Product*>& products, vector<Order>& orders, Customer* customer) {
  Cart* cart = findCartByCustomer(carts, customer->getUsername());
  if (!cart || cart->items.empty()) {
    cout << "Gio hang rong.\n";
    return;
  }
  vector<OrderItem> items = buildCartItems(*cart, products);
  if (items.empty()) {
    cout << "Khong co san pham hop le trong gio hang.\n";
    return;
  }
  double total = 0.0;
  for (size_t i = 0; i < items.size(); ++i) total += items[i].finalPrice;
  cout << "\nThong tin gio hang:\n";
  printCartTable(items);
  cout << "Tong tien: " << formatDouble(total) << "\n";
  int pay = inputInt("Chon phuong thuc thanh toan (1: Tien mat, 2: Chuyen khoan): ", 1, 2);
  string method = (pay == 1) ? "Tien mat" : "Chuyen khoan";
  int confirm = inputInt("Xac nhan thanh toan? (1: Co, 0: Khong): ", 0, 1);
  if (confirm == 0) {
    cout << "Da huy thanh toan.\n";
    return;
  }
  for (size_t i = 0; i < cart->items.size(); ++i) {
    Product* p = findProductByID(products, cart->items[i].productID);
    if (!p || p->getStock() < cart->items[i].quantity) {
      cout << "San pham khong du ton kho. Vui long kiem tra lai.\n";
      return;
    }
  }
  for (size_t i = 0; i < cart->items.size(); ++i) {
    Product* p = findProductByID(products, cart->items[i].productID);
    if (p) p->reduceStock(cart->items[i].quantity);
  }
  Order order;
  order.orderID = "ORD" + to_string(nextOrderNumber(orders));
  order.customerID = customer->getUsername();
  order.orderDate = nowDateTime();
  order.status = "Cho xac nhan";
  order.paymentMethod = method;
  order.totalAmount = total;
  order.items = items;
  orders.push_back(order);
  int points = static_cast<int>(total / 10000);
  customer->addPoints(points);
  cart->items.clear();
  cout << "Da tao don hang: " << order.orderID << "\n";
}

void viewOrderHistory(const vector<Order>& orders, const string& customerID) {
  vector<Order> filtered;
  for (size_t i = 0; i < orders.size(); ++i) if (orders[i].customerID == customerID) filtered.push_back(orders[i]);
  if (filtered.empty()) {
    cout << "Chua co don hang.\n";
    return;
  }
  printOrdersTable(filtered);
}

void updateOrderStatus(vector<Order>& orders) {
  if (orders.empty()) {
    cout << "Chua co don hang.\n";
    return;
  }
  printOrdersTable(orders);
  string id = inputLine("Nhap ma don hang can cap nhat: ");
  for (size_t i = 0; i < orders.size(); ++i) {
    if (orders[i].orderID == id) {
      cout << "Chon trang thai:\n";
      cout << "1. Cho xac nhan\n";
      cout << "2. Dang giao\n";
      cout << "3. Da thanh toan\n";
      cout << "4. Huy don\n";
      int choice = inputInt("Nhap lua chon: ", 1, 4);
      if (choice == 1) orders[i].status = "Cho xac nhan";
      if (choice == 2) orders[i].status = "Dang giao";
      if (choice == 3) orders[i].status = "Da thanh toan";
      if (choice == 4) orders[i].status = "Huy don";
      cout << "Da cap nhat trang thai don hang.\n";
      return;
    }
  }
  cout << "Khong tim thay don hang.\n";
}

void reportBestSeller(const vector<Order>& orders, const vector<Product*>& products) {
  if (orders.empty()) {
    cout << "Chua co du lieu don hang.\n";
    return;
  }
  vector<pair<string, int>> counts;
  for (size_t i = 0; i < products.size(); ++i) counts.push_back(make_pair(products[i]->getID(), 0));
  for (size_t i = 0; i < orders.size(); ++i) {
    if (orders[i].status == "Huy don") continue;
    for (size_t j = 0; j < orders[i].items.size(); ++j)
      for (size_t k = 0; k < counts.size(); ++k)
        if (counts[k].first == orders[i].items[j].productID) counts[k].second += orders[i].items[j].quantity;
  }
  sort(counts.begin(), counts.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
    return a.second > b.second;
  });
  vector<string> headers = { "ID", "Ten san pham", "So luong ban" };
  vector<vector<string>> rows;
  for (size_t i = 0; i < counts.size(); ++i) {
    Product* p = findProductByID(const_cast<vector<Product*>&>(products), counts[i].first);
    if (!p) continue;
    vector<string> row;
    row.push_back(p->getID());
    row.push_back(p->getName());
    row.push_back(to_string(counts[i].second));
    rows.push_back(row);
  }
  vector<int> widths = computeWidths(headers, rows);
  printSeparator(widths);
  printRow(headers, widths);
  printSeparator(widths);
  for (size_t i = 0; i < rows.size(); ++i) printRow(rows[i], widths);
  printSeparator(widths);
}

void saveAll(const vector<User*>& users, const vector<Product*>& products, const vector<Cart>& carts, const vector<Order>& orders) {
  saveUsers(users);
  saveProducts(products);
  saveCarts(carts);
  saveOrders(orders);
}

void registerAdmin(vector<User*>& users) {
  string username = inputLine("Ten dang nhap: ");
  if (findUserByUsername(users, username)) {
    cout << "Ten dang nhap da ton tai.\n";
    return;
  }
  string password = inputPassword("Mat khau: ");
  string email = inputLine("Email: ");
  string fullName = inputLine("Ho ten: ");
  string phone = inputLine("So dien thoai: ");
  int level = 1;
  users.push_back(new Admin(username, password, email, fullName, phone, level, nowDateTime()));
  cout << "Da them tai khoan quan tri vien.\n";
}

void adminMenu(Admin* admin, vector<User*>& users, vector<Product*>& products, vector<Cart>& carts, vector<Order>& orders) {
  admin->updateLastLogin();
  while (true) {
    cout << "\n--- MENU ADMIN ---\n";
    cout << "1. Xem san pham\n";
    cout << "2. Them san pham\n";
    cout << "3. Cap nhat ton kho\n";
    cout << "4. Xem don hang\n";
    cout << "5. Cap nhat trang thai don hang\n";
    cout << "6. Bao cao san pham ban chay\n";
    cout << "7. Them tai khoan quan tri vien\n";
    cout << "0. Dang xuat\n";
    int choice = inputInt("Chon chuc nang: ", 0, 7);
    if (choice == 1) printProductsTable(products);
    else if (choice == 2) addProduct(products);
    else if (choice == 3) updateInventory(products);
    else if (choice == 4) printOrdersTable(orders);
    else if (choice == 5) updateOrderStatus(orders);
    else if (choice == 6) reportBestSeller(orders, products);
    else if (choice == 7) registerAdmin(users);
    else if (choice == 0) {
      saveAll(users, products, carts, orders);
      cout << "Da dang xuat.\n";
      break;
    }
  }
}

void customerMenu(Customer* customer, vector<User*>& users, vector<Product*>& products, vector<Cart>& carts, vector<Order>& orders) {
  while (true) {
    cout << "\n--- MENU KHACH HANG ---\n";
    cout << "1. Xem san pham\n";
    cout << "2. Tim san pham\n";
    cout << "3. Them vao gio hang\n";
    cout << "4. Xem gio hang\n";
    cout << "5. Thanh toan\n";
    cout << "6. Xem lich su don hang\n";
    cout << "7. Doi mat khau\n";
    cout << "0. Dang xuat\n";
    int choice = inputInt("Chon chuc nang: ", 0, 7);
    if (choice == 1) printProductsTable(products);
    else if (choice == 2) searchProducts(products);
    else if (choice == 3) addToCart(carts, customer->getUsername(), products);
    else if (choice == 4) viewCart(carts, customer->getUsername(), products);
    else if (choice == 5) checkout(carts, products, orders, customer);
    else if (choice == 6) viewOrderHistory(orders, customer->getUsername());
    else if (choice == 7) {
      string newPass = inputPassword("Mat khau moi: ");
      customer->changePassword(newPass);
      cout << "Da doi mat khau.\n";
    } else if (choice == 0) {
      saveAll(users, products, carts, orders);
      cout << "Da dang xuat.\n";
      break;
    }
  }
}

void registerCustomer(vector<User*>& users) {
  string username = inputLine("Ten dang nhap: ");
  if (findUserByUsername(users, username)) {
    cout << "Ten dang nhap da ton tai.\n";
    return;
  }
  string password = inputPassword("Mat khau: ");
  string email = inputLine("Email: ");
  string fullName = inputLine("Ho ten: ");
  string phone = inputLine("So dien thoai: ");
  string address = inputLine("Dia chi giao hang: ");
  string type = "Thanh vien";
  int points = 0;
  users.push_back(new Customer(username, password, email, fullName, phone, address, type, points));
  cout << "Dang ky thanh cong.\n";
}

int main() {
  vector<User*> users;
  vector<Product*> products;
  vector<Cart> carts;
  vector<Order> orders;
  loadUsers(users);
  loadProducts(products);
  loadCarts(carts);
  loadOrders(orders);
  cout << "==== PHAN MEM QUAN LY BAN HANG ====" << "\n";
  while (true) {
    cout << "\n1. Dang nhap\n";
    cout << "2. Dang ky khach hang\n";
    cout << "0. Thoat\n";
    int choice = inputInt("Chon chuc nang: ", 0, 2);
    if (choice == 0) {
      saveAll(users, products, carts, orders);
      cout << "Da thoat.\n";
      break;
    } else if (choice == 2) {
      registerCustomer(users);
      saveUsers(users);
    } else if (choice == 1) {
      string username = inputLine("Ten dang nhap: ");
      string password = inputPassword("Mat khau: ");
      User* user = findUserByUsername(users, username);
      if (!user || !user->checkLogin(username, password)) {
        cout << "Sai thong tin dang nhap.\n";
        continue;
      }
      if (user->getRole() == "ADMIN") adminMenu(static_cast<Admin*>(user), users, products, carts, orders);
      else customerMenu(static_cast<Customer*>(user), users, products, carts, orders);
    }
  }
  for (size_t i = 0; i < users.size(); ++i) delete users[i];
  for (size_t i = 0; i < products.size(); ++i) delete products[i];
  return 0;
}