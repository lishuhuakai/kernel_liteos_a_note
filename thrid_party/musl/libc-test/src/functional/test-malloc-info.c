/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>

#include "test-malloc-info-stats-print.h"
#include "libxml/parser.h"

static const xmlChar *get_text_from_children(xmlNodePtr children)
{
	for (xmlNodePtr child_node = children; child_node != NULL; child_node = child_node->next) {
		if (child_node->type == XML_TEXT_NODE) {
			return child_node->content;
		}
	}
	return NULL;
}

static const xmlChar *get_attribute(const char *attr_name, xmlNodePtr node)
{
	for (xmlAttrPtr curr_attr = node->properties; curr_attr != NULL; curr_attr = curr_attr->next) {
		if (xmlStrEqual(curr_attr->name, (const xmlChar *) attr_name)) {
			return get_text_from_children(curr_attr->children);
		}
	}
	return NULL;
}

static xmlNodePtr find_child_node_with_attr(const char *name, const char *attr_name, const char *attr_value, xmlNodePtr parent)
{
	if (parent == NULL) {
		return NULL;
	}
	for (xmlNodePtr curr_node = parent->children; curr_node != NULL; curr_node = curr_node->next) {
		if (curr_node->type == XML_ELEMENT_NODE && xmlStrEqual(curr_node->name, (xmlChar *) name)) {
			if (attr_name == NULL) {
				return curr_node;
			}
			if (xmlStrEqual(get_attribute(attr_name, curr_node), (const xmlChar *) attr_value)) {
				return curr_node;
			}
		}
	}
	return NULL;
}

static xmlNodePtr find_child_node(const char *name, xmlNodePtr parent)
{
	return find_child_node_with_attr(name, NULL, NULL, parent);
}

static const char *get_node_text(xmlNodePtr node_ptr)
{
	if (node_ptr == NULL) {
		return NULL;
	}
	return (const char *) get_text_from_children(node_ptr->children);
}

static void stderr_stats_cb(void)
{
	malloc_info(0, stderr);
}

static long long parse_amount(const char *s)
{
	if (s == NULL) {
		return -1;
	}
	char *end_ptr;
	long long result = strtoll(s, &end_ptr, 10);
	if (end_ptr != s + strlen(s)) {
		return -1;
	}
	if (result < 0) {
		return -1;
	}
	return result;
}

static xmlNodePtr find_thread_in_document(xmlDocPtr doc_ptr, const char *thread_id)
{
	xmlNodePtr root_element = xmlDocGetRootElement(doc_ptr);
	return find_child_node_with_attr("thread", "id", thread_id, find_child_node("threads", root_element));
}

static int populate_thread_stats(const char *output, const char *thread_id, malloc_thread_stats_t *stats)
{
	xmlDocPtr doc_ptr = xmlParseDoc((const xmlChar *) output);
	if (doc_ptr == NULL) {
		return 0;
	}
	xmlNodePtr thread_root = find_thread_in_document(doc_ptr, thread_id);
	long long total_allocated_memory =
		parse_amount(get_node_text(find_child_node("total_allocated_memory", thread_root)));
	long long total_mmapped_memory =
		parse_amount(get_node_text(find_child_node("total_mmapped_memory", thread_root)));
	long long mmapped_regions =
		parse_amount(get_node_text(find_child_node("mmapped_regions", thread_root)));
	xmlFreeDoc(doc_ptr);

	if (total_allocated_memory == -1 || total_mmapped_memory == -1 || mmapped_regions == -1) {
		return 0;
	}
	stats->total_allocated_memory = total_allocated_memory;
	stats->total_mmapped_memory = total_mmapped_memory;
	stats->mmapped_regions = mmapped_regions;
	return 1;
}

static int populate_total_free_heap_space(const char *output, long long *total_free_heap_space)
{
	xmlDocPtr doc_ptr = xmlParseDoc((const xmlChar *) output);
	if (doc_ptr == NULL) {
		return 0;
	}
	xmlNodePtr heap_space_root = find_child_node("total_free_heap_space", xmlDocGetRootElement(doc_ptr));
	long long total_free_heap_space_parsed = parse_amount(get_node_text(heap_space_root));
	xmlFreeDoc(doc_ptr);

	if (total_free_heap_space_parsed == -1) {
		return 0;
	}
	*total_free_heap_space = total_free_heap_space_parsed;
	return 1;
}

static int is_thread_in_output(const char *output, const char *thread_id)
{
	xmlDocPtr doc_ptr = xmlParseDoc((const xmlChar *) output);
	if (doc_ptr == NULL) {
		return 0;
	}
	int result = find_thread_in_document(doc_ptr, thread_id) != NULL;
	xmlFreeDoc(doc_ptr);
	return result;
}
