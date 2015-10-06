#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <Windows.h>
#include <random>
#include <time.h>
#include <cmath>
#include <typeinfo>
#include "betaheap.h"
//using namespace std;
using std::vector;
using std::cout;
using std::endl;

//Exceptions

std::string MyBadCastEXC::Message()
{
	return "ERROR: Meldable heap has uncorrect type";
}

std::string DifferentSizeMeldedTrees::Message()
{
	return "ERROR: in MeldSimilar: Meldable trees have a different degree";
}

std::string BadIndexEXC::Message()
{
	return "WARNING: one or more index has bad number";
}

std::string BadTestWork::Message()
{
	return "WARNING: proram give wrong answer";
}


// Test Generator

TestGenerator::TestGenerator()
{
	RealGeneratedScenario.clear();

	BinomialHeapDataVector.clear();
	LeftistHeapDataVector.clear();
	SkewHeapDataVector.clear();

	binomialHeapWorkTime = 0;
	leftistHeapWorkTime = 0;
	skewHeapWorkTime = 0;

	currentSize = 0;

	cntERRORS = 0;
	cntWARNINGS = 0;
}

//technical part

void TestGenerator::BuildRandomTest(size_t quantityOfSteps)
{
	vector <size_t> watchingCnt;
	watchingCnt.clear();
	RealGeneratedScenario.clear();
	srand(time(NULL));
	size_t quantityOfHeaps = 0;
	std::default_random_engine generator;
	std::uniform_int_distribution <size_t> distributionAction(0, 3);
	std::uniform_int_distribution <int> distributionKey(0, INT_MAX);
	for (size_t i = 0; i < quantityOfSteps; ++i)
	{
		if (quantityOfHeaps == 0)
		{
			++quantityOfHeaps;
			int key = distributionKey(generator);
			RealGeneratedScenario.push_back(pair < ScenarioAction, pair<int, int> >(addHeap, pair<int, int>(key, 0)));
			continue;
		}	
		size_t realRandomIndex = rand() % RealGeneratedScenario.size();
		switch (distributionAction(generator))
		{
		case 0:
			++quantityOfHeaps;
			RealGeneratedScenario.push_back(pair < ScenarioAction, pair<int, int> >(addHeap, pair <int, int>(distributionKey(generator), 0)));
			break;
		case 1:
			RealGeneratedScenario.push_back(pair < ScenarioAction, pair<int, int> >(insert, pair <int, int>(distributionKey(generator), rand() % max(1,quantityOfHeaps))));
			break;
		case 2:
			RealGeneratedScenario.push_back(pair < ScenarioAction, pair<int, int> >(extractMin, pair <int, int>(rand() % max(1,quantityOfHeaps),0)));
			break;
		case 3:
			--quantityOfHeaps;
			RealGeneratedScenario.push_back(pair < ScenarioAction, pair<int, int> >(meld, pair <int, int>(rand() % max(1,quantityOfHeaps), rand() % max(1,quantityOfHeaps))));
			break;
		}
	}
}

void TestGenerator::RunRealTest()
{
//	std::ofstream cout("output.txt");
	for (size_t i = 0; i < RealGeneratedScenario.size(); ++i)
	{
		switch (RealGeneratedScenario[i].first)
		{
		case addHeap:
			AddedHeap(RealGeneratedScenario[i].second.first);
	//		cout << "OK: AddHeap, key: " << RealGeneratedScenario[i].second.first << endl;
			break;
		case insert:	
			Insert(RealGeneratedScenario[i].second.second, RealGeneratedScenario[i].second.first);
	//		cout << "OK: Insert, index: " << RealGeneratedScenario[i].second.second << " key: " << RealGeneratedScenario[i].second.first << endl;
			break;
		case extractMin:
			ExtractMin(RealGeneratedScenario[i].second.first);
	//		cout << "OK: ExtractMin, index: " << RealGeneratedScenario[i].second.first << endl;
			break;
		case meld:
			Meld(RealGeneratedScenario[i].second.first, RealGeneratedScenario[i].second.second);
	//		cout << "OK: Meld, index1: " << RealGeneratedScenario[i].second.first << " index2: " << RealGeneratedScenario[i].second.second << endl;
			break;
		}	
	}
	//cout.close();
	cout << endl;
	cout << "total quantity of warnings: " << cntWARNINGS << endl;
	cout << "total quantity of errors: " << cntERRORS << endl;
	cout << "BinomialHeapWorkTime: " << binomialHeapWorkTime/1000 << "s" << endl;
	cout << "LeftistHeapWorkTime: " << leftistHeapWorkTime/1000 << "s" << endl;
	cout << "SkewHeapWorkTime: " << skewHeapWorkTime/1000  << "s" << endl;
}

void TestGenerator::TestLogTime()
{
	size_t cnt = 0;
	size_t currentSteps = 1;

	vector <double> TimesBinomial;
	vector <double> TimesLeftist;
	vector <double> TimesSkew;

	TimesBinomial.clear();
	TimesLeftist.clear();
	TimesSkew.clear();

	while (currentSteps < 131073)//131072 = 2^17
	{
		cout << "test # " << cnt << " with " << currentSteps << " events:" << endl;

		BuildRandomTest(currentSteps);
		RunRealTest();

		TimesBinomial.push_back(binomialHeapWorkTime);
		TimesLeftist.push_back(leftistHeapWorkTime);
		TimesSkew.push_back(skewHeapWorkTime);

		++cnt;
		currentSteps *= 2;
	}
	cout << "ALL_TIMES:" << endl;

	cout << "	BINOMIAL_HEAP_TIME:" << endl;
	cout << "events:	time:" << endl;
	for (size_t i = 0; i < TimesBinomial.size(); ++i)
	{
		cout << (1 << i) << "	" << TimesBinomial[i] << endl;
	}
	cout << endl;

	cout << "	LEFTIST_HEAP_TIME:" << endl;
	cout << "events:	time:" << endl;
	for (size_t i = 0; i < TimesLeftist.size(); ++i)
	{
		cout << (1 << i) << "	 " << TimesLeftist[i] << endl;
	}
	cout << endl;

	cout << "	SKEW_HEAP_TIME:" << endl;
	cout << "events:	time:" << endl;
	for (size_t i = 0; i < TimesSkew.size(); ++i)
	{
		cout << (1 << i) << "	 " << TimesSkew[i] << endl;
	}
	cout << endl;
}

//standart part

void TestGenerator::AddedHeap(int key)
{
	double time0 = clock();
	BinomialHeapDataVector.push_back(new BinomialHeap(key));
	binomialHeapWorkTime += (clock() - time0);

	time0 = clock();
	LeftistHeapDataVector.push_back(new LeftistHeap(key));
	leftistHeapWorkTime += (clock() - time0);

	time0 = clock();
	SkewHeapDataVector.push_back(new SkewHeap(key));
	skewHeapWorkTime += (clock() - time0);

	++currentSize;
	//cout << "currentSize: " << currentSize << endl;
}

void TestGenerator::Insert(size_t index, int key)
{
	if (index < currentSize)
	{
		double time0 = clock();
		BinomialHeapDataVector[index]->Meld(new BinomialHeap(key));
		binomialHeapWorkTime += (clock() - time0);

		time0 = clock();
		LeftistHeapDataVector[index]->Meld(new LeftistHeap(key));
		leftistHeapWorkTime += (clock() - time0);

		time0 = clock();
		SkewHeapDataVector[index]->Meld(new SkewHeap(key));
		skewHeapWorkTime += (clock() - time0);
	}
	else
	{
		++cntWARNINGS;
		throw BadIndexEXC();
	}
}

void TestGenerator::Meld(size_t index1, size_t index2)
{
	if (index1 < currentSize && index2 < currentSize && index1 != index2)
	{
		double time0 = clock();
		BinomialHeapDataVector[index1]->Meld(BinomialHeapDataVector[index2]);
		binomialHeapWorkTime += (clock() - time0);
		BinomialHeapDataVector.erase(BinomialHeapDataVector.begin()+index2);

		time0 = clock();
		LeftistHeapDataVector[index1]->Meld(LeftistHeapDataVector[index2]);
		leftistHeapWorkTime += (clock() - time0);
		LeftistHeapDataVector.erase(LeftistHeapDataVector.begin() + index2);

		time0 = clock();
		SkewHeapDataVector[index1]->Meld(SkewHeapDataVector[index2]);
		skewHeapWorkTime += (clock() - time0);
		SkewHeapDataVector.erase(SkewHeapDataVector.begin() + index2);

		--currentSize;
	}
	else
	{
		++cntWARNINGS;
		throw BadIndexEXC();
	}
}

void TestGenerator::ExtractMin(size_t index)
{
	//cout << "currentSize: " << currentSize << endl;
	if (index < currentSize)
	{
		double time0 = clock();
		int currentBinomialHeapMin = BinomialHeapDataVector[index]->ExtractMin();
		binomialHeapWorkTime += (clock() - time0);

		//BinomialHeapDataVector[index]->printHeap();
		if (BinomialHeapDataVector[index]->vectorHeapData.size() == 0)
		{
			BinomialHeapDataVector.erase(BinomialHeapDataVector.begin() + index);
			--currentSize;
		}

		time0 = clock();
		int currentLeftistHeapMin =	LeftistHeapDataVector[index]->ExtractMin();
		leftistHeapWorkTime += (clock() - time0);

		if (LeftistHeapDataVector[index]->mainTreeData == NULL)
		{
			LeftistHeapDataVector.erase(LeftistHeapDataVector.begin() + index);
		}

		time0 = clock();
		int currentSkewHeapMin = SkewHeapDataVector[index]->ExtractMin();
		skewHeapWorkTime += (clock() - time0);

		if (SkewHeapDataVector[index]->mainTreeData == NULL)
		{
			SkewHeapDataVector.erase(SkewHeapDataVector.begin() + index);
		}

		if (currentBinomialHeapMin != currentLeftistHeapMin || currentBinomialHeapMin != currentSkewHeapMin)
		{
			++cntERRORS;
			throw BadTestWork();
		}
	}
	else
	{
		++cntWARNINGS;
		throw BadIndexEXC();
	}
}

//Basic Tree

BasicTree::BasicTree(int key)
{
	data = key;
	child = NULL;
	right = NULL;
	degree = 0;
}

BasicTree::BasicTree(int key,size_t currentDegree)
{
	data = key;
	child = NULL;
	right = NULL;
	degree = currentDegree;
}

void BasicTree::PrintCharacters()
{
	cout << "data: " << data << endl;
	if (child != NULL)
	{
		cout << "data in child: " << child->data << endl;
	}
	else
	{
		cout << "childs don't exist" << endl;
	}

	if (right != NULL)
	{
		cout << "data in right" << right->data << endl;
	}
	else
	{
		cout << "right brothers don't exist" << endl;
	}

	cout << "degree: " << degree << endl;
	cout << endl;
}

void BasicTree::PrintTree()
{
	if (this == NULL)
	{
		return;
	}
	for (int i = 0; i < 2*degree; ++i)
		cout << " ";
	cout << data << " " << degree << "	";
	if ( right != NULL ) right->PrintTree();
	cout << endl;
	if ( child != NULL ) child->PrintTree();
}

//Binomial Heap 

int BinomialHeap::getMin()
{
	int minKey = INT_MAX;
	vector <BasicTree*> ::iterator it = vectorHeapData.begin();
	while (it != vectorHeapData.end())
	{
		minKey = min((*it)->data, minKey);
		++it;
	}
	return minKey;
}

BasicTree* BinomialHeap::meldSimilar(BasicTree* tree1, BasicTree* tree2)
{
	if (tree1 == NULL) return tree2;
	if (tree2 == NULL) return tree1;
	if (tree1->degree != tree2->degree)
	{
		throw DifferentSizeMeldedTrees();
	}
	//cout << "current_degree: " << tree1->degree << endl;
	if (tree1->data > tree2->data)
	{
		std::swap(tree1, tree2);
	}
	tree2->right = tree1->child;
	tree1->child = tree2;
	++tree1->degree;
	tree2->~BasicTree();
	return tree1;
}

void BinomialHeap::Meld(iMeldableHeap* addedHeapExc)
{
	if (addedHeapExc == NULL)
	{
		return;
	}
	BinomialHeap* addedHeap = dynamic_cast <BinomialHeap*> (addedHeapExc);
	if (addedHeap == NULL)
	{
		throw MyBadCastEXC();
	}
	
	
	vector <BasicTree*> bufferHeapData = this->vectorHeapData;
	this->vectorHeapData.clear();
	vector <BasicTree*>::iterator firstHeapIt = bufferHeapData.begin();
	vector <BasicTree*>::iterator secondHeapIt = addedHeap->vectorHeapData.begin();
	BasicTree* currentTreeRadical = NULL;
	bool frangeCondition;
	while (!(firstHeapIt == bufferHeapData.end() && secondHeapIt == addedHeap->vectorHeapData.end()))
	{
		frangeCondition = false;
		if (firstHeapIt == bufferHeapData.end())
		{
			frangeCondition = true;
		}

		if (frangeCondition == false && (secondHeapIt == addedHeap->vectorHeapData.end() || (*firstHeapIt)->degree < (*secondHeapIt)->degree))
		{
			if (currentTreeRadical == NULL || currentTreeRadical->degree < (*firstHeapIt)->degree)
			{
				if (currentTreeRadical != NULL)
				{
					this->vectorHeapData.push_back(currentTreeRadical);
					currentTreeRadical = NULL;
				}
				this->vectorHeapData.push_back(*firstHeapIt);
			}
			else
			{
				currentTreeRadical = meldSimilar(currentTreeRadical, *firstHeapIt);
			}
			++firstHeapIt;
			continue;
		}
		if (firstHeapIt == bufferHeapData.end() || (*firstHeapIt)->degree != (*secondHeapIt)->degree)
		{
			if (currentTreeRadical == NULL || currentTreeRadical->degree < (*secondHeapIt)->degree)
			{
				if (currentTreeRadical != NULL)
				{
					this->vectorHeapData.push_back(currentTreeRadical);
					currentTreeRadical = NULL;
				}
				this->vectorHeapData.push_back(*secondHeapIt);
			}
			else
			{
				currentTreeRadical = meldSimilar(currentTreeRadical, *secondHeapIt);
			}
			++secondHeapIt;
			continue;
		}
		if (currentTreeRadical != NULL)
		{
			this->vectorHeapData.push_back(currentTreeRadical);
		}
		currentTreeRadical = meldSimilar(*firstHeapIt, *secondHeapIt);
		++firstHeapIt;
		++secondHeapIt;
	}
	if (currentTreeRadical != NULL)
	{
		this->vectorHeapData.push_back(currentTreeRadical);
	}
}
	
BinomialHeap::BinomialHeap(int key)
{
	this->vectorHeapData.push_back(new BasicTree(key));
}

void BinomialHeap::Insert(int key)
{
	BinomialHeap heapDef(key);
	Meld(&heapDef);
}

int BinomialHeap::ExtractMin()
{
	int minKey = INT_MAX;
	vector <BasicTree*> ::iterator it = vectorHeapData.begin();
	int minTreeNumb = -1;
	while (it != vectorHeapData.end())
	{
		if ((*it)->data < minKey)
		{
			minKey = (*it)->data;
			minTreeNumb = it - vectorHeapData.begin();
		}
	++it;
	}
	
	if (minTreeNumb == -1)
	{
		return 0;
	}
	BinomialHeap addedHeapWithoutExtractedMin;
	addedHeapWithoutExtractedMin.vectorHeapData.clear();
	BasicTree* minTreeIt = vectorHeapData[minTreeNumb];
	if (minTreeIt->child != NULL)
	{
		minTreeIt = minTreeIt->child;
		while (minTreeIt != NULL)
		{
			//cout << minTreeIt->child << endl;
			BasicTree* buffer = new BasicTree;
			buffer = minTreeIt->right;
			minTreeIt->right = NULL;
			addedHeapWithoutExtractedMin.vectorHeapData.push_back(minTreeIt);
			minTreeIt = buffer;
		}
		std::reverse(addedHeapWithoutExtractedMin.vectorHeapData.begin(), addedHeapWithoutExtractedMin.vectorHeapData.end());
		//for (int i = 0; i < addedHeapWithoutExtractedMin.vectorHeapData.size(); ++i)
		//	addedHeapWithoutExtractedMin.vectorHeapData[i]->PrintTree();
	}
	vectorHeapData.erase(vectorHeapData.begin()+minTreeNumb);
	Meld(&addedHeapWithoutExtractedMin);
	
	return minKey;
}

void BinomialHeap::printHeap()
{
	cout << "PRINT_HEAP: " << typeid(*this).name() << endl;
	for (int i = 0; i < this->vectorHeapData.size(); ++i)
	{
		cout << "	" << i << " tree:" << endl;
		this->vectorHeapData[i]->PrintTree();
		cout << endl;
	}
}

//

//Leftist Heap

LeftistHeap::LeftistHeap(int key)
{
	this->mainTreeData = new BasicTree(key, 1);// 1 is rank for one-root-tree

}

int LeftistHeap::getMin()
{
	if (this->mainTreeData != NULL)
		return this->mainTreeData->data;
}

inline size_t LeftistHeap::calcRank(BasicTree* tree0)
{
	return (tree0 == NULL ? NULL : tree0->degree);
}

void LeftistHeap::Meld(iMeldableHeap* addedHeapExc)
{
	if (addedHeapExc == NULL)
	{
		return;
	}
	LeftistHeap* addedHeap = dynamic_cast <LeftistHeap*> (addedHeapExc);
	if (addedHeap == NULL)
	{
		throw MyBadCastEXC();
	}
	
	this->mainTreeData = meldSimilar(this->mainTreeData, addedHeap->mainTreeData);
}

BasicTree* LeftistHeap::meldSimilar(BasicTree* tree1, BasicTree* tree2)
{
	if (tree1 == NULL) return tree2;
	if (tree2 == NULL) return tree1;
	//cout << "current_degree: " << tree1->degree << endl;
	if (tree1->data > tree2->data)
	{
		std::swap(tree1, tree2);
	}
	if (tree1->child != NULL)
	{
		tree1->child->right = meldSimilar(tree1->child->right,tree2); // избавиться от рекурсии
		if (calcRank(tree1->child) < calcRank(tree1->child->right))//U-tirn need
		{
			//default function swap is not correct
			BasicTree* buffer = new BasicTree;
			buffer = tree1->child;
			tree1->child = tree1->child->right;
			tree1->child->right = buffer;
			tree1->child->right->right = NULL;
		}
		tree1->degree = 1 + calcRank(tree1->child->right);
	}
	else
	{
		tree1->child = tree2;
	}
	return tree1;
}

void LeftistHeap::Insert(int key)
{
	Meld(new LeftistHeap(key));
}

inline BasicTree* LeftistHeap::correctRight(BasicTree* tree0)
{
	return (tree0 == NULL ? NULL : tree0->right);
}

int LeftistHeap::ExtractMin()
{
	int curMin = this->mainTreeData->data;
//	cout << "PRINT_TEST_PARAMS: " << this->mainTreeData->child << " " << correctRight(this->mainTreeData->child) << endl;
	BasicTree* bufferRight = new BasicTree;
	bufferRight = correctRight(this->mainTreeData->child);
	if (this->mainTreeData->child != NULL)
	{
		this->mainTreeData->child->right = NULL;
	}
		this->mainTreeData = meldSimilar(this->mainTreeData->child, bufferRight);
		if (this->mainTreeData != NULL)
		{
			this->mainTreeData->right = NULL;
		}
	return curMin;
}

void LeftistHeap::printHeap()
{
	cout << "PRINT_TREE: " << typeid(this).name() << endl;
	this->mainTreeData->PrintTree();
	cout << endl;
}

//SkewHeap
SkewHeap::SkewHeap(int key)
{
	mainTreeData = new BasicTree(key);
}

void SkewHeap::Meld(iMeldableHeap* addedHeapExc)
{
	if (addedHeapExc == NULL)
	{
		return;
	}
	SkewHeap* addedHeap = dynamic_cast <SkewHeap*> (addedHeapExc);
	if (addedHeap == NULL)
	{
		throw MyBadCastEXC();
	}
	
	this->mainTreeData = meldSimilar(this->mainTreeData, addedHeap->mainTreeData);
}

BasicTree* SkewHeap::meldSimilar(BasicTree* tree1, BasicTree* tree2)
{
	if (tree1 == NULL) return tree2;
	if (tree2 == NULL) return tree1;
	//cout << "current_degree: " << tree1->degree << endl;
	if (tree1->data > tree2->data)
	{
		std::swap(tree1, tree2);
	}
	if (tree1->child != NULL)
	{
		tree1->child->right = meldSimilar(tree1->child->right, tree2); // избавиться от рекурсии
		//default function swap is not correct
			BasicTree* buffer = new BasicTree;
			buffer = tree1->child;
			tree1->child = tree1->child->right;
			tree1->child->right = buffer;
			tree1->child->right->right = NULL;
	}
	else
	{
		tree1->child = tree2;
	}
	return tree1;
}
