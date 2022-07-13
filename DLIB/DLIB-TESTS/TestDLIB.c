#include <stdio.h> /* printf, scanf */
#include <stdlib.h> /* atoi */
#include "TestDLIB.h"

typedef enum DataS
{
    ALL = 0,
    HEAP,
    BST,
    LIST,
    QUEUE,
    VEC,
    HASH
} DataS;

void PrintMenu();
void TestCall(DataS _command);

int main(int argc, char const *argv[])
{
    int command;
    Overall recap;
    if (argc == 1)
    {
        PrintMenu();
        scanf("%d", &command);
    } else {
        command = atoi(argv[1]);
    }
    TestCall((DataS)command);
    recap = PrintTestResult(0);
    printf("\n\nOverall:\n%d tests Passed\n%d tests Failed\n\n", recap.m_passed, recap.m_failed);
    return 0;
}

void PrintMenu()
{
printf("\n\nPlease Insert Test Command No.\n"
            "[0] - Test All\n"
            "[1] - Test Heap\n"
            "[2] - Test Binary Search Tree\n"
            "[3] - Test List\n"
            "[4] - Test Queue\n"
            "[5] - Test Vector\n"
            "[6] - Test Hash Map\n"
            "\nInsert Here: ");
}

Overall PrintTestResult(test _result)
{
	static Overall recap;
    if (_result == PASS)
	{
		printf(" PASSED!\n");
        ++recap.m_passed;
	}
	if (_result == FAIL)
	{
		printf(" *** FAILED! ***\n");
        ++recap.m_failed;
	}
    return recap;
}

void TestCall(DataS _command)
{
    switch ((DataS)_command)
    {
    case ALL:
    default:
        _command = ALL;
    case HEAP:
        printf("\n\nHeap Tests:\n\n");
        TestHeap();
        if (_command) {break;}
    case BST:
        printf("\n\nBinary Search Tree Tests:\n\n");
        TestBST();
        if (_command) {break;}
    case LIST:
        printf("\n\nList Tests:\n\n");
        TestList();
        if (_command) {break;}
    case QUEUE:
        printf("\n\nQueue Tests:\n\n");
        TestQueue();
        if (_command) {break;}
    case VEC:
        printf("\n\nVector Tests:\n\n");
        TestVector();
        if (_command) {break;}
    case HASH:
        printf("\n\nHash Tests:\n\n");
        TestHash();
        if (_command) {break;}
    }
}