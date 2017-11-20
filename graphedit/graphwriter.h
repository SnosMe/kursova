#ifndef GRAPHWRITER_H
#define GRAPHWRITER_H

#include <QXmlStreamWriter>
#include "graph.h"

class GraphWriter {
    Graph g;
    QXmlStreamWriter* writer;
    void writeNodes();
    void writeEdges();
    void writeKeys();

public:
      GraphWriter(const Graph& g, QIODevice& device);
      ~GraphWriter();
      void writeToFile();

private:

};

#endif // GRAPHWRITER_H
