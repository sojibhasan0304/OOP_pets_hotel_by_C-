#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "PetHotelController.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "Rodent.hpp"
#include "ConsoleUtil.hpp"
#include "DateUtil.hpp"
#include "Persistence.hpp"
#include "User.hpp"

// This one for the text-file save in database.
const std::string DB_FILE = "../pet_hotel_db.txt"; // This is for reservation
const std::string ACCOUNTS_FILE = "../accounts.txt";// login for account

// Simple account structure
// I have used "struct" to group data. 
struct Account {
    std::string  username;
    std::string password;
    UserRole    role;      // Customer or Employee
};

//  kennels by using a loop
// "static" for only showing in file.
static void seedKennels(PetHotelController& hotel) {
    int id = 1;
    for (int i= 0; i < 10; ++i) hotel.addKennel(Kennel(id++, KennelType::LargeDog, 1)); // big dogs kennael(1-10)
    for (int i = 0; i < 20; ++i) hotel.addKennel(Kennel(id++, KennelType::MediumDog, 2)); // // Medium dogs kennael(10-20) 
    for (int i = 0; i < 50; ++i) hotel.addKennel(Kennel(id++, KennelType::SmallDogOrCat, 2));// bsmall dogs and cats kennael(20-50)
    for (int i = 0; i < 30; ++i) hotel.addKennel(Kennel(id++, KennelType::RodentGroup, 4));/// Rodent 
}

// creat new accounts and login. 
bool saveAccounts(const std::vector<Account>& accounts) {   
    std::ofstream out(ACCOUNTS_FILE);                       // opingin file
    if (!out) {
        std::cout << "Warning: could not open " << ACCOUNTS_FILE << " for writing.\n"; // If there is no file say no
        return false;
    }
    for (const auto& acc : accounts) {          // Loop through each account in the vector 
        out << acc.username << "," << acc.password << ","
            << (acc.role == UserRole::Employee ? "Employee" : "Customer") << "\n";//if not we will craet one.
    }
    return true;
}

bool loadAccounts(std::vector<Account>& accounts) { // Load accounts from file into the vector
    std::ifstream in(ACCOUNTS_FILE);            // open files for reading. 
    if (!in) {
                        // if no file yet is not an error: first run
        return false;
    }

    std::string line;                           // it will store one line from file
    while (std::getline(in, line)) {            // it's like a statments 
        if (line.empty()) continue;            // If we have a line is empty, it skip to next line
        std::istringstream ss(line);
        std::string u, p, r;                      // It takes variables for username, password, role
        std::getline(ss, u, ',');
        std::getline(ss, p, ',');
        std::getline(ss, r, ',');

        if (u.empty() || p.empty() || r.empty()) continue;  // If  we have missing fields, skip line
        
        // It's converting the of role string "Employee" and "Customer" into the UserRole enum. and adding data to the file.

        UserRole role = (r == "Employee" ? UserRole::Employee : UserRole::Customer);
        accounts.push_back({ u, p, role });
    }
    return true;
}

// Now I have the login parts 

UserRole login(const std::vector<Account>& accounts) {
                // it's just a loop to find the right person for login. 
    while (true) {
        std::string u = readLine("Username: ");
        std::string p = readLine("Password: ");

        for (const auto& acc : accounts) {
            if (acc.username == u && acc.password == p) {
                std::cout << "Login successful as " // if yeas it will show ok. 
                    << (acc.role == UserRole::Customer ? "Customer" : "Employee")
                    << "\n";
                return acc.role;
            }
        }
        std::cout << "Invalid username or password. Try again.\n"; // if not found it will say no. 
    }
}
    //registration parts

void registerCustomer(std::vector<Account>& accounts) { // This is a function to create a new Customer account
    std::string u = readLine("Choose username: "); // name

    // Check if username already exists

    for (const auto& acc : accounts) {       // Loop accounts to check user name is doublr or not
        if (acc.username == u) {        // if alredey taken 
            std::cout << "That username is already taken. Please choose another.\n"; // 
            return; // stop registration
        }
    }
     // for the pass. 
    std::string p = readLine("Choose password: ");
    accounts.push_back({ u, p, UserRole::Customer });
    saveAccounts(accounts); // if we have data in your database it will show. 
    std::cout << "Customer account created. You can now log in.\n";
}

// If login as a customer. 

static void customerMenu(PetHotelController& hotel) {
    for (;;) {                  // there is a list for customer 
        std::cout << "\nCustomer Menu:\n"
            << "1) Register pet\n"
            << "2) Check availability (registered pet)\n"
            << "3) Make reservation (YYYY-MM-DD)\n"
            << "4) Preview availability (no registration)\n"
            << "5) Save hotel data to file\n"
            << "6) Load hotel data from file\n"
            << "0) Back\n> ";

        int c = readIntInRange("", 0, 6); // ask for option 
        if (c == 0) break;  // 0 for exsit

        //  Register for thw pets
        //  if one then. 
        if (c == 1) {
            int species = readIntInRange("Pet type (1=Dog,2=Cat,3=Rodent): ", 1, 3); // pets
            std::string name = readLine("Name: "); // name
            int age = readIntInRange("Age: ", 0, 100); // age
            std::string breed = readLine("Breed: ");// Bread number

            // if yiu have dog then 

            if (species == 1) {
                double w;
                std::cout << "Weight (kg): ";
                std::cin >> w;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                hotel.registerPet(std::make_shared<Dog>(name, age, breed, w));
            }
            // cats
            else if (species == 2) {
                hotel.registerPet(std::make_shared<Cat>(name, age, breed));
            }
            // Rabbit type 
            else {
                int k = readIntInRange("Kind (1=Rabbit,2=GuineaPig,3=Mouse): ", 1, 3);
                RodentKind kind = (k == 1 ? RodentKind::Rabbit
                    : k == 2 ? RodentKind::GuineaPig
                    : RodentKind::Mouse);
                hotel.registerPet(std::make_shared<Rodent>(name, age, breed, kind));
            }
            // Registration done 
            std::cout << "Pet registered.\n";
        }

        //  Cheaking the avaiable place for pets  
        else if (c == 2) {
            std::string name = readLine("Pet name: ");
            bool cohab = readYesNo("Allow cohabitation?");
            auto p = hotel.getPet(name);

            if (!p) {
                std::cout << "Pet not found.\n";
                continue;
            }
            std::cout << "Available kennels: "
                << hotel.availableKennelsFor(*p, cohab) << "\n";
        }

        // Option 3 make reservation. 
        else if (c == 3) {

            std::string cust = readLine("Customer name: "); 
            std::string petName = readLine("Pet name: ");
            bool cohab = readYesNo("Allow cohabitation?");
            std::string startS = readLine("Start date (YYYY-MM-DD): ");
            std::string endS = readLine("End date (YYYY-MM-DD): ");

            std::string finalCareNotes;

            // it will show if pet exists which care schedule it's need. 
            // this part for care schedule. 
            auto p = hotel.getPet(petName);

            if (p) {
                std::cout << "\nDefault care schedule for this pet type:\n";
                std::cout << "  " << p->careSchedule() << "\n";

                // I have added those so that it could be easy to understand. 

                std::cout << "\nExample Extra Notes:\n";
                std::cout << "  - Medication: 1 tablet Rimadyl at 20:00\n";
                std::cout << "  - No chicken treats (allergy)\n";
                std::cout << "  - Playtime 30 minutes daily\n\n";

                std::string extra = readLine("Extra care notes (optional): ");

                finalCareNotes = p->careSchedule();
                if (!extra.empty()) {
                    finalCareNotes += " | Extra: " + extra;
                }
            }
            else {
                // If the pet isn't registered yet than it will show general examples
                std::cout << "\nExamples of Care Notes:\n";
                std::cout << "  - Feed 2x/day (8:00, 18:00); Walk 3x/day\n";
                std::cout << "  - Medication: Eye drops 2x/day\n";
                std::cout << "  - Clean litter every morning\n\n";

                finalCareNotes = readLine("Care notes: ");
            }

            // this is the try and catch function so that the comiler can understand wrong data type.
            try {
                DateRange dr{ dateutil::parseYMD(startS), dateutil::parseYMD(endS) };
                Reservation r{ cust, petName, dr, CareSchedule(finalCareNotes), cohab };

                if (hotel.createReservation(r))
                    std::cout << "Reservation created.\n";
                else
                    std::cout << "Reservation must be 2–28 days.\n";
            }
            catch (const std::exception& ex) {
                std::cout << "Date error: " << ex.what() << "\n";
            }
        }

        // without registation cheak the avaiblity. 
        else if (c == 4) {
            int species = readIntInRange("Pet type (1=Dog,2=Cat,3=Rodent): ", 1, 3);
            bool cohab = readYesNo("Allow cohabitation?");
            double weight = 0.0;

            if (species == 1) {
                std::cout << "Dog weight (kg): ";
                std::cin >> weight;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            PetType t = (species == 1 ? PetType::Dog :
                species == 2 ? PetType::Cat : PetType::Rodent);

            std::cout << "Available kennels: "
                << hotel.previewAvailability(t, weight, cohab) << "\n";
        }

        //  Saving data  
        else if (c == 5) {
            std::string err;
            if (persistence::saveToFile(hotel, DB_FILE, err))
                std::cout << "Saved hotel data to " << DB_FILE << "\n";
            else
                std::cout << "Save failed: " << err << "\n";
        }

        // Loaidng data
        else if (c == 6) {
            std::string err;
            if (persistence::loadFromFile(hotel, DB_FILE, err))
                std::cout << "Loaded hotel data from " << DB_FILE << "\n";
            else
                std::cout << "Load failed: " << err << "\n";
        }
    }
}


// login as a employee 

static void employeeMenu(PetHotelController& hotel) {   // This function for the employee menu
    for (;;) {                                          // This one for the infinite loop 
        std::cout << "\nEmployee Menu:\n"
            << "1) Add kennel\n"
            << "2) Remove kennel\n"
            << "3) List kennels with pets\n"
            << "4) Check-in pet\n"
            << "5) Move pet\n"
            << "6) Check-out pet\n"
            << "7) Cancel reservation\n"
            << "8) Save hotel data to file\n"
            << "9) Load hotel data from file\n"
            << "0) Back\n> ";
        int c = readIntInRange("", 0, 9);            // Read choice from 0 tp 9
        if (c == 0) break;
        // adding kennel
        if (c == 1) {
            int id = readIntInRange("New kennel id: ", 1, 1000000);
            int t = readIntInRange("Type (1=LargeDog,2=MediumDog,3=SmallDogOrCat,4=RodentGroup): ", 1, 4);
            int cap = readIntInRange("Capacity: ", 1, 1000);
            hotel.addKennel(Kennel(id, static_cast<KennelType>(t - 1), cap));
            std::cout << "Kennel added.\n";
        }
        else if (c == 2) {
            int id = readIntInRange("Kennel id: ", 1, 1000000);
            std::cout << (hotel.removeKennel(id) ? "Removed.\n" : "Not found.\n");
        }
        // List kennels with pets
        else if (c == 3) {
            for (const auto& pair : hotel.listKennelsWithPets()) {
                std::cout << "Kennel " << pair.first << ": ";
                if (pair.second.empty()) std::cout << "(empty)";
                else for (auto& n : pair.second) std::cout << n << " ";
                std::cout << "\n";
            }
        }
        // Check-in pet
        else if (c == 4) {
            std::string pet = readLine("Pet name: ");
            int id = readIntInRange("Kennel id: ", 1, 1000000);
            bool cohab = readYesNo("Allow cohabitation?");
            std::cout << (hotel.checkIn(pet, id, cohab) ? "Checked in.\n" : "Failed.\n");
        }
        // Move pet
        else if (c == 5) {
            std::string pet = readLine("Pet name: ");
            int id = readIntInRange("Move to kennel id: ", 1, 1000000);
            bool cohab = readYesNo("Allow cohabitation?");
            std::cout << (hotel.movePet(pet, id, cohab) ? "Moved.\n" : "Failed.\n");
        }
        // Check-out pet
        else if (c == 6) {
            std::string pet = readLine("Pet name: ");
            std::cout << (hotel.checkOut(pet) ? "Checked out.\n" : "Failed.\n");
        }
        // Cancel reservation
        else if (c == 7) {
            std::string pet = readLine("Pet name: ");
            std::cout << (hotel.cancelReservation(pet) ? "Canceled.\n" : "Not found.\n");
        }
        // Save hotel
        else if (c == 8) {
            std::string err;
            if (persistence::saveToFile(hotel, DB_FILE, err))
                std::cout << "Saved hotel data to " << DB_FILE << "\n";
            else
                std::cout << "Save failed: " << err << "\n";
        }
        // Load hotel
        else if (c == 9) {
            std::string err;
            if (persistence::loadFromFile(hotel, DB_FILE, err))
                std::cout << "Loaded hotel data from " << DB_FILE << "\n";
            else
                std::cout << "Load failed: " << err << "\n";
        }
    }
}

//  main function 

int main() {
    PetHotelController hotel;       // I have Created hotel controller object

    // 1) Load hotel data
    std::string err;
    if (!persistence::loadFromFile(hotel, DB_FILE, err)) {
        std::cout << "No existing hotel database found or load failed: " << err << "\n";
        std::cout << "Starting with empty hotel.\n";
    }

    if (hotel.kennels().empty()) {  // If no kennels exist then
        seedKennels(hotel);         // it will create default kennels
    }

    // 2) Load accounts (users)
    std::vector<Account> accounts;
    loadAccounts(accounts);

    // Ensure at least one employee account exists
    bool hasEmployee = false;
    for (const auto& a : accounts) {
        if (a.role == UserRole::Employee) {
            hasEmployee = true;
            break;
        }
    }
    if (!hasEmployee) {
        accounts.push_back({ "emp1", "admin", UserRole::Employee });
        saveAccounts(accounts);
        std::cout << "Default employee account created: username=emp1, password=admin\n";
    }

    std::cout << "Welcome to Pet Hotel\n";

    // 3) Top-level loop: login or register
    for (;;) {
        std::cout << "\nMain Menu:\n"
            << "1) Login\n"
            << "2) Register as new customer\n"
            << "0) Exit\n";
        int choice = readIntInRange("Choice: ", 0, 2);

        if (choice == 0) break;

        if (choice == 2) {
            registerCustomer(accounts);
            continue;
        }

        // choice == 1 -> login
        UserRole role = login(accounts);

        if (role == UserRole::Customer) {
            customerMenu(hotel);
        }
        else {
            employeeMenu(hotel);
        }
    }

    // 4) Save hotel data on exit
    if (!persistence::saveToFile(hotel, DB_FILE, err)) {
        std::cout << "Warning: could not save hotel database: " << err << "\n";
    }
    else {
        std::cout << "Hotel database saved to " << DB_FILE << "\n";
    }

    // 5) Save accounts (in case they changed)
    saveAccounts(accounts);

    std::cout << "Goodbye!\n";
    return 0;
}
