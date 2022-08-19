#include "hashtable.h"

#include <cstring>
#include <algorithm>
#include <string>

#include "gtest/gtest.h"

namespace {

  struct Dummy {
  };

}

TEST(ValueTest, MakeNil) {
  struct value val = value_make_nil();

  EXPECT_TRUE(value_is_nil(&val));
  EXPECT_EQ(value_get_kind(&val), VALUE_NIL);
}

TEST(ValueTest, MakeBoolean) {
  struct value val1 = value_make_boolean(true);

  EXPECT_TRUE(value_is_boolean(&val1));
  EXPECT_EQ(value_get_kind(&val1), VALUE_BOOLEAN);
  EXPECT_EQ(value_get_boolean(&val1), true);

  struct value val2 = value_make_boolean(false);

  EXPECT_TRUE(value_is_boolean(&val2));
  EXPECT_EQ(value_get_kind(&val2), VALUE_BOOLEAN);
  EXPECT_EQ(value_get_boolean(&val2), false);
}

TEST(ValueTest, MakeInteger) {
  struct value val1 = value_make_integer(42);

  EXPECT_TRUE(value_is_integer(&val1));
  EXPECT_EQ(value_get_kind(&val1), VALUE_INTEGER);
  EXPECT_EQ(value_get_integer(&val1), 42);

  struct value val2 = value_make_integer(-69);

  EXPECT_TRUE(value_is_integer(&val2));
  EXPECT_EQ(value_get_kind(&val2), VALUE_INTEGER);
  EXPECT_EQ(value_get_integer(&val2), -69);
}

TEST(ValueTest, MakeReal) {
  struct value val1 = value_make_real(42.0);

  EXPECT_TRUE(value_is_real(&val1));
  EXPECT_EQ(value_get_kind(&val1), VALUE_REAL);
  EXPECT_EQ(value_get_real(&val1), 42.0);

  struct value val2 = value_make_real(-69.0);

  EXPECT_TRUE(value_is_real(&val2));
  EXPECT_EQ(value_get_kind(&val2), VALUE_REAL);
  EXPECT_EQ(value_get_real(&val2), -69.0);
}

TEST(ValueTest, MakeCustom) {
  char str[] = "the cake is a lie";

  struct value val1 = value_make_custom(str);

  EXPECT_TRUE(value_is_custom(&val1));
  EXPECT_EQ(value_get_kind(&val1), VALUE_CUSTOM);
  EXPECT_EQ(value_get_custom(&val1), str);

  Dummy dummy;

  struct value val2 = value_make_custom(&dummy);

  EXPECT_TRUE(value_is_custom(&val2));
  EXPECT_EQ(value_get_kind(&val2), VALUE_CUSTOM);
  EXPECT_EQ(value_get_custom(&val2), &dummy);
}

TEST(ValueTest, Set) {
  struct value val = value_make_nil();

  EXPECT_TRUE(value_is_nil(&val));
  EXPECT_EQ(value_get_kind(&val), VALUE_NIL);

  value_set_boolean(&val, true);

  EXPECT_TRUE(value_is_boolean(&val));
  EXPECT_EQ(value_get_kind(&val), VALUE_BOOLEAN);
  EXPECT_EQ(value_get_boolean(&val), true);

  value_set_integer(&val, 42);

  EXPECT_TRUE(value_is_integer(&val));
  EXPECT_EQ(value_get_kind(&val), VALUE_INTEGER);
  EXPECT_EQ(value_get_integer(&val), 42);

  value_set_real(&val, 69.0);

  EXPECT_TRUE(value_is_real(&val));
  EXPECT_EQ(value_get_kind(&val), VALUE_REAL);
  EXPECT_EQ(value_get_real(&val), 69.0);

  value_set_nil(&val);

  EXPECT_TRUE(value_is_nil(&val));
  EXPECT_EQ(value_get_kind(&val), VALUE_NIL);

  Dummy dummy;
  value_set_custom(&val, &dummy);

  EXPECT_TRUE(value_is_custom(&val));
  EXPECT_EQ(value_get_kind(&val), VALUE_CUSTOM);
  EXPECT_EQ(value_get_custom(&val), &dummy);
}



TEST(HashtableTest, CreateEmpty) {
  struct hashtable h;
  hashtable_create(&h);

  EXPECT_EQ(hashtable_get_count(&h), 0u);
  EXPECT_EQ(hashtable_get_size(&h), static_cast<size_t>(HASHTABLE_INITIAL_SIZE));

  hashtable_destroy(&h);
}

TEST(HashtableTest, InsertOne) {
  struct hashtable h;
  hashtable_create(&h);

  char s1[] = "foo";

  EXPECT_TRUE(hashtable_insert(&h, s1, value_make_integer(42)));

  s1[0] = '\0';

  EXPECT_EQ(hashtable_get_count(&h), 1u);
  EXPECT_TRUE(hashtable_contains(&h, "foo"));
  printf("contains\n");

  struct value val = hashtable_get(&h, "foo");
  printf("got the value\n");

  ASSERT_TRUE(value_is_integer(&val));
  EXPECT_EQ(value_get_integer(&val), 42);

  hashtable_destroy(&h);
}

TEST(HashtableTest, InsertSame) {
  struct hashtable h;
  hashtable_create(&h);

  char s1[] = "foo";
  char s2[] = "foo";

  EXPECT_TRUE(hashtable_insert(&h, s1, value_make_integer(42)));
  EXPECT_FALSE(hashtable_insert(&h, s2, value_make_real(3.14)));

  s1[0] = '\0';
  s2[0] = '\0';

  EXPECT_EQ(hashtable_get_count(&h), 1u);
  EXPECT_TRUE(hashtable_contains(&h, "foo"));

  struct value val = hashtable_get(&h, "foo");

  ASSERT_TRUE(value_is_real(&val));
  EXPECT_EQ(value_get_real(&val), 3.14);

  hashtable_destroy(&h);
}

TEST(HashtableTest, InsertMany) {
  struct hashtable h;
  hashtable_create(&h);

  char s[] = "fooX";

  for (char c = '0'; c <= '9'; ++c) {
    s[3] = c;
    EXPECT_TRUE(hashtable_insert(&h, s, value_make_integer(c - '0')));
  }

  s[0] = '\0';

  EXPECT_EQ(hashtable_get_count(&h), 10u);

  char t[] = "fooX";

  for (char c = '0'; c <= '9'; ++c) {
    t[3] = c;
    EXPECT_TRUE(hashtable_contains(&h, t));

    struct value val = hashtable_get(&h, t);

    ASSERT_TRUE(value_is_integer(&val));
    EXPECT_EQ(value_get_integer(&val), c - '0');
  }

  hashtable_destroy(&h);
}

TEST(HashtableTest, RemoveEmpty) {
  struct hashtable h;
  hashtable_create(&h);

  EXPECT_FALSE(hashtable_contains(&h, "foo"));
  EXPECT_FALSE(hashtable_remove(&h, "foo"));
  EXPECT_FALSE(hashtable_contains(&h, "foo"));

  hashtable_destroy(&h);
}

TEST(HashtableTest, RemovePresent) {
  struct hashtable h;
  hashtable_create(&h);

  EXPECT_FALSE(hashtable_contains(&h, "foo"));

  char s1[] = "foo";

  EXPECT_TRUE(hashtable_insert(&h, s1, value_make_integer(42)));
  EXPECT_EQ(hashtable_get_count(&h), 1u);
  EXPECT_TRUE(hashtable_contains(&h, "foo"));

  s1[0] = '\0';

  EXPECT_TRUE(hashtable_remove(&h, "foo"));

  EXPECT_EQ(hashtable_get_count(&h), 0u);
  EXPECT_FALSE(hashtable_contains(&h, "foo"));

  hashtable_destroy(&h);
}

TEST(HashtableTest, RehashEmpty) {
  struct hashtable h;
  hashtable_create(&h);

  EXPECT_EQ(hashtable_get_count(&h), 0u);
  EXPECT_EQ(hashtable_get_size(&h), static_cast<size_t>(HASHTABLE_INITIAL_SIZE));

  hashtable_rehash(&h);

  EXPECT_EQ(hashtable_get_count(&h), 0u);
  EXPECT_EQ(hashtable_get_size(&h), static_cast<size_t>(2 * HASHTABLE_INITIAL_SIZE));

  hashtable_destroy(&h);
}

TEST(HashtableTest, RehashAutomatic) {
  struct hashtable h;
  hashtable_create(&h);

  EXPECT_EQ(hashtable_get_count(&h), 0u);
  EXPECT_EQ(hashtable_get_size(&h), static_cast<size_t>(HASHTABLE_INITIAL_SIZE));

  hashtable_insert(&h, "foo1", value_make_nil());

  EXPECT_EQ(hashtable_get_count(&h), 1u);
  EXPECT_EQ(hashtable_get_size(&h), static_cast<size_t>(HASHTABLE_INITIAL_SIZE));

  hashtable_insert(&h, "foo2", value_make_nil());

  EXPECT_EQ(hashtable_get_count(&h), 2u);
  EXPECT_EQ(hashtable_get_size(&h), static_cast<size_t>(HASHTABLE_INITIAL_SIZE));

  hashtable_insert(&h, "foo3", value_make_nil());

  EXPECT_EQ(hashtable_get_count(&h), 3u);
  EXPECT_EQ(hashtable_get_size(&h), static_cast<size_t>(2 * HASHTABLE_INITIAL_SIZE));

  hashtable_insert(&h, "foo4", value_make_nil());

  EXPECT_EQ(hashtable_get_count(&h), 4u);
  EXPECT_EQ(hashtable_get_size(&h), static_cast<size_t>(2 * HASHTABLE_INITIAL_SIZE));

  hashtable_insert(&h, "foo5", value_make_nil());

  EXPECT_EQ(hashtable_get_count(&h), 5u);
  EXPECT_EQ(hashtable_get_size(&h), static_cast<size_t>(4 * HASHTABLE_INITIAL_SIZE));

  EXPECT_TRUE(hashtable_contains(&h, "foo1"));
  EXPECT_TRUE(hashtable_contains(&h, "foo2"));
  EXPECT_TRUE(hashtable_contains(&h, "foo3"));
  EXPECT_TRUE(hashtable_contains(&h, "foo4"));
  EXPECT_TRUE(hashtable_contains(&h, "foo5"));
  EXPECT_FALSE(hashtable_contains(&h, "foo6"));

  hashtable_destroy(&h);
}

TEST(HashtableTest, SetGet) {
  struct hashtable h;
  hashtable_create(&h);

  struct value val = value_make_nil();

  EXPECT_FALSE(hashtable_contains(&h, "bar"));

  // boolean

  hashtable_set_boolean(&h, "bar", false);

  EXPECT_EQ(hashtable_get_count(&h), 1u);
  EXPECT_TRUE(hashtable_contains(&h, "bar"));

  val = hashtable_get(&h, "bar");

  ASSERT_TRUE(value_is_boolean(&val));
  EXPECT_EQ(value_get_boolean(&val), false);

  // integer

  hashtable_set_integer(&h, "bar", 42);

  EXPECT_EQ(hashtable_get_count(&h), 1u);
  EXPECT_TRUE(hashtable_contains(&h, "bar"));

  val = hashtable_get(&h, "bar");

  ASSERT_TRUE(value_is_integer(&val));
  EXPECT_EQ(value_get_integer(&val), 42);

  // real

  hashtable_set_real(&h, "bar", 69.0);

  EXPECT_EQ(hashtable_get_count(&h), 1u);
  EXPECT_TRUE(hashtable_contains(&h, "bar"));

  val = hashtable_get(&h, "bar");

  ASSERT_TRUE(value_is_real(&val));
  EXPECT_EQ(value_get_real(&val), 69.0);

  // custom

  Dummy dummy;

  hashtable_set_custom(&h, "bar", &dummy);

  EXPECT_EQ(hashtable_get_count(&h), 1u);
  EXPECT_TRUE(hashtable_contains(&h, "bar"));

  val = hashtable_get(&h, "bar");

  ASSERT_TRUE(value_is_custom(&val));
  EXPECT_EQ(value_get_custom(&val), &dummy);

  // nil

  hashtable_set_nil(&h, "bar");

  EXPECT_EQ(hashtable_get_count(&h), 1u);
  EXPECT_TRUE(hashtable_contains(&h, "bar"));

  val = hashtable_get(&h, "bar");

  ASSERT_TRUE(value_is_nil(&val));

  hashtable_destroy(&h);
}

TEST(HashtableTest, GetNotPresent) {
  struct hashtable h;
  hashtable_create(&h);

  EXPECT_EQ(hashtable_get_count(&h), 0u);
  EXPECT_FALSE(hashtable_contains(&h, "bar"));

  struct value val = hashtable_get(&h, "bar");

  ASSERT_TRUE(value_is_nil(&val));
  EXPECT_EQ(hashtable_get_count(&h), 0u);
  EXPECT_FALSE(hashtable_contains(&h, "bar"));

  hashtable_destroy(&h);
}

TEST(HashtableTest, Stress) {
  struct hashtable h;
  hashtable_create(&h);

  std::string key = "abcdefgh";
  std::size_t count = 0;

  do {
    ASSERT_TRUE(hashtable_insert(&h, key.c_str(), value_make_integer(count)));
    ++count;
  } while(std::next_permutation(key.begin(), key.end()));

  EXPECT_EQ(hashtable_get_count(&h), count);
  EXPECT_EQ(hashtable_get_size(&h), static_cast<size_t>(32768 * HASHTABLE_INITIAL_SIZE));

  do {
    ASSERT_TRUE(hashtable_contains(&h, key.c_str()));
  } while(std::next_permutation(key.begin(), key.end()));

  std::sort(key.begin(), key.end(), std::greater<char>());

  do {
    ASSERT_TRUE(hashtable_remove(&h, key.c_str()));
  } while(std::prev_permutation(key.begin(), key.end()));

  hashtable_destroy(&h);
}

/*
 * main
 */

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
