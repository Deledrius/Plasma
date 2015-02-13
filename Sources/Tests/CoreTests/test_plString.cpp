#include <plString.h>
#include <HeadSpin.h>

#include <gtest/gtest.h>
#include <wchar.h>

static const plUniChar test_data[] = {
    0x20,       0x7f,       /* Normal ASCII chars */
    0xff,       0x100,      /* 8-bit boundary chars */
    0x7fff,                 /* UTF-8 2-byte boundary */
    0xffff,     0x10000,    /* 16-bit boundary chars */
    0x10020,    0x40000,    /* Non-edge UTF-16 surrogate pairs */
    0x10ffff,               /* Highest Unicode character */
    0                       /* Null terminator */
};

/* UTF-8 version of above test data */
static const char utf8_test_data[] =
    "\x20"              "\x7f"
    "\xc3\xbf"          "\xc4\x80"
    "\xe7\xbf\xbf"
    "\xef\xbf\xbf"      "\xf0\x90\x80\x80"
    "\xf0\x90\x80\xa0"  "\xf1\x80\x80\x80"
    "\xf4\x8f\xbf\xbf";

/* UTF-16 version of test data */
static const uint16_t utf16_test_data[] = {
    0x20, 0x7f,
    0xff, 0x100,
    0x7fff,
    0xffff,
    /* surrogate pairs for chars >0xffff */
    0xd800, 0xdc00,
    0xd800, 0xdc20,
    0xd8c0, 0xdc00,
    0xdbff, 0xdfff,
    0
};

/* Utility for comparing plUniChar buffers */
template <typename _Ch>
static int T_strcmp(const _Ch *left, const _Ch *right)
{
    for ( ;; ) {
        if (*left != *right)
            return *left - *right;
        if (*left == 0)
            return (*right == 0) ? 0 : -1;
        if (*right == 0)
            return 1;

        ++left;
        ++right;
    }
}

TEST(PlStringTest, TestHelpers)
{
    /* Ensure the utilities for testing the module function properly */
    EXPECT_EQ(0, T_strcmp("abc", "abc"));
    EXPECT_LT(0, T_strcmp("abc", "aba"));
    EXPECT_GT(0, T_strcmp("abc", "abe"));
    EXPECT_LT(0, T_strcmp("abc", "ab"));
    EXPECT_GT(0, T_strcmp("abc", "abcd"));
}

TEST(PlStringTest, ConvertUtf8)
{
    // From UTF-8 to plString
    plString from_utf8 = plString::FromUtf8(utf8_test_data);
    EXPECT_STREQ(utf8_test_data, from_utf8.c_str());
    plUnicodeBuffer unicode = from_utf8.GetUnicodeArray();
    EXPECT_EQ(0, T_strcmp(test_data, unicode.GetData()));

    // From plString to UTF-8
    plString to_utf8 = plString::FromUtf32(test_data);
    EXPECT_STREQ(utf8_test_data, to_utf8.c_str());
}

TEST(PlStringTest, ConvertUtf16)
{
    // From UTF-16 to plString
    plString from_utf16 = plString::FromUtf16(utf16_test_data);
    plUnicodeBuffer unicode = from_utf16.GetUnicodeArray();
    EXPECT_EQ(0, T_strcmp(test_data, unicode.GetData()));

    // From plString to UTF-16
    plStringBuffer<uint16_t> to_utf16 = plString::FromUtf32(test_data).ToUtf16();
    EXPECT_EQ(0, T_strcmp(utf16_test_data, to_utf16.GetData()));
}

TEST(PlStringTest, ConvertIso8859_1)
{
    // From ISO-8859-1 to plString
    const char latin1[] = "\x20\x7e\xa0\xff";
    const plUniChar unicode_cp0[] = { 0x20, 0x7e, 0xa0, 0xff, 0 };
    plString from_latin1 = plString::FromIso8859_1(latin1);
    plUnicodeBuffer unicode = from_latin1.GetUnicodeArray();
    EXPECT_EQ(0, T_strcmp(unicode_cp0, unicode.GetData()));

    // From plString to ISO-8859-1
    plStringBuffer<char> to_latin1 = plString::FromUtf32(unicode_cp0).ToIso8859_1();
    EXPECT_STREQ(latin1, to_latin1.GetData());
}

TEST(PlStringTest, ConvertWchar)
{
    // UTF-8 and UTF-16 are already tested, so just make sure we test
    // wchar_t and L"" conversions

    const wchar_t wtext[] = L"\x20\x7f\xff\u0100\uffff";
    const plUniChar unicode_text[] = { 0x20, 0x7f, 0xff, 0x100, 0xffff, 0 };
    plString from_wchar = plString::FromWchar(wtext);
    plUnicodeBuffer unicode = from_wchar.GetUnicodeArray();
    EXPECT_EQ(0, T_strcmp(unicode_text, unicode.GetData()));

    // From plString to wchar_t
    plStringBuffer<wchar_t> to_wchar = plString::FromUtf32(unicode_text).ToWchar();
    EXPECT_STREQ(wtext, to_wchar.GetData());
}

TEST(PlStringTest,FindChar)
{
    plString input = plString("abCdcBAeab");
    int result=0;
    //available char, case sensitive
    result = input.Find('B',plString::kCaseSensitive);
    EXPECT_EQ(5,result);

    //available char, case insensitive
    result = input.Find('B',plString::kCaseInsensitive);
    EXPECT_EQ(1,result);

    //unavailable char, case sensitive
    result = input.Find('f',plString::kCaseSensitive);
    EXPECT_EQ(-1,result);

    //unavailable char, case insensitive
    result=0;
    result = input.Find('f',plString::kCaseInsensitive);
    EXPECT_EQ(-1,result);

    plString input1 = plString("abCdcBÁèab");
    //available accented char, case sensitive
    result = input1.Find('Á',plString::kCaseSensitive);
    EXPECT_EQ(7,result);

    //available accented char, case insensitive
    result = input1.Find('è',plString::kCaseInsensitive);
    EXPECT_EQ(9,result);
}

TEST(PlStringTest,FindLast)
{
    plString input = plString("abCdcBAeab");
    int result=0;
    //available char, case sensitive
    result = input.FindLast('B',plString::kCaseSensitive);
    EXPECT_EQ(5,result);

    //available char, case insensitive
    result = input.FindLast('B',plString::kCaseInsensitive);
    EXPECT_EQ(9,result);

    //unavailable char, case sensitive
    result = input.FindLast('f',plString::kCaseSensitive);
    EXPECT_EQ(-1,result);

    //unavailable char, case insensitive
    result=0;
    result = input.FindLast('f',plString::kCaseInsensitive);
    EXPECT_EQ(-1,result);

    plString input1 = plString("éeÉß");
    //available accented char, case sensitive
    result = input1.FindLast('e',plString::kCaseSensitive);
    EXPECT_EQ(2,result);

    //available accented char, case insensitive
    result = input1.FindLast('ß',plString::kCaseInsensitive);
    EXPECT_EQ(6,result);
}

TEST(PlStringTest,FindString)
{
    plString input = plString("abABÁè");
    int result=0;
    //available string, case sensitive
    result = input.Find("AB",plString::kCaseSensitive);
    EXPECT_EQ(2,result);

    //available string, case insensitive
    result = input.Find("ab",plString::kCaseInsensitive);
    EXPECT_EQ(0,result);

    //unavailable string, case sensitive
    result = input.Find("cd",plString::kCaseSensitive);
    EXPECT_EQ(-1,result);

    //unavailable string, case insensitive
    result=0;
    result = input.Find("cd",plString::kCaseInsensitive);
    EXPECT_EQ(-1,result);

    plString input1 = plString("àbéCdcBÀéab");
    //available accented string, case sensitive
    result = input1.Find("À",plString::kCaseSensitive);
    EXPECT_EQ(9,result);

    //the strnicmp method used does not support unicode
    //available accented string, case insensitive
   // result = input1.Find("À",plString::kCaseInsensitive);
   // EXPECT_EQ(1,result);
}

//TODO: test regex functions

TEST(PlStringTest,TrimLeft)
{
    plString input = plString("abcdefgh");
    plString output = input.TrimLeft("abc");
    plString expected = plString("defgh");
    EXPECT_EQ(expected,output);

    plString input1 = plString("abcdefgh");
    plString output1 = input1.TrimLeft("bc");
    EXPECT_EQ(input1,output1);
}

TEST(PlStringTest,TrimRight)
{
    plString input = plString("abcdefgh");
    plString output = input.TrimRight("fgh");
    plString expected = plString("abcde");
    EXPECT_EQ(expected,output);

    plString input1 = plString("abcdefgh");
    plString output1 = input1.TrimRight("fg");
    EXPECT_EQ(input1,output1);
}

TEST(PlStringTest,Trim)
{
    plString input = plString("abcdefba");
    plString output = input.Trim("ab");
    plString expected = plString("cdef");
    EXPECT_EQ(expected,output);

    plString input1 = plString("abcdefba");
    plString output1 = input1.Trim("f");
    EXPECT_EQ(input1,output1);
}

TEST(PlStringTest,Substr)
{
    plString input = plString("abcdefgh");

    //start > size returns null
    plString output = input.Substr(15,1);
    EXPECT_EQ(plString::Null,output);

    //start<0
    plString output1 =input.Substr(-3,3);
    plString expected1 = plString("fgh");
    EXPECT_EQ(expected1,output1);

    //start+size>size string
    plString output2 =input.Substr(4,6);
    plString expected2 = plString("efgh");
    EXPECT_EQ(expected2,output2);

    //start =0 size = length string
    plString output3 =input.Substr(0,input.GetSize());
    EXPECT_EQ(input,output3);

    //normal case
    plString output4 =input.Substr(1,3);
    plString expected4 = plString("bcd");
    EXPECT_EQ(expected4,output4);
}

TEST(PlStringTest,Replace)
{
    plString input = plString("abcdabcd");

    plString output = input.Replace("ab","cd");
    plString expected = plString("cdcdcdcd");
    EXPECT_EQ(expected,output);

    plString output1 = input.Replace("a","cd");
    plString expected1 = plString("cdbcdcdbcd");
    EXPECT_EQ(expected1,output1);

}

TEST(PlStringTest,ToUpper)
{
    plString input = plString("abCDe");
    plString output = input.ToUpper();
    plString expected = plString("ABCDE");
    EXPECT_EQ(expected,output);
}

TEST(PlStringTest,ToLower)
{
    plString input = plString("aBcDe");
    plString output = input.ToLower();
    plString expected = plString("abcde");
    EXPECT_EQ(expected,output);
}

TEST(PlStringTest,Tokenize)
{
    std::vector<plString> expected;
    expected.push_back(plString("a"));
    expected.push_back(plString("b"));
    expected.push_back(plString("c"));
    expected.push_back(plString("d"));
    expected.push_back(plString("è"));

    plString input = plString("a\t\tb\n;c-d;è");
    std::vector<plString> output = input.Tokenize("\t\n-;");
    EXPECT_EQ(expected,output);

}

TEST(PlStringTest,Split)
{
    std::vector<plString> expected;
    expected.push_back(plString("a"));
    expected.push_back(plString("b"));
    expected.push_back(plString("c"));
    expected.push_back(plString("d"));
    expected.push_back(plString("è"));

    plString input = plString("a-b-c-d-è");
    std::vector<plString> output = input.Split("-",4);
    EXPECT_EQ(expected,output);

}

TEST(PlStringTest,Fill)
{
    plString expected = plString("aaaaa");
    plString output = plString::Fill(5,'a');
    EXPECT_EQ(expected,output);
}

//overload operator+
TEST(PlStringTest,Addition)
{
    plString expected = "abcde";
    plString input1 = "ab";
    plString input2 = "cde";

    //plstring+plstring
    plString output = input1+input2;
    EXPECT_EQ(expected,output);

    //plstring+char*
    plString output1 = input1 + input2.c_str();
    EXPECT_EQ(expected,output1);

    //char*+plstring
    plString output2 = input1.c_str() + input2;
    EXPECT_EQ(expected,output2);
}
