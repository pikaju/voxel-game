#pragma once

#include <vector>

#include "VoxelArrayContainer.h"
#include "VoxelArrayBorderContainer.h"

template<typename Data, typename Amount>
class VoxelRunLength
{
public:
	VoxelRunLength(const Data& data, const Amount& amount) : m_data(data), m_amount(amount) {  }
	~VoxelRunLength() {  }

	inline const Data& getData() const { return m_data; }
	inline const Amount& getAmount() const { return m_amount; }

	inline void setAmount(const Amount& amount) { m_amount = amount; }
private:
	Data m_data;
	Amount m_amount;
};

template<typename T, unsigned int width, unsigned int height, unsigned int length>
class VoxelRunLengthContainer
{
public:
	VoxelRunLengthContainer() : m_data() { m_data.push_back(VoxelRunLength<T, unsigned short>(T(), width * height * length)); }
	~VoxelRunLengthContainer() {  }

	const T& getData(unsigned int x, unsigned int y, unsigned int z) const
	{
		unsigned int index = x + y * width + z * width * height;
		unsigned int pointer = 0;

		for (unsigned int i = 0; i < m_data.size(); i++) {
			pointer += m_data[i].getAmount();
			if (pointer > index) {
				return m_data[i].getData();
			}
		}

		return m_data.back().getData();
	}

	void setData(unsigned int x, unsigned int y, unsigned int z, const T& data)
	{
		unsigned int index = x + y * width + z * width * height;
		unsigned int pointer = 0;

		for (unsigned int i = 0; i < m_data.size(); i++) {
			pointer += m_data[i].getAmount();
			if (pointer > index) {
				if (pointer - index - 1 > 0) {
					m_data.emplace(m_data.begin() + i + 1, m_data[i].getData(), pointer - index - 1);
				}
				m_data[i].setAmount(m_data[i].getAmount() - (pointer - index));
				if (m_data[i].getAmount() <= 0) {
					m_data[i] = VoxelRunLength<T, unsigned short>(data, 1);
				}
				else {
					m_data.emplace(m_data.begin() + i + 1, data, 1);
				}
				break;
			}
		}
	}

	void compress(const VoxelArrayContainer<T, width, height, length>& data)
	{
		m_data.clear();
		m_data.emplace_back(data.getData(0), 1);
		for (unsigned int i = 1; i < width * height * length; i++) {
			if (data.getData(i) == m_data.back().getData()) {
				m_data.back().setAmount(m_data.back().getAmount() + 1);
			}
			else {
				m_data.emplace_back(data.getData(i), 1);
			}
		}
	}

	void decompress(const VoxelArrayContainer<T, width, height, length>& data)
	{
		unsigned int index = 0;
		for (unsigned int i = 0; i < m_data.size(); i++) {
			for (unsigned int j = 0; j < m_data[i].getAmount(); j++) {
				data.setData(index, m_data[i].getData());
				index++;
			}
		}
	}

	template<int border> void decompress(VoxelArrayBorderContainer<T, width, height, length, border>& data)
	{
		unsigned int index = 0;
		for (unsigned int i = 0; i < m_data.size(); i++) {
			for (unsigned int j = 0; j < m_data[i].getAmount(); j++) {
				data.setData(index, m_data[i].getData());
				index++;
			}
		}
	}
private:
	std::vector<VoxelRunLength<T, unsigned short>> m_data;
};