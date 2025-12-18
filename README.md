# Online-Market
# Online Market System (C++)

## Project Overview
The Online Market System is a console-based C++ application that simulates an online shopping platform.
Sellers can add products, and customers can browse, purchase, and rate products and sellers.

This project is designed for Data Structures & Algorithms (DSA) courses and demonstrates practical usage
of multiple data structures in one system.

## Features

### Sellers
- Register a new seller
- Login using email
- Add products
- Receive ratings from customers
- Seller data is stored in a file

### Products
- Stored using a Binary Search Tree (BST)
- Search by product ID
- Browse by category
- Browse by name
- Product rating system
- Display top-rated products
- Products are saved and loaded from file

### Customers
- Register a new customer
- Login using email
- Browse products
- Add products to cart
- Checkout and purchase
- Customer data is stored in a file

### Cart & Checkout
- Add products to cart
- Stack used to track last added products
- Queue used to process orders
- Total price calculation
- Product quantity update after purchase
- Rate product and seller after checkout

---

## Data Structures Used

| Component | Data Structure |
|--------|----------------|
| Sellers | Singly Linked List |
| Customers | Singly Linked List |
| Products | Binary Search Tree (BST) |
| Cart History | Stack |
| Order Processing | Queue |
| Top Rated Products | Priority Queue |

---

## Data Files

- sellers.txt → Stores sellers data
- customers.txt → Stores customers data
- products.txt → Stores products data

All files are stored in:
---

## How to Run

1. Open the project in any C++ IDE (Code::Blocks / Visual Studio)
2. Make sure the folder path exists on your computer
3. Compile and run the program
4. Choose Seller or Customer from the Home Page

---

## Example Flow

### Seller
1. Register
2. Login
3. Add Product

### Customer
1. Register
2. Login
3. Browse Products
4. Add to Cart
5. Checkout
6. Rate Product & Seller

---

## Notes
- Input validation is handled
- Rating values must be between 1 and 5
- Products with zero quantity are not shown
- All data is persistent using text files

---

## Future Improvements
- Delete or update products
- Remove items from cart
- Sort products by price
- Add password authentication
- Convert console app to GUI

---

## Project Info
- Language: C++
- Paradigm: Procedural + Object-Oriented Programming
- Suitable for: DSA Mini / Final Project

---

This project demonstrates real-world usage of data structures in C++.
