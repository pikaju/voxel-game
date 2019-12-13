#pragma once

#include "Chunk.h"

#define AMBIENT_OCCLUSION_SAMPLE_AREA 2
#define AMBIENT_OCCLUSION_HEMISPHERE 1
#define POLYGON_SCALE_INCREASE (1.0f / 512.0f)
#define GREEDY_MESHING 1

class VoxelQuad
{
public:
	VoxelQuad() {  }
	VoxelQuad(int x, int y, int width, int height, const vt::Vec3f& p0, const vt::Vec3f& p1, const vt::Vec3f& p2, const vt::Vec3f& p3, const vt::Vec4b& c0, const vt::Vec4b& c1, const vt::Vec4b& c2, const vt::Vec4b& c3, const vt::Vec3f& normal, bool flipped) : m_x(x), m_y(y), m_width(width), m_height(height), m_p0(p0), m_p1(p1), m_p2(p2), m_p3(p3), m_c0(c0), m_c1(c1), m_c2(c2), m_c3(c3), m_normal(normal), m_flipped(flipped) {  }
	~VoxelQuad() {  }

	void merge(VoxelQuad& quad);
	bool canMerge(const VoxelQuad& quad);

	void add(vt::IndexedMesh<VoxelVertex, unsigned int>& mesh);
private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;

	vt::Vec3f m_p0;
	vt::Vec3f m_p1;
	vt::Vec3f m_p2;
	vt::Vec3f m_p3;

	vt::Vec4b m_c0;
	vt::Vec4b m_c1;
	vt::Vec4b m_c2;
	vt::Vec4b m_c3;

	vt::Vec3f m_normal;

	bool m_flipped;
};

class ChunkMesher
{
public:
	static void createMesh(Chunk& chunk, vt::IndexedMesh<VoxelVertex, unsigned int>& mesh, vt::IndexedMesh<VoxelVertex, unsigned int>& waterMesh, vt::IndexedMesh<VoxelVertex, unsigned int>& grassMesh);
	static void createMeshMarchingCubes(Chunk& chunk, vt::IndexedMesh<VoxelVertex, unsigned int>& mesh, vt::IndexedMesh<VoxelVertex, unsigned int>& waterMesh);
	static void createWaterMesh(Chunk& chunk, vt::IndexedMesh<VoxelVertex, unsigned int>& waterMesh, VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA>* container);
	static void createGrassMesh(Chunk& chunk, vt::IndexedMesh<VoxelVertex, unsigned int>& grassMesh, VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA>* container);
private:
	static VoxelQuad getQuad(const Chunk& chunk, VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA>& container, int x, int y, const vt::Vec3f& p0, const vt::Vec3f& p1, const vt::Vec3f& p2, const vt::Vec3f& p3, const vt::Vec3b& color, const vt::Vec3f normal);
	static void optimizeSlice(VoxelQuad* quads);
	static float getAmbientOcclusion(VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA>& container, const vt::Vec3f& vertex);
	static float getAmbientOcclusion(VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA>& container, const vt::Vec3f& vertex, const vt::Vec3f& normal);
	static float getAmbientOcclusionMarchingCubes(VoxelArrayBorderContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, AMBIENT_OCCLUSION_SAMPLE_AREA + 1>& container, const vt::Vec3f& vertex);
};