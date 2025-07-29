#include "gui/guilayoutbuffer.hpp"

GUILayoutBuffer::GUILayoutBuffer() : head(0), buffer() {}

void GUILayoutBuffer::allocate(const size_t size) {
  head = 0;

  if (size > buffer.size()) {
    buffer.resize(size);
  }
}

bool GUILayoutBuffer::push(GUILayoutData data) {
  if (head < static_cast<int>(buffer.size())) {
    buffer[head++] = data;

    return true;
  }

  return false;
}
