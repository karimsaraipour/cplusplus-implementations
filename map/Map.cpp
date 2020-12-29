//  Map.cpp
//  Project1
//
//  Created by Karim on 2/29/20.
//  Copyright © 2020 Karim. All rights reserved.
//
#include <iostream>
#include "Map.h"

using namespace std;

Map::Map() {
    count = 0;
    for (int i = 0; i < CAPACITY; i++) //sets all buckets to nullptr
        bucketlist[i] = nullptr;
}

Map::Map(const Map& rhs) {
    count = 0;
    for (int i = 0; i < CAPACITY; i++) //sets all buckets to nullptr
        bucketlist[i] = nullptr;
    
    for (int i = 0; i < CAPACITY; i++) {
        if (rhs.bucketlist[i] != nullptr) {
            HashNode* transversal = rhs.bucketlist[i];
            while (transversal != nullptr) {
                this->insert(transversal->key, transversal->value);
                transversal = transversal->next;
            }
        }
    }
    
}

Map::~Map() {
    for (int i = 0; i < CAPACITY; i++) {
        if (bucketlist[i] != nullptr) {
            HashNode* transversal = bucketlist[i];
            while (transversal != nullptr) {
                bucketlist[i] = transversal->next;
                delete transversal;
                count--;
                transversal = bucketlist[i];
            }
        }
    }
}

const Map& Map::operator=(const Map& rhs) {
    if(this == &rhs)
        return *this;
    
    for (int i = 0; i < CAPACITY; i++) {
        if (bucketlist[i] != nullptr) {
            HashNode* transversal = bucketlist[i];
            while (transversal != nullptr) {
                bucketlist[i] = transversal->next;
                delete transversal;
                count--;
                transversal = bucketlist[i];
            }
        }
    }
    
    count = 0;
    for (int i = 0; i < CAPACITY; i++) //sets all buckets to nullptr
        bucketlist[i] = nullptr;
    
    for (int i = 0; i < CAPACITY; i++) {
        if (rhs.bucketlist[i] != nullptr) {
            HashNode* transversal = rhs.bucketlist[i];
            insert(transversal->key, transversal->value);
        }
    }
    return *this;
}



bool Map::empty() const {
    for (int i = 0; i < CAPACITY; i++) {
        if (bucketlist[i] != nullptr)
            return false;
    }
    return true;
}

int Map::size() const {
    return count;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    int keyBucket = Convert_Key(key) % CAPACITY;
    /*if (bucketlist[keyBucket] == nullptr) {
     bucketlist[keyBucket] = new HashNode(key, value);
     count++;
     return true;
     }*/
    
    if (!contains(key)) {
        HashNode* transversal = new HashNode(key, value);
        transversal->next = bucketlist[keyBucket];
        bucketlist[keyBucket] = transversal;
        count++;
        return true;
    }
    return false;
}
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map).

bool Map::update(const KeyType& key, const ValueType& value) {
    int keyBucket = Convert_Key(key) % CAPACITY;
    if (bucketlist[keyBucket] == nullptr)
        return false;
    else {
        HashNode* transversal = bucketlist[keyBucket];
        while (transversal != nullptr) {
            if (transversal->key == key) {
                transversal->value = value;
                return true;
            }
            transversal = transversal->next;
        }
    }
    return false;
}
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    int keyBucket = Convert_Key(key) % CAPACITY;
    if (bucketlist[keyBucket] == nullptr) {
        insert(key, value);
        return true;
    }
    else {
        HashNode* transversal = bucketlist[keyBucket];
        while (transversal != nullptr) {
            if (transversal->key == key) {
                transversal->value = value;
                return true;
            }
            transversal = transversal->next;
        }
        insert(key, value);
        return true;
    }
    return false; //if it returns false, we know theres a problem. It shouldnt get to this point
}
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map then add it and
// return true. In fact this function always returns true.

bool Map::erase(const KeyType& key) {
    int keyBucket = Convert_Key(key) % CAPACITY;
    if (bucketlist[keyBucket] == nullptr)
        return false;
    else {
        HashNode* transversal = bucketlist[keyBucket];
        if (transversal->next == nullptr) {
            bucketlist[keyBucket] = nullptr;
            delete transversal;
            count--;
            return true;
        }
        while (transversal->next != nullptr) {
            if (transversal->next->key == key) {
                HashNode* temp = transversal->next;
                transversal->next = temp->next;
                delete temp;
                count--;
                return true;
            }
            transversal = transversal->next;
        }
        return false;
    }
}
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.

bool Map::contains(const KeyType& key) const {
    int keyBucket = Convert_Key(key) % CAPACITY;
    if (bucketlist[keyBucket] == nullptr)
        return false;
    else {
        HashNode* transversal = bucketlist[keyBucket];
        while (transversal != nullptr) {
            if (transversal->key == key) {
                return true;
            }
            transversal = transversal->next;
        }
    }
    return false;
}
// Return true if key is equal to a key currently in the map, otherwise
// false.

bool Map::get(const KeyType& key, ValueType& value) const {
    int keyBucket = Convert_Key(key) % CAPACITY;
    if (bucketlist[keyBucket] == nullptr)
        return false;
    else {
        HashNode* transversal = bucketlist[keyBucket];
        while (transversal != nullptr) {
            if (transversal->key == key) {
                value = transversal->value;
                return true;
            }
            transversal = transversal->next;
        }
    }
    return false;
}
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.

bool Map::get(int i, KeyType& key, ValueType& value) const {
    if (i > size())
        return false;
    
    int counter = i;
    for (int t = 0; t < CAPACITY; t++) {
        if (bucketlist[t] != nullptr) {
            HashNode* transversal = bucketlist[t];
            while (transversal != nullptr) {
                if (counter == 0) {
                    key = transversal->key;
                    value = transversal->value;
                    return true;
                }
                counter--;
                transversal = transversal->next;
            }
        }
    }
    return false;
}
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)

void Map::swap(Map& other) {
    //note other will have the same capacity or size of hashing array
    HashNode* temp;
    int sizeOther = other.size();
    other.count = size();
    this->count = sizeOther;
    for (int i = 0; i < CAPACITY; i++) {
        temp = bucketlist[i];
        bucketlist[i] = other.bucketlist[i];
        other.bucketlist[i] = temp;
    }
}
// Exchange the contents of this map with the other one.



int Convert_Key(string key) {
    int total = 0;
    for (int i = 0; i < key.length(); i++)
        total += (key[i] * (i + 1));
    
    return total;
}
int Convert_Key(double key) {
    return (int)(key * 1000);
}


bool combine(const Map& m1, const Map& m2, Map& result) {
    Map primary;
    bool primaryBool = true;
    
    if(&m1 == &m2){
        result = m1;
        return true;
    }
    
    if(&m1 == &result) {
        bool x = combine(m1, m2, primary);
        result = primary;
        return x;
    }
    
    if(&m2 == &result) {
        bool x = combine(m1, m2, primary);
        result = primary;
        return x;
    }
   
    
    for (int i = 0; i < m1.size(); i++) {
        KeyType k;
        ValueType v;
        m1.get(i, k, v);
        primary.insert(k, v);
    }
    
    for (int i = 0; i < m2.size(); i++) {
        KeyType k;
        ValueType v, v2;
        m2.get(i, k, v);
        if (!primary.insert(k, v)) {
            primary.get(k, v2);
            if (v != v2) {
                primary.erase(k);
                primaryBool = false;
            }
        }
    }
    
    result = primary;
    return primaryBool;
}
void subtract(const Map& m1, const Map& m2, Map& result) {
    Map primary;
    if(&m1 == &m2) {
        result = primary;
        return;
    }
    if(&m1 == &result) {
        subtract(m1, m2, primary);
        result = primary;
        return;
    }
    
    if(&m2 == &result) {
        subtract(m1, m2, primary);
        result = primary;
        return;
    }
    
    primary = m1;
    for (int i = 0; i < m1.size(); i++) {
        KeyType k;
        ValueType v;
        m1.get(i, k, v);
        if (m2.contains(k))
            primary.erase(k);
    }
    result = primary;
}







