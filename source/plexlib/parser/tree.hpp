#pragma once

#include <ostream>
#include <memory>

class _FileNode;
typedef std::shared_ptr<_FileNode> FileNode;

class _FileNode
{
public:
	static FileNode New();
	friend std::ostream& operator<<(std::ostream& out, FileNode node);
};
