/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include "test.h"
#include "functionalext.h"

#define NODE_TEN 10
#define NODE_TWENTY 20
#define NODE_THIRTY 30

// 定义链表节点结构
struct Node {
    int data;
    LIST_ENTRY(Node) entries;
};

// 定义链表头结构
LIST_HEAD(Head, Node) head = LIST_HEAD_INITIALIZER(head);

// 插入节点到链表
void insert_node(int data)
{
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    EXPECT_PTRNE("insert_node", new_node, NULL);
    new_node->data = data;
    LIST_INSERT_HEAD(&head, new_node, entries);
}

// 删除节点
void delete_node(int data)
{
    struct Node *node;
    LIST_FOREACH(node, &head, entries) {
        if (node->data == data) {
            LIST_REMOVE(node, entries);
            free(node);
            return;
        }
    }
}

// 遍历链表并返回所有节点值的字符串
void traverse_list(char *result, size_t result_len)
{
    struct Node *node;
    char buffer[20];
    size_t pos = 0;
    LIST_FOREACH(node, &head, entries) {
        int res = snprintf(buffer, sizeof(buffer), "%d ", node->data);
        EXPECT_GTE("traverse_list -> snprintf", res, 0);
        size_t len = strlen(buffer);
        if (pos + len < result_len) {
            strcpy(result + pos, buffer);
            pos += len;
        }
    }
}

// C 测试用例
void slist_0100()
{
    char result[100];

    // 插入节点到链表
    insert_node(NODE_TEN);
    insert_node(NODE_TWENTY);
    insert_node(NODE_THIRTY);

    // 验证链表内容是否符合预期
    traverse_list(result, sizeof(result));
    EXPECT_STREQ("slist_0100", result, "30 20 10 ");

    // 删除节点并验证链表内容
    delete_node(NODE_TWENTY);
    traverse_list(result, sizeof(result));
    EXPECT_STREQ("slist_0100", result, "30 10 ");
}

int main()
{
    slist_0100();
    return t_status;
}
