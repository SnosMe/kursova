#include "kruskal.h"

Kruskal::Kruskal(QList<Graph>* states)
    : Algorithm(states)
{
}

bool Kruskal::run()
{
    int m = gCopy.edges.size();
    int n = gCopy.nodes.size();

    QVector<GraphEdge*> g;
    for (GraphEdge& edge : gCopy.edges)
        g.append(&edge);

    qSort(g.begin(), g.end(), GraphEdge::lessThan);

    QHash<GraphNode*, int> tree_id;
    for (int i=0; i<n; ++i)
        tree_id.insert(&gCopy.nodes[i], i);

    for (int i=0; i<m; ++i)
    {
        GraphNode* a = g[i]->node1;
        GraphNode* b = g[i]->node2;

        if (tree_id.value(a) != tree_id.value(b))
        {
            g[i]->node1->existInTrees = true;
            g[i]->node2->existInTrees = true;
            g[i]->node1->color = ColorMode::BOLD;
            g[i]->node2->color = ColorMode::BOLD;
            g[i]->color = ColorMode::BOLD;
            states->append(gCopy);

            int old_id = tree_id.value(b);
            int new_id = tree_id.value(a);
            QMutableHashIterator<GraphNode*, int> j(tree_id);
            while (j.hasNext())
            {
                j.next();
                if (j.value() == old_id)
                {
                    j.value() = new_id;
                }
            }
        }
    }

    return true;
}
