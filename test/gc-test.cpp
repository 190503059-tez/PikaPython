#include "test_common.h"
TEST_START

TEST(gc, root) {
    PikaObj* root = newRootObj("root", New_PikaMain);
    EXPECT_EQ(obj_refcntNow(root), 1);
    Arg* refArg = arg_setRef(NULL, "", root);
    EXPECT_EQ(obj_refcntNow(root), 2);
    arg_deinit(refArg);
    EXPECT_EQ(obj_refcntNow(root), 1);
    obj_deinit(root);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(gc, ref1) {
    /* init */
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    obj_run(pikaMain, "mem = PikaStdLib.MemChecker()");
    PikaObj* mem = (PikaObj*)obj_getPtr(pikaMain, "mem");
    EXPECT_EQ(obj_refcntNow(mem), 1);
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(gc, ref121) {
    /* init */
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    obj_run(pikaMain, "mem = PikaStdLib.MemChecker()");
    PikaObj* mem = (PikaObj*)obj_getPtr(pikaMain, "mem");
    EXPECT_EQ(obj_refcntNow(mem), 1);
    obj_run(pikaMain, "mem2 = mem");
    EXPECT_EQ(obj_refcntNow(mem), 2);
    obj_removeArg(pikaMain, "mem2");
    EXPECT_EQ(obj_refcntNow(mem), 1);
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(gc, ref12) {
    /* init */
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    obj_run(pikaMain, "mem = PikaStdLib.MemChecker()");
    PikaObj* mem = (PikaObj*)obj_getPtr(pikaMain, "mem");
    EXPECT_EQ(obj_refcntNow(mem), 1);
    obj_run(pikaMain, "mem2 = mem");
    EXPECT_EQ(obj_refcntNow(mem), 2);
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(gc, ref1210) {
    /* init */
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    obj_run(pikaMain, "mem = PikaStdLib.MemChecker()");
    PikaObj* mem = (PikaObj*)obj_getPtr(pikaMain, "mem");
    EXPECT_EQ(obj_refcntNow(mem), 1);
    obj_run(pikaMain, "mem2 = mem");
    EXPECT_EQ(obj_refcntNow(mem), 2);
    obj_removeArg(pikaMain, "mem2");
    EXPECT_EQ(obj_refcntNow(mem), 1);
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(gc, ref_move) {
    /* init */
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    obj_run(pikaMain, "mem = PikaStdLib.MemChecker()");
    PikaObj* mem = (PikaObj*)obj_getPtr(pikaMain, "mem");
    EXPECT_EQ(obj_refcntNow(mem), 1);
    obj_run(pikaMain, "mem2 = mem");
    EXPECT_EQ(obj_refcntNow(mem), 2);
    obj_removeArg(pikaMain, "mem");
    EXPECT_EQ(obj_refcntNow(mem), 1);
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(gc, factory) {
    /* init */
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    obj_run(pikaMain,
            "def factory():\n"
            "    _mem = PikaStdLib.MemChecker()\n"
            "    return _mem\n"
            "mem = factory()\n");
    /* collect */
    PikaObj* mem = obj_getObj(pikaMain, "mem");
    EXPECT_EQ(obj_refcntNow(mem), 1);
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(gc, not_out) {
    /* init */
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    obj_run(pikaMain, "PikaStdLib.MemChecker()\n");
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(gc, heap_failed1) {
    /* init */
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    obj_run(pikaMain,
            "recv_buf = PikaStdData.List()\n"
            "RECV_MAX_SIZE=128\n"
            "iteri = 0\n"
            "for i in range(0, int(RECV_MAX_SIZE)):\n"
            "    recv_buf.append(0)\n"
            "    iteri += 1\n"
            "iteri");
    /* assert */
    EXPECT_STREQ("128\r\n", log_buff[0]);
#if PIKA_GC_MARK_SWEEP_ENABLE
    int cnt = pikaGC_count();
    EXPECT_EQ(cnt != 0, 1);
    pikaGC_markDump();
    int cnt_marked = pikaGC_countMarked();
    EXPECT_EQ(cnt, cnt_marked);
    /* deinit */
#endif
    obj_deinit(pikaMain);
#if PIKA_GC_MARK_SWEEP_ENABLE
    cnt = pikaGC_count();
    EXPECT_EQ(cnt, 0);
#endif
    EXPECT_EQ(pikaMemNow(), 0);
}

#if PIKA_GC_MARK_SWEEP_ENABLE
TEST(gc, circle) {
    /* init */
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    pikaVM_runSingleFile(pikaMain, "test/python/gc/gc_circle.py");
    /* assert */
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(gc, circle2) {
    /* init */
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    pikaVM_runSingleFile(pikaMain, "test/python/gc/gc_circle2.py");
    /* assert */
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(gc, tree1) {
    /* init */
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    /* run */
    pikaVM_runSingleFile(pikaMain, "test/python/gc/gc_tree1.py");
    /* assert */
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}
#endif

TEST_END