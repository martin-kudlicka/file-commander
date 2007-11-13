#include "FileSystem.h"

// destructor
cFileSystem::~cFileSystem()
{
} // ~cFileSystem

// check if file system is connected to a panel
const bool cFileSystem::IsInPanel() const
{
	return receivers(SIGNAL(ContentChanged(const cFileSystem *))) > 0;
} // IsInPanel
