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

#include "rst/strings/format.h"

#include <algorithm>
#include <cstring>

#include "rst/check/check.h"
#include "rst/macros/optimization.h"
#include "rst/stl/resize_uninitialized.h"

namespace rst {
namespace internal {

std::string FormatAndReturnString(const NotNull<const char*> not_null_format,
                                  const size_t format_size,
                                  const Nullable<const Arg*> values,
                                  const size_t size) {
  auto format = not_null_format.get();

  RST_DCHECK(format_size == std::strlen(format));
  auto new_size = format_size;
  for (size_t i = 0; i < size; i++) {
    RST_DCHECK(values != nullptr);
    new_size += values[i].size();
  }
  RST_DCHECK(new_size >= size * 2);
  new_size -= size * 2;

  std::string output;
  StringResizeUninitialized(NotNull(&output), new_size);

  size_t arg_idx = 0;
  auto target = output.data();
  for (auto c = '\0'; (c = *format) != '\0'; format++) {
    switch (c) {
      case '{': {
        switch (RST_LIKELY_EQ(*(format + 1), '}')) {
          case '{': {
            format++;
            *target++ = '{';
            break;
          }
          case '}': {
            RST_DCHECK(arg_idx < size && "Extra arguments");
            const auto src = values[arg_idx].view();
            target = std::copy_n(src.data(), src.size(), target);
            format++;
            arg_idx++;
            break;
          }
          default: {
            RST_DCHECK(false && "Invalid format string");
          }
        }
        break;
      }
      case '}': {
        switch (*(format + 1)) {
          case '}': {
            format++;
            *target++ = '}';
            break;
          }
          default: {
            RST_DCHECK(false && "Unmatched '}' in format string");
          }
        }
        break;
      }
      default: {
        *target++ = c;
        break;
      }
    }
  }

  RST_DCHECK(arg_idx == size && "Numbers of parameters should match");

  output.resize(static_cast<size_t>(target - output.data()));
  return output;
}

}  // namespace internal
}  // namespace rst
