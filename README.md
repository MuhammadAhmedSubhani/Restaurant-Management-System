# Restaurant Management System




A **console-based Restaurant Management System built in C++** that simulates a real-world restaurant workflow.
The system supports **four different user portals (Admin, Manager, Employee, Customer)** and includes features such as **order processing, stock management, sales tracking, and CSV data export**.

---

## Features

* **Admin Portal**

  * Secure login system
  * Manage restaurant branches
  * Manage customer records
  * Manage menu items
  * View dashboard with total sales and orders
  * Export system data to Excel-compatible CSV

* **Manager Portal**

  * View total restaurant sales
  * Monitor stock inventory by cuisine
  * Track pending and completed orders
  * Export reports to CSV

* **Employee Portal**

  * View grouped pending orders
  * Process customer order batches
  * Mark orders as completed
  * Automatically update branch sales

* **Customer Portal**

  * Register or login using name
  * Browse cuisine-based menus
  * Select dishes and toppings
  * Place orders
  * Receive a printed receipt

* **Stock Management**

  * Each dish tracks available inventory
  * Prevents ordering of out-of-stock items

* **Order Batching**

  * Items ordered in a single session are grouped into one batch
  * Batch identification uses **customer name + timestamp**

* **Receipt Printing**

  * Generates an itemized receipt at checkout

* **CSV Export**

  * Exports:
  * Branch data
  * Customers
  * Menu items
  * Inventory stock
  * Pending orders
  * Completed orders

All data is exported to:

```
RestaurantData.csv
```

which can be opened directly in **Microsoft Excel or Google** **Sheets**.

---

## Portals Overview

| Portal   | Access              | Key Actions                                         |
| -------- | ------------------- | --------------------------------------------------- |
| Admin    | Username + Password | Manage branches, customers, menu, dashboard, export |
| Manager  | Open access         | View sales, inventory, and orders                   |
| Employee | Open access         | Process pending orders                              |
| Customer | Name-based          | Browse menu, order food, receive receipt            |

---

## Cuisines & Menu

| Cuisine | Dishes                            | Toppings                        |
| ------- | --------------------------------- | ------------------------------- |
| Italian | Pizza, Pasta, Risotto             | Tomato Sauce, Mozzarella, Basil |
| Chinese | Dumplings, Sweet & Sour, Kung Pao | Soy Sauce, Ginger, Garlic       |
| Mexican | Tacos, Burritos, Enchiladas       | Salsa, Guacamole, Sour Cream    |

---

## Tech Stack

**Language**

* C++

**Libraries**

Standard C++ libraries only:

* `<iostream>`
* `<fstream>`
* `<string>`
* `<vector>`
* `<ctime>`

**Dependencies**

No external libraries required.
Runs with **any standard-compliant C++ compiler**.

---

## How to Run

### 1. Clone the Repository

```bash
git clone https://github.com/MuhammadAhmedSubhani/Restaurant-Management-System.git
cd Restaurant-Management-System
```

### 2. Compile the Program

```bash
g++ Rest_Project.cpp -o RestaurantSystem
```

### 3. Run the Program

Linux / macOS

```bash
./RestaurantSystem
```

Windows

```bash
RestaurantSystem.exe
```

---

## Admin Credentials

| Field    | Value |
| -------- | ----- |
| Username | admin |
| Password | 1234  |

---

## Project Structure

```
Restaurant-Management-System/
│
├── Rest_Project.cpp
│   Main source code
│
├── RestaurantData.csv
│   Auto-generated export file
│
└── README.md
    Project documentation
```

---

## Possible Enhancements

Future improvements that could extend the project:

* Add a **graphical user interface (GUI)** using frameworks such as Qt or SFML
* Integrate a **real database system** (MySQL, SQLite, PostgreSQL)
* Implement **unit testing** for core features (order processing, stock updates, CSV export)
* Improve **input validation and error handling**
* Add **tax calculation and discount systems**
* Implement **multi-branch analytics dashboard**
* Add **user authentication and role management**

---

## Author

Muhammad Ahmed Subhani
Software Engineering Student – Superior University

---

## Contributions

This project was created as an **academic learning project**.

Feedback, suggestions, and improvements are welcome.
If you discover a bug or have an idea for improvement, feel free to **open an issue**.

---

## License

This project is licensed under the **MIT License**.
