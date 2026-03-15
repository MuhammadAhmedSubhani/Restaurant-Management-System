# Restaurant Management System

A console-based **Restaurant Management System** built in **C++**. This project simulates a real-world restaurant workflow with four separate portals: Admin, Manager, Employee, and Customer. 
---

## Features

- **Admin Portal** — Secure login, manage branches, customers, and menu items; view dashboard with sales and orders; export data to Excel (CSV).
- **Manager Portal** — View total sales, stock inventory per cuisine, and all pending/completed orders; export data to CSV.
- **Employee Portal** — View and process grouped pending orders by customer session; mark orders as completed and update branch sales.
- **Customer Portal** — Register/login by name, browse cuisine menus (Italian, Chinese, Mexican), select dishes and toppings, place orders, and receive a printed receipt.
- **Stock Management** — Each dish tracks available stock; out-of-stock items cannot be ordered.
- **Order Batching** — All items ordered in one customer session are grouped under a single batch (by customer name + timestamp).
- **Receipt Printing** — Auto-generated itemized receipt printed at checkout.
- **CSV Export** — Export branches, customers, menu, stock inventory, pending orders, and completed orders to `RestaurantData.csv` (Excel-compatible).

---

## Portals Overview

| Portal   | Access               | Key Actions                                   |
|----------|----------------------|-----------------------------------------------|
| Admin    | Username + Password  | Manage branches, customers, menu; dashboard; export |
| Manager  | Open access          | View sales, stock, orders; export to CSV      |
| Employee | Open access          | Process and complete grouped customer orders  |
| Customer | Name-based           | Browse menu, add toppings, place orders, get receipt |


---

## Cuisines & Menu

| Cuisine | Dishes                         | Toppings                          |
|---------|--------------------------------|-----------------------------------|
| Italian | Pizza, Pasta, Risotto         | Tomato Sauce, Mozzarella, Basil   |
| Chinese | Dumplings, Sweet & Sour, Kung Pao | Soy Sauce, Ginger, Garlic    |
| Mexican | Tacos, Burritos, Enchiladas   | Salsa, Guacamole, Sour Cream      |



---

## Tech Stack

- **Language:** C++
- **Libraries:** Standard C++ headers such as `<iostream>`, `<fstream>`, `<string>`, `<vector>`, etc. (no third-party libraries required).
- **Dependencies:** No external dependencies — runs with any standard-compliant C++ compiler.

---

## How to Run

1. **Clone the repository**

   ```bash
   git clone https://github.com/MuhammadAhmedSubhani/Restaurant-Management-System.git
   cd Restaurant-Management-System
2.	Compile the source file
bash
g++ Rest_Project.cpp -o RestaurantSystem
3.	Run the executable
•	On Linux/macOS:
bash
./RestaurantSystem
•	On Windows:
bash
RestaurantSystem.exe

________________________________________
Admin Credentials
Field	Value
Username	admin
Password	1234

________________________________________
Project Structure
text
Restaurant-Management-System/
├── Rest_Project.cpp     # Main source code
├── RestaurantData.csv   # Auto-generated export file
└── README.md            # Project documentation

________________________________________
Future Improvements
•	Add a graphical user interface (GUI) using a framework like Qt or SFML.
•	Integrate a real database (e.g., MySQL, SQLite) instead of CSV-based storage.
•	Implement unit tests for core functionalities (order processing, stock updates, CSV export).
•	Enhance validation, error handling, and input sanitization.
•	Add support for discounts, tax calculation, and multi-branch analytics dashboard.

________________________________________
Author
Muhammad Ahmed Subhani
________________________________________
📋 Contribution
This repository is a personal academic project for learning and practice, but feedback and suggestions are always welcome! 
If you notice a bug, have an improvement idea, or want to suggest better design patterns, feel free to open an issue for constructive feedback.
________________________________________
📧 Contact
Name: Muhammad Ahmed Subhani
University: Superior University
Program: Software Engineering
Last Updated: January 2026

