#include "Chunk.h"

#include <fstream>
#include <iostream>

#include "Terrain.h"
#include "Model.h"
#include "Transform.h"
#include "Camera.h"
#include "ModelAsset.h"

namespace Engine
{
	void updateCount(int delta)
	{
		static int count = 0;

		count += delta;

		std::cout << "chunks alive: " << count << std::endl;
	}

	Chunk::~Chunk()
	{
		//updateCount(-1);
	}

	void Chunk::Initialize()
	{
		//updateCount(1);

		
	}

	void Chunk::Update(float)
	{
		if (RegenerateGeometry)
			Regenerate();

		RegenerateGeometry = false;
	}

	void Chunk::SetTerrain(const std::shared_ptr<Object>& terrain, const Coordinates& chunk)
	{
		ParentTerrain = terrain;
		Index = chunk;
		MaterialProperties = terrain->Cast<Terrain>()->GetMaterial();

		std::shared_ptr<Transform> transform = Engine::Create<Transform>();

		//transform->IsStatic = false;
		transform->SetParent(This.lock());
		transform->Transformation = Matrix3(float(chunk.X) * 16, float(chunk.Y) * 16, float(chunk.Z) * 16);

		std::shared_ptr<GraphicsEngine::Model> model = Engine::Create<GraphicsEngine::Model>();

		model->SetParent(transform);
		model->MaterialProperties = MaterialProperties;

		std::shared_ptr<ModelAsset> chunkMesh = MeshLoader::NewAsset(terrain->Cast<Terrain>()->GetChunkPath(chunk));

		chunkMesh->SetParent(model);

		//model->Asset = MeshLoader::GetAsset(1);
		model->Asset = chunkMesh;

		ChunkModel = model;
		ChunkMesh = chunkMesh;
	}

	void Chunk::Save() const
	{

	}

	void Chunk::Load()
	{
		if (ParentTerrain.expired())
			return;

		std::string path = ParentTerrain.lock()->Cast<Terrain>()->DataDirectory + "/" + GetTerrain<Terrain>()->GetChunkPath(Index);
		std::ifstream file(path, std::ios_base::in | std::ios_base::binary);
		
		if (!file.is_open() || !file.good())
			throw std::string("cannot open file: '") + path + "'";

		CurrentStatus.State = Enum::ChunkStatus::Loading;

		for (int x = 0; x < 16; ++x)
		{
			for (int y = 0; y < 16; ++y)
			{
				char repetitionFlag = 0;
		
				file.read(&repetitionFlag, 1);
		
				if (unsigned char(repetitionFlag) == 0xFF)
					break;
		
				int emptyRows = (unsigned char(repetitionFlag) & 0xF0) >> 4;
				int repeatedRows = (unsigned char(repetitionFlag) & 0xF);
				
				y += emptyRows;

				if (y > 16)
				{
					std::cout << path.c_str() << std::endl; throw std::string("chunk loader error: y out of bounds");
				}
		
				for (int z = 0; z < 16; ++z)
				{
					char repetitionFlag = 0;
		
					file.read(&repetitionFlag, 1);

					if (unsigned char(repetitionFlag) == 0xFF)
						break;
		
					int emptyCells = (unsigned char(repetitionFlag) & 0xF0) >> 4;
					int repeatedCells = (unsigned char(repetitionFlag) & 0xF);
		
					char propertyFlags = 0;

					z += emptyCells;

					if (z > 16)
					{
						std::cout << path.c_str() << std::endl; throw std::string("chunk loader error: z out of bounds");
					}
		
					file.read(&propertyFlags, 1);
		
					bool isFull = (unsigned char(propertyFlags) & 0x80) != 0;
					bool hasData = (unsigned char(propertyFlags) & 0x40) != 0;
					bool expandMaterial = (unsigned char(propertyFlags) & 0x20) != 0;
					unsigned short material = unsigned short(unsigned char(propertyFlags) & 0x1F);
					unsigned short dataIndex = 0xFFF;
					char occupancy = 0;
		
					if (expandMaterial)
					{
						char leastSignificant = 0;
		
						file.read(&leastSignificant, 1);
		
						material = (material << 5) | unsigned char(leastSignificant);
					}
		
					if (!isFull)
						file.read(&occupancy, 1);
		
					TerrainTypes::VoxelData& cell = Data[x][y][z];
		
					cell.Occupancy = occupancy;
					cell.MaterialData = material;
					cell.Data = unsigned short(isFull ? 0x8000 : 0) | dataIndex;

					if (z + repeatedCells > 16)
					{
						std::cout << path.c_str() << std::endl; throw std::string("chunk loader error: z out of bounds");
					}
		
					for (int i = 1; i <= repeatedCells; ++i)
						Data[x][y][z + i] = Data[x][y][z];
		
					z += repeatedCells;
				}

				if (y > 16)
				{
					std::cout << path.c_str() << std::endl; throw std::string("chunk loader error: y out of bounds");
				}
		
				for (int i = 1; i <= repeatedRows; ++i)
					for (int z = 0; z < 16; ++z)
						Data[x][y + i][z] = Data[x][y][z];
		
				y += repeatedRows;
			}
		}
		
		CurrentStatus.State = Enum::ChunkStatus::Loaded;

		RegenerateGeometry = true;

		RegenerateNeighbors();
	}

	void Chunk::Regenerate()
	{
		MeshData data;
		VoxelTriangulation triangulator;

		triangulator.PushTriangle = [&data] (const Vector3& pointA, const Vector3& pointB, const Vector3& pointC)
		{
			Vector3 normal = (pointB - pointA).Cross(pointC - pointA);

			VertexData vertex;

			vertex.Position = pointA;
			vertex.Normal = normal;

			data.VertexBuffer.push_back(vertex);

			vertex.Position = pointB;

			data.VertexBuffer.push_back(vertex);

			vertex.Position = pointC;

			data.VertexBuffer.push_back(vertex);

			data.IndexBuffer.push_back(int(data.IndexBuffer.size()));
			data.IndexBuffer.push_back(int(data.IndexBuffer.size()));
			data.IndexBuffer.push_back(int(data.IndexBuffer.size()));
		};

		std::shared_ptr<Chunk> chunks[8] = {
			This.lock()->Cast<Chunk>(),
			FetchChunk(Index.X + 1, Index.Y,     Index.Z),
			FetchChunk(Index.X,     Index.Y + 1, Index.Z),
			FetchChunk(Index.X + 1, Index.Y + 1, Index.Z),
			FetchChunk(Index.X,     Index.Y,     Index.Z + 1),
			FetchChunk(Index.X + 1, Index.Y,     Index.Z + 1),
			FetchChunk(Index.X,     Index.Y + 1, Index.Z + 1),
			FetchChunk(Index.X + 1, Index.Y + 1, Index.Z + 1),
		};

		std::shared_ptr<Terrain> terrain = ParentTerrain.lock()->Cast<Terrain>();

		bool chunksCanLoad[8] = {
			true,
			terrain->CanLoadChunk(Coordinates(Index.X + 1, Index.Y,     Index.Z)),
			terrain->CanLoadChunk(Coordinates(Index.X,     Index.Y + 1, Index.Z)),
			terrain->CanLoadChunk(Coordinates(Index.X + 1, Index.Y + 1, Index.Z)),
			terrain->CanLoadChunk(Coordinates(Index.X,     Index.Y,     Index.Z + 1)),
			terrain->CanLoadChunk(Coordinates(Index.X + 1, Index.Y,     Index.Z + 1)),
			terrain->CanLoadChunk(Coordinates(Index.X,     Index.Y + 1, Index.Z + 1)),
			terrain->CanLoadChunk(Coordinates(Index.X + 1, Index.Y + 1, Index.Z + 1)),
		};

		for (int x = 0; x < 16; ++x)
			for (int y = 0; y < 16; ++y)
				for (int z = 0; z < 16; ++z)
					Triangulate(triangulator, chunks, chunksCanLoad, x, y, z);

		data.UpdateBounds();

		MeshLoader::LoadAsset(ChunkMesh.lock(), MeshData(&data));
	}

	void Chunk::Triangulate(VoxelTriangulation& triangulator, std::shared_ptr<Chunk>* chunks, bool* chunksCanLoad, int cellX, int cellY, int cellZ)
	{
		for (int x = 0; x < 2; ++x)
		{
			int flagX = int(cellX + x == 16);

			for (int y = 0; y < 2; ++y)
			{
				int flagY = int(cellY + y == 16) << 1;

				for (int z = 0; z < 2; ++z)
				{
					int flagZ = int(cellZ + z == 16) << 2;

					std::shared_ptr<Chunk> chunk = chunks[flagX | flagY | flagZ];

					if (chunk == nullptr)
					{
						if (chunksCanLoad[flagX | flagY | flagZ])
							return;
						else
							triangulator.Occupancy[x][y][z] = 0;
					}
					else
					{
						TerrainTypes::VoxelData& voxel = chunk->Get(Coordinates((cellX + x) % 16, (cellY + y) % 16, (cellZ + z) % 16));

						if (voxel.MaterialData != 0 && voxel.MaterialData != 22)
							triangulator.Occupancy[x][y][z] = voxel.GetOccupancy();
						else
							triangulator.Occupancy[x][y][z] = 0;
					}
				}
			}
		}

		if (Index.X == 3 && Index.Y == 1 && Index.Z == -7 && cellX == 1 && cellY == 0 && cellZ == 4)
			cellX += 0;

		triangulator.Center.Set(float(cellX) + 0.5f, float(cellY) + 0.5f, float(cellZ) + 0.5f, 1);
		triangulator.Triangulate();
	}

	void Chunk::Draw(bool drawLiquid, const std::shared_ptr<GraphicsEngine::Camera>& targetCamera)
	{
		if (!drawLiquid)
			ChunkModel.lock()->Cast<GraphicsEngine::Model>()->Draw(targetCamera);
	}

	std::shared_ptr<TerrainTypes::Status> Chunk::GetStatus()
	{
		std::shared_ptr<TerrainTypes::Status> status;

		if (StatusHandle.expired())
		{
			status = std::shared_ptr<TerrainTypes::Status>(&CurrentStatus, [this] (void*) {
				Coordinates index;
				std::shared_ptr<Terrain> terrain;

				index = GetCoordinates();
				terrain = GetTerrain()->Cast<Terrain>();

				//std::cout << "attempt unload: " << index.X << ", " << index.Y << ", " << index.Z << std::endl;

				if (terrain != nullptr)
					terrain->Cast<Terrain>()->UnloadChunk(index);
			});

			StatusHandle = status;
		}
		else
			status = StatusHandle.lock();

		return status;
	}

	CellData Chunk::GetCell(const Coordinates& cell) const
	{
		const TerrainTypes::VoxelData& voxel = Get(cell);

		int index = voxel.GetDataIndex();

		return CellData(
			voxel.GetOccupancy(),
			ParentTerrain.lock()->Cast<Terrain>()->GetMaterial(voxel.MaterialData),
			index == 0xFFF ? nullptr : Voxels[index - 1].lock()
		);
	}

	unsigned short Chunk::GetMaterialID(const Coordinates& cell) const
	{
		const TerrainTypes::VoxelData& voxel = Get(cell);

		return voxel.MaterialData;
	}

	void Chunk::SetCell(const Coordinates& cell, const CellData& data)
	{
		SetCell(cell, data.Occupancy, data.MaterialType, data.VoxelData);
	}

	void Chunk::SetCell(const Coordinates& cell, float occupancy, const std::shared_ptr<TerrainMaterial>& material, const std::shared_ptr<Voxel>& data)
	{
		SetOccupancy(cell, occupancy);
		SetMaterial(cell, material);
		SetData(cell, data);
	}

	void Chunk::SetOccupancy(const Coordinates& cell, float occupancy)
	{
		Get(cell).SetOccupancy(occupancy);
	}

	void Chunk::SetMaterial(const Coordinates& cell, const std::shared_ptr<TerrainMaterial>& material)
	{
		TerrainTypes::VoxelData& voxel = Get(cell);

		if (voxel.MaterialData == 0 && material->GetID() != 0)
			++OccupiedCells;
		else if (voxel.MaterialData != 0 && material->GetID() == 0)
			--OccupiedCells;

		voxel.MaterialData = material->GetID();
	}

	void Chunk::SetData(const Coordinates& cell, const std::shared_ptr<Voxel>& data)
	{
		TerrainTypes::VoxelData& voxel = Get(cell);

		int index = voxel.GetDataIndex();

		if (data == nullptr)
		{
			if (index != 0xFFF)
			{
				int back = int(Voxels.size()) - 1;

				if (index != back)
				{
					Get(Voxels.back().lock()->GetPosition()).SetDataIndex(index);

					std::swap(Voxels[index], Voxels.back());

					Voxels.pop_back();
				}
				else
					Voxels.pop_back();

				index = 0xFFF;
			}
		}
		else
		{
			if (index == 0xFFF)
			{
				index = int(Voxels.size());

				Voxels.push_back(std::weak_ptr<Voxel>());
				Voxels.back() = data;
			}
			else
				Voxels[index] = data;
		}

		voxel.SetDataIndex(index);
	}

	std::shared_ptr<Object> Chunk::GetTerrain() const
	{
		return ParentTerrain.lock();
	}

	TerrainTypes::VoxelData& Chunk::Get(const Coordinates& cell)
	{
		if (cell.X < 0 || cell.Y < 0 || cell.Z < 0 || cell.X > 15 || cell.Y > 15 || cell.Z > 15)
			throw std::string("coordinates out of bounds");

		return Data[cell.X][cell.Y][cell.Z];
	}

	const TerrainTypes::VoxelData& Chunk::Get(const Coordinates& cell) const
	{
		if (cell.X < 0 || cell.Y < 0 || cell.Z < 0 || cell.X > 15 || cell.Y > 15 || cell.Z > 15)
			throw std::string("coordinates out of bounds");

		return Data[cell.X][cell.Y][cell.Z];
	}

	std::shared_ptr<Chunk> Chunk::FetchChunk(int chunkX, int chunkY, int chunkZ)
	{
		std::shared_ptr<Object> chunkObject = ParentTerrain.lock()->Cast<Terrain>()->GetChunk(Coordinates(chunkX, chunkY, chunkZ));
		
		if (chunkObject == nullptr)
			return nullptr;

		std::shared_ptr<Chunk> chunk = chunkObject->Cast<Chunk>();

		if (chunk != nullptr && chunk->Cast<Chunk>()->CurrentStatus.State == Enum::ChunkStatus::Loaded)
			return chunk;

		return nullptr;
	}

	void Chunk::RegenerateNeighbors(bool changedLeft, bool changedBottom, bool changedBack)
	{
		std::shared_ptr<Terrain> terrain = ParentTerrain.lock()->Cast<Terrain>();

		if (changedBack)
		{
			if (changedBottom)
			{
				if (changedLeft)
					AttemptRegeneration(Index.X - 1, Index.Y - 1, Index.Z - 1);

				AttemptRegeneration(Index.X, Index.Y - 1, Index.Z - 1);
			}

			if (changedLeft)
				AttemptRegeneration(Index.X - 1, Index.Y, Index.Z - 1);

			AttemptRegeneration(Index.X, Index.Y, Index.Z - 1);
		}

		if (changedBottom)
		{
			if (changedLeft)
				AttemptRegeneration(Index.X - 1, Index.Y - 1, Index.Z);

			AttemptRegeneration(Index.X, Index.Y - 1, Index.Z);
		}

		if (changedLeft)
			AttemptRegeneration(Index.X - 1, Index.Y, Index.Z);
	}

	void Chunk::AttemptRegeneration(int chunkX, int chunkY, int chunkZ)
	{
		std::shared_ptr<Object> chunk = FetchChunk(chunkX, chunkY, chunkZ);

		if (chunk != nullptr)
			chunk->Cast<Chunk>()->RegenerateGeometry = true;
	}
}