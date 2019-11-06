// Copyright (c) 2017, Sergey Abbakumov
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef RST_STATUS_STATUS_OR_H_
#define RST_STATUS_STATUS_OR_H_

#include <new>
#include <optional>
#include <utility>

#include "rst/check/check.h"
#include "rst/macros/macros.h"
#include "rst/not_null/not_null.h"
#include "rst/status/status.h"

namespace rst {

template <class T>
class [[nodiscard]] StatusOr {
 public:
  StatusOr() = delete;

  StatusOr(StatusOr && other) noexcept
      : status_(std::move(other.status_)), value_(std::move(other.value_)) {
    RST_DCHECK(value_.has_value() ? (status_.error_ == nullptr) : true);
#if RST_BUILDFLAG(DCHECK_IS_ON)
    other.was_checked_ = true;
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)
  }

  template <class U>
  StatusOr(U && value) : value_(std::forward<U>(value)) {}

  StatusOr(Status status) : status_(std::move(status)) {
    RST_DCHECK(status_.error_ != nullptr);
  }

  ~StatusOr() {
#if RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(was_checked_);
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)
  }

  StatusOr& operator=(StatusOr&& rhs) noexcept {
#if RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(was_checked_);
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)

    status_ = std::move(rhs.status_);
    value_ = std::move(rhs.value_);

#if RST_BUILDFLAG(DCHECK_IS_ON)
    was_checked_ = false;
    rhs.was_checked_ = true;
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)

    return *this;
  }

  template <class U>
  StatusOr& operator=(U&& value) {
#if RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(was_checked_);
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)

    status_ = Status::OK();
    value_.emplace(std::forward<U>(value));

#if RST_BUILDFLAG(DCHECK_IS_ON)
    was_checked_ = false;
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)

    return *this;
  }

  StatusOr& operator=(Status status) {
#if RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(was_checked_);
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(status.error_ != nullptr);

    status_ = std::move(status);
    value_.reset();
#if RST_BUILDFLAG(DCHECK_IS_ON)
    was_checked_ = false;
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)

    return *this;
  }

  bool err() {
#if RST_BUILDFLAG(DCHECK_IS_ON)
    was_checked_ = true;
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)
    return status_.err();
  }

  T& operator*() {
#if RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(was_checked_);
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(value_.has_value());

    return *value_;
  }

  NotNull<T*> operator->() {
#if RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(was_checked_);
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(value_.has_value());

    return &*value_;
  }

  Status TakeStatus()&& {
#if RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(was_checked_);
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(status_.error_ != nullptr);

    return std::move(status_);
  }

  const Status& status() const {
#if RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(was_checked_);
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)
    RST_DCHECK(status_.error_ != nullptr);

    return status_;
  }

  void Ignore() {
#if RST_BUILDFLAG(DCHECK_IS_ON)
    was_checked_ = true;
    status_.was_checked_ = true;
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)
  }

 private:
  Status status_;
  std::optional<T> value_;

#if RST_BUILDFLAG(DCHECK_IS_ON)
  bool was_checked_ = false;
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)

  RST_DISALLOW_COPY_AND_ASSIGN(StatusOr);
};

}  // namespace rst

#endif  // RST_STATUS_STATUS_OR_H_