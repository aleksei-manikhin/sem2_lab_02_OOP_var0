#include "test.h"

#include <iostream>
#include <stdexcept>
#include <utility>

#include "iterator.h"

void ListTests::runTest(const std::string& testName, void (ListTests::*testFunction)())
{
    try {
        (this->*testFunction)();
        ++passedCount;
        std::cout << "[OK] " << testName << std::endl;
    } catch (const std::exception& exception) {
        ++failedCount;
        std::cout << "[FAIL] " << testName << ": " << exception.what() << std::endl;
    }
}

void ListTests::requireCondition(bool condition, const std::string& message)
{
    if (!condition) {
        throw std::runtime_error(message);
    }
}

int ListTests::intComparatorAsc(const int& left, const int& right)
{
    if (left < right) {
        return -1;
    }
    if (left > right) {
        return 1;
    }
    return 0;
}

void ListTests::testListBasicOperations()
{
    List<int> lst;
    requireCondition(lst.getLength() == 0, "default constructor failed");

    lst.add(10);
    lst.add(20);
    requireCondition(lst.getLength() == 2, "add failed");
    requireCondition(lst.getElem(0) == 10, "getElem failed");
    requireCondition(lst[1] == 20, "operator[] failed");

    lst.setElem(1, 25);
    requireCondition(lst.getElem(1) == 25, "setElem failed");
}

void ListTests::testListRangeCombineAndIndex()
{
    List<int> a{1, 2};
    List<int> b{3, 4};

    a.addRange(b);
    requireCondition(a.getLength() == 4, "addRange(list) failed");
    requireCondition(a[0] == 1 && a[1] == 2 && a[2] == 3 && a[3] == 4, "addRange(list) content failed");

    requireCondition(a.getIndex(3) == 2, "getIndex existing failed");
    requireCondition(a.getIndex(100) == -1, "getIndex missing failed");

    List<int> extra{5};
    List<int> combined = a.combine(extra);
    requireCondition(combined.getLength() == 5, "combine length failed");
    requireCondition(combined[4] == 5, "combine content failed");
    requireCondition(a.getLength() == 4, "combine changed source list");
}

void ListTests::testListSelfRangeAndArrayRange()
{
    List<int> self{7, 8};
    self.addRange(self);
    requireCondition(self.getLength() == 4, "addRange(self) length failed");
    requireCondition(self[0] == 7 && self[1] == 8 && self[2] == 7 && self[3] == 8, "addRange(self) content failed");

    int arr[] = {9, 10, 11};
    List<int> lst;
    lst.addRange(arr, 3);
    requireCondition(lst.getLength() == 3, "addRange(array) length failed");
    requireCondition(lst[0] == 9 && lst[1] == 10 && lst[2] == 11, "addRange(array) content failed");

    int* values = lst.toArray();
    requireCondition(values != nullptr, "toArray non-empty failed");
    requireCondition(values[0] == 9 && values[1] == 10 && values[2] == 11, "toArray content failed");
    delete[] values;

    List<int> empty;
    requireCondition(empty.toArray() == nullptr, "toArray empty failed");
}

void ListTests::testListRemoveAndSort()
{
    List<int> lst{4, 1, 3, 2};
    lst.sort(intComparatorAsc);
    requireCondition(lst[0] == 1 && lst[1] == 2 && lst[2] == 3 && lst[3] == 4, "sort failed");

    lst.removeElem(1);
    requireCondition(lst.getLength() == 3, "removeElem middle length failed");
    requireCondition(lst[0] == 1 && lst[1] == 3 && lst[2] == 4, "removeElem middle content failed");

    lst.removeElem(0);
    requireCondition(lst[0] == 3 && lst[1] == 4, "removeElem head failed");

    lst.removeElem(1);
    requireCondition(lst.getLength() == 1 && lst[0] == 3, "removeElem tail failed");
}

void ListTests::testListCopyMoveSemantics()
{
    List<int> source{1, 2, 3};

    List<int> copied(source);
    requireCondition(copied.getLength() == 3, "copy constructor length failed");
    requireCondition(copied[0] == 1 && copied[1] == 2 && copied[2] == 3, "copy constructor content failed");
    copied.setElem(0, 99);
    requireCondition(source[0] == 1, "copy constructor deep copy failed");

    List<int> moved(std::move(source));
    requireCondition(moved.getLength() == 3, "move constructor length failed");
    requireCondition(moved[0] == 1 && moved[1] == 2 && moved[2] == 3, "move constructor content failed");
    requireCondition(source.getLength() == 0, "move constructor source state failed");

    List<int> copyAssigned{42};
    copyAssigned = moved;
    requireCondition(copyAssigned.getLength() == 3, "copy assignment length failed");
    requireCondition(copyAssigned[0] == 1 && copyAssigned[1] == 2 && copyAssigned[2] == 3, "copy assignment content failed");
    copyAssigned.setElem(1, 77);
    requireCondition(moved[1] == 2, "copy assignment deep copy failed");

    List<int> moveAssigned{5};
    moveAssigned = std::move(moved);
    requireCondition(moveAssigned.getLength() == 3, "move assignment length failed");
    requireCondition(moveAssigned[0] == 1 && moveAssigned[1] == 2 && moveAssigned[2] == 3, "move assignment content failed");
    requireCondition(moved.getLength() == 0, "move assignment source state failed");
}

void ListTests::testListExceptions()
{
    List<int> lst;
    int arr[] = {1, 2, 3};

    bool caughtInvalidSize = false;
    bool caughtNullArray = false;
    bool caughtNullComparator = false;
    bool caughtOutGet = false;
    bool caughtOutSet = false;
    bool caughtOutRemove = false;
    bool caughtOutIndexOp = false;

    try {
        lst.addRange(arr, -1);
    } catch (const ListInvalidArgument&) {
        caughtInvalidSize = true;
    }

    try {
        lst.addRange(nullptr, 1);
    } catch (const ListNullPointer&) {
        caughtNullArray = true;
    }

    try {
        lst.sort(nullptr);
    } catch (const ListNullPointer&) {
        caughtNullComparator = true;
    }

    try {
        lst.getElem(0);
    } catch (const ListIndexOutOfRange&) {
        caughtOutGet = true;
    }

    try {
        lst.setElem(-1, 0);
    } catch (const ListIndexOutOfRange&) {
        caughtOutSet = true;
    }

    try {
        lst.removeElem(0);
    } catch (const ListIndexOutOfRange&) {
        caughtOutRemove = true;
    }

    try {
        (void)lst[5];
    } catch (const ListIndexOutOfRange&) {
        caughtOutIndexOp = true;
    }

    requireCondition(caughtInvalidSize, "no exception for negative size");
    requireCondition(caughtNullArray, "no exception for null array pointer");
    requireCondition(caughtNullComparator, "no exception for null comparator");
    requireCondition(caughtOutGet, "no exception for getElem out of range");
    requireCondition(caughtOutSet, "no exception for setElem out of range");
    requireCondition(caughtOutRemove, "no exception for removeElem out of range");
    requireCondition(caughtOutIndexOp, "no exception for operator[] out of range");

    lst.addRange(nullptr, 0);
    requireCondition(lst.getLength() == 0, "addRange(nullptr, 0) failed");
}

void ListTests::testIteratorTraversalAndAccess()
{
    List<int> lst{10, 20};
    Iterator<int> it(lst);

    requireCondition(!it.is_end(), "iterator begin should not be end");
    requireCondition(it.value() == 10, "iterator value failed");
    requireCondition(*it == 10, "iterator dereference failed");

    *it = 15;
    requireCondition(lst[0] == 15, "iterator write through * failed");

    ++it;
    requireCondition(it.value() == 20, "iterator ++ failed");

    it.next();
    requireCondition(it.is_end(), "iterator next to end failed");
}

void ListTests::testIteratorCompareAndExceptions()
{
    List<int> lst{1, 2};
    Iterator<int> a(lst);
    Iterator<int> b(lst);

    requireCondition(a == b, "iterator == failed");
    requireCondition(!(a != b), "iterator != failed for equal iterators");

    b.next();
    requireCondition(a != b, "iterator != failed for different positions");

    List<int> other{1, 2};
    Iterator<int> c(other);
    requireCondition(a != c, "iterators from different lists should not be equal");

    Iterator<int> endIt(lst);
    endIt.next().next();
    requireCondition(endIt.is_end(), "iterator should be end");

    bool caughtNext = false;
    bool caughtValue = false;
    bool caughtDeref = false;

    try {
        endIt.next();
    } catch (const ListIteratorOutOfRange&) {
        caughtNext = true;
    }

    try {
        (void)endIt.value();
    } catch (const ListIteratorOutOfRange&) {
        caughtValue = true;
    }

    try {
        (void)(*endIt);
    } catch (const ListIteratorOutOfRange&) {
        caughtDeref = true;
    }

    requireCondition(caughtNext, "no exception for iterator.next() at end");
    requireCondition(caughtValue, "no exception for iterator.value() at end");
    requireCondition(caughtDeref, "no exception for iterator* at end");
}

int ListTests::runAll()
{
    passedCount = 0;
    failedCount = 0;

    runTest("list basic operations", &ListTests::testListBasicOperations);
    runTest("list range/combine/index", &ListTests::testListRangeCombineAndIndex);
    runTest("list self-range/array-range/toArray", &ListTests::testListSelfRangeAndArrayRange);
    runTest("list remove/sort", &ListTests::testListRemoveAndSort);
    runTest("list copy/move semantics", &ListTests::testListCopyMoveSemantics);
    runTest("list exceptions", &ListTests::testListExceptions);
    runTest("iterator traversal/access", &ListTests::testIteratorTraversalAndAccess);
    runTest("iterator compare/exceptions", &ListTests::testIteratorCompareAndExceptions);

    std::cout << "Passed: " << passedCount << ", Failed: " << failedCount << std::endl;
    return failedCount != 0;
}
