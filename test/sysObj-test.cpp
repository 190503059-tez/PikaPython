#include "test_common.h"
TEST_START

TEST(sysObj, print) {
    PikaObj* obj = newRootObj("test", New_PikaStdLib_SysObj);
    VMParameters* globals = obj_run(obj, "print('hello world')");
    // char* sysOut = args_getSysOut(globals->list);
    int errCode = args_getErrorCode(globals->list);
    // printf("sysout = %s\r\n", sysOut);
    EXPECT_STREQ(log_buff[0], "hello world\r\n");
    // ASSERT_STREQ("hello world", sysOut);
    ASSERT_EQ(0, errCode);
    // obj_deinit(globals);
    obj_deinit(obj);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(sysObj, noMethod) {
    PikaObj* obj = newRootObj("test", New_PikaStdLib_SysObj);
    __platform_printf("BEGIN\r\n");
    obj_run(obj, "printttt('hello world')");
    // char* sysOut = args_getSysOut(globals->list);
    // int errCode = args_getErrorCode(globals->list);
    // printf("sysout = %s\r\n", sysOut);
    // ASSERT_EQ(1, strEqu("[error] runner: method no found.", sysOut));
    EXPECT_STREQ(log_buff[4], "NameError: name 'printttt' is not defined\r\n");
    // ASSERT_EQ(2, errCode);
    // obj_deinit(globals);
    obj_deinit(obj);
    EXPECT_EQ(pikaMemNow(), 0);
}

#if !PIKA_NANO_ENABLE
TEST(sysObj, getattr) {
    char* lines =
        "class Test:\n"
        "    def __init__(self):\n"
        "        self.a = 1\n"
        "test = Test()\n"
        "aa = getattr(test, 'a')\n";
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    __platform_printf("BEGIN\r\n");
    obj_run(pikaMain, lines);
    /* collect */
    int aa = obj_getInt(pikaMain, "aa");
    /* assert */
    EXPECT_EQ(1, aa);
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}
#endif

#if !PIKA_NANO_ENABLE
TEST(sysObj, setattr) {
    char* lines =
        "class Test:\n"
        "\n"
        "test = Test()\n"
        "setattr(test, 'a', 2)\n"
        "aa = test.a\n";
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    __platform_printf("BEGIN\r\n");
    obj_run(pikaMain, lines);
    /* collect */
    int aa = obj_getInt(pikaMain, "aa");
    /* assert */
    EXPECT_EQ(2, aa);
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}
#endif

#if !PIKA_NANO_ENABLE
TEST(sysobj, print_no_end) {
    char* line = "print('test', end='')\n";
    PikaObj* self = newRootObj("root", New_PikaStdLib_SysObj);
    obj_run(self, line);
    /* collect */
    /* assert */
    EXPECT_STREQ(log_buff[0], "test");
    /* deinit */
    obj_deinit(self);
    EXPECT_EQ(pikaMemNow(), 0);
}
#endif

#if !PIKA_NANO_ENABLE
TEST(sysobj, print) {
    char* line = "print(0, ['Hi'])\n";
    PikaObj* self = newRootObj("root", New_PikaStdLib_SysObj);
    obj_run(self, line);
    /* collect */
    /* assert */
    EXPECT_STREQ(log_buff[0], "0 ['Hi']\r\n");
    /* deinit */
    obj_deinit(self);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(sysobj, print2) {
    char* line = "print(0, ['Hi'], b'test')\n";
    PikaObj* self = newRootObj("root", New_PikaStdLib_SysObj);
    obj_run(self, line);
    /* collect */
    /* assert */
    EXPECT_STREQ(log_buff[0], "0 ['Hi'] b'\\x74\\x65\\x73\\x74'\r\n");
    /* deinit */
    obj_deinit(self);
    EXPECT_EQ(pikaMemNow(), 0);
}
#endif

TEST_END