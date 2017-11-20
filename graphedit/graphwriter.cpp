#include "graphwriter.h"

GraphWriter::GraphWriter(const Graph& g, QIODevice& device)
{
    this->g = g;
    writer = new QXmlStreamWriter(&device);
    writer->setAutoFormatting(true);
    writer->setAutoFormattingIndent(2);
    writer->setCodec("UTF-8");
}

GraphWriter::~GraphWriter()
{
    delete writer;
}

void GraphWriter::writeToFile()
{
    writer->writeStartDocument();

    writer->writeStartElement("graphml");
    writer->writeAttribute("xmlns", "http://graphml.graphdrawing.org/xmlns");
    writer->writeNamespace("http://www.w3.org/2001/XMLSchema-instance", "xsi");
    writer->writeNamespace("http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd", "schemaLocation");

    writeKeys();

    writer->writeStartElement("graph");
    writer->writeAttribute("edgedefault", "undirected");

    writeNodes();
    writeEdges();

    writer->writeEndDocument();
}

void GraphWriter::writeKeys()
{
    writer->writeStartElement("key");
    writer->writeAttribute("id", "x");
    writer->writeAttribute("for", "node");
    writer->writeAttribute("attr.name", "x");
    writer->writeAttribute("attr.type", "float");
    writer->writeEndElement();

    writer->writeStartElement("key");
    writer->writeAttribute("id", "y");
    writer->writeAttribute("for", "node");
    writer->writeAttribute("attr.name", "y");
    writer->writeAttribute("attr.type", "float");
    writer->writeEndElement();

    writer->writeStartElement("key");
    writer->writeAttribute("id", "weight");
    writer->writeAttribute("for", "edge");
    writer->writeAttribute("attr.name", "weight");
    writer->writeAttribute("attr.type", "int");
    writer->writeEndElement();
}

void GraphWriter::writeNodes()
{
    for (auto& node : g.nodes)
    {
        writer->writeStartElement("node");
        writer->writeAttribute("id", QString::number(node.id));

        writer->writeStartElement("data");
        writer->writeAttribute("key", "x");
        writer->writeCharacters(QString::number(node.x));
        writer->writeEndElement();

        writer->writeStartElement("data");
        writer->writeAttribute("key", "y");
        writer->writeCharacters(QString::number(node.y));
        writer->writeEndElement();

        writer->writeEndElement();
    }
}

void GraphWriter::writeEdges()
{
    for (auto& edge : g.edges)
    {
        writer->writeStartElement("edge");
        writer->writeAttribute("source", QString::number(edge.node1->id));
        writer->writeAttribute("target", QString::number(edge.node2->id));

        writer->writeStartElement("data");
        writer->writeAttribute("key", "weight");
        writer->writeCharacters(QString::number(edge.w));
        writer->writeEndElement();

        writer->writeEndElement();
    }
}
