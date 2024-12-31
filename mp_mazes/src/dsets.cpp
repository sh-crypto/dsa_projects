/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num) {
    for(int i = 0; i < num; i++) {
        set_.push_back(-1);
    }
}

int DisjointSets::find(int elem) {
    if(set_[elem] < 0) {
        return elem;
    } else {
        set_[elem] = find(set_[elem]);
        return find(set_[elem]);
    }
}

void DisjointSets::setunion(int a, int b) {
    a = find(a);
    b = find(b);
    int n_size = set_[a] + set_[b];
    if(set_[a] <= set_[b]) {
        set_[b] = a;
        set_[a] = n_size;
    } else {
        set_[a] = b;
        set_[b] = n_size;
    }
}

int DisjointSets::size(int elem) {
    return -(set_[find(elem)]);
}