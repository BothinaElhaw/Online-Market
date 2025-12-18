#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>

using namespace std;

const string folderPath = "C:\\Users\\Mr Magic\\Desktop\\DSA Project\\";

struct Seller {
    int id;
    string name;
    string email;
    double totalRating = 0;
    int ratingCount = 0;

    double getRating() const {
        if (ratingCount == 0) 
            return 0;
        return totalRating / ratingCount;
    }
};

struct SellerNode {
    Seller data;
    SellerNode* next;
    SellerNode(Seller s) : data(s), next(NULL) {}
};

class SellerList {
public:
    SellerNode* head;
    SellerList() : head(NULL) {}

    void addSeller(Seller s) {
        SellerNode* node = new SellerNode(s);
        node->next = head;
        head = node;
    }

    Seller* findByEmail(string email) {
        SellerNode* cur = head;
        while (cur) {
            if (cur->data.email == email)
                return &cur->data;
            cur = cur->next;
        }
        return NULL;
    }

    void display() {
        SellerNode* cur = head;
        while (cur) {
            cout << "Name: " << cur->data.name << " \n ID:" << cur->data.id
                << " \n Rate:" << cur->data.getRating() << endl;
            cur = cur->next;
        }
    }

    void saveToFile() {
        try {
            ofstream file(folderPath + "sellers.txt");
            if (!file.is_open()) {
                cout << "Error: Cannot create sellers.txt file\n";
                return;
            }
            SellerNode* cur = head;
            while (cur) {
                file << cur->data.id << "," << cur->data.name << ","
                    << cur->data.email << "," << cur->data.totalRating << ","
                    << cur->data.ratingCount << "\n";
                cur = cur->next;
            }
            file.close();
        }
        catch (...) {
            cout << "Error saving sellers file\n";
        }
    }

    void loadFromFile(int& sellerCounter) {
        try {
            ifstream file(folderPath + "sellers.txt");
            if (!file.is_open()) return;

            string line;
            vector<Seller> tempSellers;
            while (getline(file, line)) {
                if (line.empty()) continue;

                try {
                    stringstream ss(line);
                    string id, name, email, totalRating, ratingCount;
                    getline(ss, id, ',');
                    getline(ss, name, ',');
                    getline(ss, email, ',');
                    getline(ss, totalRating, ',');
                    getline(ss, ratingCount, ',');

                    if (id.empty() || name.empty() || email.empty()) continue;

                    Seller s;
                    s.id = stoi(id);
                    s.name = name;
                    s.email = email;
                    s.totalRating = (totalRating.empty() ? 0 : stod(totalRating));
                    s.ratingCount = (ratingCount.empty() ? 0 : stoi(ratingCount));

                    tempSellers.push_back(s);

                    if (s.id >= sellerCounter) sellerCounter = s.id + 1;
                }
                catch (...) {
                    cout << "Error reading seller line: " << line << endl;
                    continue;
                }
            }
            file.close();

            for (int i = tempSellers.size() - 1; i >= 0; i--) {
                addSeller(tempSellers[i]);
            }
        }
        catch (...) {
            cout << "Error loading sellers file\n";
        }
    }
};

struct Product {
    int id;
    string name;
    double price;
    string category;
    int quantity;
    int sellerId;

    double totalRating = 0;
    int ratingCount = 0;

    double getRating() const {
        if (ratingCount == 0) return 0;
        return totalRating / ratingCount;
    }
};

struct ProductNode {
    Product data;
    ProductNode* left;
    ProductNode* right;
    ProductNode(Product p) : data(p), left(nullptr), right(nullptr) {}
};

class ProductBST {
public:
    ProductNode* root;
    ProductBST() : root(nullptr) {}

    void insert(Product p) {
        root = insertRec(root, p);
    }

    Product* searchById(int id) {
        ProductNode* cur = root;
        while (cur) {
            if (cur->data.id == id) return &cur->data;
            else if (id < cur->data.id) cur = cur->left;
            else cur = cur->right;
        }
        return nullptr;
    }

    vector<Product*> getAllProducts() {
        vector<Product*> res;
        inorder(root, res);
        return res;
    }

    void saveToFile(const string& filename) {
        try {
            ofstream file(folderPath + filename);
            if (!file.is_open()) {
                cout << "Error: Cannot create products file\n";
                return;
            }
            saveRec(root, file);
            file.close();
        }
        catch (...) {
            cout << "Error saving products file\n";
        }
    }

    void loadFromFile(const string& filename, int& productCounter) {
        try {
            ifstream file(folderPath + filename);
            if (!file.is_open()) return;

            string line;
            while (getline(file, line)) {
                if (line.empty()) continue;

                try {
                    stringstream ss(line);
                    string id, sellerId, name, price, category, quantity, totalRating, ratingCount;
                    getline(ss, id, ',');
                    getline(ss, sellerId, ',');
                    getline(ss, name, ',');
                    getline(ss, price, ',');
                    getline(ss, category, ',');
                    getline(ss, quantity, ',');
                    getline(ss, totalRating, ',');
                    getline(ss, ratingCount, ',');

                    if (id.empty() || sellerId.empty() || name.empty()) continue;

                    Product p;
                    p.id = stoi(id);
                    p.sellerId = stoi(sellerId);
                    p.name = name;
                    p.price = (price.empty() ? 0 : stod(price));
                    p.category = category;
                    p.quantity = (quantity.empty() ? 0 : stoi(quantity));
                    p.totalRating = (totalRating.empty() ? 0 : stod(totalRating));
                    p.ratingCount = (ratingCount.empty() ? 0 : stoi(ratingCount));

                    insert(p);

                    if (p.id >= productCounter) productCounter = p.id + 1;
                }
                catch (...) {
                    cout << "Error reading product line: " << line << endl;
                    continue;
                }
            }
            file.close();
        }
        catch (...) {
            cout << "Error loading products file\n";
        }
    }

private:
    ProductNode* insertRec(ProductNode* node, Product p) {
        if (!node) return new ProductNode(p);
        if (p.id < node->data.id) node->left = insertRec(node->left, p);
        else node->right = insertRec(node->right, p);
        return node;
    }

    void inorder(ProductNode* node, vector<Product*>& res) {
        if (!node) return;
        inorder(node->left, res);
        res.push_back(&node->data);
        inorder(node->right, res);
    }

    void saveRec(ProductNode* node, ofstream& file) {
        if (!node) return;
        saveRec(node->left, file);
        file << node->data.id << "," << node->data.sellerId << "," << node->data.name << ","
            << node->data.price << "," << node->data.category << "," << node->data.quantity << ","
            << node->data.totalRating << "," << node->data.ratingCount << "\n";
        saveRec(node->right, file);
    }
};

struct Customer {
    int id;
    string name;
    string address;
    string phone;
    string email;
};

struct CustomerNode {
    Customer data;
    CustomerNode* next;
    CustomerNode(Customer c) : data(c), next(NULL) {}
};

class CustomerList {
public:
    CustomerNode* head;
    CustomerList() : head(NULL) {}

    void addCustomer(Customer c) {
        CustomerNode* node = new CustomerNode(c);
        node->next = head;
        head = node;
    }

    Customer* findByEmail(string email) {
        CustomerNode* cur = head;
        while (cur) {
            if (cur->data.email == email)
                return &cur->data;
            cur = cur->next;
        }
        return NULL;
    }

    void saveToFile() {
        try {
            ofstream file(folderPath + "customers.txt");
            if (!file.is_open()) {
                cout << "Error: Cannot create customers file\n";
                return;
            }
            CustomerNode* cur = head;
            while (cur) {
                file << cur->data.id << "," << cur->data.name << "," << cur->data.email << ","
                    << cur->data.address << "," << cur->data.phone << "\n";
                cur = cur->next;
            }
            file.close();
        }
        catch (...) {
            cout << "Error saving customers file\n";
        }
    }

    void loadFromFile(int& customerCounter) {
        try {
            ifstream file(folderPath + "customers.txt");
            if (!file.is_open()) return;

            string line;
            vector<Customer> tempCustomers;
            while (getline(file, line)) {
                if (line.empty()) continue;

                try {
                    stringstream ss(line);
                    string id, name, email, address, phone;
                    getline(ss, id, ',');
                    getline(ss, name, ',');
                    getline(ss, email, ',');
                    getline(ss, address, ',');
                    getline(ss, phone, ',');

                    if (id.empty() || name.empty() || email.empty()) continue;

                    Customer c;
                    c.id = stoi(id);
                    c.name = name;
                    c.email = email;
                    c.address = address;
                    c.phone = phone;

                    tempCustomers.push_back(c);

                    if (c.id >= customerCounter) customerCounter = c.id + 1;
                }
                catch (...) {
                    cout << "Error reading customer line: " << line << endl;
                    continue;
                }
            }
            file.close();

            for (int i = tempCustomers.size() - 1; i >= 0; i--) {
                addCustomer(tempCustomers[i]);
            }
        }
        catch (...) {
            cout << "Error loading customers file\n";
        }
    }
};

struct CartItem { int productId; int quantity; };

class Cart {
public:
    vector<CartItem> items;
    stack<int> lastAdded;

    void addProduct(int productId, int qty) {
        for (auto& item : items) {
            if (item.productId == productId) {
                item.quantity += qty;
                lastAdded.push(productId);
                return;
            }
        }
        items.push_back({ productId, qty });
        lastAdded.push(productId);
    }
};

SellerList sellers;
CustomerList customers;
ProductBST productsBST;

int sellerCounter = 1;
int productCounter = 1;
int customerCounter = 1;

void sellerRegister() {
    Seller s;
    s.id = sellerCounter++;
    cin.ignore();
    cout << "Enter name: "; getline(cin, s.name);
    cout << "Enter email: "; getline(cin, s.email);
    sellers.addSeller(s);
    sellers.saveToFile();
    cout << "Seller Registered\n";
}

Seller* sellerLogin() {
    cin.ignore();
    string email;
    cout << "Enter seller email: "; getline(cin, email);
    Seller* s = sellers.findByEmail(email);
    if (!s) { cout << "Not found\n"; return NULL; }
    cout << "Welcome, " << s->name << endl;
    return s;
}

void addProduct(Seller& s, ProductBST& productsBST) {
    Product p;
    p.id = productCounter++;
    p.sellerId = s.id;
    cin.ignore();
    cout << "Product name: "; getline(cin, p.name);
    cout << "Price: "; cin >> p.price;
    cin.ignore();
    cout << "Category: "; getline(cin, p.category);
    cout << "Quantity: "; cin >> p.quantity; cin.ignore();

    productsBST.insert(p);
    productsBST.saveToFile("products.txt");

    cout << "Product added and saved\n";
}

void customerRegister() {
    Customer c;
    c.id = customerCounter++;
    cin.ignore();
    cout << "Enter name: "; getline(cin, c.name);
    cout << "Enter email: "; getline(cin, c.email);
    cout << "Enter address: "; getline(cin, c.address);
    cout << "Enter phone: "; getline(cin, c.phone);
    customers.addCustomer(c);
    customers.saveToFile();
    cout << "Customer Registered\n";
}

Customer* customerLogin() {
    cin.ignore();
    string email;
    cout << "Enter email: "; getline(cin, email);
    Customer* c = customers.findByEmail(email);
    if (!c) { cout << "Not found\n"; return NULL; }
    cout << "Welcome, " << c->name << endl;
    return c;
}

void browseByCategory(Cart& cart) {
    cin.ignore();
    string cat;
    cout << "Enter category: ";
    getline(cin, cat);

    vector<Product*> allProducts = productsBST.getAllProducts();
    bool found = false;

    for (auto* p : allProducts) {
        if (p->category == cat && p->quantity > 0) {
            cout << "ID:" << p->id << " | " << p->name << " | Price:" << p->price
                << " | Qty:" << p->quantity << " | Rate:" << p->getRating() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No products found in this category.\n";
        return;
    }

    cout << "Enter product ID to add to cart (0 to exit): ";
    int pid;
    cin >> pid;
    if (pid == 0) return;

    cout << "Quantity: ";
    int q;
    cin >> q;

    Product* p = productsBST.searchById(pid);
    if (!p) {
        cout << "Invalid product ID\n";
        return;
    }

    if (q <= 0) {
        cout << "Quantity must be greater than 0\n";
        return;
    }

    if (q > p->quantity) {
        cout << "Not enough quantity available, Only " << p->quantity << " left\n";
        return;
    }

    cart.addProduct(pid, q);
    cout << "Product added to cart successfully\n";
}

void browseByName(Cart& cart) {
    cin.ignore();
    string name;
    cout << "Enter product name: ";
    getline(cin, name);

    vector<Product*> allProducts = productsBST.getAllProducts();
    bool found = false;

    for (auto* p : allProducts) {
        if (p->name.find(name) != string::npos && p->quantity > 0) {
            cout << "ID:" << p->id << " | " << p->name << " | Price:" << p->price
                << " | Qty:" << p->quantity << " | Rate:" << p->getRating() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No products found\n";
        return;
    }

    cout << "Enter product ID to add (0 to exit): ";
    int pid;
    cin >> pid;
    if (pid == 0) return;

    cout << "Quantity: ";
    int q;
    cin >> q;

    Product* p = productsBST.searchById(pid);
    if (p && q > 0 && q <= p->quantity) {
        cart.addProduct(pid, q);
        cout << "Product added to cart successfully\n";
    }
    else cout << "Invalid ID or quantity\n";
}

struct CompareRating {
    bool operator()(Product* p1, Product* p2) { return p1->getRating() < p2->getRating(); }
};

void rateProduct(Product& p) {
    int rate;
    cout << "Rate product (" << p.name << ") from 1 to 5: ";
    cin >> rate;

    if (rate < 1 || rate > 5) {
        cout << "Invalid rating\n";
        return;
    }

    p.totalRating += rate;
    p.ratingCount++;
}

void rateSeller(int sellerId) {
    SellerNode* cur = sellers.head;
    while (cur) {
        if (cur->data.id == sellerId) {
            int rate;
            cout << "Rate seller (" << cur->data.name << ") from 1 to 5: ";
            cin >> rate;

            if (rate < 1 || rate > 5) {
                cout << "Invalid rating\n";
                return;
            }

            cur->data.totalRating += rate;
            cur->data.ratingCount++;
            sellers.saveToFile();
            return;
        }
        cur = cur->next;
    }
}

void showTopRatedProducts() {
    vector<Product*> allProducts = productsBST.getAllProducts();

    if (allProducts.empty()) {
        cout << "No products available\n";
        return;
    }

    priority_queue<Product*, vector<Product*>, CompareRating> pq;
    for (auto* p : allProducts) pq.push(p);

    int count = 0;
    cout << "Top Rated Products:\n";
    while (!pq.empty() && count < 5) {
        Product* p = pq.top(); pq.pop();
        cout << p->name << " | Rate: " << p->getRating() << endl;
        count++;
    }
}

void checkout(Customer& c, Cart& cart) {
    if (cart.items.empty()) {
        cout << "Cart is empty\n";
        return;
    }

    queue<Product*> orderQueue;
    double total = 0;

    cout << "\nRECEIPT:\n";
    for (auto& item : cart.items) {
        Product* p = productsBST.searchById(item.productId);
        if (p) {
            orderQueue.push(p);
            total += p->price * item.quantity;
            p->quantity -= item.quantity;
        }
    }

    while (!orderQueue.empty()) {
        Product* p = orderQueue.front();
        orderQueue.pop();

        cout << p->name << " processed\n";

        rateProduct(*p);
        rateSeller(p->sellerId);
    }

    cout << "Total = " << total << endl;
    cout << "Purchase confirmed\n";

    productsBST.saveToFile("products.txt");
    cart.items.clear();
}

int main() {
    sellers.loadFromFile(sellerCounter);
    customers.loadFromFile(customerCounter);
    productsBST.loadFromFile("products.txt", productCounter);

    cout << "Online Market\n\n";

    while (true) {
        cout << "\n1) Seller\n2) Customer\n3) Exit\nChoice: ";
        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input\n";
            continue;
        }

        if (choice == 1) {
            cout << "1) Register\n2) Login\nChoice: ";
            int c; cin >> c;

            if (c == 1) {
                sellerRegister();
            }
            else if (c == 2) {
                Seller* s = sellerLogin();
                if (s) {
                    cout << "1) Add product\nChoice: ";
                    int x; cin >> x;
                    if (x == 1) {
                        addProduct(*s, productsBST);
                    }
                }
            }
        }
        else if (choice == 2) {
            cout << "1) Register\n2) Login\nChoice: ";
            int c; cin >> c;

            if (c == 1) customerRegister();
            else if (c == 2) {
                Customer* cu = customerLogin();
                if (cu) {
                    Cart cart;
                    while (true) {
                        cout << "\n1) Browse by category\n2) Browse by name\n3) Show top-rated\n4) Checkout\n5) Back\nChoice: ";
                        int x; cin >> x;

                        if (cin.fail()) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "Invalid input\n";
                            continue;
                        }

                        if (x == 1) browseByCategory(cart);
                        else if (x == 2) browseByName(cart);
                        else if (x == 3) showTopRatedProducts();
                        else if (x == 4) checkout(*cu, cart);
                        else break;
                    }
                }
            }
        }
        else if (choice == 3) break;
    }
    productsBST.saveToFile("products.txt");
    cout << "Goodbye\n";

    return 0;
}