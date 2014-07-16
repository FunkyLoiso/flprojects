#ifndef KOSARAJUSSCCS_H
#define KOSARAJUSSCCS_H

#include "Graph.h"

class KosarajusSCCs
{
public:
    KosarajusSCCs(const DirectedWeightedGraph& graph);
    std::list<std::vector<int>> SCCs() const;

private:
    std::list<std::vector<int>> m_sccs;
};

#endif // KOSARAJUSSCCS_H
