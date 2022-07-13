#ifndef __TESTDLIB_H
#define __TESTDLIB_H

typedef enum test 
{
	PASS = 40,
	FAIL
} test;

typedef struct Overall
{
    int m_passed;
    int m_failed;
} Overall;

/**
 * @brief recives test enum, prints test result and keeps count
 * 
 * @param _result 
 * @return Overall 
 */
Overall PrintTestResult(test _result);

void TestHeap(void);

void TestBST(void);

void TestList(void);

void TestQueue(void);

void TestVector(void);

void TestHash(void);

#endif /* __TESTDLIB_H */