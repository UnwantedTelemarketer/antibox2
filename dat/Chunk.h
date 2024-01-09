#include "items.h"

#define CHUNK_WIDTH 30
#define CHUNK_HEIGHT 30

struct Saved_Chunk {
	int global_x, global_y;
	Saved_Tile tiles[CHUNK_WIDTH][CHUNK_HEIGHT];
};

struct Chunk {
	bool beenBuilt = false;
	Vector2_I globalChunkCoord;
	Tile localCoords[CHUNK_WIDTH][CHUNK_HEIGHT];
	std::vector<Entity*> entities;

	Tile* GetTileAtCoords(int x, int y) {
		return GetTileAtCoords({ x, y });
	}
	Tile* GetTileAtCoords(Vector2_I pos) {
		if (pos.x >= CHUNK_WIDTH) { pos.x = CHUNK_WIDTH - 1; }
		if (pos.x < 0) { pos.x = 0; }
		if (pos.y >= CHUNK_HEIGHT) { pos.y = CHUNK_HEIGHT - 1; }
		if (pos.y < 0) { pos.y = 0; }

		return &localCoords[pos.x][pos.y];
	}

	void LoadChunk(Vector2_I coords) {

		Saved_Chunk sChunk;
		std::string filePath = ("dat/map/Chunk" + std::to_string(coords.x) + std::to_string(coords.y));

		std::ifstream inFile(filePath, std::ios::binary);
		if (inFile.is_open()) {

			for (size_t i = 0; i < CHUNK_WIDTH; i++)
			{
				for (size_t j = 0; j < CHUNK_HEIGHT; j++)
				{
					sChunk.tiles[i][j].Deserialize(inFile);
				}
			}

			inFile.close();
		}
		globalChunkCoord = { sChunk.global_x, sChunk.global_y };

		//once deserialized, read the data into the tiles
		for (size_t i = 0; i < CHUNK_WIDTH; i++)
		{
			for (size_t j = 0; j < CHUNK_HEIGHT; j++)
			{
				localCoords[i][j].LoadTile(sChunk.tiles[i][j]);
			}
		}
	}

	void SaveChunk() {
		std::string filePath = ("dat/map/Chunk" + std::to_string(globalChunkCoord.x) + std::to_string(globalChunkCoord.y));
		std::ofstream outFile(filePath, std::ios::binary);

		Saved_Chunk *sChunk = new Saved_Chunk();
		for (size_t i = 0; i < CHUNK_WIDTH; i++)
		{
			for (size_t j = 0; j < CHUNK_HEIGHT; j++)
			{
				CreateSavedTile(&sChunk->tiles[i][j], localCoords[i][j]);
			}
		}

		for (size_t i = 0; i < CHUNK_WIDTH; i++)
		{
			for (size_t j = 0; j < CHUNK_HEIGHT; j++)
			{
				sChunk->tiles[i][j].Serialize(outFile);
			}
		}
		delete sChunk;
		outFile.close();
	}
};

static void CreateSavedChunk(Saved_Chunk* sChunk, Chunk ch) {
	for (size_t i = 0; i < CHUNK_WIDTH; i++)
	{
		for (size_t j = 0; j < CHUNK_HEIGHT; j++)
		{
			sChunk->tiles[i][j].id = ch.localCoords[i][j].id;
			sChunk->tiles[i][j].liquid = ch.localCoords[i][j].liquid;
			sChunk->tiles[i][j].burningFor = ch.localCoords[i][j].burningFor;
			sChunk->tiles[i][j].ticksPassed = ch.localCoords[i][j].ticksPassed;
			sChunk->tiles[i][j].ticksNeeded = ch.localCoords[i][j].ticksNeeded;
			sChunk->tiles[i][j].hasItem = ch.localCoords[i][j].hasItem;
			sChunk->tiles[i][j].itemName = ch.localCoords[i][j].itemName;
			sChunk->tiles[i][j].x = ch.localCoords[i][j].coords.x;
			sChunk->tiles[i][j].y = ch.localCoords[i][j].coords.y;
		}
	}
}

struct T_Chunk //we dont need the current chunk copy to have an entity list
{
	int localCoords[CHUNK_WIDTH][CHUNK_HEIGHT];
};
