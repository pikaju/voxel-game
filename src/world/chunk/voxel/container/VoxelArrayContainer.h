#pragma once

template<typename T, unsigned int width, unsigned int height, unsigned int length>
class VoxelArrayContainer
{
public:
	VoxelArrayContainer() { memset(m_data, 0, sizeof(m_data)); }
	~VoxelArrayContainer() {  }

	inline const T& getData(unsigned int x, unsigned int y, unsigned int z) const { return m_data[x + y * width + z * width * height]; }
	inline const T& getData(unsigned int index) const { return m_data[index]; }
	inline void setData(unsigned int x, unsigned int y, unsigned int z, const T& data) { m_data[x + y * width + z * width * height] = data; }
	inline void setData(unsigned int index, const T& data) { m_data[index] = data; }
private:
	T m_data[width * height * length];
};