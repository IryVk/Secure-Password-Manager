#include "helpers.h" //where all the helper functions are

// globals to store key, iv and username
std::string KEY = "";
std::string IV = "";
std::string USER = "";

// cleanup functions to delete unencrypted file buffers on exit
void cleanup();
void signalHandler(int signal);

int main() {
    // call cleanup on exit (any exit of the program will call cleanup())
    std::atexit(cleanup);

    // register the signal handler for SIGINT (to handle keyboard interrupts and other interrupts)
    std::signal(SIGINT, signalHandler);

    // welcome the user
    std::cout << "Welcome to the Password Manager!\n";

    // loop to handle invalid inputs
    while (true) {
        // display initial menu to user
        displayInitMenu();

        // read user input
        int choice;
        std::string prompt = "Enter your choice: ";
        choice = getInt(prompt);

        // switch case to handle user input
        switch (choice) {
            case 1:
                // create user will create a new user and store the key and iv and username
                if (!createUser(KEY, IV, USER)) {
                    std::exit(1);
                }
                break;
            case 2:
                // verify user will verify the user and store the key and iv and username
                if (!verifyUser(KEY, IV, USER)) {
                    std::exit(1);
                }
                break;
            case 3:
                // exiting the program cleanly
                std::exit(0);
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
        break;
    }
    // decrypt passwords file
    // cast key and iv to SecByteBlock
    SecByteBlock keyblock((byte*)KEY.data(), KEY.size());
    SecByteBlock ivblock((byte*)IV.data(), IV.size());
    AESCrypto aesCrypto(keyblock, ivblock);
    aesCrypto.DecryptFile("assets/" + USER + ".csv", "assets/" + USER + "_decrypted.csv");
    // read passwords file
    HashTable ht;
    CSVReader reader(ht, "assets/" + USER + "_decrypted.csv");
    reader.load();

    // loop to handle invalid inputs
    while (true) {
        // display second menue
        displayMenu2();
        int choice;
        std::string prompt = "Enter your choice: ";
        choice = getInt(prompt);

        int n;

        // index to hold bucket
        Bucket* index = nullptr;
        switch (choice) {
            case 1:
                if (!addPassword(reader, ht, KEY)) {
                    std::exit(1);
                }
                break;
            case 2:
                // search for passwords will change our existing pointer, so we dereference it in the call to change its value directly
                n = searchPasswords(reader, ht, &index);
                if (n > 0){
                    // ask user if they would like to update, delete or show a password using record number
                    manipulatePassword(reader, ht, index, KEY);
                }
                break;
            case 3:
                // view passwords
                viewPasswords(reader, ht);
                break;
            case 4:
                // generate a secure password for the user
                generatePassword(reader, ht, KEY);
                break;
            case 5:
                std::exit(0);
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}



// cleanup function to delete unencrypted file buffers on exit
void cleanup() {
    std::cout << "Cleaning up...\n";
    // encrypt unencrypted files
    if (KEY == "" || IV == "" || USER == "") {
        return;
    }
    // cast key and iv to SecByteBlock
    SecByteBlock keyblock((byte*)KEY.data(), KEY.size());
    SecByteBlock ivblock((byte*)IV.data(), IV.size());
    AESCrypto aesCrypto(keyblock, ivblock);
    // encrypt passwords file
    aesCrypto.EncryptFile("assets/" + USER + "_decrypted.csv", "assets/" + USER + ".csv");
    // remove unencrypted file
    std::remove(("assets/" + USER + "_decrypted.csv").c_str());
}

// signal handler for SIGINT
void signalHandler(int signal) {
    std::cout << " Signal " << signal << " caught.\n";
    // exiting will invoke cleanup()
    std::exit(EXIT_SUCCESS);
}