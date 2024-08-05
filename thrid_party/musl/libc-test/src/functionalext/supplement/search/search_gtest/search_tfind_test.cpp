#include <gtest/gtest.h>
#include <search.h>

using namespace testing::ext;

class SearchTfindTest : public testing::Test {
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
 * @tc.name: tfind_001
 * @tc.desc: Testing the behavior of the tree search and insertion functions in a binary search tree context.
 * @tc.type: FUNC
 **/
HWTEST_F(SearchTfindTest, tfind_001, TestSize.Level1)
{
    void* root = nullptr;
    Node n1(1);
    Node n2(2);
    EXPECT_EQ(nullptr, tfind(&n1, &root, NodeCmp));
    EXPECT_EQ(nullptr, tfind(&n2, &root, NodeCmp));

    void* i = tsearch(&n1, &root, NodeCmp);
    EXPECT_NE(nullptr, i);
    EXPECT_EQ(i, tfind(&n1, &root, NodeCmp));
    EXPECT_EQ(nullptr, tfind(&n2, &root, NodeCmp));
}