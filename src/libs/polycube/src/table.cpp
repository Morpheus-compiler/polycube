/*
 * Copyright 2017 The Polycube Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "polycube/services/table.h"

#include <libbpf/src/bpf.h>
#include <api/BPFTable.h>

using ebpf::BPFTable;

namespace polycube {
namespace service {

// TRADITIONAL eBPF MAPS impl
// new api
class RawTable::impl {
 public:
  explicit impl(void *op);
  ~impl() = default;

  void get(const void *key, void *value);
  void set(const void *key, const void *value);
  void remove(const void *key);

  int get_batch(void *keys, void *values, unsigned int *count, void *in_batch = nullptr, void *out_batch = nullptr) const;
  int get_and_delete_batch(void *keys, void *values, unsigned int *count, void *in_batch = nullptr, void *out_batch = nullptr) const;
  int update_batch(void *keys, void *values, unsigned int *count) const;

  int get_fd() const;

  int first(void *key) const;
  int next(const void *key, void *next);

 private:
  int fd_;
};



RawTable::impl::impl(void *op) : fd_(*(int *)op) {}

void RawTable::impl::get(const void *key, void *value) {
  if (bpf_lookup_elem(fd_, const_cast<void *>(key),
                      const_cast<void *>(value))) {
    throw std::runtime_error("Table get error: " +
                             std::string(std::strerror(errno)));
  }
}

void RawTable::impl::set(const void *key, const void *value) {
  if (bpf_update_elem(fd_, const_cast<void *>(key), const_cast<void *>(value),
                      0)) {
    throw std::runtime_error("Table set error: " +
                             std::string(std::strerror(errno)));
  }
}

void RawTable::impl::remove(const void *key) {
  if (bpf_delete_elem(fd_, const_cast<void *>(key))) {
    throw std::runtime_error("Table remove error: " +
                             std::string(std::strerror(errno)));
  }
}

int RawTable::impl::first(void *key) const {
  /* TODO: where to get key size from? */
  return bpf_get_first_key(fd_, key, 32 /*sizeof(first)*/);
}

int RawTable::impl::next(const void *key, void *next) {
  return bpf_get_next_key(fd_, const_cast<void *>(key), next);
}

int RawTable::impl::get_batch(void *keys, void *values, unsigned int *count, void *in_batch, void *out_batch) const {
  if(out_batch == nullptr) out_batch = count;
  return bpf_map_lookup_batch(fd_, in_batch, out_batch, keys, values, count, nullptr);
}

int RawTable::impl::get_and_delete_batch(void *keys, void *values, unsigned int *count, void *in_batch, void *out_batch) const {
  if(out_batch == nullptr) out_batch = count;
  return bpf_map_lookup_and_delete_batch(fd_, in_batch, out_batch, keys, values, count, nullptr);
}

int RawTable::impl::update_batch(void *keys, void *values, unsigned int *count) const {
  return bpf_map_update_batch(fd_, keys, values, count, nullptr);
}

int RawTable::impl::get_fd() const {
  return fd_;
}

// QUEUE/STACK eBPF maps impl
class RawQueueStackTable::impl {
 public:
  explicit impl(void *op);
  ~impl() = default;

  int pop(void *value) const;
  int push(const void *value) const;

 private:
  int fd_;
};

RawQueueStackTable::impl::impl(void *op) : fd_(*(int *)op) {
  if(!polycubed::utils::check_kernel_version(QUEUE_STACK_KERNEL_RELEASE))
    throw std::runtime_error("eBPF map Type Queue/Stack is not supported in your"
        "current kernel version");
}

int RawQueueStackTable::impl::pop(void *value) const {
  return bpf_map_lookup_and_delete_elem(fd_, nullptr, value);
}

int RawQueueStackTable::impl::push(const void *value) const {
  return bpf_map_update_elem(fd_, nullptr, value, 0);
}


// PIMPL for traditional maps
RawTable::~RawTable() = default;

RawTable::RawTable(void *op) : pimpl_(new impl(op)) {}

void RawTable::get(const void *key, void *value) {
  return pimpl_->get(key, value);
}

void RawTable::set(const void *key, const void *value) {
  return pimpl_->set(key, value);
}

void RawTable::remove(const void *key) {
  return pimpl_->remove(key);
}

int RawTable::first(void *key) {
  return pimpl_->first(key);
}

int RawTable::next(const void *key, void *next) {
  return pimpl_->next(key, next);
}

int RawTable::get_batch(void *keys, void *values, unsigned int *count, void *in_batch, void *out_batch){
  return pimpl_->get_batch(keys, values, count, in_batch, out_batch);
}

int RawTable::get_and_delete_batch(void *keys, void *values, unsigned int *count, void *in_batch, void *out_batch){
  return pimpl_->get_and_delete_batch(keys, values, count, in_batch, out_batch);
}

int RawTable::update_batch(void *keys, void *values, unsigned int *count){
  return pimpl_->update_batch(keys, values, count);
}

int RawTable::get_fd() const {
  return pimpl_->get_fd();
}

//PIMPL for QUEUE/STACK maps

RawQueueStackTable::~RawQueueStackTable() = default;

RawQueueStackTable::RawQueueStackTable(void *op) : pimpl_(new impl(op)) {}

int RawQueueStackTable::pop(void *value) {
  return pimpl_->pop(value);
}

int RawQueueStackTable::push(const void *value) {
  return pimpl_->push(value);
}

// QUEUE/STACK eBPF maps impl
class MapInMapTable::impl {
 public:
  explicit impl(void *op);
  ~impl() = default;

  bool update(const int &index, const int &inner_map_fd);
  bool remove(const int &index);

  int get_fd();

 private:
  int fd_;
};

MapInMapTable::impl::impl(void *op) : fd_(*(int *)op) {}

bool MapInMapTable::impl::update(const int &index, const int &inner_map_fd) {
  return bpf_map_update_elem(fd_, static_cast<const void*>(&index), static_cast<const void*>(&inner_map_fd), 0) >= 0;
}

bool MapInMapTable::impl::remove(const int &index) {
  return bpf_map_delete_elem(fd_, static_cast<const void*>(&index)) >= 0;
}

int MapInMapTable::impl::get_fd() {
  return fd_;
}

MapInMapTable::~MapInMapTable() = default;

MapInMapTable::MapInMapTable(void *op) : pimpl_(new impl(op)) {}

bool MapInMapTable::update(const int &index, const int &inner_map_fd) {
  return pimpl_->update(index, inner_map_fd);
}

bool MapInMapTable::remove(const int &index) {
  return pimpl_->remove(index);
}

int MapInMapTable::get_fd() {
  return pimpl_->get_fd();
}

}  // namespace service
}  // namespace polycube
