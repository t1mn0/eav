#include "Utils.hpp"

namespace tmn::test_utils {

int TestObject::constructor_count = 0;
int TestObject::destructor_count = 0;
int TestObject::copy_count = 0;
int TestObject::move_count = 0;

int TestDeleter::delete_count = 0;
int TestDeleter::last_deleted_value = 0;

} // namespace tmn::test_utils;
