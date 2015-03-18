#include "CCanvas.hpp"

Canvas::Canvas() {
	bufferID = 0;
}
Canvas::~Canvas() {}

void Canvas::SetSize(const unsigned int width, const unsigned int height) {
	buffers[0].SetSize(width, height);
	buffers[1].SetSize(width, height);
}
void Canvas::GetSize(unsigned int& width, unsigned int& height) {
	GetBuffer().GetSize(width, height);
}

// Coordinate Space Context
void Canvas::Push() {}
void Canvas::Pop() {}
void Canvas::Translate(const unsigned int x, const unsigned int y) {}
void Canvas::Rotate(const float radians) {}
void Canvas::Scale(const unsigned int sx, const unsigned int sy) {}

// Drawing
void Canvas::Draw(const TextBuffer buffer, const unsigned int x, const unsigned int y) {
	printf("Rendering to buffer %d...\n", bufferID);
	GetBuffer().Put(x, y, buffer);
}

// Maintenance
void Canvas::Clear() {
	printf("Clearing buffer %d...\n", bufferID);
	GetBuffer().Clear();
}
void Canvas::Swap() {
	switch(bufferID) {
		case 0:
			bufferID = 1;
			break;
		case 1:
			bufferID = 0;
			break;
		default:
			break;
	}
}

TextBuffer& Canvas::GetBuffer() {
	printf("Giving buffer %d...\n", bufferID);
	return buffers[bufferID];
}

