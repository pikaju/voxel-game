#pragma once

#include <vt/math/Vec3.h>
#include <vt/math/Vec4.h>
#include <vt/util/String.h>

class VoxelVertex
{
public:
	VoxelVertex(const vt::Vec3f& position, const vt::Vec4b& color, const vt::Vec3f& normal) : m_position(position), m_color(color), m_normal(normal) {  }
	~VoxelVertex() {  }
private:
	vt::Vec3f m_position;
	vt::Vec4b m_color;
	vt::Vec3f m_normal;
};

typedef unsigned char VoxelID;
typedef unsigned char VoxelMetadata;

class VoxelData
{
public:
	VoxelData(const VoxelID& id = 0, const VoxelMetadata& metadata = 0) : m_id(id), m_metadata(metadata) {  }
	~VoxelData() {  }

	inline const VoxelID& getID() const { return m_id; }
	inline const VoxelMetadata& getMetadata() const { return m_metadata; }

	inline bool operator==(const VoxelData& v) const { return m_id == v.m_id && m_metadata == v.m_metadata; }
private:
	VoxelID m_id;
	VoxelMetadata m_metadata;
};

class Voxel
{
public:
	Voxel(const vt::String& name);
	~Voxel();

	inline const VoxelID& getID() const { return m_id; }
	inline const vt::String& getName() const { return m_name; }
	virtual inline bool isOpaque() const { return false; }

	inline void setID(const VoxelID& id) { m_id = id; }

	inline operator int() const { return m_id; }
	inline operator VoxelData() const { return VoxelData(m_id); }
private:
	VoxelID m_id;
	vt::String m_name;
};