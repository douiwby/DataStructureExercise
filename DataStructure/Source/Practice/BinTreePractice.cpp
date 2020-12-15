#include "Practice\BinTreePractice.h"

#include <cstdlib>
#include <ctime>

#include "Algorithm.h"

//const int PFCTree::N_CHAR = (0x80 - 0x20);
const char startChar = 0x20;
const char endChar = 0x7f;
const int PFCTree::N_CHAR = endChar - startChar + 1;

void PFCTree::initPFCForest()
{
	for (int i = 0; i < N_CHAR; ++i)
	{
		PFCTreeType* tree = new PFCTreeType;
		tree->insertAsRoot(startChar + i);
		PFCForest.push_back(tree);
	}
}

void PFCTree::generatePFCTree()
{
	srand((unsigned)time(NULL));
	while (PFCForest.size() > 1)
	{
		PFCTreeType* tree = new PFCTreeType;
		tree->insertAsRoot(0);

		int n = rand() % PFCForest.size();
		tree->attachAsLChild(tree->root(), *PFCForest[n]);
		PFCForest.erase(PFCForest.begin() + n);

		int n2 = rand() % PFCForest.size();
		tree->attachAsRChild(tree->root(), *PFCForest[n2]);
		PFCForest.erase(PFCForest.begin() + n2);

		PFCForest.push_back(tree);
	}
	PFCCodeTree = PFCForest[0];
}

void PFCTree::generatePFCTable()
{
	PFCCodeType code;
	internalGeneratePFCTable(PFCCodeTree->root(), code);
}

void PFCTree::internalGeneratePFCTable(PFCTreeNodeType* node, PFCCodeType& code)
{
	if (node->hasLChild())
	{
		code.push_back(0);
		internalGeneratePFCTable(node->getLChild(), code);
	}
	if (node->hasRChild())
	{
		code.push_back(1);
		internalGeneratePFCTable(node->getRChild(), code);
	}
	if(node->isLeaf())
	{
		PFCTable.emplace(node->getData(), code);
	}
	if(!code.empty()) code.pop_back();
}

void PFCTree::encode(const char * str)
{
	codeArray.clear();
	while (*str!= '\0')
	{
		if (*str<= endChar && *str>=startChar)
		{
			codeArray.push_back(PFCTable[*str]);
		}
		++str;
	}

	for (auto it = codeArray.begin(); it != codeArray.end(); ++it)
	{
		printContainer(*it,'\0');
	}
}

void PFCTree::decode()
{
	for (auto it = codeArray.begin(); it != codeArray.end(); ++it)
	{
		PFCTreeNodeType* node = PFCCodeTree->root();
		PFCCodeType code = *it;

		for (auto itCode = code.begin(); itCode != code.end(); ++itCode)
		{
			if (*itCode)
			{
				node = node->getRChild();
			}
			else
			{
				node = node->getLChild();
			}
		}
		assert(node->isLeaf());

		std::cout << node->getData();
	}
}

void PFCTree::printPFCCodeTree()
{
	if (PFCCodeTree)
	{
		PFCCodeTree->traversalPostorder([](char d) {std::cout << d << ","; });
		std::cout << std::endl;
	}
}
