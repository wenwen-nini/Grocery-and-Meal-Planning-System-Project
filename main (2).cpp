#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#include <chrono>
#include <thread>
#define MAX_SIZE 10

using namespace std;

//STRUCTURES
struct Grocery {
    string name;
    string category;
    string expiration;
    int priority;
    int daysLeft;
};

struct Meal {
    string name;
    string description;
};


//GLOBAL VARIABLES
Grocery groceryList[MAX_SIZE];
Meal mealPlan[MAX_SIZE];
int front = - 1;
int rear = - 1;
int groceryListSize = 0;


//DESIGN PURPOSES FUNCTIONS DECLARATIONS
void clearScreen();
void fancyLoader(int seconds);
void typeText(const string &text, int delayMs = 50);
void typeTextWithVar(const string &variable, const string &text, int delayMs = 50);
void typeTextWithVarInt(const int &variable, const string &text, int delayMs = 50);


//USER DEFINED FUNCTIONS DECLARATIONS
int calculateDaysUntilExpiration(const string& expiration);
void addGroceryItem(string name, string category, string expiration);
void removeGroceryItem();
void displayGroceryItems();
void searchGroceryItem();
void checkLatestExpiration();
void sortByExpiration();
void sortByName();
void sortByCategory();
void addMealPlan(string name, string description);
void removeCurrentMealPlan();
void displayMealPlan();
void checkTodayMeal();
void loadGroceryList();
void loadMealPlan();
void saveToFile();
bool confirmationMenu();


//USER-DEFINED FUNCTIONS OF OPERATIONS FULL AND EMPTY
bool isEmptyForMeal() {
    return front == -1;
}

bool isFullForMeal() {
    return (front == 0 && rear == MAX_SIZE - 1) || (rear == (front - 1 + MAX_SIZE) % MAX_SIZE);
}

bool isEmpty() {
    return groceryListSize == 0;
}

bool isFull() {
    return groceryListSize == MAX_SIZE;
}

//MAIN PROGRAM

int main () {

    Grocery grocery;
    Meal meal;

    bool exitProgram = false;
    
    fancyLoader(5);
    //LOAD FUNCTION CALL
    loadGroceryList();
    loadMealPlan();

    int choice;
    do {
        cout << "\n========================================================\n\t    Grocery And Meal Planning System\n========================================================\n\n";
        cout << "1.  Add Grocery Item\n";
        cout << "2.  Remove Grocery Item (removes the soonest expiration)\n";
        cout << "3.  Display Grocery Items\n";
        cout << "4.  Search an Item\n";
        cout << "5.  Check the soonest expiration item\n";
        cout << "6.  Sort Items\n";
        cout << "7.  Add Meal Plan\n";
        cout << "8.  Remove Meal Plan\n";
        cout << "9.  Display Meal Plan\n";
        cout << "10. Check today's Meal Plan\n";
        cout << "11. Save to File\n";
        cout << "0.  Exit\n\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            clearScreen();
            typeText("\nInvalid input. Please enter a number!\n\n", 30);
            continue;
        }
        
        char extra;
        if (cin.get(extra) && extra != '\n') {
            cin.ignore(10000, '\n');
            clearScreen();
            typeText("\nInvalid input. No letters after numbers!\n\n", 30);
            continue;
        }

        cin.ignore();
        cout << endl;

        switch (choice) {

        case 1: //INSERTION PRIORITY QUEUE
            clearScreen();
            typeText("Enter the name of the grocery: ", 20);
            getline(cin, grocery.name);

            typeText("Enter the category of the grocery: ", 20);
            getline(cin, grocery.category);

            typeText("Enter the expiration date of the grocery (YYYY-MM-DD): ", 20);
            getline(cin, grocery.expiration);

            addGroceryItem(grocery.name, grocery.category, grocery.expiration);
            break;

        case 2: //REMOVE PRIORITY QUEUE
            removeGroceryItem();
            break;

        case 3: //DISPLAY PRIORITY QUEUE
            displayGroceryItems();
            break;

        case 4: //SEARCH PRIORITY QUEUE
            searchGroceryItem();
            break;

        case 5: //CHECK PEEK PRIORITY QUEUE
            checkLatestExpiration();
            break;

        case 6: //SORTING PRIORITY QUEUE

            char sortChoice;

            cout << "[A]. Sort by Expiration\n";
            cout << "[B]. Sort by Name\n";
            cout << "[C]. Sort by Category\n";
            cout << "[D]. Go back to Main Menu\n\n";
            cout << "Enter your choice: ";
            cin >> sortChoice;
            cin.ignore();
            
            char extra;
            if (cin.get(extra) && extra != '\n') {
                cin.ignore(10000, '\n');
                clearScreen();
                typeText("Invalid input. Please Try Again.\n\n", 30);
                continue;
            }
            
            cout << endl;
            
            sortChoice = toupper(sortChoice);

            switch (sortChoice) {

            case 'A':
                sortByExpiration();
                break;

            case 'B':
                sortByName();
                break;

            case 'C':
                sortByCategory();
                break;
                
            case 'D':
                clearScreen();
                break;   

            default:
                clearScreen();
                typeText("Invalid input! Please try again.\n\n", 30);
                break;
            }

            break;

        case 7: //INSERTION CIRCULAR QUEUE
            typeText("Enter the meal's name: ", 30);
            getline(cin, meal.name);

            typeText("Enter the Description: ", 30);
            getline(cin, meal.description);

            addMealPlan(meal.name, meal.description);
            break;

        case 8: //REMOVE CIRCULAR QUEUE
            removeCurrentMealPlan();
            break;

        case 9: //DISPLAY CIRCULAR QUEUE
            displayMealPlan();
            break;

        case 10: //CHECK PEEK CIRCULAR QUEUE
            checkTodayMeal();
            break;

        case 11: //FILE HANDLING
            saveToFile();
            break;

        case 0: //EXIT PROGRAM
            if (confirmationMenu()) {
                exitProgram = true;
                typeText("\nThank you for using Grocery and Meal Planning System!\n", 30);
                break;
            }

            else {
                break;
            }

        default: //INVALID INPUTS
            clearScreen();
            typeText("Invalid Input. Please Try again!\n\n", 30);
            break;
        }
    } while (!exitProgram);
}

int calculateDaysUntilExpiration(const string& expiration) {

    // Check if the string length is at least the length of a valid date (10 characters for YYYY-MM-DD)
    if (expiration.length() != 10) {
        cerr << "\nError: Invalid date length. Expected format is YYYY-MM-DD." << endl;
        return -2;
    }

    // Ensure the expiration date is correctly formatted (i.e., the string should contain only numbers and hyphens)
    if (expiration[4] != '-' || expiration[7] != '-') {
        cerr << "\nError: Invalid date format. Expected YYYY-MM-DD." << endl;
        return -2;
    }

    // Try to parse the expiration date into year, month, and day
    int expYear, expMonth, expDay;
    if (sscanf(expiration.c_str(), "%d-%d-%d", &expYear, &expMonth, &expDay) != 3) {
        cerr << "\nError: Invalid date format. Expected YYYY-MM-DD." << endl;
        return -2; // Invalid format
    }

    // Validate if parsed year, month, and day are in valid ranges
    if (expYear < 1900 || expYear > 9999 || expMonth < 1 || expMonth > 12 || expDay < 1 || expDay > 31) {
        cerr << "\nError: Invalid date values." << endl;
        return -2; // Invalid date values
    }

    time_t now = time(0); // Get the current date in seconds
    tm* currentTm = localtime(&now); // Convert the current date in seconds to tm struct

    int currentYear = currentTm->tm_year + 1900; // Get the current year in human-readable format
    int currentMonth = currentTm->tm_mon + 1; // Get the current month in human-readable format
    int currentDay = currentTm->tm_mday; // Get the current day of the month

    // Create the tm structures for current and expiration dates
    tm current = {};
    current.tm_year = currentYear - 1900; // Adjust for tm struct
    current.tm_mon = currentMonth - 1; // Adjust for tm struct
    current.tm_mday = currentDay;

    tm expTion = {};
    expTion.tm_year = expYear - 1900; // Adjust for tm struct
    expTion.tm_mon = expMonth - 1; // Adjust for tm struct
    expTion.tm_mday = expDay;

    time_t currentTime = mktime(&current); // Convert the current date into time_t (seconds)
    time_t expirationTime = mktime(&expTion); // Convert the expiration date into time_t (seconds)

    if (currentTime == -1 || expirationTime == -1) { // Handle the case where mktime() fails to convert time.
        cerr << "Error: Invalid date conversion with mktime()." << endl;
        return -2; // Date conversion failed
    }

    double diff = difftime(expirationTime, currentTime); // Calculate the difference in seconds
    double result = static_cast<int>(diff / (60 * 60 * 24)); // Convert seconds to days

    result = round(result);

    if (result < 0) {
        return -1;
    }

    else {
        return result;
    }
}


void addGroceryItem(string name, string category, string expiration) {

    if (isFull()) {
        typeText("Grocery List is full.\n\n", 30);
        return;
    }

    int daysLeft = calculateDaysUntilExpiration(expiration);

    if (daysLeft == -2) {
        cout << "Error in calculating time difference. Invalid date format?\n\n";
        return;
    }

    if (daysLeft == -1) { //Condition if the product is already expired
        typeText("Your product is already expired!\n\n", 30);
        return;
    }



    int priority = (daysLeft >= 0) ? daysLeft : 9999; //Initializes the days left if only the daysLeft is greater than or equal to 0 or else the priority will turn into 9999
    int i = groceryListSize - 1;

    while (i >= 0 && groceryList[i].priority >= priority) {
        groceryList[i + 1] = groceryList[i];
        i--;
    }

    groceryList[i + 1].name = name;
    groceryList[i + 1].category = category;
    groceryList[i + 1].expiration = expiration;
    groceryList[i + 1].daysLeft = daysLeft;
    groceryList[i + 1].priority = priority;
    groceryListSize++;
    
    cout << endl;
    clearScreen();
    typeTextWithVar(name, " has been successfully added to the grocery list.\n\n", 20);
    
}

void removeGroceryItem() {
    clearScreen();
    
    if (isEmpty()) {
        typeText("Your grocery list is empty!\n\n", 30);
        return;
    }

    string removedName = groceryList[0].name;
    int removedDaysLeft = groceryList[0].daysLeft;

    for (int i = 0; i < groceryListSize - 1; i++) {
        groceryList[i] = groceryList[i + 1];
    }

    groceryListSize--;

    groceryList[groceryListSize] = Grocery(); 

    
    typeTextWithVar(removedName, " (", 30);
    if (removedDaysLeft == 1 || removedDaysLeft == 0) {
        typeTextWithVarInt(removedDaysLeft, " Day Left) has been removed.\n\n", 30);
    } else {
        typeTextWithVarInt(removedDaysLeft, " Days Left) has been removed.\n\n", 30);
    }

    displayGroceryItems();
}

void displayGroceryItems() {
    
    clearScreen();
    
    if (isEmpty()) {
        typeText("Your grocery list is empty!\n\n", 30);
        return;
    }

    cout << "\n========================================================\n\t\t      Grocery List\n========================================================\n\n";
    for (int i = 0; i < groceryListSize; i++) {

        cout << "Name: " << groceryList[i].name << " (";

        if (groceryList[i].daysLeft == 1 || groceryList[i].daysLeft == 0) {
            cout << groceryList[i].daysLeft << " Day Left)\n" << "Category: " << groceryList[i].category << "\n\n";
        }
        else {
            cout << groceryList[i].daysLeft << " Days Left)\n" << "Category: " << groceryList[i].category << "\n\n";
        }
    }
}

void searchGroceryItem() {
    
    clearScreen();
    
    string searchItem;

    if (isEmpty()) {
        typeText("Your grocery list is empty!\n\n", 30);
        return;
    }

    typeText("Enter the name of the Item to search: ", 30);
    getline(cin, searchItem);

    bool found = false;

    for (int i = 0; i < groceryListSize; i++) {
        if (searchItem == groceryList[i].name) {
            found = true;
            break;
        }
    }

    if (!found) {
        typeText("\nThe item is not on the list.\n\n", 30);
    }

    else {
        typeText("\nThe item is found in the list.\n\n", 30);
    }
}

void checkLatestExpiration() {
    
    clearScreen();
    
    if (isEmpty()) {
        typeText("Your grocery list is empty!\n\n", 30);
        return;
    }
    
    sortByExpiration();
    clearScreen();
    typeText("Generating the Latest Expiration Item...\n", 40);
    cout << "\n========================================================\n\t\t Latest Expiration Date\n========================================================\n\n";
    cout << "Name: " << groceryList[0].name << " (";

    if (groceryList[0].daysLeft == 1 || groceryList[0].daysLeft == 0) {
        cout << groceryList[0].daysLeft << " Day Left)\n" << "Category: " << groceryList[0].category << "\n\n";
    }
    else {
        cout << groceryList[0].daysLeft << " Days Left)\n" << "Category: " << groceryList[0].category << "\n\n";
    }
}

void sortByExpiration() {
    
    clearScreen();
    
    if (isEmpty()) {
        typeText("Your grocery list is empty!\n\n", 30);
        return;
    }

    Grocery temp[MAX_SIZE];
    for (int i = 0; i < groceryListSize - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < groceryListSize - i - 1; j++) {
            if (groceryList[j].daysLeft > groceryList[j + 1].daysLeft) {
                temp[j] = groceryList[j];
                groceryList[j] = groceryList[j + 1];
                groceryList[j + 1] = temp[j];
                swapped = true;
            }
        }

        if (!swapped) {
            typeText("\nThe grocery list is already sorted by expiration!\n\n", 30);
            return;
        } 
    }

    typeText("\nThe grocery list has been sorted by expiration successfully!\n", 30);

    displayGroceryItems();

}

void sortByName() {
    
    clearScreen();
    
    if (isEmpty()) {
        typeText("Your grocery list is empty!\n\n", 30);
        return;
    }

    Grocery temp[MAX_SIZE];
    for (int i = 0; i < groceryListSize - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < groceryListSize - i - 1; j++) {
            if (groceryList[j].name > groceryList[j + 1].name) {
                temp[j] = groceryList[j];
                groceryList[j] = groceryList[j + 1];
                groceryList[j + 1] = temp[j];
                swapped = true;
            }
        }

        if (!swapped) {
            typeText("\nThe grocery list is already sorted by name!\n\n", 30);
            return;
        } 
    }

    typeText("\nThe grocery list has been sorted by name successfully!\n", 30);

    displayGroceryItems();

}

void sortByCategory() {
    
    clearScreen();
    
    if (isEmpty()) {
        typeText("Your grocery list is empty!\n\n", 30);
        return;
    }

    Grocery temp[MAX_SIZE];
    for (int i = 0; i < groceryListSize - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < groceryListSize - i - 1; j++) {
            if (groceryList[j].category > groceryList[j + 1].category) {
                temp[j] = groceryList[j];
                groceryList[j] = groceryList[j + 1];
                groceryList[j + 1] = temp[j];
                swapped = true;
            }
        }

        if (!swapped) {
            typeText("\nThe grocery list is already sorted by category!\n\n", 30);
            return;
        }
    }

    typeText("\nThe grocery list has been sorted by category successfully!\n\n", 30);

    displayGroceryItems();

}

void addMealPlan(string name, string description) {
    
    clearScreen();
    
    if (isFullForMeal()) {
        typeText("Meal Plan is full.\n\n", 30);
        return;
    }

    if (isEmptyForMeal()) {
        front = rear = 0;
    }

    else if (rear == MAX_SIZE - 1 && front != 0) {
        rear = 0;
    }

    else {
        rear++;
    }

    mealPlan[rear].name = name;
    mealPlan[rear].description = description;
    
    cout << endl;
    typeTextWithVar(name, " has been successfully added to the meal plan.\n\n", 30);
}

void removeCurrentMealPlan() {
    
    clearScreen();
    
    if (isEmptyForMeal()) {
        typeText("Your meal plan is empty!\n\n", 30);
        return;
    }

    typeTextWithVar(mealPlan[front].name, " has been removed from the meal plan.\n\n", 30);

    if (front == rear) {
        front = rear = -1;
    }

    else if (front == MAX_SIZE - 1) {
        front = 0;
    }
    else {
        front++;
    }
}

void displayMealPlan() {
    
    clearScreen();
    
    if (isEmptyForMeal()) {
        typeText("Your meal plan is empty!\n\n", 30);
        return;
    }

    cout << "\n========================================================\n\t\t\tMeal Plan\n========================================================\n\n";
    if (rear >= front) {
        for (int i = front; i <= rear; i++) {
            cout << "Meal Name: " << mealPlan[i].name << endl;
            cout << "Description: " << mealPlan[i].description << endl << endl;
        }
    }

    else {
        for (int i = front; i < MAX_SIZE; i++) {
            cout << "Meal Name: " << mealPlan[i].name << endl;
            cout << "Description: " << mealPlan[i].description << endl << endl;
        }

        for (int i = 0; i <= rear; i++) {
            cout << "Meal Name: " << mealPlan[i].name << endl;
            cout << "Description: " << mealPlan[i].description << endl << endl;
        }
    }
}

void checkTodayMeal() {
    
    clearScreen();
    
    if (isEmptyForMeal()) {
        typeText("Your meal plan is empty!\n\n", 30);
        return;
    }

    cout << "\n========================================================\n\t\t      Today's Meal\n========================================================\n\n";
    cout << "Meal Name: " << mealPlan[front].name << endl;
    cout << "Description: " << mealPlan[front].description << "\n\n";
}

//LOAD FILE FUNCTIONS
//FOR GROCERY LIST
void loadGroceryList() {
    ifstream groceryFile("Grocery_List.txt");

    if (!groceryFile.is_open()) {
        typeText("Grocery list file not found. Starting with empty list.\n\n", 30);
        return;
    }

    groceryListSize = 0; // Reset size before loading

    while (groceryFile >> ws && getline(groceryFile, groceryList[groceryListSize].name)) {
        getline(groceryFile, groceryList[groceryListSize].category);
        string expirationDate;
        getline(groceryFile, expirationDate);

        groceryList[groceryListSize].expiration = expirationDate;
        groceryList[groceryListSize].daysLeft = calculateDaysUntilExpiration(expirationDate);
        groceryList[groceryListSize].priority = groceryList[groceryListSize].daysLeft >= 0 ? groceryList[groceryListSize].daysLeft : 9999;

        groceryListSize++;

        if (groceryListSize == MAX_SIZE) break; // prevent overflow
    }

    groceryFile.close();
    typeText("Grocery list loaded successfully!\n\n", 30);
}


//FOR MEAL PLAN
void loadMealPlan() {
    ifstream mealFile("Meal_Plan.txt");

    if (!mealFile.is_open()) {
        typeText("Meal plan file not found. Starting with empty meal plan.\n\n", 30);
        return;
    }

    front = -1;
    rear = -1;

    string name, description;
    while (mealFile >> ws && getline(mealFile, name)) {
        getline(mealFile, description);

        if (isEmptyForMeal()) {
            front = rear = 0;
        } else if (rear == MAX_SIZE - 1 && front != 0) {
            rear = 0;
        } else {
            rear++;
        }

        mealPlan[rear].name = name;
        mealPlan[rear].description = description;

        if ((rear + 1) % MAX_SIZE == front) break; // prevent overflow
    }

    mealFile.close();
    typeText("Meal plan loaded successfully!\n\n", 30);
}

void saveToFile() {
    ofstream groceryListFile("Grocery_List.txt");
    ofstream mealPlanFile("Meal_Plan.txt");

    if (!groceryListFile.is_open() || !mealPlanFile.is_open()) {
        cout << "Error opening file for writing! Please try again.\n\n";
        return;
    }

    // Save Grocery List
    for (int i = 0; i < groceryListSize; i++) {
        groceryListFile << groceryList[i].name << endl;     // Name
        groceryListFile << groceryList[i].category << endl; // Category
        groceryListFile << groceryList[i].expiration << endl;  // Expiration Date
    }
    groceryListFile.close();

    // Save Meal Plan
    if (rear >= front && front != -1) {
        for (int i = front; i <= rear; i++) {
            mealPlanFile << mealPlan[i].name << endl;
            mealPlanFile << mealPlan[i].description << endl;
        }
    } else if (front > rear) {
        for (int i = front; i < MAX_SIZE; i++) {
            mealPlanFile << mealPlan[i].name << endl;
            mealPlanFile << mealPlan[i].description << endl;
        }
        for (int i = 0; i <= rear; i++) {
            mealPlanFile << mealPlan[i].name << endl;
            mealPlanFile << mealPlan[i].description << endl;
        }
    }

    mealPlanFile.close();

    typeText("Data saved successfully to files!\n\n", 30);
    
    clearScreen();
}


bool confirmationMenu() {

    char choice;
    typeText("Are you sure your grocery list and meal plan is saved?\n(Save your grocery list and meal plan to a file in order not to lose the data)\n", 30);
    loop:
    typeText("[Y]. Exit\n",30);
    typeText("[N]. Main Menu\n\n", 30);
    typeText("Enter your choice: ", 30);
    cin >> choice;
    cin.ignore();
    
    char extra;
    if (cin.get(extra) && extra != '\n') {
        cin.ignore(10000, '\n');
        typeText("\nInvalid input. Please Try Again.\n\n", 30);
        goto loop;
    }

    cout << endl;

    choice = toupper(choice);

    switch (choice) {
    case 'Y':
        return true;

    case 'N':
        clearScreen();
        typeText("Welcome Back to Grocery and Meal Planning System!\n\n", 30);
        return false;

    default:
        cout << "Invalid Input. Please Try again!\n";
        goto loop;
    }
}

//FOR DESIGN PURPOSES FUNCTIONS

//CLEAR SCREEN FUNCTION
void clearScreen() { cout << "\033[2J\033[1;1H"; }

//LOADING ANIMATION FUNCTION
void fancyLoader(int seconds) {
    const string frames[] = {
        "[=------]",
        "[-=-----]",
        "[--=----]",
        "[---=---]",
        "[----=--]",
        "[-----=-]",
        "[------=]",
        "[-----=-]",
        "[----=--]",
        "[---=---]",
        "[--=----]",
        "[-=-----]"
    };
    
    cout << "Working ";
    for (int i = 0; i < seconds * 4; ++i) {
        cout << "\r" << frames[i % 12];
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(250));
    }
    cout << endl;
    clearScreen();
}

// TYPING ANIMATION FUNCTION
void typeText(const string &text, int delayMs) {
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}

//TYPING ANIMATION WITH STRING VARIABLE FUNCTION
void typeTextWithVar(const string &variable, const string &text, int delayMs) {
    
    cout << variable;
    
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}

//TYPING ANIMATION WITH INTEGER VARIABLE FUNCTION
void typeTextWithVarInt(const int &variable, const string &text, int delayMs) {
    
    cout << variable;
    
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}
    