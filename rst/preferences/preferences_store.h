// Copyright (c) 2019, Sergey Abbakumov
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

#ifndef RST_PREFERENCES_PREFERENCES_STORE_H_
#define RST_PREFERENCES_PREFERENCES_STORE_H_

#include <string_view>

#include "rst/not_null/not_null.h"
#include "rst/value/value.h"

namespace rst {

// Interface for reading and writing from/to a persistent preferences store.
//
// A |path| used in the class has the form "<key>" or "<key>.<key>.[...]",
// where "." indexes into the next Value down.
class PreferencesStore {
 public:
  virtual ~PreferencesStore();

  // Returns a value for a given preference |path|.
  virtual Nullable<const Value*> GetValue(std::string_view path) const = 0;
  // Sets a |value| for a |path| in the store.
  virtual void SetValue(std::string_view path, Value&& value) = 0;
};

}  // namespace rst

#endif  // RST_PREFERENCES_PREFERENCES_STORE_H_
