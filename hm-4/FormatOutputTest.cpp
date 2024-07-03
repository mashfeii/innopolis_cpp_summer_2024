#include <gtest/gtest.h>
#include "FormatOutput.hpp"

// Simple output without exceptions
TEST(FormatedOutput, EmptyFormatStringEmptyArguments) {
  ASSERT_EQ(format(""), "");
}
TEST(FormatedOutput, NoFormatElementEmptyArguments) {
  ASSERT_EQ(format("Hello, world!"), "Hello, world!");
}
TEST(FormatedOutput, SimpleSubstitutionSingleArgument) {
  ASSERT_EQ(format("Hello, {0}", "world!"), "Hello, world!");
}
TEST(FormatedOutput, SimpleSubstitutionSeveralArgument) {
  ASSERT_EQ(format("{0}, {1}", "Hello", "world!"), "Hello, world!");
  ASSERT_EQ(format("{0} {1} {0} {1} {0} {1} {0} {1} ", "Hello", "world!"),
            "Hello world! Hello world! Hello world! Hello world! ");
}
TEST(FormatedOutput, ReversedSubstitutionSeveralArgument) {
  ASSERT_EQ(format("{1} {0} {0} {1} {1} {0} {0} {1} ", "Hello", "world!"),
            "world! Hello Hello world! world! Hello Hello world! ");
}
TEST(FormatedOutput, ManyArgumentsSubstitution) {
  ASSERT_EQ(format("{0}{1}{2}{3}{4}{5}{6}{7}{8}{9}{10}", 1, '2', 3, '4', 5, "6",
                   7, '8', 9.5, " Htich!", std::to_string(5)),
            "123456789.5 Htich!5");
}
TEST(FormatedOutput, ManyArgumentsReversedSubstitution) {
  ASSERT_EQ(format("{10}{9}{8}{7}{6}{5}{4}{3}{2}{1}", 10, 9, 8, 7, 6, 5, 4, 3,
                   2, 1, 0),
            "0123456789");
}
TEST(FormatedOutput, ManyArgumentsMixedSubstitution) {
  ASSERT_EQ(format("{0} in the power of {1} = {0}", "two", "1"),
            "two in the power of 1 = two");
}
// Exceptions validating
TEST(FormattedOutput, MissingOpeningBracket) {
  EXPECT_THROW(
      {
        try {
          format("Hello, I want to throw an excpetion! }", '{');
        } catch (const WrongStringFormat& except) {
          EXPECT_STREQ("Unexpected '}' before '{' in string format",
                       except.what());
          throw;
        }
      },
      WrongStringFormat);
}
TEST(FormattedOutput, MissingClosingBracketAfterOpeningOne) {
  EXPECT_THROW(
      {
        try {
          format("Hello, I want to throw an {0", "excpetion!");
        } catch (const WrongStringFormat& except) {
          EXPECT_STREQ("No mathing closing bracket", except.what());
          throw;
        }
      },
      WrongStringFormat);
}
TEST(FormattedOutput, WeirdContextInsideFormatBrackets) {
  EXPECT_THROW(
      {
        try {
          format("Hello, I want to print {my awesome arguments}",
                 "It's not me!");
        } catch (const WrongStringFormat& except) {
          EXPECT_STREQ("Incorrect format inside curved brackets",
                       except.what());
          throw;
        }
      },
      WrongStringFormat);
}
// Couldn't implement a similar test
// TEST(FormattedOutput, UnexpectedSubstitutionType) {
//   EXPECT_THROW(
//       {
//         try {
//           class NoStringConvertion {
//             long long int a;
//           };
//           NoStringConvertion incorrect_type;
//           format("Hello, I want to print {0}", incorrect_type);
//         } catch (const WrongStringFormat& except) {
//           EXPECT_STREQ("No viable overalod operator<< for such argument",
//                        except.what());
//           throw;
//         }
//       },
//       StreamOutputException);
// }
TEST(FormattedOutput, NotEnoughArguments) {
  EXPECT_THROW(
      {
        try {
          format("Hello, I want to print {0} and {1} and also {2}", "first",
                 "second");
        } catch (const InvalidArguments& except) {
          EXPECT_STREQ("Not enough argument for substitution", except.what());
          throw;
        }
      },
      InvalidArguments);
}
TEST(FormattedOutput, ArgumentsOverflow) {
  EXPECT_THROW(
      {
        try {
          format("Hello, I want to print {0} and {1} and also {2}", "first",
                 "second", "third", "fourth");
        } catch (const InvalidArguments& except) {
          EXPECT_STREQ("Too many arguments for format string", except.what());
          throw;
        }
      },
      InvalidArguments);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
