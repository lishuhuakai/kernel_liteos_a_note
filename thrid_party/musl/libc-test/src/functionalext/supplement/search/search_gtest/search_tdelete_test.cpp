#include <gtest/gtest.h>
#include <search.h>

using namespace testing::ext;

class SearchTdeleteTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

struct Node {
    explicit Node(int i) : i(i) {}
    int i;
};

static int NodeCmp(const void* lhs, const void* rhs)
{
    return reinterpret_cast<const Node*>(rhs)->i - reinterpret_cast<const Node*>(lhs)->i;
}

/**
 * @tc.name: tdelete_001
 * @tc.desc: Verify the correctness of the "tdelete" function in the context of a binary search tree.
 * @tc.type: FUNC
 **/
HWTEST_F(SearchTdeleteTest, tdelete_001, TestSize.Level1)
{
    void* root = nullptr;
    Node n1(1);
    EXPECT_NE(nullptr, tsearch(&n1, &root, NodeCmp));

    Node n2(2);
    EXPECT_EQ(nullptr, tdelete(&n2, &root, NodeCmp));
    EXPECT_NE(nullptr, tdelete(&n1, &root, NodeCmp));
}