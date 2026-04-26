#include "test.h"

#include <iostream>
#include <stdexcept>
#include <utility>

#include "iterator.h"

void ListTests::requireCondition(bool condition, const std::string& message)
{
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void ListTests::runTest(const std::string& testName, void (ListTests::*testFunction)())
{
    try {
        (this->*testFunction)();
        ++passedCount;
        std::cout << "[OK] " << testName << std::endl;
    } catch (const std::exception& exception) {
        ++failedCount;
        std::cout << "[FAIL] " << testName << ": " << exception.what() << std::endl;
    } catch (...) {
        ++failedCount;
        std::cout << "[FAIL] " << testName << ": unknown error" << std::endl;
    }
}

int ListTests::intComparatorAsc(const int& left, const int& right)
{
    return (left > right) - (left < right);
}

void ListTests::testListBasicOperations()
{
    // Тест методов: List(initializer_list), List(), add, getLength, getElem, setElem, operator[], getIndex.
    List<int> initListValue{1, 2, 3};
    requireCondition(initListValue.getLength() == 3, "initializer_list constructor length failed");
    requireCondition(initListValue[0] == 1 && initListValue[1] == 2 && initListValue[2] == 3,
                     "initializer_list constructor content failed");

    List<int> listValue;
    requireCondition(listValue.getLength() == 0, "default constructor failed");
    listValue.add(10);
    listValue.add(20);
    requireCondition(listValue.getLength() == 2, "add/getLength failed");
    requireCondition(listValue.getElem(0) == 10, "getElem failed");
    requireCondition(listValue[1] == 20, "operator[] failed");
    listValue.setElem(1, 25);
    requireCondition(listValue.getElem(1) == 25, "setElem failed");
    requireCondition(listValue.getIndex(10) == 0, "getIndex existing failed");
    requireCondition(listValue.getIndex(100) == -1, "getIndex missing failed");
}

void ListTests::testListAddRangeAndCombine()
{
    // Тест методов: addRange(const List&), addRange(T*, int), combine(const List&).
    List<int> first{1, 2};
    List<int> second{3, 4};
    first.addRange(second);
    requireCondition(first.getLength() == 4, "addRange(list) length failed");
    requireCondition(first[0] == 1 && first[1] == 2 && first[2] == 3 && first[3] == 4, "addRange(list) content failed");

    int arr[] = {5, 6};
    first.addRange(arr, 2);
    requireCondition(first.getLength() == 6, "addRange(array) length failed");
    requireCondition(first[4] == 5 && first[5] == 6, "addRange(array) content failed");

    List<int> combined = first.combine(List<int>{9});
    requireCondition(combined.getLength() == 7, "combine length failed");
    requireCondition(combined[0] == 1 && combined[5] == 6 && combined[6] == 9, "combine content failed");
    requireCondition(first.getLength() == 6 && first[5] == 6, "combine changed source list");
    requireCondition(second.getLength() == 2 && second[0] == 3 && second[1] == 4, "addRange(list) modified source");
}

void ListTests::testListSelfAddRange()
{
    // Тест методов: addRange(const List&) with self source.
    List<int> self{7, 8};
    self.addRange(self);
    requireCondition(self.getLength() == 4, "addRange(self) length failed");
    requireCondition(self[0] == 7 && self[1] == 8 && self[2] == 7 && self[3] == 8, "addRange(self) content failed");
}

void ListTests::testListRemoveAndSort()
{
    // Тест методов: sort(comp), removeElem(int), operator[].
    List<int> values{4, 1, 3, 2};
    values.sort(intComparatorAsc);
    requireCondition(values[0] == 1 && values[1] == 2 && values[2] == 3 && values[3] == 4, "sort failed");

    values.removeElem(1);
    requireCondition(values.getLength() == 3, "removeElem middle length failed");
    requireCondition(values[0] == 1 && values[1] == 3 && values[2] == 4, "removeElem middle content failed");

    values.removeElem(0);
    requireCondition(values.getLength() == 2 && values[0] == 3 && values[1] == 4, "removeElem head failed");

    values.removeElem(1);
    requireCondition(values.getLength() == 1 && values[0] == 3, "removeElem tail failed");
}

void ListTests::testListCopySemantic()
{
    //Тест методов: copy ctor, operator=(const List&).
    List<int> source{1, 2, 3};
    List<int> copied(source);
    requireCondition(copied.getLength() == 3 && copied[0] == 1 && copied[2] == 3, "copy constructor failed");
    copied.setElem(0, 99);
    requireCondition(source[0] == 1, "copy constructor must deep copy");

    List<int> copyAssigned{42};
    copyAssigned = copied;
    requireCondition(copyAssigned.getLength() == 3 && copyAssigned[1] == 2, "copy assignment failed");
    copyAssigned.setElem(1, 77);
    requireCondition(copied[1] == 2, "copy assignment must deep copy");
}

void ListTests::testListMoveSemantic()
{
    //Тест методов: move ctor, operator=(List&&), toArray().
    List<int> source{1, 2, 3};
    List<int> moved(std::move(source));
    requireCondition(moved.getLength() == 3 && moved[0] == 1 && moved[2] == 3, "move constructor failed");

    List<int> moveAssigned{5};
    moveAssigned = std::move(moved);
    requireCondition(moveAssigned.getLength() == 3 && moveAssigned[0] == 1 && moveAssigned[2] == 3, "move assignment failed");

    int* values = moveAssigned.toArray();
    requireCondition(values != nullptr, "toArray non-empty failed");
    requireCondition(values[0] == 1 && values[1] == 2 && values[2] == 3, "toArray content failed");
    delete[] values;

    List<int> empty;
    int* emptyValues = empty.toArray();
    requireCondition(emptyValues == nullptr, "toArray empty failed");
}

void ListTests::testListExceptionArguments()
{
    // Тест методов: addRange(T*, int), sort(comp).
    List<int> lst;
    int arr[] = {1};
    bool caughtInvalid = false, caughtNullArray = false, caughtNullComparator = false;
    try { lst.addRange(arr, -1); } catch (const ListInvalidArgument&) { caughtInvalid = true; } catch (...) {}
    try { lst.addRange(nullptr, 1); } catch (const ListNullPointer&) { caughtNullArray = true; } catch (...) {}
    try { lst.sort(nullptr); } catch (const ListNullPointer&) { caughtNullComparator = true; } catch (...) {}
    requireCondition(caughtInvalid, "no exception for negative size");
    requireCondition(caughtNullArray, "no exception for null array pointer");
    requireCondition(caughtNullComparator, "no exception for null comparator");
    lst.addRange(nullptr, 0);
    requireCondition(lst.getLength() == 0, "addRange(nullptr, 0) failed");
}

void ListTests::testListExceptionIndices()
{
    // Тест методов: getElem, setElem, removeElem, operator[].
    List<int> lst;
    bool caughtGet = false, caughtSet = false, caughtRemove = false, caughtIndex = false;
    try { (void)lst.getElem(0); } catch (const ListIndexOutOfRange&) { caughtGet = true; } catch (...) {}
    try { lst.setElem(-1, 0); } catch (const ListIndexOutOfRange&) { caughtSet = true; } catch (...) {}
    try { lst.removeElem(0); } catch (const ListIndexOutOfRange&) { caughtRemove = true; } catch (...) {}
    try { (void)lst[5]; } catch (const ListIndexOutOfRange&) { caughtIndex = true; } catch (...) {}
    requireCondition(caughtGet, "no exception for getElem out of range");
    requireCondition(caughtSet, "no exception for setElem out of range");
    requireCondition(caughtRemove, "no exception for removeElem out of range");
    requireCondition(caughtIndex, "no exception for operator[] out of range");
}

void ListTests::testIteratorTraversalAndAccess()
{
    // Тест методов: Iterator(List&), is_end(), value(), operator*(), operator++(), next().
    List<int> values{10, 20};
    Iterator<int> it(values);
    requireCondition(!it.is_end(), "iterator begin should not be end");
    requireCondition(it.value() == 10 && *it == 10, "iterator read failed");
    *it = 15;
    requireCondition(values[0] == 15, "iterator write through * failed");
    ++it;
    requireCondition(it.value() == 20, "iterator ++ failed");
    it.next();
    requireCondition(it.is_end(), "iterator next to end failed");
}

void ListTests::testIteratorCompare()
{
    // Тест методов: operator==, operator!=.
    List<int> base{1, 2};
    List<int> other{1, 2};
    Iterator<int> left(base), same(base), shifted(base), foreign(other);
    requireCondition(left == same && !(left != same), "iterator equality failed");
    shifted.next();
    requireCondition(left != shifted, "iterator inequality by position failed");
    requireCondition(left != foreign, "iterators from different lists must differ");
}

void ListTests::testIteratorEndExceptions()
{
    //Тест методов: next(), value(), operator*() at end.
    List<int> base{1, 2};
    Iterator<int> end(base);
    end.next().next();
    bool caughtNext = false, caughtValue = false, caughtDeref = false;
    try { end.next(); } catch (const ListIteratorOutOfRange&) { caughtNext = true; } catch (...) {}
    try { (void)end.value(); } catch (const ListIteratorOutOfRange&) { caughtValue = true; } catch (...) {}
    try { (void)(*end); } catch (const ListIteratorOutOfRange&) { caughtDeref = true; } catch (...) {}
    requireCondition(end.is_end(), "iterator should be at end");
    requireCondition(caughtNext, "no exception for iterator.next() at end");
    requireCondition(caughtValue, "no exception for iterator.value() at end");
    requireCondition(caughtDeref, "no exception for iterator* at end");
}

int ListTests::runAll()
{
    passedCount = 0;
    failedCount = 0;

    runTest("List: basic operations", &ListTests::testListBasicOperations);
    runTest("List: addRange and combine", &ListTests::testListAddRangeAndCombine);
    runTest("List: self addRange", &ListTests::testListSelfAddRange);
    runTest("List: remove and sort", &ListTests::testListRemoveAndSort);
    runTest("List: copy semantics", &ListTests::testListCopySemantic);
    runTest("List: move semantics", &ListTests::testListMoveSemantic);
    runTest("List: invalid arguments", &ListTests::testListExceptionArguments);
    runTest("List: invalid indices", &ListTests::testListExceptionIndices);
    runTest("Iterator: traversal and access", &ListTests::testIteratorTraversalAndAccess);
    runTest("Iterator: comparisons", &ListTests::testIteratorCompare);
    runTest("Iterator: end exceptions", &ListTests::testIteratorEndExceptions);

    std::cout << "Passed: " << passedCount << ", Failed: " << failedCount << std::endl;
    return failedCount != 0;
}
