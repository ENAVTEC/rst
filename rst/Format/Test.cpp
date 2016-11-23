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

#include <limits>
#include <string>

#include <gtest/gtest.h>

#include "Format.h"

using std::numeric_limits;
using std::string;

using namespace rst;
using namespace rst::literals;
using namespace internal;

TEST(FormatterTest, Escape) {
  EXPECT_EQ("{", format("{{"));
  EXPECT_EQ("before {", format("before {{"));
  EXPECT_EQ("{ after", format("{{ after"));
  EXPECT_EQ("before { after", format("before {{ after"));

  EXPECT_EQ("}", format("}}"));
  EXPECT_EQ("before }", format("before }}"));
  EXPECT_EQ("} after", format("}} after"));
  EXPECT_EQ("before } after", format("before }} after"));

  EXPECT_EQ("{}", format("{{}}"));
  EXPECT_EQ("{42}", format("{{{}}}", 42));
}

TEST(FormatterTest, UnmatchedBraces) {
  EXPECT_THROW(format("{"), FormatError);

  EXPECT_THROW(format("}"), FormatError);

  EXPECT_THROW(format("{0{}"), FormatError);
}

TEST(FormatterTest, UnmatchedBraces2) {
  EXPECT_THROW(format("{", 1), FormatError);

  EXPECT_THROW(format("}", 1), FormatError);

  EXPECT_THROW(format("{0{}", 1), FormatError);
}

TEST(FormatterTest, NoArgs) { EXPECT_EQ("test", format("test")); }

TEST(FormatterTest, ArgsInDifferentPositions) {
  EXPECT_EQ("42", format("{}", 42));
  EXPECT_EQ("before 42", format("before {}", 42));
  EXPECT_EQ("42 after", format("{} after", 42));
  EXPECT_EQ("before 42 after", format("before {} after", 42));
  EXPECT_EQ("answer = 42", format("{} = {}", "answer", 42));
  EXPECT_EQ("42 is the answer", format("{} is the {}", 42, "answer"));
  EXPECT_EQ("abracadabra", format("{}{}{}", "abra", "cad", "abra"));
}

TEST(FormatterTest, ArgErrors) {
  EXPECT_THROW(format("{"), FormatError);

  EXPECT_THROW(format("{?}"), FormatError);

  EXPECT_THROW(format("{0"), FormatError);

  EXPECT_THROW(format("{}"), FormatError);
}

TEST(FormatterTest, ArgErrors2) {
  EXPECT_THROW(format("{", 1), FormatError);

  EXPECT_THROW(format("{?}", 1), FormatError);

  EXPECT_THROW(format("{0", 1), FormatError);
}

TEST(FormatTest, Variadic) { EXPECT_EQ("abc1", format("{}c{}", "ab", 1)); }

TEST(FormatTest, MaxArgs) {
  EXPECT_EQ("0123456789abcde1.2300001.230000",
            format("{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}", 0, 1, 2, 3, 4, 5, 6, 7,
                   8, 9, 'a', 'b', 'c', 'd', 'e', 1.23, 1.23f));
}

TEST(FormatTest, ExtraArgument) { EXPECT_THROW(format("", 1), FormatError); }

TEST(Literals, Commong) {
  EXPECT_EQ("", ""_format());
  EXPECT_EQ("1234 Hello, world",
            "{} {}, {}{}{}"_format(1234, "Hello", "wor", 'l', 'd'));
}

TEST(Writer, DefaultCtor) {
  Writer writer;
  EXPECT_EQ("", writer.CopyString());
  EXPECT_EQ("", writer.MoveString());
}

TEST(Writer, FormatAndWriteNullStr) {
  Writer writer;
  EXPECT_THROW(writer.FormatAndWrite(nullptr, 10, "format", 0), FormatError);
}

TEST(Writer, FormatAndWriteNullFormat) {
  Writer writer;
  char str[10];
  EXPECT_THROW(writer.FormatAndWrite(str, 10, nullptr, 0), FormatError);
}

TEST(Writer, FormatAndWriteZeroSize) {
  Writer writer;
  char str[10];
  writer.FormatAndWrite(str, 0, "%d", 0);
  EXPECT_EQ("", writer.CopyString());
}

TEST(Writer, FormatAndWriteOneSize) {
  Writer writer;
  char str[10];
  writer.FormatAndWrite(str, 1, "%d", 0);
  EXPECT_EQ("", writer.CopyString());
}

TEST(Writer, FormatAndWriteTwoSize) {
  Writer writer;
  char str[10];
  writer.FormatAndWrite(str, 2, "%d", 0);
  EXPECT_EQ("0", writer.CopyString());
}

TEST(Writer, FormatAndWriteThreeSize) {
  Writer writer;
  char str[10];
  writer.FormatAndWrite(str, 3, "%d", 10);
  EXPECT_EQ("10", writer.CopyString());
}

TEST(Writer, FormatAndWriteThreeSizeNotFull) {
  Writer writer;
  char str[10];
  writer.FormatAndWrite(str, 3, "%d", 103);
  EXPECT_EQ("10", writer.CopyString());
}

TEST(FormatTemplate, SNullPtr) {
  Writer writer;
  EXPECT_THROW(Format(writer, nullptr, 0), FormatError);
}

TEST(Writer, Numbers) {
  const short short_val = 0;
  const int int_val = 1;
  const long long_val = 2;
  const long long long_long_val = 3;
  const unsigned short unsigned_short_val = 4;
  const unsigned int unsigned_int_val = 5;
  const unsigned long unsigned_long_val = 6;
  const unsigned long long unsigned_long_long_val = 7;
  const float float_val = 8.0f;
  const double double_val = 9.0;
  const long double long_double_val = 10.0;

  Writer writer;
  writer.Write(short_val);
  writer.Write(int_val);
  writer.Write(long_val);
  writer.Write(long_long_val);
  writer.Write(unsigned_short_val);
  writer.Write(unsigned_int_val);
  writer.Write(unsigned_long_val);
  writer.Write(unsigned_long_long_val);
  writer.Write(float_val);
  writer.Write(double_val);
  writer.Write(long_double_val);
  EXPECT_EQ("012345678.0000009.00000010.000000", writer.CopyString());
}

TEST(Writer, MinMax) {
  const auto min_short_val = numeric_limits<short>::min();
  const auto max_short_val = numeric_limits<short>::max();
  const auto min_int_val = numeric_limits<int>::min();
  const auto max_int_val = numeric_limits<int>::max();
  const auto min_long_val = numeric_limits<long>::min();
  const auto max_long_val = numeric_limits<long>::max();
  const auto min_long_long_val = numeric_limits<long long>::min();
  const auto max_long_long_val = numeric_limits<long long>::max();
  const auto min_unsigned_short_val = numeric_limits<unsigned short>::min();
  const auto max_unsigned_short_val = numeric_limits<unsigned short>::max();
  const auto min_unsigned_int_val = numeric_limits<unsigned int>::min();
  const auto max_unsigned_int_val = numeric_limits<unsigned int>::max();
  const auto min_unsigned_long_val = numeric_limits<unsigned long>::min();
  const auto max_unsigned_long_val = numeric_limits<unsigned long>::max();
  const auto min_unsigned_long_long_val =
      numeric_limits<unsigned long long>::min();
  const auto max_unsigned_long_long_val =
      numeric_limits<unsigned long long>::max();
  const auto min_float_val = numeric_limits<float>::min();
  const auto max_float_val = numeric_limits<float>::max();
  const auto min_double_val = numeric_limits<double>::min();
  const auto max_double_val = numeric_limits<double>::max();
  const auto min_long_double_val = numeric_limits<long double>::min();
  const auto max_long_double_val = numeric_limits<long double>::max();

  Writer writer;
  writer.Write(min_short_val);
  writer.Write(max_short_val);
  writer.Write(min_int_val);
  writer.Write(max_int_val);
  writer.Write(min_long_val);
  writer.Write(max_long_val);
  writer.Write(min_long_long_val);
  writer.Write(max_long_long_val);
  writer.Write(min_unsigned_short_val);
  writer.Write(max_unsigned_short_val);
  writer.Write(min_unsigned_int_val);
  writer.Write(max_unsigned_int_val);
  writer.Write(min_unsigned_long_val);
  writer.Write(max_unsigned_long_val);
  writer.Write(min_unsigned_long_long_val);
  writer.Write(max_unsigned_long_long_val);
  writer.Write(min_float_val);
  writer.Write(max_float_val);
  writer.Write(min_double_val);
  writer.Write(max_double_val);
  writer.Write(min_long_double_val);
  writer.Write(max_long_double_val);

  string result;
  result += std::to_string(numeric_limits<short>::min());
  result += std::to_string(numeric_limits<short>::max());
  result += std::to_string(numeric_limits<int>::min());
  result += std::to_string(numeric_limits<int>::max());
  result += std::to_string(numeric_limits<long>::min());
  result += std::to_string(numeric_limits<long>::max());
  result += std::to_string(numeric_limits<long long>::min());
  result += std::to_string(numeric_limits<long long>::max());
  result += std::to_string(numeric_limits<unsigned short>::min());
  result += std::to_string(numeric_limits<unsigned short>::max());
  result += std::to_string(numeric_limits<unsigned int>::min());
  result += std::to_string(numeric_limits<unsigned int>::max());
  result += std::to_string(numeric_limits<unsigned long>::min());
  result += std::to_string(numeric_limits<unsigned long>::max());
  result += std::to_string(numeric_limits<unsigned long long>::min());
  result += std::to_string(numeric_limits<unsigned long long>::max());
  result += std::to_string(numeric_limits<float>::min());
  result += std::to_string(numeric_limits<float>::max());
  result += std::to_string(numeric_limits<double>::min());
  result += std::to_string(numeric_limits<double>::max());
  result += std::to_string(numeric_limits<long double>::min());
  result += std::to_string(numeric_limits<long double>::max());

  EXPECT_EQ(result, writer.CopyString());
}

TEST(Writer, EmptyStdString) {
  Writer writer;
  const string str;
  writer.Write(str);
  EXPECT_EQ("", writer.CopyString());
}

TEST(Writer, EmptyCharPtr) {
  Writer writer;
  const char* str = "";
  writer.Write(str);
  EXPECT_EQ("", writer.CopyString());
}

TEST(Writer, NullCharPtr) {
  Writer writer;
  EXPECT_THROW(writer.Write(nullptr), FormatError);
}

TEST(Writer, NormalStdString) {
  Writer writer;
  const string str = "Normal";
  writer.Write(str);
  EXPECT_EQ("Normal", writer.CopyString());
}

TEST(Writer, NormalCharPtr) {
  Writer writer;
  const char* str = "Normal";
  writer.Write(str);
  EXPECT_EQ("Normal", writer.CopyString());
}

TEST(Writer, NormalChar) {
  Writer writer;
  const char c = 'C';
  writer.Write(c);
  EXPECT_EQ("C", writer.CopyString());
}

TEST(Writer, BigStdString) {
  Writer writer;
  const string str(Writer::kStaticBufferSize, 'A');
  writer.Write(str);
  EXPECT_EQ(str, writer.CopyString());
}

TEST(Writer, BigCharPtr) {
  Writer writer;
  const string str(Writer::kStaticBufferSize, 'A');
  writer.Write(str.c_str());
  EXPECT_EQ(str, writer.CopyString());
}

TEST(Writer, AppendBigStdString) {
  Writer writer;
  const string initial_str = "Initial";
  const string str(Writer::kStaticBufferSize, 'A');
  writer.Write(initial_str);
  writer.Write(str);
  writer.Write(initial_str);
  EXPECT_EQ(initial_str + str + initial_str, writer.CopyString());
}

TEST(Writer, AppendBigCharPtr) {
  Writer writer;
  const char* initial_str = "Initial";
  const string str(Writer::kStaticBufferSize, 'A');
  writer.Write(initial_str);
  writer.Write(str.c_str());
  writer.Write(initial_str);
  EXPECT_EQ(initial_str + str + initial_str, writer.CopyString());
}

TEST(Writer, AppendBigChar) {
  Writer writer;
  const string str(Writer::kStaticBufferSize, 'A');
  writer.Write(str);
  const char c = 'C';
  writer.Write(c);
  EXPECT_EQ(str + c, writer.CopyString());
}

TEST(Writer, WriteNullPtr) {
  Writer writer;
  EXPECT_THROW(writer.Write(nullptr, 10), FormatError);
}

TEST(Writer, WriteZeroSize) {
  Writer writer;
  const char* str = "Initial";
  writer.Write(str, 0);
  EXPECT_EQ("", writer.CopyString());
}

TEST(Writer, MoveStringStatic) {
  Writer writer;
  writer.Write("Initial");
  EXPECT_EQ("Initial", writer.MoveString());
  EXPECT_THROW(writer.MoveString(), FormatError);
}

TEST(Writer, MoveStringDynamic) {
  Writer writer;
  const string str(Writer::kStaticBufferSize, 'A');
  writer.Write(str);
  EXPECT_EQ(str, writer.MoveString());
  EXPECT_THROW(writer.MoveString(), FormatError);
}

TEST(Writer, CopyStringStatic) {
  Writer writer;
  writer.Write("Initial");
  EXPECT_EQ("Initial", writer.CopyString());
  EXPECT_EQ("Initial", writer.CopyString());
}

TEST(Writer, CopyStringDynamic) {
  Writer writer;
  const string str(Writer::kStaticBufferSize, 'A');
  writer.Write(str);
  EXPECT_EQ(str, writer.CopyString());
  EXPECT_EQ(str, writer.CopyString());
}

TEST(HandleCharacter, SNullPtr) {
  const char* s = nullptr;
  EXPECT_THROW(HandleCharacter('a', s), FormatError);
}

TEST(HandleCharacter, AnotherString) {
  const char* s = "abc";
  EXPECT_THROW(HandleCharacter('d', s), FormatError);
}

TEST(HandleCharacter, CaseOpenOpen) {
  const char* s = "{{";
  EXPECT_TRUE(HandleCharacter(*s, s));
  EXPECT_STREQ("{", s);
}

TEST(HandleCharacter, CaseOpenClose) {
  const char* s = "{}";
  EXPECT_FALSE(HandleCharacter(*s, s));
  EXPECT_STREQ("{}", s);
}

TEST(HandleCharacter, CaseOpenOther) {
  const char* s = "{o";
  EXPECT_THROW(HandleCharacter(*s, s), FormatError);
}

TEST(HandleCharacter, CaseCloseClose) {
  const char* s = "}}";
  EXPECT_TRUE(HandleCharacter(*s, s));
  EXPECT_STREQ("}", s);
}

TEST(HandleCharacter, CaseCloseOther) {
  const char* s = "}o";
  EXPECT_THROW(HandleCharacter(*s, s), FormatError);
}

TEST(Format, SNullPtr) {
  Writer writer;
  EXPECT_THROW(Format(writer, nullptr), FormatError);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}