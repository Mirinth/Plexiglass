#include <parser/tree.hpp>

FileNode _FileNode::New()
{
	return std::make_shared<_FileNode>();
}

std::ostream& operator<<(std::ostream& out, FileNode node)
{
	out << "File:\n";
	return out;
}