#pragma once

#include <iostream>
#include <cstdint>

#include "lexem.hpp"

struct tree
{
  bool is_leaf;
  int32_t data;
  std::vector<tree> children;
  tree()
  {
    this->is_leaf = false;
    this->data = 0;
    this->children.reserve(5);
  }
  tree(bool is_leaf, int32_t data, const std::vector<tree> &children)
  : is_leaf(is_leaf)
  , data(data)
  , children(children)
  {}
};

struct parse_pair
{
  int32_t sym;
  tree *node;
};

inline static tree*
add_child(tree* parent)
{
  parent->children.push_back(tree());
  return(&(parent->children.back()));
}

inline static tree*
add_child(tree* parent, tree child)
{
  parent->children.push_back(child);
  return(&(parent->children.back()));
}

inline static std::string
print_tree_load(const tree &t)
{
  return(symbol_name(t.data));
}

static void
print_tree(const tree &t, uint32_t depth, std::vector<bool> last)
{
  for (uint32_t i = 1; i < depth; ++i)
    std::cout << (last[i] ? "    " : "│   ");

  if (depth > 0)
    std::cout << (last.back() ? "└───" : "├───");

  std::cout << print_tree_load(t) << std::endl;

  for (uint32_t i = 0; i < t.children.size(); ++i)
  {
    last.push_back(i == t.children.size() - 1);
    print_tree(t.children[i], depth + 1, last);
    last.pop_back();
  }
}

static void
print_tree(const tree &t)
{
  print_tree(t, 0, { true });
}