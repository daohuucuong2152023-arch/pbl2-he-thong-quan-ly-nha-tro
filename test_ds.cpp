#include <iostream>
#include <cassert>
#include <string>

#include "data_structures/Stack.h"
#include "data_structures/Queue.h"
#include "data_structures/DoublyLinkedList.h"
#include "data_structures/BST.h"
#include "models/Room.h"

using namespace std;

void logPass(const string& testName) {
    cout << "[PASS] " << testName << endl;
}

void testStack() {
    Stack<int> s;
    assert(s.isEmpty());

    s.push(10);
    s.push(20);
    s.push(30);
    assert(s.size() == 3);

    int val;
    assert(s.top(val) && val == 30);

    assert(s.pop(val) && val == 30);
    assert(s.pop(val) && val == 20);
    assert(s.size() == 1);

    s.pop();
    assert(s.isEmpty());
    assert(!s.pop(val));

    logPass("Stack (push, pop, top, isEmpty, size)");
}

void testQueue() {
    Queue<string> q;
    assert(q.isEmpty());

    q.enqueue("Khach_A");
    q.enqueue("Khach_B");
    q.enqueue("Khach_C");
    assert(q.size() == 3);

    string frontVal;
    assert(q.front(frontVal) && frontVal == "Khach_A");

    assert(q.dequeue(frontVal) && frontVal == "Khach_A");
    assert(q.dequeue(frontVal) && frontVal == "Khach_B");
    assert(q.size() == 1);

    q.dequeue();
    assert(q.isEmpty());
    assert(!q.dequeue(frontVal));

    logPass("Queue (enqueue, dequeue, front, FIFO order)");
}

bool isTargetRoom(const string& name) {
    return name == "Room_102";
}

void renameRoom(string& name) {
    name = "Room_102_VIP";
}

void testDoublyLinkedList() {
    DoublyLinkedList<string> list;
    
    list.insertBack("Room_101");
    list.insertBack("Room_102");
    list.insertBack("Room_103");
    list.insertFront("Room_100");
 
    assert(list.size() == 4);

    string found;
    assert(list.findIf(isTargetRoom, found) && found == "Room_102");

    assert(list.updateIf(isTargetRoom, renameRoom));
    assert(!list.findIf(isTargetRoom, found));

    cout << "  > Duyet xuoi: ";
    list.traverseForward([](const string& item) { cout << item << " "; });
    cout << "\n  > Duyet nguoc: ";
    list.traverseBackward([](const string& item) { cout << item << " "; });
    cout << endl;

    auto matchVIP = [](const string& s) { return s == "Room_102_VIP"; };
    assert(list.removeIf(matchVIP));
    assert(list.size() == 3);

    DoublyLinkedList<string> copyList = list;
    assert(copyList.size() == list.size());

    logPass("DoublyLinkedList (insertFront/Back, removeIf, findIf, updateIf, copy)");
}

void testBST() {
    BST<Room> tree;
    assert(tree.isEmpty());

    Room r1("P201", 25.0, 1500000.0, "Trong");
    Room r2("P101", 30.0, 1800000.0, "Dang thue");
    Room r3("P301", 35.0, 2000000.0, "Trong");

    tree.insert("P201", r1);
    tree.insert("P101", r2);
    tree.insert("P301", r3);
    assert(!tree.isEmpty());

    Room result;
    assert(tree.search("P101", result));
    assert(result.getMaPhong() == "P101");
    assert(!tree.search("P999", result));

    tree.remove("P201");
    assert(!tree.search("P201", result));
    assert(tree.search("P301", result));

    logPass("BST (insert, search, remove, destructor)");
}

int main() {
    cout << "=== BAT DAU KIEM TRA CAC CAU TRUC DU LIEU ===" << endl;

    testStack();
    testQueue();
    testDoublyLinkedList();
    testBST();

    cout << "\n>>> TAT CA TEST DEU THANH CONG! <<<" << endl;
    return 0;
}
