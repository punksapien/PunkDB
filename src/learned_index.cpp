#include "learned_index.h"
#include <cmath>
#include <iostream>

learned_idx::learned_idx() {
    slope = 1.0;
    intercept = 0.0;
    sz = 0;
}

// convert key to numeric value
// just use simple hash for now
double learned_idx::key_to_num(const string& k) {
    double val = 0;
    for (int i = 0; i < k.length(); i++) {
        val = val * 31 + k[i];  // simple polynomial hash
    }
    return val;
}

// train linear regression model
// learns y = mx + b where x is key hash, y is position
void learned_idx::train_model() {
    if (data.size() < 2) {
        slope = 1.0;
        intercept = 0.0;
        return;
    }

    // sort data by key first
    sort(data.begin(), data.end());

    // update positions after sorting
    for (int i = 0; i < data.size(); i++) {
        data[i].second = i;
    }

    sz = data.size();

    // compute linear regression
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;
    int n = data.size();

    for (int i = 0; i < n; i++) {
        double x = key_to_num(data[i].first);
        double y = i;  // position in sorted array
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_xx += x * x;
    }

    // calculate slope and intercept
    // slope = (n*sum_xy - sum_x*sum_y) / (n*sum_xx - sum_x*sum_x)
    double denom = n * sum_xx - sum_x * sum_x;
    if (abs(denom) < 1e-10) {
        slope = 1.0;
        intercept = 0.0;
    } else {
        slope = (n * sum_xy - sum_x * sum_y) / denom;
        intercept = (sum_y - slope * sum_x) / n;
    }
}

void learned_idx::insert(const string& key, int pos) {
    // check if key already exists
    for (int i = 0; i < data.size(); i++) {
        if (data[i].first == key) {
            return;  // already exists, dont add again
        }
    }

    data.push_back({key, pos});
    train_model();  // retrain after insert
}

// search for key using learned model
int learned_idx::search(const string& key) {
    if (data.size() == 0) return -1;

    // use model to predict position
    double x = key_to_num(key);
    double pred = slope * x + intercept;

    // clamp prediction to valid range
    int predicted_pos = (int)pred;
    if (predicted_pos < 0) predicted_pos = 0;
    if (predicted_pos >= sz) predicted_pos = sz - 1;

    // do small binary search around predicted position
    // this corrects for model errors
    int search_range = 32;  // search +/- 32 positions
    int left = max(0, predicted_pos - search_range);
    int right = min(sz - 1, predicted_pos + search_range);

    // linear search in small range (could use binary but this is simple)
    for (int i = left; i <= right; i++) {
        if (data[i].first == key) {
            return data[i].second;
        }
    }

    // if not found in range, do full binary search as fallback
    // this handles cases where model is way off
    left = 0;
    right = sz - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (data[mid].first == key) {
            return data[mid].second;
        } else if (data[mid].first < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;  // not found
}

void learned_idx::remove(const string& key) {
    for (int i = 0; i < data.size(); i++) {
        if (data[i].first == key) {
            data.erase(data.begin() + i);
            train_model();  // retrain after removal
            return;
        }
    }
}

vector<string> learned_idx::get_keys() {
    vector<string> keys;
    for (auto& p : data) {
        keys.push_back(p.first);
    }
    sort(keys.begin(), keys.end());
    return keys;
}

void learned_idx::rebuild() {
    train_model();
}
