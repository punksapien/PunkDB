#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "db.h"

using namespace std;

// ansi color codes for pretty output
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// split string by spaces (simple parser)
vector<string> split_cmd(const string& line) {
    vector<string> tokens;
    stringstream ss(line);
    string token;

    while (ss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

// convert string to uppercase for case-insensitive commands
string to_upper(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] - 'a' + 'A';
        }
    }
    return s;
}

// show nice banner on startup
void show_banner() {
    cout << CYAN << BOLD;
    cout << " ╔═══════════════════════════════════════╗" << endl;
    cout << " ║                                       ║" << endl;
    cout << " ║   " << MAGENTA << "LEARNDB" << CYAN << " - learned index database  ║" << endl;
    cout << " ║                                       ║" << endl;
    cout << " ║   " << RESET << CYAN << "using ML instead of b-trees!      " << BOLD << "║" << endl;
    cout << " ║                                       ║" << endl;
    cout << " ╚═══════════════════════════════════════╝" << RESET << endl;
    cout << endl;
    cout << YELLOW << " type 'help' for commands" << RESET << endl;
    cout << endl;
}

int main() {
    db database;

    show_banner();

    while (true) {
        cout << CYAN << "learndb> " << RESET;
        string line;
        getline(cin, line);

        if (cin.eof()) break;

        vector<string> tokens = split_cmd(line);

        if (tokens.empty()) continue;

        string cmd = to_upper(tokens[0]);

        if (cmd == "QUIT" || cmd == "EXIT") {
            cout << YELLOW << "✓ " << RESET << "goodbye!" << endl;
            break;
        } else if (cmd == "HELP") {
            cout << endl;
            cout << BOLD << "Available Commands:" << RESET << endl;
            cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
            cout << GREEN << "  SET" << RESET << " key value    - store a key-value pair" << endl;
            cout << GREEN << "  GET" << RESET << " key          - retreive value for key" << endl;
            cout << GREEN << "  DEL" << RESET << " key          - delete a key" << endl;
            cout << GREEN << "  EXISTS" << RESET << " key       - check if key exists" << endl;
            cout << GREEN << "  KEYS" << RESET << "             - list all keys" << endl;
            cout << GREEN << "  SIZE" << RESET << "             - show number of keys" << endl;
            cout << GREEN << "  STATS" << RESET << "            - show db statistics" << endl;
            cout << GREEN << "  CLEAR" << RESET << "            - clear screen" << endl;
            cout << GREEN << "  HELP" << RESET << "             - show this help" << endl;
            cout << GREEN << "  QUIT" << RESET << "             - exit database" << endl;
            cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
            cout << endl;
            cout << YELLOW << "Example:" << RESET << " SET name alice" << endl;
            cout << endl;
        } else if (cmd == "CLEAR" || cmd == "CLS") {
            cout << "\033[2J\033[1;1H";  // clear screen
            show_banner();
        } else if (cmd == "STATS") {
            cout << endl;
            cout << BOLD << "Database Statistics:" << RESET << endl;
            cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
            cout << CYAN << "  Keys stored:  " << RESET << database.size() << endl;
            cout << CYAN << "  Index type:   " << RESET << "Learned (linear regression)" << endl;
            cout << CYAN << "  Storage:      " << RESET << "In-memory" << endl;
            cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
            cout << endl;
        } else if (cmd == "SET") {
            if (tokens.size() < 3) {
                cout << RED << "✗ " << RESET << "error: SET requires key and value" << endl;
                continue;
            }

            string key = tokens[1];
            string val = tokens[2];

            // support multi-word values
            for (int i = 3; i < tokens.size(); i++) {
                val += " " + tokens[i];
            }

            database.set(key, val);
            cout << GREEN << "✓ " << RESET << "OK" << endl;
        } else if (cmd == "GET") {
            if (tokens.size() < 2) {
                cout << RED << "✗ " << RESET << "error: GET requires key" << endl;
                continue;
            }

            string key = tokens[1];
            string val = database.get(key);

            if (val.empty()) {
                cout << YELLOW << "→ " << RESET << "(nil)" << endl;
            } else {
                cout << GREEN << "→ " << RESET << "\"" << val << "\"" << endl;
            }
        } else if (cmd == "DEL") {
            if (tokens.size() < 2) {
                cout << RED << "✗ " << RESET << "error: DEL requires key" << endl;
                continue;
            }

            string key = tokens[1];
            bool deleted = database.del(key);

            if (deleted) {
                cout << GREEN << "✓ " << RESET << "(integer) 1" << endl;
            } else {
                cout << YELLOW << "→ " << RESET << "(integer) 0" << endl;
            }
        } else if (cmd == "EXISTS") {
            if (tokens.size() < 2) {
                cout << RED << "✗ " << RESET << "error: EXISTS requires key" << endl;
                continue;
            }

            string key = tokens[1];
            bool exists = database.exists(key);

            if (exists) {
                cout << GREEN << "✓ " << RESET << "(integer) 1" << endl;
            } else {
                cout << YELLOW << "→ " << RESET << "(integer) 0" << endl;
            }
        } else if (cmd == "KEYS") {
            vector<string> all_keys = database.keys();

            if (all_keys.empty()) {
                cout << YELLOW << "→ " << RESET << "(empty list)" << endl;
            } else {
                cout << endl;
                for (int i = 0; i < all_keys.size(); i++) {
                    cout << CYAN << setw(3) << (i + 1) << ") " << RESET << all_keys[i] << endl;
                }
                cout << endl;
            }
        } else if (cmd == "SIZE") {
            cout << CYAN << "→ " << RESET << "(integer) " << database.size() << endl;
        } else {
            cout << RED << "✗ " << RESET << "unknown command: " << YELLOW << cmd << RESET << endl;
            cout << "  type " << GREEN << "'help'" << RESET << " for available commands" << endl;
        }
    }

    return 0;
}
