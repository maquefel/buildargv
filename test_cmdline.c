#include "buildargv.h"
#include <check.h>
#include <stdlib.h>
#include <errno.h>

enum tests_enum {
    CMDLINE_SIMPLE_LINE,
    CMDLINE_SINGLE_QUOTED,
    CMDLINE_DOUBLE_QUOTED,
    CMDLINE_EMBDED_QUOTED_WHITESPACE,
    CMDLINE_EMBED_SINGLE_QUOTED,
    CMDLINE_EMBED_DOUBLE_QUOTED,
    CMDLINE_EMBED_SINGLE_INTO_DOUBLE,
    CMDLINE_EMBED_DOUBLE_INTO_DOUBLE,
    CMDLINE_SINGLE_CHARS,
    CMDLINE_FW_WHITESPACE,
    CMDLINE_TR_WHITESPACE,
    CMDLINE_MID_WHITESPACE,
    CMDLINE_EMPTY_LINE,
    CMDLINE_ESCAPED,
    CMDLINE_NOT_CLOSED_SINGLE,
    CMDLINE_NOT_CLOSED_DOUBLE,
    CMDLINE_SINGLE_BACKSLASH,
    CMDLINE_SINGLE_DQUOTE,
    CMDLINE_SINGLE_SQUOTE,
    CMDLINE_MAX
};

static char* tests[] =
{
    /* 0 */  "a SIMPLE command LINE",
    /* 1 */  "arg 'foo' is single quoted",
    /* 2 */  "arg \"bar\" is double quoted",
    /* 3 */  "arg \"foo bar\" has embedded whitespace",
    /* 4 */  "arg 'Jack said \\'hi\\'' has single quotes",
    /* 5 */  "arg 'Jack said \\\"hi\\\"' has double quotes",
    /* 6 */ "arg \"Jack said 'hi' in double quotes\"",
    /* 7 */ "arg \"Jack said \\\"hi\\\" in double quotes\"",
    /* 8 */  "a b c d e f g h i j k l m n o p q r s t u v w x y z 1 2 3 4 5 6 7 8 9",
    /* 9 */  "     forward-whitespace",
    /* 10 */  "trailing-whitespace     ",
    /* 11 */  "middle      \t        whitespace",
    /* 12 */  "",
    /* 13 */ "a\\ escaped\\ path\\ with\\ spaces",
    /* 14 */ "arg \"not-enslosed-dquote arg",
    /* 15 */ "arg 'not-enslosed-squote arg",
    /* 16 */ "\\",
    /* 17 */ "\"",
    /* 18 */ "'",
    /* 19 */  0
};

#define SETUP \
char **argv; \
int argc;

#define FREE_ARGV do { \
for(int i = 0; i < argc; i++) \
    free(argv[i]); \
free(argv); \
} while(0);

START_TEST (simple_line)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_SIMPLE_LINE], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_SIMPLE_LINE]);
    ck_assert_msg(argc == 4, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (single_quoted)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_SINGLE_QUOTED], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_SINGLE_QUOTED]);
    ck_assert_msg(argc == 5, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (double_qouted)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_DOUBLE_QUOTED], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_DOUBLE_QUOTED]);
    ck_assert_msg(argc == 5, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (embed_quoted_whitespace)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_EMBDED_QUOTED_WHITESPACE], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_EMBDED_QUOTED_WHITESPACE]);
    ck_assert_msg(argc == 5, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (embed_single_quoted)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_EMBED_SINGLE_QUOTED], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_EMBED_SINGLE_QUOTED]);
    ck_assert_msg(argc == 5, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (embed_double_quoted)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_EMBED_DOUBLE_QUOTED], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_EMBED_DOUBLE_QUOTED]);
    ck_assert_msg(argc == 5, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (embed_single_into_double)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_EMBED_SINGLE_INTO_DOUBLE], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_EMBED_SINGLE_INTO_DOUBLE]);
    ck_assert_msg(argc == 2, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (embed_double_into_double)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_EMBED_DOUBLE_INTO_DOUBLE], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_EMBED_DOUBLE_INTO_DOUBLE]);
    ck_assert_msg(argc == 2, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (single_chars)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_SINGLE_CHARS], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_SINGLE_CHARS]);
    ck_assert_msg(argc == 35, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (forward_whitespace)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_FW_WHITESPACE], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_FW_WHITESPACE]);
    ck_assert_msg(argc == 1, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (trailing_whitespace)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_TR_WHITESPACE], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_TR_WHITESPACE]);
    ck_assert_msg(argc == 1, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (middle_whitespace)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_MID_WHITESPACE], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_MID_WHITESPACE]);
    ck_assert_msg(argc == 2, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (empty_line)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_EMPTY_LINE], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_EMPTY_LINE]);
    ck_assert_msg(argc == 0, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (escaped)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_ESCAPED], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_ESCAPED]);
    ck_assert_msg(argc == 1, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (not_closed_single_quote)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_NOT_CLOSED_SINGLE], &argv, &argc);
    ck_assert_msg(ret != 0, "parsing %s succeded but was supposed to fail", tests[CMDLINE_NOT_CLOSED_SINGLE]);
    ck_assert_msg(errno != BUILDARGV_ESQUOTE, "errno not matching BUILDARGV_ESQUOTE");
    FREE_ARGV
}
END_TEST

START_TEST (not_closed_double_quote)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_NOT_CLOSED_DOUBLE], &argv, &argc);
    ck_assert_msg(ret != 0, "parsing %s succeded but was supposed to fail", tests[CMDLINE_NOT_CLOSED_DOUBLE]);
    ck_assert_msg(errno != BUILDARGV_EDQUOTE, "errno not matching BUILDARGV_EDQUOTE");
    FREE_ARGV
}
END_TEST

START_TEST (single_backslash)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_SINGLE_BACKSLASH], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_SINGLE_BACKSLASH]);
    ck_assert_msg(argc == 0, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (single_dquote)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_SINGLE_DQUOTE], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_SINGLE_DQUOTE]);
    ck_assert_msg(argc == 0, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

START_TEST (single_squote)
{
    SETUP
    int ret = buildargv(tests[CMDLINE_SINGLE_SQUOTE], &argv, &argc);
    ck_assert_msg(ret == 0, "parsing %s failed", tests[CMDLINE_SINGLE_SQUOTE]);
    ck_assert_msg(argc == 0, "argument count doesn't match");
    FREE_ARGV
}
END_TEST

Suite * cmdline_test_suite(void)
{
    Suite *s;
    TCase *tc_parse_cmdlines;

    s = suite_create("Testing buildargv function");

    tc_parse_cmdlines = tcase_create("parse argument lines");

    tcase_add_test(tc_parse_cmdlines, simple_line);
    tcase_add_test(tc_parse_cmdlines, single_quoted);
    tcase_add_test(tc_parse_cmdlines, double_qouted);
    tcase_add_test(tc_parse_cmdlines, embed_quoted_whitespace);
    tcase_add_test(tc_parse_cmdlines, embed_single_quoted);
    tcase_add_test(tc_parse_cmdlines, embed_double_quoted);
    tcase_add_test(tc_parse_cmdlines, embed_single_into_double);
    tcase_add_test(tc_parse_cmdlines, embed_double_into_double);
    tcase_add_test(tc_parse_cmdlines, single_chars);
    tcase_add_test(tc_parse_cmdlines, forward_whitespace);
    tcase_add_test(tc_parse_cmdlines, trailing_whitespace);
    tcase_add_test(tc_parse_cmdlines, middle_whitespace);
    tcase_add_test(tc_parse_cmdlines, empty_line);
    tcase_add_test(tc_parse_cmdlines, escaped);
    tcase_add_test(tc_parse_cmdlines, not_closed_single_quote);
    tcase_add_test(tc_parse_cmdlines, not_closed_double_quote);
    tcase_add_test(tc_parse_cmdlines, single_backslash);
    tcase_add_test(tc_parse_cmdlines, single_dquote);
    tcase_add_test(tc_parse_cmdlines, single_squote);

    suite_add_tcase(s, tc_parse_cmdlines);

    return s;
}


int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = cmdline_test_suite();
    sr = srunner_create(s);

    if(srunner_has_tap(sr))
        srunner_run_all(sr, CK_SILENT);
    else
        srunner_run_all(sr, CK_VERBOSE);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
