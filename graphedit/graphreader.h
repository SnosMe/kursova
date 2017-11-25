#ifndef GRAPHREADER_H
#define GRAPHREADER_H

#include <QXmlStreamReader>
#include "graph.h"

class GraphReader {
    Graph g;
    QXmlStreamReader reader;
    void readNode();
    void readEdge();

public:
      GraphReader(QIODevice& device);
      Graph& readFromFile();
};

#endif // GRAPHREADER_H
