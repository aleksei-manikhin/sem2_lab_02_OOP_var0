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
    void testListAddRangeAndCombine();
    void testListSelfAddRange();
    void testListRemoveAndSort();
    void testListCopySemantic();
    void testListMoveSemantic();
    void testListExceptionArguments();
    void testListExceptionIndices();
    void testIteratorTraversalAndAccess();
    void testIteratorCompare();
    void testIteratorEndExceptions();
};

#endif // TEST_H
