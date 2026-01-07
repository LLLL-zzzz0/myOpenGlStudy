#include "object.h"

class Node : public Object<Node>
{
	using Ptr = std::shared_ptr<Node>;

	static Ptr Create()
	{
		return Ptr(new Node());
	}
protected:
	Node() :Object(ObjectType::NODE)
	{
	}
};