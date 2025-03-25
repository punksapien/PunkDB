#ifndef LEARNED_IDX_H
#define LEARNED_IDX_H

// learned index implementation
// based on: "The Case for Learned Index Structures"
// kraska et al., SIGMOD 2018
// paper: https://arxiv.org/abs/1712.01208

#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// simple learned index using linear model
// this shoud predict where keys are in sorted array
class learned_idx {
private:
    vector<pair<string, int>> data;  // key -> original position mapping
    double slope;
    double intercept;
    int sz;

    // train teh model on current data
    void train_model();

    // convert string key to numeric val for model
    double key_to_num(const string& k);

public:
    learned_idx();

    // insert or update key with position
    void insert(const string& key, int pos);

    // find position of key, returns -1 if not found
    int search(const string& key);

    // remove key from index
    void remove(const string& key);

    // get all keys sorted
    vector<string> get_keys();

    // retrain model (call after bulk inserts)
    void rebuild();
};

#endif
