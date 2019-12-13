#pragma once

#include <vt/graphics/model/IndexedMesh.h>
#include <vt/math/Vec4.h>

#include "../../world/chunk/voxel/Voxel.h"

class ModelLoader
{
public:
	static vt::GLIndexedModel loadVOXModel(const char* file);
private:
	static vt::Vec4b getColor(int x, int y, int z, vt::Vec4b* data, unsigned int width, unsigned int height, unsigned int length);
	static void addQuad(vt::IndexedMesh<VoxelVertex, unsigned int>& mesh, int x, int y, int z, vt::Vec4b* data, unsigned int width, unsigned int height, unsigned int length, const vt::Vec3f& p0, const vt::Vec3f& p1, const vt::Vec3f& p2, const vt::Vec3f& p3, const vt::Vec4b& color, const vt::Vec3f& normal);
	static float getAmbientOcclusion(int x, int y, int z, vt::Vec4b* data, int width, int height, int length, const vt::Vec3f& vertex, const vt::Vec3f& normal);
};