#include "graphreader.h"

GraphReader::GraphReader(QIODevice& device)
{
    reader.setDevice(&device);
}

Graph& GraphReader::readFromFile()
{
    while (reader.readNextStartElement())
    {
        if (reader.name() == "node") {
            readNode();
        }
        else if (reader.name() == "edge") {
            readEdge();
        }
        else if (reader.name() != "graph" &&
                 reader.name() != "graphml") {
            reader.skipCurrentElement();
        }
    }

    return this->g;
}

void GraphReader::readNode()
{
    GraphNode node;

    for(QXmlStreamAttribute& attr : reader.attributes())
    {
        if (attr.name() == "id") {
            node.id = attr.value().toInt();
        }
    }

    while(reader.readNextStartElement())
    {
        if (reader.name() == "data")
        {
            for(QXmlStreamAttribute& attr : reader.attributes())
            {
                if (attr.name() == "key") {
                    if (attr.value() == "x") {
                        node.x = reader.readElementText().toFloat();
                    }
                    if (attr.value() == "y") {
                        node.y = reader.readElementText().toFloat();
                    }
                }
            }
        }
    }
    g.AddNode(node.x, node.y, node.id);
}

void GraphReader::readEdge()
{
    GraphEdge edge;

    for(QXmlStreamAttribute& attr : reader.attributes())
    {
        if (attr.name() == "source") {
            edge.node1 = g.GetNodeByID(attr.value().toInt());
        }
        if (attr.name() == "target") {
            edge.node2 = g.GetNodeByID(attr.value().toInt());
        }
    }

    while(reader.readNextStartElement())
    {
        if (reader.name() == "data")
        {
            for(QXmlStreamAttribute& attr : reader.attributes())
            {
                if (attr.name() == "key") {
                    if (attr.value() == "weight") {
                        edge.w = reader.readElementText().toInt();
                    }
                }
            }
        }
    }
    g.AddEdge(edge.node1, edge.node2, edge.w);
}
