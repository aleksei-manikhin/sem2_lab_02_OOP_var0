#ifndef TEST_H
#define TEST_H

#include <string>

class ListTests
{
public:
    int runAll();

private:
    int passedCount = 0;
    int failedCount = 0;

    void runTest(const std::string& testName, void (ListTests::*testFunction)());
    static void requireCondition(bool condition, const std::string& message);

    static int intComparatorAsc(const int& left, const int& right);

    void testListBasicOperations();
    void testListRangeCombineAndIndex();
    void testListSelfRangeAndArrayRange();
    void testListRemoveAndSort();
    void testListCopyMoveSemantics();
    void testListExceptions();
    void testIteratorTraversalAndAccess();
    void testIteratorCompareAndExceptions();
};

#endif // TEST_H
