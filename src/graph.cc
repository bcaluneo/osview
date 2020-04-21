#include "graph.hh"

Graph::Graph(int dataCount) : _dataCount(dataCount) {
  rects = new SDL_Rect[dataCount];
}

Graph::~Graph() {
  delete [] rects;
}

void Graph::draw() {

}
