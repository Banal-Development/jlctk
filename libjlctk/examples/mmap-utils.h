// -*- c++ -*-
#pragma once

#include <cstddef>

struct mmap_descr_t
{
  int fd{-1};
  void* start_addr{nullptr};
  std::size_t segment_size{0};
};

constexpr std::size_t operator""_GB(unsigned long long v){ return v*1024*1024*1024; }

mmap_descr_t create_mmap_segment(const char* fn, std::size_t seg_size);
