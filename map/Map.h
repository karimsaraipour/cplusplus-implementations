#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <string>

using KeyType = std::string;
using ValueType = double;


const static int CAPACITY = 53;
class Map
{
private:

    int count;
    struct HashNode
    {
        KeyType key;
        ValueType value;
        HashNode* next; //useful for hashing(singly linked list)

        HashNode(KeyType key, ValueType value) {
            this->key = key;
            this->value = value;
            next = nullptr;
        }
    };

    HashNode* bucketlist[CAPACITY];

public:
    Map();
    Map(const Map& rhs);
    ~Map();
    const Map& operator=(const Map& rhs);
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);

};

int Convert_Key(std::string key);
int Convert_Key(double key);
bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif /* Map_hpp */


