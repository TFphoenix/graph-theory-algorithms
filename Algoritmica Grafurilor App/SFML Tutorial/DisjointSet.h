#pragma once
#include <unordered_map>

class DisjointSet
{
public:
	void MakeSet(uint16_t nrOfElements);
	uint16_t Find(uint16_t element);
	void Union(uint16_t element1, uint16_t element2);

private:
	std::unordered_map<uint16_t, uint16_t> m_parent;
};

