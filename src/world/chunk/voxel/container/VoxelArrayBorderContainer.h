#pragma once

template<typename T, int width, int height, int length, int border>
class VoxelArrayBorderContainer
{
public:
	VoxelArrayBorderContainer() { memset(m_data, 0, sizeof(m_data)); }
	~VoxelArrayBorderContainer() {  }

	inline const T& getData(int x, int y, int z) const { return m_data[x + border][y + border][z + border]; }
	inline const T& getData(unsigned int index) const { return getData(index % width, (index / width) % height, index / (width * height)); }
	inline void setData(int x, int y, int z, const T& data) { m_data[x + border][y + border][z + border] = data; }
	inline void setData(unsigned int index, const T& data) { setData(index % width, (index / width) % height, index / (width * height), data); }
private:
	T m_data[width + border * 2][height + border * 2][length + border * 2];
};