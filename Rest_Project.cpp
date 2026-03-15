#include <iostream>
#include <vector>
#include <string>
#include <ctime> 
#include <iomanip> // for formatting output
#include <algorithm> // for transform and remove_if
#include <limits> // to be studied
#include <fstream> 
#include <unordered_map> // key-value pairs

using namespace std;

// ==================== DATA STRUCTURES ====================

struct DishInfo
{
    string name;
    double price;
    int stock;
};

struct Branch
{
    string name;
    double sales;
};

struct Customer
{
    string name;
    string joinDate;
};

struct MenuItem
{
    string name;
    double price;
};

struct Order
{
    string datetime; // used as batch id for a customer's checkout session
    string customerName;
    string cuisineName;
    string dish;
    vector<string> toppings;
    double price;
};

// ==================== GLOBAL DATA ====================

// Note: branch names here are kept consistent with cuisine names ("Italian", "Chinese", "Mexican")
DishInfo italianMenu[] = {{"Pizza", 1200.0, 10}, {"Pasta", 900.0, 15}, {"Risotto", 1500.0, 8}};
DishInfo chineseMenu[] = {{"Dumplings", 700.0, 20}, {"Sweet and Sour", 800.0, 15}, {"Kung Pao Chicken", 500.0, 25}};
DishInfo mexicanMenu[] = {{"Tacos", 600.0, 18}, {"Burritos", 1000.0, 12}, {"Enchiladas", 1100.0, 10}};

vector<Branch> branches = {{"Italian", 0.0}, {"Chinese", 0.0}, {"Mexican", 0.0}};
vector<Customer> customers;
vector<MenuItem> menuItems; // will be initialized at startup from DishInfo arrays
vector<Order> pendingOrders;
vector<Order> completedOrders;

// ==================== UTILITY FUNCTIONS ====================

string currentDateTime()
{
    time_t now = time(0);
    char buf[80];
    tm *ltm = localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buf);
}

void line()
{
    cout << "========================================\n";
}

// Read integer safely, return true if got a valid integer, false otherwise.
bool safeReadInt(int &out)
{
    if (!(cin >> out))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    // clear trailing newline for subsequent getline if needed
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

// Quote and escape CSV fields; also guard against CSV injection by prefixing a single quote
string csvQuoteSafe(const string &s)
{
    if (s.empty())
        return "\"\"";

    string result = "\"";
    for (char c : s)
    {
        if (c == '"')
            result += "\"\""; // escape double quote
        else
            result.push_back(c);
    }
    result += "\"";

    if (!s.empty())
    {
        char first = s[0];
        if (first == '=' || first == '+' || first == '-' || first == '@')
        {
            string inner = "\"'";
            for (char c : s)
            {
                if (c == '"')
                    inner += "\"\"";
                else
                    inner.push_back(c);
            }
            inner += "\"";
            return inner;
        }
    }

    return result;
}

// ==================== EXPORT TO EXCEL FUNCTION ====================

void exportDataToExcel()
{
    ofstream outFile("RestaurantData.csv"); // open file for writing
    if (!outFile) // check if file opened successfully
    {
        cout << "Error opening file for export.\n";
        return;
    }

    // Export Branches
    outFile << "Branches,Sales\n"; 
    for (auto &b : branches)
        outFile << csvQuoteSafe(b.name) << "," << fixed << setprecision(2) << b.sales << "\n"; // each branch on new line

    outFile << "\n"; // blank line

    // Export Customers
    outFile << "Customers,Join Date\n";
    for (auto &c : customers)
        outFile << csvQuoteSafe(c.name) << "," << csvQuoteSafe(c.joinDate) << "\n";

    outFile << "\n";

    // Export Menu Items
    outFile << "Menu Items,Price\n";
    for (auto &m : menuItems)
        outFile << csvQuoteSafe(m.name) << "," << fixed << setprecision(2) << m.price << "\n";

    outFile << "\n";

    // Export Stock Inventory
    outFile << "Stock Inventory\n";
    outFile << "Italian Menu,Price,Stock\n"; 
    for (auto &d : italianMenu)
        outFile << csvQuoteSafe(d.name) << "," << d.price << "," << d.stock << "\n";

    outFile << "Chinese Menu,Price,Stock\n";
    for (auto &d : chineseMenu)
        outFile << csvQuoteSafe(d.name) << "," << d.price << "," << d.stock << "\n";

    outFile << "Mexican Menu,Price,Stock\n";
    for (auto &d : mexicanMenu)
        outFile << csvQuoteSafe(d.name) << "," << d.price << "," << d.stock << "\n";

    outFile << "\n";

    // Export Pending Orders
    outFile << "Pending Orders,Customer,Cuisine,Dish,Price,DateTime\n";
    for (auto &o : pendingOrders)
    {
        outFile << csvQuoteSafe(o.customerName) << ","
                << csvQuoteSafe(o.cuisineName) << ","
                << csvQuoteSafe(o.dish) << ","
                << fixed << setprecision(2) << o.price << ","
                << csvQuoteSafe(o.datetime) << "\n";
    }

    outFile << "\n";

    // Export Completed Orders
    outFile << "Completed Orders,Customer,Cuisine,Dish,Price,DateTime\n"; // header
    for (auto &o : completedOrders)
    {
        outFile << csvQuoteSafe(o.customerName) << ","
                << csvQuoteSafe(o.cuisineName) << ","
                << csvQuoteSafe(o.dish) << ","
                << fixed << setprecision(2) << o.price << ","
                << csvQuoteSafe(o.datetime) << "\n";
    }

    outFile.close();
    cout << "Data exported successfully to RestaurantData.csv! You can open it in Excel.\n";
}

// ==================== ADMIN FUNCTIONS ====================

bool adminLogin()
{
    string username, password;
    cout << "\n--- Admin Login ---\nUsername: ";
    // NOTE: removed redundant cin.ignore() so we don't block here
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);
    if (username == "admin" && password == "1234")
    {
        cout << "Login Successful!\n";
        return true;
    }
    cout << "Invalid credentials.\n";
    return false;
}

void manageBranches()
{
    int choice;
    do
    {
        cout << "\n--- Manage Branches ---\n1. Add Branch\n2. Remove Branch\n3. List Branches\n4. Back\nChoice: ";
        if (!safeReadInt(choice))
        {
            cout << "Invalid selection. Try again.\n";
            continue;
        }

        if (choice == 1)
        {
            Branch b;
            cout << "Enter branch name: ";
            getline(cin, b.name);
            cout << "Enter sales: ";
            double s;
            if (!(cin >> s))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                s = 0.0;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            b.sales = s;
            branches.push_back(b);
            cout << "Branch added.\n";
        }
        else if (choice == 2)
        {
            string name;
            cout << "Enter branch name to remove: ";
            getline(cin, name);
            size_t originalSize = branches.size();
            branches.erase(remove_if(branches.begin(), branches.end(), [&](const Branch &b)
                                     { return b.name == name; }),
                           branches.end());
            if (branches.size() < originalSize)
                cout << "Branch removed.\n";
            else
                cout << "No matching branch found.\n";
        }
        else if (choice == 3)
        {
            cout << "\nBranches:\n";
            for (auto &b : branches)
                cout << "- " << b.name << " | Sales: $" << fixed << setprecision(2) << b.sales << endl;
        }
    } while (choice != 4);
}

void manageCustomers()
{
    int choice;
    do
    {
        cout << "\n--- Manage Customers ---\n1. Add Customer\n2. Remove Customer\n3. List Customers\n4. Back\nChoice: ";
        if (!safeReadInt(choice))
        {
            cout << "Invalid selection. Try again.\n";
            continue;
        }

        if (choice == 1)
        {
            Customer c;
            cout << "Enter customer name: ";
            getline(cin, c.name);
            c.joinDate = currentDateTime();
            customers.push_back(c);
            cout << "Customer added.\n";
        }
        else if (choice == 2)
        {
            string name;
            cout << "Enter customer name to remove: ";
            getline(cin, name);
            size_t originalSize = customers.size();
            customers.erase(remove_if(customers.begin(), customers.end(), [&](const Customer &c)
                                      { return c.name == name; }),
                            customers.end());
            if (customers.size() < originalSize)
                cout << "Customer removed.\n";
            else
                cout << "No matching customer found.\n";
        }
        else if (choice == 3)
        {
            cout << "\nCustomers:\n";
            for (auto &c : customers)
                cout << "- " << c.name << " | Joined: " << c.joinDate << endl;
        }
    } while (choice != 4);
}

void manageMenu()
{
    int choice;
    do
    {
        cout << "\n--- Manage Menu ---\n1. Add Dish\n2. Remove Dish\n3. List Menu\n4. Back\nChoice: ";
        if (!safeReadInt(choice))
        {
            cout << "Invalid selection. Try again.\n";
            continue;
        }

        if (choice == 1)
        {
            MenuItem m;
            cout << "Enter dish name: ";
            getline(cin, m.name);
            cout << "Enter price: ";
            double p;
            if (!(cin >> p))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                p = 0.0;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            m.price = p;
            menuItems.push_back(m);
            cout << "Menu item added.\n";
        }
        else if (choice == 2)
        {
            string name;
            cout << "Enter dish name to remove: ";
            getline(cin, name);
            size_t originalSize = menuItems.size();
            menuItems.erase(remove_if(menuItems.begin(), menuItems.end(), [&](const MenuItem &m)
                                      { return m.name == name; }),
                            menuItems.end());
            if (menuItems.size() < originalSize)
                cout << "Menu item removed.\n";
            else
                cout << "No matching menu item found.\n";
        }
        else if (choice == 3)
        {
            cout << "\nMenu:\n";
            for (auto &m : menuItems)
                cout << "- " << m.name << " | $" << fixed << setprecision(2) << m.price << endl;
        }
    } while (choice != 4);
}

void displayDashboard()
{
    double totalSales = 0;
    for (auto &b : branches)
        totalSales += b.sales;

    line();
    cout << "===== ADMIN DASHBOARD =====\n";
    cout << "Total Branches: " << branches.size() << endl;
    cout << "Total Customers: " << customers.size() << endl;
    cout << "Total Menu Items: " << menuItems.size() << endl;
    cout << "Total Sales: $" << fixed << setprecision(2) << totalSales << endl;

    cout << "\n--- Pending Orders ---\n";
    if (pendingOrders.empty())
        cout << "No pending orders.\n";
    else
        for (size_t i = 0; i < pendingOrders.size(); i++)
            cout << i + 1 << ". " << pendingOrders[i].dish << " | Customer: " << pendingOrders[i].customerName
                 << " | Cuisine: " << pendingOrders[i].cuisineName << " | Price: " << fixed << setprecision(2) << pendingOrders[i].price
                 << " | Ordered at: " << pendingOrders[i].datetime << endl;

    cout << "\n--- Completed Orders ---\n";
    if (completedOrders.empty())
        cout << "No completed orders.\n";
    else
        for (size_t i = 0; i < completedOrders.size(); i++)
            cout << i + 1 << ". " << completedOrders[i].dish << " | Customer: " << completedOrders[i].customerName
                 << " | Cuisine: " << completedOrders[i].cuisineName << " | Price: " << fixed << setprecision(2) << completedOrders[i].price
                 << " | Completed at: " << completedOrders[i].datetime << endl;

    line();
}

// ==================== MANAGER PORTAL ====================

void managerPortal()
{
    cout << "\n--- MANAGER PORTAL ---\n";

    double totalSales = 0;
    for (auto &b : branches)
        totalSales += b.sales;
    cout << "Total Sales: $" << fixed << setprecision(2) << totalSales << endl;

    // Display stock inventory
    cout << "\n--- Stock Inventory ---\nItalian Menu:\n";
    for (auto &d : italianMenu)
        cout << d.name << " | Price: " << d.price << " | Stock: " << d.stock << endl;
    cout << "Chinese Menu:\n";
    for (auto &d : chineseMenu)
        cout << d.name << " | Price: " << d.price << " | Stock: " << d.stock << endl;
    cout << "Mexican Menu:\n";
    for (auto &d : mexicanMenu)
        cout << d.name << " | Price: " << d.price << " | Stock: " << d.stock << endl;

    // Pending Orders
    cout << "\n--- Pending Orders ---\n";
    if (pendingOrders.empty())
        cout << "No pending orders.\n";
    else
        for (size_t i = 0; i < pendingOrders.size(); i++)
            cout << i + 1 << ". " << pendingOrders[i].dish << " | Customer: " << pendingOrders[i].customerName
                 << " | Cuisine: " << pendingOrders[i].cuisineName
                 << " | Price: " << fixed << setprecision(2) << pendingOrders[i].price
                 << " | Ordered at: " << pendingOrders[i].datetime << endl;

    // Completed Orders
    cout << "\n--- Completed Orders ---\n";
    if (completedOrders.empty())
        cout << "No completed orders.\n";
    else
        for (size_t i = 0; i < completedOrders.size(); i++)
            cout << i + 1 << ". " << completedOrders[i].dish << " | Customer: " << completedOrders[i].customerName
                 << " | Cuisine: " << completedOrders[i].cuisineName
                 << " | Price: " << fixed << setprecision(2) << completedOrders[i].price
                 << " | Completed at: " << completedOrders[i].datetime << endl;

    // Ask to export
    cout << "\nExport data to Excel? (1 Yes, 0 No): ";
    int choice;
    if (safeReadInt(choice) && choice == 1)
        exportDataToExcel();
    line();
}

// ==================== EMPLOYEE PORTAL ====================

// Simplified employee portal — replace existing employeePortal() with this.
void employeePortal()
{
    cout << "\n--- Employee Portal ---\n";

    while (true)
    {
        if (pendingOrders.empty())
        {
            cout << "No pending orders!\n";
            cout << "Press Enter to return to main menu...";
            // Wait for user to press Enter (safe way)
            string wait;
            getline(cin, wait);
            return;
        }

        // Build groups using simple vectors (group key = customerName + '|' + datetime)
        vector<string> groupKeys;         // holds unique keys in insertion order
        vector<vector<int>> groupIndices; // parallel: indices of pendingOrders per group

        for (size_t i = 0; i < pendingOrders.size(); ++i)
        {
            string key = pendingOrders[i].customerName + "|" + pendingOrders[i].datetime;

            // linear search for key (simple and easy to explain)
            int found = -1;
            for (size_t k = 0; k < groupKeys.size(); ++k)
            {
                if (groupKeys[k] == key)
                {
                    found = static_cast<int>(k);
                    break;
                }
            }

            if (found == -1)
            {
                // new group
                groupKeys.push_back(key);
                groupIndices.push_back(vector<int>()); // push empty vector
                groupIndices.back().push_back(static_cast<int>(i));
            }
            else
            {
                // existing group
                groupIndices[found].push_back(static_cast<int>(i));
            }
        }

        // Display grouped orders
        cout << "\nPending grouped orders:\n";
        for (size_t g = 0; g < groupKeys.size(); ++g)
        {
            // split key into customer and datetime
            string key = groupKeys[g];
            size_t pos = key.find('|');
            string customer = (pos == string::npos) ? key : key.substr(0, pos);
            string datetime = (pos == string::npos) ? "" : key.substr(pos + 1);

            // compute total and show items
            double groupTotal = 0.0;
            cout << (g + 1) << ". Customer: " << customer << " | Ordered at: " << datetime << " | Items: ";
            for (size_t j = 0; j < groupIndices[g].size(); ++j)
            {
                const Order &o = pendingOrders[groupIndices[g][j]];
                cout << o.dish;
                if (!o.toppings.empty())
                {
                    cout << " (";
                    for (size_t t = 0; t < o.toppings.size(); ++t)
                    {
                        cout << o.toppings[t];
                        if (t + 1 < o.toppings.size())
                            cout << ", ";
                    }
                    cout << ")";
                }
                if (j + 1 < groupIndices[g].size())
                    cout << ", ";
                groupTotal += o.price;
            }
            cout << " | Total: " << fixed << setprecision(2) << groupTotal << "\n";
        }

        // Ask user which grouped order to complete
        cout << "Select grouped order number to complete (0 to exit employee portal): ";
        int sel;
        if (!safeReadInt(sel))
        {
            cout << "Invalid selection.\n";
            continue; // stay in employee portal
        }

        if (sel == 0)
        {
            cout << "Exiting Employee Portal.\n";
            return; // only exit when user chooses 0
        }

        if (sel < 1 || sel > static_cast<int>(groupKeys.size()))
        {
            cout << "Invalid order number.\n";
            continue;
        }

        // Get the selected group's indices (copy)
        vector<int> indicesToProcess = groupIndices[sel - 1];

        // compute group total and update branch sales per item
        double groupTotal = 0.0;
        for (int idx : indicesToProcess)
        {
            groupTotal += pendingOrders[idx].price;

            // update corresponding branch sales for each item's cuisine
            for (auto &b : branches)
            {
                if (b.name == pendingOrders[idx].cuisineName)
                {
                    b.sales += pendingOrders[idx].price;
                    break;
                }
            }
        }

        // Move items to completedOrders and erase from pendingOrders safely:
        // erase by sorting indices descending and erasing by index
        sort(indicesToProcess.begin(), indicesToProcess.end(), greater<int>());

        for (int idx : indicesToProcess)
        {
            completedOrders.push_back(pendingOrders[idx]);
            pendingOrders.erase(pendingOrders.begin() + idx);
        }

        cout << "Grouped order completed. Sales updated by " << fixed << setprecision(2) << groupTotal << ".\n";

        // After finishing, loop continues and shows remaining groups (or "No pending orders!" when none).
    }
}

// ==================== RECEIPT PRINTING ====================

void printReceiptForBatch(const string &customerName, const string &batchDatetime)
{
    // Collect items for this batch
    vector<Order> items;
    double total = 0.0;
    for (const auto &o : pendingOrders)
    {
        if (o.customerName == customerName && o.datetime == batchDatetime)
        {
            items.push_back(o);
            total += o.price;
        }
    }

    // If no items in pendingOrders for this batch (should be rare), still try completedOrders
    if (items.empty())
    {
        for (const auto &o : completedOrders)
        {
            if (o.customerName == customerName && o.datetime == batchDatetime)
            {
                items.push_back(o);
                total += o.price;
            }
        }
    }

    // Print receipt to console
    line();
    cout << "               RECEIPT\n";
    line();
    cout << "Customer: " << customerName << "\n";
    cout << "Order Time: " << batchDatetime << "\n";
    cout << "-------------------------------\n";
    for (size_t i = 0; i < items.size(); ++i)
    {
        cout << i + 1 << ". " << items[i].dish;
        if (!items[i].toppings.empty())
        {
            cout << " (";
            for (size_t t = 0; t < items[i].toppings.size(); ++t)
            {
                cout << items[i].toppings[t];
                if (t + 1 < items[i].toppings.size())
                    cout << ", ";
            }
            cout << ")";
        }
        cout << " - Rs." << fixed << setprecision(2) << items[i].price << "\n";
    }
    cout << "-------------------------------\n";
    cout << "TOTAL: Rs." << fixed << setprecision(2) << total << "\n";
    line();
}

// ==================== CUSTOMER PORTAL ====================

void customerPortal()
{
    // Removed the redundant cin.ignore() here so the user won't have to press Enter.
    string customerName;
    cout << "Enter your name (or type 0 to exit): ";
    getline(cin, customerName);

    if (customerName == "0") // User wants to exit
    {
        cout << "Exiting Customer Portal.\n";
        return;
    }

    // If customer is new, add them to customers list
    bool exists = false;
    for (auto &c : customers)
    {
        if (c.name == customerName)
        {
            exists = true;
            break;
        }
    }
    if (!exists)
    {
        Customer c;
        c.name = customerName;
        c.joinDate = currentDateTime();
        customers.push_back(c);
        cout << "Welcome, " << customerName << "! You have been registered.\n";
    }

    // Create a single batch datetime for this customer's entire session
    string batchDatetime = currentDateTime();

    bool continueOrdering = true;

    // Dishes per cuisine
    string italianDishes[] = {"Pizza", "Pasta", "Risotto"};
    string chineseDishes[] = {"Dumplings", "Sweet and Sour", "Kung Pao Chicken"};
    string mexicanDishes[] = {"Tacos", "Burritos", "Enchiladas"};

    // Toppings per cuisine
    string italianToppings[] = {"Tomato Sauce", "Mozzarella", "Basil"};
    string chineseToppings[] = {"Soy Sauce", "Ginger", "Garlic"};
    string mexicanToppings[] = {"Salsa", "Guacamole", "Sour Cream"};

    // Topping prices per cuisine
    double italianToppingPrices[] = {100.0, 150.0, 120.0};
    double chineseToppingPrices[] = {80.0, 70.0, 60.0};
    double mexicanToppingPrices[] = {50.0, 70.0, 60.0};

    while (continueOrdering)
    {
        cout << "\nSelect Cuisine:\n1. Italian\n2. Chinese\n3. Mexican\nChoice: ";
        int choice;
        if (!safeReadInt(choice))
        {
            cout << "Invalid choice.\n";
            continue;
        }

        string *dishes = nullptr; // pointer to selected cuisine dishes
        string *toppings = nullptr;
        double *toppingPrices = nullptr;
        DishInfo *menu = nullptr;
        string cuisineName;

        switch (choice)
        {
        case 1:
            dishes = italianDishes;
            toppings = italianToppings;
            toppingPrices = italianToppingPrices;
            menu = italianMenu;
            cuisineName = "Italian";
            break;
        case 2:
            dishes = chineseDishes;
            toppings = chineseToppings;
            toppingPrices = chineseToppingPrices;
            menu = chineseMenu;
            cuisineName = "Chinese";
            break;
        case 3:
            dishes = mexicanDishes;
            toppings = mexicanToppings;
            toppingPrices = mexicanToppingPrices;
            menu = mexicanMenu;
            cuisineName = "Mexican";
            break;
        default:
            cout << "Invalid choice.\n";
            continue;
        }

        // Show dishes (customers don't see exact stock)
        cout << "\nSelect Dish:\n";
        for (int i = 0; i < 3; i++)
            cout << i + 1 << ". " << dishes[i] << " | Price: " << menu[i].price << endl;

        cout << "Choice (1-3): ";
        int dishChoice;
        if (!safeReadInt(dishChoice))
        {
            cout << "Invalid input.\n";
            continue;
        }

        if (dishChoice < 1 || dishChoice > 3)
        {
            cout << "Invalid choice.\n";
            continue;
        }

        if (menu[dishChoice - 1].stock <= 0)
        {
            cout << "Dish out of stock.\n";
            continue;
        }

        menu[dishChoice - 1].stock--; // Update stock
        double totalPrice = menu[dishChoice - 1].price;
        vector<string> selectedToppings;

        // Topping selection
        cout << "Add toppings? (1 Yes, 0 No): ";
        int addToppingInt;
        if (!safeReadInt(addToppingInt))
        {
            cout << "Invalid input. Assuming no toppings.\n";
            addToppingInt = 0;
        }
        bool addTopping = (addToppingInt == 1);

        while (addTopping)
        {
            cout << "\nSelect Topping:\n";
            for (int i = 0; i < 3; i++)
                cout << i + 1 << ". " << toppings[i] << " | Price: " << toppingPrices[i] << endl;

            cout << "Choice (1-3): ";
            int toppingChoice;
            if (!safeReadInt(toppingChoice))
            {
                cout << "Invalid topping input.\n";
            }
            else if (toppingChoice < 1 || toppingChoice > 3)
            {
                cout << "Invalid topping choice.\n";
            }
            else
            {
                selectedToppings.push_back(toppings[toppingChoice - 1]);
                totalPrice += toppingPrices[toppingChoice - 1];
            }

            cout << "Add another topping? (1 Yes, 0 No): ";
            if (!safeReadInt(addToppingInt))
            {
                cout << "Invalid input. Stopping topping selection.\n";
                addTopping = false;
            }
            else
            {
                addTopping = (addToppingInt == 1);
            }
        }

        // Add to pending orders using batchDatetime as the batch id
        pendingOrders.push_back({batchDatetime, customerName, cuisineName, dishes[dishChoice - 1], selectedToppings, totalPrice});
        cout << "Dish with toppings added. Total price: " << fixed << setprecision(2) << totalPrice << endl;

        cout << "Order another dish? (1 Yes, 0 No): ";
        int continueOrderingInt;
        if (!safeReadInt(continueOrderingInt))
        {
            cout << "Invalid input. Exiting ordering.\n";
            continueOrdering = false;
        }
        else
        {
            continueOrdering = (continueOrderingInt == 1);
        }
    }

    // Final Order Summary for this batch
    double grandTotal = 0;
    cout << "\n--- Final Order Summary for " << customerName << " ---\n";
    int count = 0;
    for (size_t i = 0; i < pendingOrders.size(); i++)
    {
        if (pendingOrders[i].customerName == customerName && pendingOrders[i].datetime == batchDatetime)
        {
            count++;
            cout << count << ". " << pendingOrders[i].dish << " | Toppings: ";
            if (!pendingOrders[i].toppings.empty())
            {
                for (size_t j = 0; j < pendingOrders[i].toppings.size(); j++)
                {
                    cout << pendingOrders[i].toppings[j];
                    if (j < pendingOrders[i].toppings.size() - 1)
                        cout << ", ";
                }
            }
            else
            {
                cout << "None";
            }
            cout << " | Price: " << fixed << setprecision(2) << pendingOrders[i].price << endl;
            grandTotal += pendingOrders[i].price;
        }
    }
    if (count == 0)
        cout << "No items found in your session (this shouldn't happen).\n";
    cout << "Total Bill: " << fixed << setprecision(2) << grandTotal << endl;

    // Print a receipt for this batch
    printReceiptForBatch(customerName, batchDatetime);

    cout << "Press Enter to return to main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Initialize menuItems vector from the DishInfo arrays so Admin sees the menu by default
void initializeMenuItems()
{
    menuItems.clear();
    for (auto &d : italianMenu)
        menuItems.push_back({d.name, d.price});
    for (auto &d : chineseMenu)
        menuItems.push_back({d.name, d.price});
    for (auto &d : mexicanMenu)
        menuItems.push_back({d.name, d.price});
}

// ==================== MAIN ====================

int main()
{
    // Initialize the admin-visible menu from the stock arrays
    initializeMenuItems();

    bool running = true;
    while (running)
    {
        cout << "\n--- Restaurant System ---\n1. Admin\n2. Manager\n3. Employee\n4. Customer\n5. Exit\nChoice: ";
        int role;
        if (!safeReadInt(role))
        {
            cout << "Invalid input. Please enter a number from 1 to 5.\n";
            continue;
        }

        switch (role)
        {
        case 1:
            if (adminLogin())
            {
                int choice;
                do
                {
                    cout << "\n--- Admin Menu ---\n1. Branches\n2. Customers\n3. Menu\n4. Dashboard\n5. Export to Excel\n6. Logout\nChoice: ";
                    if (!safeReadInt(choice))
                    {
                        cout << "Invalid input. Try again.\n";
                        continue;
                    }
                    switch (choice)
                    {
                    case 1:
                        manageBranches();
                        break;
                    case 2:
                        manageCustomers();
                        break;
                    case 3:
                        manageMenu();
                        break;
                    case 4:
                        displayDashboard();
                        break;
                    case 5:
                        exportDataToExcel();
                        break;
                    case 6:
                        cout << "Logging out of admin.\n";
                        break;
                    default:
                        cout << "Invalid selection.\n";
                        break;
                    }
                } while (choice != 6);
            }
            break;
        case 2:
            managerPortal();
            break;
        case 3:
            employeePortal();
            break;
        case 4:
            customerPortal();
            break;
        case 5:
            running = false;
            cout << "Exiting system. Goodbye!\n";
            break;
        default:
            cout << "Invalid role.\n";
        }
    }
    return 0;
}
