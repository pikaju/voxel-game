#include "ModelLoader.h"

#include <fstream>
#include <vt/graphics/model/IndexedMesh.h>

#define AMBIENT_OCCLUSION_SAMPLE_AREA 2

vt::GLIndexedModel ModelLoader::loadVOXModel(const char* file)
{

	std::ifstream in("res/models/Palette.act", std::ios::binary);
	if (!in.is_open()) {
		vt::log::error("Could not open palette file: \"", "res/models/Palette.act", "\"");
		return vt::GLIndexedModel();
	}

	vt::Vec3b palette[256];
	for (unsigned int i = 0; i < 255; i++) {
		in.read((char*)(&palette[i]), 3);
	}

	in = std::ifstream(file, std::ios::binary);
	if (!in.is_open()) {
		vt::log::error("Could not open model file: \"", file, "\"");
		return vt::GLIndexedModel();
	}

	char fileType[4];
	in.read(fileType, 4);

	int version;
	in.read((char*)(&version), 4);
	
	char main[4];
	int mainContent;
	int mainChildren;
	in.read(main, 4);
	in.read((char*)(&mainContent), 4);
	in.read((char*)(&mainChildren), 4);

	unsigned int width;
	unsigned int height;
	unsigned int length;

	vt::Vec4b* voxels = nullptr;

	while (!in.eof()) {
		char id[4];
		unsigned int content;
		unsigned int children;
		in.read(id, 4);
		in.read((char*)(&content), 4);
		in.read((char*)(&children), 4);

		if (id[0] == 'S' && id[1] == 'I' && id[2] == 'Z' && id[3] == 'E') {
			in.read((char*)(&width), 4);
			in.read((char*)(&length), 4);
			in.read((char*)(&height), 4);
			voxels = new vt::Vec4b[width * height * length];
			memset(voxels, 0, width * height * length * sizeof(vt::Vec4b));
		}
		else if (id[0] == 'X' && id[1] == 'Y' && id[2] == 'Z' && id[3] == 'I') {
			unsigned int numVoxels;
			in.read((char*)(&numVoxels), 4);
			for (unsigned int i = 0; i < numVoxels; i++) {
				unsigned char v[4];
				in.read((char*)v, 4);
				unsigned int x = (unsigned int)v[0];
				unsigned int z = (unsigned int)v[1];
				unsigned int y = (unsigned int)v[2];
				unsigned int index = (unsigned int)v[3];

				voxels[x + y * width + (length - z - 1) * width * height].x = index;
				voxels[x + y * width + (length - z - 1) * width * height].y = 1;
			}
		}
		else if (id[0] == 'R' && id[1] == 'G' && id[2] == 'B' && id[3] == 'A') {
			for (unsigned int i = 0; i < 255; i++) {
				in.read((char*)(&palette[i]), 4);
			}
		}
		else {
			in.ignore(content + children);
		}
	}

	for (int x = 0; x < (int)width; x++) {
		for (int y = 0; y < (int)height; y++) {
			for (int z = 0; z < (int)length; z++) {
				if (voxels[x + y * width + z * width * height].y != 0) {
					voxels[x + y * width + z * width * height] = vt::Vec4b(palette[voxels[x + y * width + z * width * height].x - 1], 255);
				}
			}
		}
	}

	vt::IndexedMesh<VoxelVertex, unsigned int> mesh;
	mesh.addAttribute(vt::Attribute(vt::TYPE_FLOAT, 3));
	mesh.addAttribute(vt::Attribute(vt::TYPE_UNSIGNED_BYTE, 4, true));
	mesh.addAttribute(vt::Attribute(vt::TYPE_FLOAT, 3));

	const vt::Vec3f v0(0.0f, 0.0f, 0.0f);
	const vt::Vec3f v1(0.0f, 0.0f, 1.0f);
	const vt::Vec3f v2(1.0f, 0.0f, 1.0f);
	const vt::Vec3f v3(1.0f, 0.0f, 0.0f);
	const vt::Vec3f v4(0.0f, 1.0f, 0.0f);
	const vt::Vec3f v5(0.0f, 1.0f, 1.0f);
	const vt::Vec3f v6(1.0f, 1.0f, 1.0f);
	const vt::Vec3f v7(1.0f, 1.0f, 0.0f);

	const vt::Vec3f normalTop(0.0f, 1.0f, 0.0f);
	const vt::Vec3f normalBottom(0.0f, -1.0f, 0.0f);
	const vt::Vec3f normalLeft(-1.0f, 0.0f, 0.0f);
	const vt::Vec3f normalRight(1.0f, 0.0f, 0.0f);
	const vt::Vec3f normalFront(0.0f, 0.0f, 1.0f);
	const vt::Vec3f normalBack(0.0f, 0.0f, -1.0f);

	for (int x = 0; x < (int)width; x++) {
		for (int y = 0; y < (int)height; y++) {
			for (int z = 0; z < (int)length; z++) {
				vt::Vec3f o((float)x, (float)y, (float)z);
				vt::Vec4b color = getColor(x, y, z, voxels, width, height, length);
				if (color.w == 0) {
					continue;
				}

				vt::Vec4b u = getColor(x, y + 1, z, voxels, width, height, length);
				vt::Vec4b d = getColor(x, y - 1, z, voxels, width, height, length);
				vt::Vec4b l = getColor(x - 1, y, z, voxels, width, height, length);
				vt::Vec4b r = getColor(x + 1, y, z, voxels, width, height, length);
				vt::Vec4b f = getColor(x, y, z + 1, voxels, width, height, length);
				vt::Vec4b b = getColor(x, y, z - 1, voxels, width, height, length);

				if (u.w == 0) addQuad(mesh, x, y, z, voxels, width, height, length, v4 + o, v5 + o, v6 + o, v7 + o, color, normalTop); // Top
				if (d.w == 0) addQuad(mesh, x, y, z, voxels, width, height, length, v1 + o, v0 + o, v3 + o, v2 + o, color, normalBottom); // Bottom
				if (l.w == 0) addQuad(mesh, x, y, z, voxels, width, height, length, v4 + o, v0 + o, v1 + o, v5 + o, color, normalLeft); // Left
				if (r.w == 0) addQuad(mesh, x, y, z, voxels, width, height, length, v6 + o, v2 + o, v3 + o, v7 + o, color, normalRight); // Right
				if (f.w == 0) addQuad(mesh, x, y, z, voxels, width, height, length, v5 + o, v1 + o, v2 + o, v6 + o, color, normalFront); // Front
				if (b.w == 0) addQuad(mesh, x, y, z, voxels, width, height, length, v7 + o, v3 + o, v0 + o, v4 + o, color, normalBack); // Back
			}
		}
	}

	delete[] voxels;

	vt::GLIndexedModel model;
	model.initialize();
	mesh.createGLModel(model);
	return model;
}

vt::Vec4b ModelLoader::getColor(int x, int y, int z, vt::Vec4b* data, unsigned int width, unsigned int height, unsigned int length)
{
	if (x < 0 || y < 0 || z < 0 || x >= (int)width || y >= (int)height || z >= (int)length) {
		return vt::Vec4b();
	}
	return data[x + y * width + z * width * height];
}

void ModelLoader::addQuad(vt::IndexedMesh<VoxelVertex, unsigned int>& mesh, int x, int y, int z, vt::Vec4b* data, unsigned int width, unsigned int height, unsigned int length, const vt::Vec3f& p0, const vt::Vec3f& p1, const vt::Vec3f& p2, const vt::Vec3f& p3, const vt::Vec4b& color, const vt::Vec3f& normal)
{
	float ao0 = getAmbientOcclusion(x, y, z, data, width, height, length, p0, normal);
	float ao1 = getAmbientOcclusion(x, y, z, data, width, height, length, p1, normal);
	float ao2 = getAmbientOcclusion(x, y, z, data, width, height, length, p2, normal);
	float ao3 = getAmbientOcclusion(x, y, z, data, width, height, length, p3, normal);
	vt::Vec4b c0 = vt::Vec4b((vt::Vec3b)((vt::Vec3f)color.getXYZ() * ao0), 255);
	vt::Vec4b c1 = vt::Vec4b((vt::Vec3b)((vt::Vec3f)color.getXYZ() * ao1), 255);
	vt::Vec4b c2 = vt::Vec4b((vt::Vec3b)((vt::Vec3f)color.getXYZ() * ao2), 255);
	vt::Vec4b c3 = vt::Vec4b((vt::Vec3b)((vt::Vec3f)color.getXYZ() * ao3), 255);
	VoxelVertex v0(p0, c0, normal);
	VoxelVertex v1(p1, c1, normal);
	VoxelVertex v2(p2, c2, normal);
	VoxelVertex v3(p3, c3, normal);
	if (ao0 + ao2 > ao1 + ao3) {
		mesh.quad(v0, v1, v2, v3);
	}
	else {
		mesh.quad(v1, v2, v3, v0);
	}
}

float ModelLoader::getAmbientOcclusion(int x, int y, int z, vt::Vec4b* data, int width, int height, int length, const vt::Vec3f& vertex, const vt::Vec3f& normal)
{
	int xo = normal.x > 0 ? 0 : -AMBIENT_OCCLUSION_SAMPLE_AREA;
	int yo = normal.y > 0 ? 0 : -AMBIENT_OCCLUSION_SAMPLE_AREA;
	int zo = normal.z > 0 ? 0 : -AMBIENT_OCCLUSION_SAMPLE_AREA;
	int xs = normal.x < 0 ? 0 : AMBIENT_OCCLUSION_SAMPLE_AREA;
	int ys = normal.y < 0 ? 0 : AMBIENT_OCCLUSION_SAMPLE_AREA;
	int zs = normal.z < 0 ? 0 : AMBIENT_OCCLUSION_SAMPLE_AREA;

	float ambientOcclusion = 0.0f;
	float numSamples = 0.0f;

	for (int x = (int)vertex.x + xo; x < (int)vertex.x + xs; x++) {
		for (int y = (int)vertex.y + yo; y < (int)vertex.y + ys; y++) {
			for (int z = (int)vertex.z + zo; z < (int)vertex.z + zs; z++) {
				float l = abs(vertex.x - 0.5f - (float)x) + abs(vertex.y - 0.5f - (float)y) + abs(vertex.z - 0.5f - (float)z);
				float sample = 1.0f / l;
				if (getColor(x, y, z, data, width, height, length).w == 0) {
					ambientOcclusion += sample;
				}
				numSamples += sample;
			}
		}
	}
	return ambientOcclusion / numSamples;
}