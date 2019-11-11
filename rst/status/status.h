// Copyright (c) 2016, Sergey Abbakumov
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

#ifndef RST_STATUS_STATUS_H_
#define RST_STATUS_STATUS_H_

#include <memory>
#include <string>
#include <utility>

#include "rst/check/check.h"
#include "rst/macros/macros.h"
#include "rst/not_null/not_null.h"
#include "rst/status/status.h"

namespace rst {

class ErrorInfoBase {
 public:
  ErrorInfoBase();
  virtual ~ErrorInfoBase();

  static NotNull<const void*> GetClassID();

  virtual const std::string& AsString() const = 0;
  virtual NotNull<const void*> GetDynamicClassID() const = 0;

  virtual bool IsA(NotNull<const void*> class_id) const;

  template <class ErrorInfoT>
  bool IsA() const {
    return IsA(ErrorInfoT::GetClassID());
  }

 private:
  static char id_;

  RST_DISALLOW_COPY_AND_ASSIGN(ErrorInfoBase);
};

template <class T, class Parent = ErrorInfoBase>
class ErrorInfo : public Parent {
 public:
  using Parent::Parent;
  ~ErrorInfo() = default;

  static NotNull<const void*> GetClassID() { return &T::id_; }

  NotNull<const void*> GetDynamicClassID() const override { return &T::id_; }

  bool IsA(const NotNull<const void*> class_id) const override {
    return class_id == GetClassID() || Parent::IsA(class_id);
  }

 private:
  RST_DISALLOW_COPY_AND_ASSIGN(ErrorInfo);
};

// A Google-like Status class for error handling.
class [[nodiscard]] Status {
 public:
  static Status OK() { return Status(); }

  // Sets the object not checked by default and moves |other| content.
  Status(Status && other) noexcept;

  ~Status();

  // Sets the object not checked by default and moves |rhs| content.
  Status& operator=(Status&& rhs) noexcept;

  // Sets the object to be checked and returns whether the object is error
  // object.
  bool err() {
#if RST_BUILDFLAG(DCHECK_IS_ON)
    was_checked_ = true;
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)
    return error_ != nullptr;
  }

  NotNull<const ErrorInfoBase*> GetError() const;

  // Sets the object to be checked.
  void Ignore() {
#if RST_BUILDFLAG(DCHECK_IS_ON)
    was_checked_ = true;
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)
  }

 private:
  template <class T>
  friend class StatusOr;

  template <class Err, class... Args>
  friend Status MakeStatus(Args && ... args);

  // Sets the object not checked by default and to be OK.
  Status();

  // Sets the object not checked by default and to be the error object.
  Status(NotNull<std::unique_ptr<ErrorInfoBase>> error);

  // Information about the error. nullptr if the object is OK.
  Nullable<std::unique_ptr<ErrorInfoBase>> error_;

#if RST_BUILDFLAG(DCHECK_IS_ON)
  // Whether the object was checked.
  bool was_checked_ = false;
#endif  // RST_BUILDFLAG(DCHECK_IS_ON)

  RST_DISALLOW_COPY_AND_ASSIGN(Status);
};

template <class Err, class... Args>
Status MakeStatus(Args&&... args) {
  return Status(std::make_unique<Err>(std::forward<Args>(args)...));
}

}  // namespace rst

#endif  // RST_STATUS_STATUS_H_
