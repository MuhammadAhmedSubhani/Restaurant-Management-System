# Restaurant Management System

A console-based **Restaurant Management System** built in **C++**. This project simulates a real-world restaurant workflow with four separate portals: Admin, Manager, Employee, and Customer.

## Features

- **Admin Portal** — Secure login, manage branches, customers, and menu items; view dashboard with sales and orders; export data to Excel (CSV).
- **Manager Portal** — View total sales, stock inventory per cuisine, and all pending/completed orders; export data to CSV.
- **Employee Portal** — View and process grouped pending orders by customer session; mark orders as completed and update branch sales.
- **Customer Portal** — Register/login by name, browse cuisine menus (Italian, Chinese, Mexican), select dishes and toppings, place orders, and receive a printed receipt.
- **Stock Management** — Each dish tracks available stock; out-of-stock items cannot be ordered.
- **Order Batching** — All items ordered in one customer session are grouped under a single batch (by customer name + timestamp).
- **Receipt Printing** — Auto-generated itemized receipt printed at checkout.
- **CSV Export** — Export branches, customers, menu, stock inventory, pending orders, and completed orders to `RestaurantData.csv` (Excel-compatible).

## Portals Overview

| Portal   | Access       | Key Actions                                              |
|----------|--------------|----------------------------------------------------------|
| Admin    | Username + Password | Manage branches, customers, menu; view dashboard; export |
| Manager  | Open access  | View sales, stock, orders; export to CSV                 |
| Employee | Open access  | Process and complete grouped customer orders             |
| Customer | Name-based   | Browse menu, add toppings, place orders, get receipt     |

## Cuisines & Menu

| Cuisine  | Dishes                              | Toppings                          |
|----------|-------------------------------------|-----------------------------------|
| Italian  | Pizza, Pasta, Risotto               | Tomato Sauce, Mozzarella, Basil   |
| Chinese  | Dumplings, Sweet & Sour, Kung Pao   | Soy Sauce, Ginger, Garlic         |
| Mexican  | Tacos, Burritos, Enchiladas         | Salsa, Guacamole, Sour Cream      |

## Tech Stack

- **Language:** C++
- **Libraries:** `<iostream>`, `<vector>`, `<string>`, `<fstream>`, `<unordered_map>`, `<algorithm>`, `<ctime>`, `<iomanip>`
- **No external dependencies** — runs with any standard C++ compiler.

## How to Run

1. Clone the repository:
   ```bash
   git clone https://github.com/MuhammadAhmedSubhani/Restaurant-Management-System.git
   ```
2. Compile the source file:
   ```bash
   g++ Rest_Project.cpp -o RestaurantSystem
   ```
3. Run the executable:
   ```bash
   ./RestaurantSystem
   ```
   *(On Windows use `RestaurantSystem.exe`)*

## Admin Credentials

| Field    | Value   |
|----------|---------|
| Username | `admin` |
| Password | `1234`  |

## Project Structure

```
Restaurant-Management-System/
├── Rest_Project.cpp    # Main source code
├── RestaurantData.csv  # Auto-generated export file
└── README.md
```

## Author

Muhammad Ahmed Subhani
