#pragma once
#include "BinTree.h"
#include "Vector"

#include <map>

class PFCTree
{
	using PFCTreeType = BinTree<char>;
	using PFCTreeNodeType = BinNode<char>;
	using PFCCodeType = Vector<bool>;
public:
	PFCTree() = default;
	~PFCTree(){}

	void initPFCForest();
	void generatePFCTree();
	void generatePFCTable();

	void encode(const char* str);
	void decode();

	void printPFCCodeTree();

private:
	static const int N_CHAR;

	void internalGeneratePFCTable(PFCTreeNodeType* node, PFCCodeType& code);

	PFCTreeType* PFCCodeTree;
	Vector<PFCTreeType*> PFCForest;
	std::map<char, PFCCodeType> PFCTable;
	Vector<PFCCodeType> codeArray;
};