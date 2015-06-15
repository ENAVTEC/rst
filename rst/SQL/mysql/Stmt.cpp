// Copyright (c) 2015, Sergey Abbakumov
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

#include "Stmt.h"

#include "Rows.h"
#include "rst/Cpp14/Memory.h"

#include <cppconn/exception.h>

using rst::Status;
using rst::StatusErr;
using rst::StatusOk;
using sql::SQLException;

namespace mysql_rst_driver {

Stmt::Stmt(std::unique_ptr<sql::PreparedStatement> prep_statement) :
    prep_statement_(std::move(prep_statement)) {}

rst::Status Stmt::Close() {
  return StatusErr("MySQL Stmt Close: Unimplemented");
}

rst::Status Stmt::Exec() {
  try {
    prep_statement_->executeUpdate();
  } catch (const SQLException& e) {
    return StatusErr("MySQL Stmt Exec: " + e.getSQLState());
  }

  return StatusOk();
}

std::unique_ptr<rst::driver::Rows> Stmt::Query(Status* status) {
  try {
    std::unique_ptr<sql::ResultSet> result_set(prep_statement_->executeQuery());

    *status = StatusOk();
    return rst::make_unique<Rows>(std::move(result_set));
  } catch (const SQLException& e) {
    *status = StatusErr("MySQL Stmt Query: " + e.getSQLState());
    return nullptr;
  }
}

Status Stmt::SetInt64(const int index, const int64_t x) {
  try {
    prep_statement_->setInt64(static_cast<unsigned int>(index), x);
  } catch (const SQLException& e) {
    return StatusErr("MySQL Stmt SetInt64: " + e.getSQLState());
  }

  return StatusOk();
}

Status Stmt::SetString(const int index, const std::string& x) {
  try {
    prep_statement_->setString(static_cast<unsigned int>(index), x);
  } catch (const SQLException& e) {
    return StatusErr("MySQL Stmt SetString: " + e.getSQLState());
  }

  return StatusOk();
}

Status Stmt::SetDouble(const int index, const double x) {
  try {
    prep_statement_->setDouble(static_cast<unsigned int>(index), x);
  } catch (const SQLException& e) {
    return StatusErr("MySQL Stmt SetDouble: " + e.getSQLState());
  }

  return StatusOk();
}

}  // namespace mysql_rst_driver

