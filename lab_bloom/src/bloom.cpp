/**
 * @file bloom.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

 #include "bloom.h"
 #include <unordered_set>

BF::BF(uint64_t size, std::vector<hashFunction> hashList){
    bv = std::vector<bool>(size, false); // Initialize bit vector with all bits set to false
    hv = hashList; 
    // Your code here 
}

BF::BF(const BF& other){
    // Your code here 
    bv = other.bv; // Copy the bit vector
    hv = other.hv;
}

BF::~BF(){
    return;
}

void BF::add(const int & key){
    for (auto hash_func : hv) {
        int index = hash_func(key) % bv.size(); // Apply hash function and take modulo with size
        bv[index] = true; // Set the corresponding bit to true
    }
}

bool BF::contains(const int& key) const{
    // Your code here 
    for (auto hash_func : hv) {
        int index = hash_func(key) % bv.size(); // Apply hash function and take modulo with size
        if (!bv[index]) { // If any of the corresponding bits is false
            return false; // The key is definitely not present
        }
    }
    return true;
}

void BF::bit_union(const BF& other){
    // Your code here 
    if (bv.size() != other.bv.size() || hv != other.hv) {
        return; // Cannot perform bitwise union
    }

    // Perform bitwise OR between the bit vectors
    for (size_t i = 0; i < bv.size(); ++i) {
        bv[i] = bv[i] || other.bv[i];
    }
}

void BF::intersect(const BF& other){
    // Your code here 
    if (bv.size() != other.bv.size() || hv != other.hv) {
        return; // Cannot perform bitwise intersection
    }

    // Perform bitwise AND between the bit vectors
    for (size_t i = 0; i < bv.size(); ++i) {
        bv[i] = bv[i] && other.bv[i];
    }
}

float measureFPR(std::vector<int> inList, uint64_t size, std::vector<hashFunction> hashList, unsigned max){
    // Your code here
    // Create a new Bloom Filter with the given size and hash functions
    BF bloomFilter(size, hashList);

    // Insert the elements from the inList into the Bloom Filter
    for (int num : inList) {
        bloomFilter.add(num);
    }

    // Create a set to store the inserted elements
    std::unordered_set<int> insertedSet(inList.begin(), inList.end());

    uint64_t falsePositives = 0;
    uint64_t trueNegatives = 0;

    // Iterate over the universe and count false positives and true negatives
    for (unsigned i = 0; i < max; ++i) {
        bool isPresent = insertedSet.count(i) > 0;
        bool bloomFilterContains = bloomFilter.contains(i);

        if (isPresent && bloomFilterContains) {
            // True Positive (TP)
            // Ignore
        } else if (!isPresent && bloomFilterContains) {
            // False Positive (FP)
            ++falsePositives;
        } else if (!isPresent && !bloomFilterContains) {
            // True Negative (TN)
            ++trueNegatives;
        } else {
            // False Negative (FN)
            // Impossible in a Bloom Filter
        }
    }

    double fpr = static_cast<double>(falsePositives) / (falsePositives + trueNegatives);
    return fpr;
}

bool getBitFromByte(char in, int index) {
    // Create a bitmask with the desired bit set to 1 and all other bits set to 0
    char bitmask = (1 << (7 - index));
    char result = in & bitmask;
    return (result != 0);
}

bool getBitFromArray(std::vector<char> bv, int index) {
    // Calculate the byte index and bit index within the byte
    std::size_t byteIndex = static_cast<std::size_t>(index / 8);
    int bitIndex = index % 8;

    // Check if the byte index is valid
    if (byteIndex >= bv.size()) {
        // Index out of range
        return false;
    }

    // Get the byte at the calculated byte index
    char byte = bv[byteIndex];

    // Get the boolean value of the bit at the calculated bit index within the byte
    return getBitFromByte(byte, bitIndex);
}

