/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
  // @TODO: graded in mp_lists part 1
    length_ = 0;
    head_ = NULL;
    tail_ = NULL;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(tail_ -> next);
}



/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in mp_lists part 1
  while(head_) {
    ListNode * n = head_->next;
    delete head_;
    head_ = n;
  }
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode * newNode = new ListNode(ndata);
  
  if (head_ != NULL) {
    newNode -> next = head_;
    newNode -> prev = NULL;
    head_ -> prev = newNode;
  }
  head_ = newNode;
  if(tail_ == NULL) {
    newNode -> next = NULL;
    tail_ = newNode;
  }
  length_++;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = NULL;
  newNode -> prev = tail_;
  if(tail_ != NULL) {
    tail_ -> next = newNode;
  }
  tail_ = newNode;
  if(head_ == NULL) {
    head_ = newNode;
    head_ -> prev = NULL;
  }
  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in mp_lists part 1
  ListNode * curr = start;
  for (int i = 0; i < splitPoint && curr != NULL; i++) {
    curr = curr->next;
  }
  if(curr != NULL) {
    if(curr->prev != NULL) {
      curr->prev->next = NULL;
    }
    curr->prev = NULL;
  }
  return curr;
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() {
  /// @todo Graded in part 1
  if(length_ < 2) {
    return;
  }
  ListNode * curr = head_;
  int c = 1;
  while(curr && curr != tail_) {
    ListNode * temp = curr -> next;
    if(c % 2 == 0) {
      curr ->prev -> next = curr -> next;
      curr ->next -> prev = curr -> prev;
      curr -> next = NULL;
      curr -> prev = tail_;
      tail_ -> next = curr;
      tail_ = curr;
    }
    curr = temp;
    c++;
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in mp_lists part 2
  if(startPoint == endPoint || startPoint == NULL || endPoint == NULL) {
    return;
  }
  ListNode* curr;
  ListNode* travel = startPoint;
  ListNode* o_start = startPoint;
  ListNode* o_startprev = startPoint -> prev;
  ListNode* o_end = endPoint;
  ListNode* o_endnext = endPoint -> next;

  while(travel != endPoint) {
    curr = travel -> prev;
    travel -> prev = travel -> next;
    travel -> next = curr;
    travel = travel -> prev;
  }
  curr = travel -> prev;
  travel -> prev = o_startprev;
  travel -> next = curr;
  endPoint = o_start;
  endPoint -> next = o_endnext;
  if(o_startprev) {
    o_startprev -> next = o_end;
  } else {
    head_ = travel;
  }
  if(o_endnext) {
    o_endnext -> prev = o_start;
  } else {
    tail_ = endPoint;
  }
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in mp_lists part 2
  if(length_ <= 1 || n <= 1) {
    return;
  }
  ListNode* temp = head_;
  while(temp) {
    ListNode* t = temp;
    for(int i = 1; i < n; ++i) {
      temp = temp -> next;
      if(temp == tail_) {
        break;
      }
    }
    reverse(t, temp);
    temp = temp -> next;
  }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in mp_lists part 2
  if(!first) {
    return second;
  }
  if(!second) {
    return first;
  }
  ListNode* head;
  if(first -> data < second -> data) {
    head = first;
  } else {
    head = second;
  }
  ListNode* curr;
  if(head == first) {
    curr = first;
    first = first -> next;
  } else {
    curr = second;
    second = second -> next;
  }
  while(first && second) {
    if(first -> data < second -> data) {
      curr -> next = first;
      curr = first;
      first = first -> next;
    } else {
      curr -> next = second;
      curr = second;
      second = second->next;
    }
  }
  if (first) {
    curr -> next = first;
  } else if (second) {
    curr -> next = second;
  }
  return head;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in mp_lists part 2
  if(!start || !start -> next) {
    return start;
  }
  /*
  if(!start -> next) {
    return start;
  }*/
  ListNode* curr = split(start, chainLength / 2);
  start = mergesort(start, chainLength / 2);
  curr = mergesort(curr, chainLength - (chainLength/2));
  return merge(start, curr);
}
