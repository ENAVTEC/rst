#include "rst/SQL/Rows.h"

namespace rst {

Rows::Rows(std::unique_ptr<driver::Rows> rows) :
    rows_(std::move(rows)) {
  status_.Ignore();
}

bool Rows::Next() {
  Status status;
  status.Ignore();
  bool has_next = rows_->Next(&status);
  if (!status.ok()) {
    status_.Ignore();
    status_ = std::move(status);
  }

  return has_next;
}

}  // namespace rst

