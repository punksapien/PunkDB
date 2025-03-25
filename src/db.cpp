#include "db.h"
#include <iostream>

db::db() {
    next_pos = 0;
}

bool db::set(const string& key, const string& val) {
    // check if key already exists
    int pos = idx.search(key);

    if (pos != -1 && pos < store.size()) {
        // update existing key
        for (int i = 0; i < store.size(); i++) {
            if (store[i].first == key) {
                store[i].second = val;
                return true;
            }
        }
    }

    // insert new key
    store.push_back({key, val});
    idx.insert(key, next_pos);
    next_pos++;
    return true;
}

string db::get(const string& key) {
    int pos = idx.search(key);

    if (pos == -1) {
        return "";  // not found
    }

    // search in store for the key
    // we cant directly use pos cuz items might be reordered
    for (int i = 0; i < store.size(); i++) {
        if (store[i].first == key) {
            return store[i].second;
        }
    }

    return "";  // not found
}

bool db::del(const string& key) {
    int pos = idx.search(key);

    if (pos == -1) {
        return false;  // key doesnt exist
    }

    // remove from store
    for (int i = 0; i < store.size(); i++) {
        if (store[i].first == key) {
            store.erase(store.begin() + i);
            idx.remove(key);
            return true;
        }
    }

    return false;
}

bool db::exists(const string& key) {
    int pos = idx.search(key);

    if (pos == -1) {
        return false;
    }

    // verify in store
    for (int i = 0; i < store.size(); i++) {
        if (store[i].first == key) {
            return true;
        }
    }

    return false;
}

vector<string> db::keys() {
    return idx.get_keys();
}

int db::size() {
    return store.size();
}
