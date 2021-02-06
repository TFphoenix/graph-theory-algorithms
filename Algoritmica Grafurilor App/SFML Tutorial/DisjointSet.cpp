#include "DisjointSet.h"

void DisjointSet::MakeSet(uint16_t nrOfElements)
{
	for (uint16_t element = 0; element < nrOfElements; ++element)
		m_parent[element] = element;
}

uint16_t DisjointSet::Find(uint16_t element)
{
	if (m_parent[element] == element)
		return element;
	return Find(m_parent[element]);
}

void DisjointSet::Union(uint16_t element1, uint16_t element2)
{
	const uint16_t uniteX = Find(element1);
	const uint16_t uniteY = Find(element2);
	m_parent[uniteX] = uniteY;
}
