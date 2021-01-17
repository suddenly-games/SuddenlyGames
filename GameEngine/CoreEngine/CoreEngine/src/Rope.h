#pragma once

#include <string>

/*
info on RB trees:
https://www.geeksforgeeks.org/red-black-tree-set-1-introduction-2/
https://www.geeksforgeeks.org/red-black-tree-set-2-insert/
https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/
https://www.cs.auckland.ac.nz/software/AlgAnim/red_black.html
https://www.topcoder.com/community/data-science/data-science-tutorials/an-introduction-to-binary-search-and-red-black-trees/
*/

class RopeNode
{
public:
	int weight = 0;
	int totalLength = 0;
	bool red = false;
	std::string string;
	RopeNode* parent = nullptr;
	RopeNode* left = nullptr;
	RopeNode* right = nullptr;

	RopeNode* previous();
	RopeNode* next();
};

class RopeNodePage
{
public:
	RopeNode page[32];
	RopeNodePage* previous = nullptr;
	RopeNodePage* next = nullptr;
};

class Rope
{
public:
	int pageAllocCount = 1;

	Rope(int maxSize = 16, int pageAllocCount = 1);
	Rope(const std::string& string, int maxSize = 16, int pageAllocCount = 1);
	Rope(const char* string, int maxSize = 16, int pageAllocCount = 1);
	Rope(const char* string, int length, int maxSize = 16, int pageAllocCount = 1);

	~Rope();

	void concat(const std::string& string);
	void concat(const char* string, int length = -1);
	void concat(const Rope& rope);

	void insert(const std::string& string, int index);
	void insert(const char* string, int index, int length = -1);
	void insert(const Rope& rope, int index);
	void insert(char character, int index);

	void remove(int index, int length = 1);

	int size() const;

	Rope& operator+=(const std::string& string);
	Rope& operator+=(const char* string);
	Rope& operator+=(const Rope& rope);

	class iterator
	{
	public:
		int startIndex() const;

		const std::string& operator*() const;
		const std::string* operator->() const;

		iterator& operator=(const iterator& rhs);
		iterator& operator++(); // prefix
		iterator& operator--(); // prefix
		iterator operator++(int); // postfix
		iterator operator--(int); // postfix

		bool operator==(const iterator& rhs) const;
		bool operator!=(const iterator& rhs) const;

		friend class Rope;

	private:
		unsigned long long editNumber = 0xFFFFFFFFFFFFFFFF;
		const Rope* rope;
		const RopeNode* node = nullptr;
		int start = -1;

		iterator(const Rope* rope, const RopeNode* node);
		void validate() const;
	};

	iterator begin() const;
	iterator back() const;
	iterator end() const;
	iterator find(int index) const;

	friend class iterator;

private:
	int maxStringSize = 16;
	RopeNode* head = nullptr;
	RopeNodePage headPage;
	RopeNode* poolHead = &headPage.page[0];
	RopeNode* poolTail = &headPage.page[31];
	RopeNodePage* tailPage = &headPage;
	unsigned long long editNumber = 0;

	RopeNode* findNode(int index);
	void eraseNode(RopeNode* node);
	RopeNode* insertNode(int index);
	void changeTotalLength(RopeNode* node, int change);

	void allocatePage();
	void initializePage(RopeNodePage* page);
	void deletePages();
	RopeNode* requestNode();
	void releaseNode(RopeNode* node);

	void concat(const RopeNode* node);
	int insert(const RopeNode* node, int index);
};