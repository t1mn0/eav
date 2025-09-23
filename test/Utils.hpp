#ifndef TMN_THROWLESS_TEST_UTILS_HPP
#define TMN_THROWLESS_TEST_UTILS_HPP

namespace tmn::test_utils {

struct TestObject {
  static int constructor_count;
  static int destructor_count;
  static int copy_count;
  static int move_count;

  int value;

  TestObject(int val = 0) : value(val) { constructor_count++; }
  ~TestObject() { destructor_count++; }

  TestObject(const TestObject& other) : value(other.value) { copy_count++; }
  TestObject(TestObject&& other) noexcept : value(other.value) {
    move_count++;
    other.value = -1;
  }

  TestObject& operator=(const TestObject& other) {
    value = other.value;
    copy_count++;
    return *this;
  }

  TestObject& operator=(TestObject&& other) noexcept {
    value = other.value;
    other.value = -1;
    move_count++;
    return *this;
  }

  static void reset_counts() {
    constructor_count = destructor_count = copy_count = move_count = 0;
  }
};

struct TestDeleter {
  static int delete_count;
  static int last_deleted_value;

  void operator()(TestObject* ptr) {
    if (ptr) {
      last_deleted_value = ptr->value;
      delete_count++;
      delete ptr;
    }
  }

  static void reset() {
    delete_count = 0;
    last_deleted_value = 0;
  }
};

} // namespace tmn::test_utils;

#endif // TMN_THROWLESS_TEST_UTILS_HPP
