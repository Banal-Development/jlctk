// -*- c++ -*-
#pragma once

template <class T> struct VectorSegment
{
  size_t v_size;
  atomic<size_t> end_idx;
  T v[0];

  static size_t get_segment_size(size_t v_size);
  static VectorSegment<T>* create_mmap_segment(const char* fn, size_t v_size);
};

template <class T> inline size_t VectorSegment<T>::get_segment_size(size_t v_size)
{
  return sizeof(VectorSegment<T>) + sizeof(T) * v_size;
}

template <class T> inline VectorSegment<T>* VectorSegment<T>::create_mmap_segment(const char* fn, size_t v_size)
{
  auto mmap_seg = ::create_mmap_segment(fn, sizeof(VectorSegment<T>) + sizeof(T) * v_size);
  auto ret = (VectorSegment<T>*)mmap_seg.start_addr;
  ret->v_size = v_size;
  ret->end_idx = 0;
  return ret;
}
