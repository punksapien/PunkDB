#ifndef DB_H
#define DB_H

#include "learned_index.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// simple in-memory db using learned index
class db {
private:
    learned_idx idx;  // teh learned index structure
    vector<pair<string, string>> store;  // actual key-value pairs
    int next_pos;  // next available position

public:
    db();

    // set key to value (insert or update)
    bool set(const string& key, const string& val);

    // get value for key, returns empty string if not found
    string get(const string& key);

    // delete key, returns true if deleted
    bool del(const string& key);

    // check if key exists
    bool exists(const string& key);

    // get all keys
    vector<string> keys();

    // get number of keys
    int size();
};

#endif
