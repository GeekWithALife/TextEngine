
void Canvas::Canvas() {}
void Canvas::~Canvas() {}

void Canvas::SetSize(const unsigned int width, const unsigned int height) {}
void Canvas::GetSize(unsigned int& width, unsigned int& height) {}

// Coordinate Space Context
void Canvas::Push() {}
void Canvas::Pop() {}
void Canvas::Translate(const unsigned int x, const unsigned int y) {}
void Canvas::Rotate(const float radians) {}
void Canvas::Scale(const unsigned int sx, const unsigned int sy) {}

// Drawing
void Canvas::Draw(const TextBuffer buffer, const unsigned int x, const unsigned int y) {}

// Maintenance
void Canvas::Clear() {}
void Canvas::Swap() {}

TextBuffer Canvas::GetBuffer() {}

