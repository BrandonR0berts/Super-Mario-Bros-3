#ifndef _NODEMAP_WORLD_MAP_H_
#define _NODEMAP_WORLD_MAP_H_

#include "Commons_SMB3.h"

#include <vector>
#include <string>
#include <map>

class NodeMap_WorldMap final
{
public:
	NodeMap_WorldMap(const std::string filePath);
	~NodeMap_WorldMap();

	void        ReplaceDataPoint(const unsigned int row, const unsigned int col, const char newVal);

	char        GetSpecificDataPoint(const unsigned int row, const unsigned int col);
	char        GetSpecificDataPoint(const Vector2D position);

	char**      GetDataStore()                       const { return mNodeMapData; }

	std::string GetLevelFilePath(const char request)       { return mLevelLoadingFilePaths[request]; }
	Vector2D    GetSpawnPoint();

	bool        GetValueIsLevel(const char value); 
	bool        GetPositionIsWalkable(const Vector2D position);

	bool        GetDataPointIsDot(const char value);
	bool        GetPositionIsAClearedLevel(const Vector2D positionToCheck);

	void        SetLevelIsCleared(char levelID);

private:
	void        LoadInDataFromFile(const std::string filePath);

	std::map<char, std::string> mLevelLoadingFilePaths;
	static std::vector<char>    mClearedLevels;

	unsigned int                mWidth;
	unsigned int                mHeight;

	char**                      mNodeMapData;
};

// --------------------------------------------------------------------------- //

#endif
