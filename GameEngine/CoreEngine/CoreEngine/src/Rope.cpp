#include "Rope.h"

RopeNode* RopeNode::previous()
{
	RopeNode* node = this;

	if (node->left == nullptr)
	{
		const RopeNode* lastNode = node;

		while (node && node->left != lastNode)
		{
			lastNode = node;
			node = node->parent;
		}
	}
	else
	{
		node = node->left;

		while (node->right)
			node = node->right;
	}

	return node;
}

RopeNode* RopeNode::next()
{
	RopeNode* node = this;

	if (node->right == nullptr)
	{
		const RopeNode* lastNode = node;

		while (node && node->right == lastNode)
		{
			lastNode = node;
			node = node->parent;
		}
	}
	else
	{
		node = node->right;

		while (node->left != nullptr)
			node = node->left;
	}

	return node;
}

Rope::Rope(int maxSize, int pageAllocCount) : maxStringSize(maxSize), pageAllocCount(pageAllocCount)
{
	initializePage(&headPage);
}

Rope::Rope(const std::string& string, int maxSize, int pageAllocCount) : maxStringSize(maxSize), pageAllocCount(pageAllocCount)
{
	initializePage(&headPage);
	concat(string);
}

Rope::Rope(const char* string, int maxSize, int pageAllocCount) : maxStringSize(maxSize), pageAllocCount(pageAllocCount)
{
	initializePage(&headPage);
	concat(string);
}

Rope::Rope(const char* string, int length, int maxSize, int pageAllocCount) : maxStringSize(maxSize), pageAllocCount(pageAllocCount)
{
	initializePage(&headPage);
	concat(string, length);
}

Rope::~Rope()
{
	deletePages();
}

void Rope::concat(const std::string& string)
{
	concat(string.c_str(), int(string.size()));
}

void Rope::concat(const char* string, int length)
{
	insert(string, size(), length);
}

void Rope::concat(const Rope& rope)
{
	concat(rope.head);
}

void Rope::insert(const std::string& string, int index)
{
	insert(string.c_str(), index, int(string.size()));
}

void Rope::insert(const char* string, int index, int length)
{
	++editNumber;

	if (length == -1)
		for (length = 0; string[length] != 0; ++length);

	RopeNode* node = findNode(index);

	while (length > 0)
	{
		if (node == nullptr)
			node = insertNode(index);

		int writeLength = length;
		int insertPosition = index - node->weight;

		int stringSize = int(node->string.size());

		if (writeLength + stringSize > maxStringSize)
		{
			writeLength = maxStringSize - stringSize - insertPosition;

			int offsettedCharacters = maxStringSize - writeLength - insertPosition;

			RopeNode* newNode = insertNode(node->weight + stringSize);

			newNode->string.append(node->string.c_str() + insertPosition, stringSize - insertPosition);

			changeTotalLength(newNode, int(newNode->string.size()));

			node->string.erase(insertPosition, stringSize);
		}

		node->string.insert(insertPosition, string, writeLength);
		node->totalLength += writeLength;

		changeTotalLength(node, writeLength);

		length -= writeLength;
		string += writeLength;

		node = node->next();
	}
}

void Rope::insert(const Rope& rope, int index)
{
	insert(rope.head, index);
}

void Rope::insert(char character, int index)
{
	insert(&character, index, 1);
}

void Rope::remove(int index, int length)
{
	++editNumber;

	RopeNode* node = findNode(index);

	if (node == nullptr)
		return;

	while (node != nullptr && length > 0)
	{
		int start = index - node->weight;
		int eraseLength = length;

		if (int(node->string.size()) < length)
			eraseLength = int(node->string.size()) - start;

		node->string.erase(node->string.begin() + start, node->string.begin() + start + eraseLength);
		node->totalLength -= eraseLength;

		changeTotalLength(node, -eraseLength);

		length -= eraseLength;

		RopeNode* previous = node;

		node = node->next();

		if (previous->string.size() == 0)
			eraseNode(previous);
	}
}

int Rope::size() const
{
	if (head != nullptr)
		return head->totalLength;

	return 0;
}

Rope& Rope::operator+=(const std::string& string)
{
	concat(string);

	return *this;
}

Rope& Rope::operator+=(const char* string)
{
	concat(string);

	return *this;
}

Rope& Rope::operator+=(const Rope& rope)
{
	concat(rope);

	return *this;
}

int Rope::iterator::startIndex() const
{
	return node->weight;
}

const std::string& Rope::iterator::operator*() const
{
	return node->string;
}

const std::string* Rope::iterator::operator->() const
{
	return &node->string;
}

Rope::iterator& Rope::iterator::operator=(const iterator& rhs)
{
	editNumber = rhs.editNumber;
	node = rhs.node;
	start = rhs.start;

	return *this;
}

Rope::iterator& Rope::iterator::operator++()// prefix
{
	validate();

	if (node == nullptr)
		return *this;

	if (node->right == nullptr)
	{
		const RopeNode* lastNode = node;

		while (node && node->right == lastNode)
		{
			lastNode = node;
			node = node->parent;
		}
	}
	else
	{
		node = node->right;

		while (node->left != nullptr)
			node = node->left;
	}

	if (node == nullptr)
		start = -1;
	else
		start = node->weight;

	return *this;
}

Rope::iterator& Rope::iterator::operator--()// prefix
{
	validate();

	if (node == nullptr)
		return *this;

	if (node->left == nullptr)
	{
		const RopeNode* lastNode = node;

		while (node && node->left != lastNode)
		{
			lastNode = node;
			node = node->parent;
		}
	}
	else
	{
		node = node->left;

		while (node->right)
			node = node->right;
	}

	if (node == nullptr)
		start = -1;
	else
		start = node->weight;

	return *this;
}

Rope::iterator Rope::iterator::operator++(int)// postfix
{
	validate();

	iterator i = iterator(*this);

	++i;

	return i;
}

Rope::iterator Rope::iterator::operator--(int)// postfix
{
	validate();

	iterator i = iterator(*this);

	--i;

	return i;
}

bool Rope::iterator::operator==(const iterator& rhs) const
{
	validate();

	return node == rhs.node;
}

bool Rope::iterator::operator!=(const iterator& rhs) const
{
	validate();

	return node != rhs.node;
}

Rope::iterator::iterator(const Rope* rope, const RopeNode* node) : rope(rope), editNumber(rope->editNumber), node(node)
{
	if (node != nullptr)
		start = node->weight;
}

void Rope::iterator::validate() const
{
	if (editNumber != rope->editNumber)
		throw;
}

Rope::iterator Rope::begin() const
{
	RopeNode* node = head;

	while (node && node->left != nullptr)
		node = node->left;

	return iterator(this, node);
}

Rope::iterator Rope::back() const
{
	RopeNode* node = head;

	while (node && node->right != nullptr)
		node = node->right;

	return iterator(this, node);
}

Rope::iterator Rope::end() const
{
	return iterator(this, nullptr);
}

Rope::iterator Rope::find(int index) const
{
	RopeNode* node = head;

	while (node && (index < node->weight || index >= node->weight + int(node->string.size())))
		node = index < node->weight ? node->left : node->right;

	return iterator(this, node);
}

RopeNode* Rope::findNode(int index)
{
	RopeNode* node = head;

	while (node && (index < node->weight || index >= node->weight + int(node->string.size())))
		node = index < node->weight ? node->left : node->right;

	return node;
}

void Rope::eraseNode(RopeNode* node)
{

}

RopeNode* Rope::insertNode(int index)
{
	RopeNode* node = requestNode();



	return node;
}

void Rope::changeTotalLength(RopeNode* node, int change)
{
	RopeNode* parent = node->parent;
	RopeNode* previous = node;
	bool changeWeight = true;

	while (parent != nullptr)
	{
		changeWeight &= parent->left == previous;

		if (changeWeight)
			parent->weight += change;

		parent->totalLength += change;
		parent = parent->parent;
	}
}

void Rope::allocatePage()
{
	RopeNodePage* newPage = new RopeNodePage[pageAllocCount];

	newPage->previous = tailPage;
	tailPage->next = newPage;
	tailPage = newPage;

	initializePage(newPage);

	poolTail->right = &newPage->page[0];
	newPage->page[0].left = poolTail;
	poolTail = &newPage->page[31];
}

void Rope::initializePage(RopeNodePage* page)
{
	RopeNode* previous = nullptr;

	for (int i = 0; i < 32; ++i)
	{
		page->page[i].left = previous;
		page->page[i].right = &page->page[i + 1];

		previous = &page->page[i];
	}
}

void Rope::deletePages()
{
	RopeNodePage* page = headPage.next;

	while (page)
	{
		RopeNodePage* next = page->next;
		
		delete[] page;

		page = next;
	}
}

RopeNode* Rope::requestNode()
{
	if (poolHead == nullptr)
		allocatePage();

	RopeNode* node = poolHead;

	poolHead = node->right;

	if (poolHead != nullptr)
		poolHead->left = nullptr;
	else
		poolTail = nullptr;

	node->right = nullptr;
	
	return node;
}

void Rope::releaseNode(RopeNode* node)
{
	node->parent = nullptr;
	node->right = nullptr;
	node->red = false;
	node->totalLength = 0;
	node->weight = 0;
	node->string.clear();
	node->left = poolTail;

	if (poolTail)
		poolTail->right = node;
	else
		poolHead = node;

	poolTail = node;
}

void Rope::concat(const RopeNode* node)
{
	if (node == nullptr)
		return;

	concat(node->left);
	concat(node->string);
	concat(node->right);
}

int Rope::insert(const RopeNode* node, int index)
{
	if (node == nullptr)
		return index;

	index = insert(node->left, index);
	insert(node->string, index);
	return insert(node->right, index + int(node->string.size()));
}