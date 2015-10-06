//#include "betaheap.h"
#pragma once

#include <vector>
#include <random>
#include "time.h"

using std::vector;
using std::pair;

class BasicTree;

class iUnionHeap;
class TestGenerator;

class BinomialHeap;
class LeftistHeap;
class SkewHeap;


class MyErrors
{
public:
	std::string Message(){}
};


class MyBadCastEXC : public MyErrors
{
public:
	std::string Message();
};

class DifferentSizeMeldedTrees : public MyErrors
{
public:
	std::string Message();
};

class BadIndexEXC : public MyErrors
{
public:
	std::string Message();
};

class BadTestWork : public MyErrors
{
public:
	std::string Message();
};


enum ScenarioAction
{
	addHeap,
	insert,
	extractMin,
	meld
};

//void RandomTestGen(size_t quantityOfSteps);
//void meld(size_t index1, size_t index2);




class TestGenerator
{
public:
	TestGenerator();
	
	size_t currentSize;

	size_t cntERRORS;
	size_t cntWARNINGS;
	
	double binomialHeapWorkTime;
	double leftistHeapWorkTime;
	double skewHeapWorkTime;

	vector <BinomialHeap*> BinomialHeapDataVector;
	vector <LeftistHeap*> LeftistHeapDataVector;
	vector <SkewHeap*> SkewHeapDataVector;

	vector < std::pair <ScenarioAction, std::pair <int,int> > > RealGeneratedScenario;

	void BuildRandomTest(size_t quantityOfSteps);
	void RunRealTest();
	void TestLogTime();

	void ExtractMin(size_t index);
	void Meld(size_t index1, size_t index2);
	void AddedHeap(int key);
	void Insert(size_t index, int key);

	~TestGenerator(){}
};

class BasicTree
{
public:
	BasicTree(){}
	BasicTree(int key);
	BasicTree(int key, size_t currentDegree);
	~BasicTree(){}

	int data;
	BasicTree* right = NULL;
	BasicTree* child = NULL;
	size_t degree;//in leftish heap it's rank

	void PrintCharacters();
	void PrintTree();
};

class iMeldableHeap
{
public:
	iMeldableHeap(){}
	
	virtual void Insert(int key) = 0;
	virtual int ExtractMin() = 0;
	virtual void Meld(iMeldableHeap* addedHeap) = 0;

	//virtual BasicTree* meldSimilar(BasicTree* tree1, BasicTree* tree2) = 0;
	//virtual int getMin() = 0;

	virtual ~iMeldableHeap(){}
};

class BinomialHeap : public iMeldableHeap
{
private:
	BasicTree* meldSimilar(BasicTree* tree1, BasicTree* tree2);
public:
	BinomialHeap(){}
	BinomialHeap(int key);
	
	std::vector <BasicTree*> vectorHeapData;

	void Insert(int key);
	int ExtractMin();
	void Meld(iMeldableHeap* addedHeap);

	void printHeap();
	int getMin();
	
	~BinomialHeap(){}
};

class LeftistHeap : public iMeldableHeap
{
private:
	BasicTree* meldSimilar(BasicTree* tree1, BasicTree* tree2);

	inline BasicTree* correctRight(BasicTree* tree0);
	inline size_t LeftistHeap::calcRank(BasicTree* tree0);
public:
	BasicTree* mainTreeData;

	LeftistHeap(){}
	LeftistHeap(int key);

	void Insert(int key);
	int ExtractMin();
	void Meld(iMeldableHeap* addedHeap);

	void printHeap();
	int getMin();

	~LeftistHeap(){}
};

class SkewHeap : public LeftistHeap
{
private:
	BasicTree* meldSimilar(BasicTree* tree1, BasicTree* tree2);
public:
	SkewHeap(){}
	SkewHeap(int key);

	void Meld(iMeldableHeap* addedHeap);

	~SkewHeap(){}
};
