#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iterator>

using strings_set = std::unordered_set<std::string>;
strings_set strings;

struct str_ptr {
  str_ptr() = default;
  str_ptr(strings_set::iterator i) : i_(i) { }

  bool operator==(const str_ptr& other) const {
    return i_ == other.i_;
  }
  bool operator<(const str_ptr& other) const {
    return *i_ < *other.i_;
  }

  strings_set::iterator i_;
};

namespace std
{
  template <>
  struct hash<str_ptr> {
    std::size_t operator()(const str_ptr& k) const {
      static std::hash<std::string> hasher;
      return hasher(*k.i_);
    }
  };
}

using graph_t = std::unordered_map < str_ptr, std::unordered_set<str_ptr> >;
graph_t graph;

using str_set = std::set < str_ptr >;

inline bool are_connected(const str_ptr& f, const str_ptr& s) {
  auto& fc = graph.at(f);
  auto& sc = graph.at(s);
  return (fc.find(s) != fc.end()) && (sc.find(f) != sc.end());
}

str_set neighbours(const str_ptr& vert) {
  str_set result;
  auto& vert_connections = graph.at(vert);
  std::for_each(vert_connections.begin(), vert_connections.end(), [&result, &vert](const graph_t::value_type::second_type::value_type& conn){
    auto& conn_connections = graph.at(conn);
    if (conn_connections.find(vert) != conn_connections.end()) {
      result.emplace(conn);
    }
  });
  return result;
}

using result_t = std::set < str_set >;
result_t result;

//str_set cur_subgraph;
void extend(str_set&& cur_subgraph, str_set&& candidates, str_set&& used) {
  if (candidates.empty() && used.empty()) {
    result.emplace(std::move(cur_subgraph));
    return;
  }

  while (!candidates.empty()) {
    auto cur_str = *candidates.begin();

    str_set new_cur_subgraph = cur_subgraph;
    new_cur_subgraph.insert(cur_str);

    auto cur_neighbours = neighbours(cur_str);
    str_set new_candidates;
    std::set_intersection(candidates.begin(), candidates.end(),
      cur_neighbours.begin(), cur_neighbours.end(),
      std::inserter(new_candidates, new_candidates.begin()));

    str_set new_used;
    std::set_intersection(used.begin(), used.end(),
      cur_neighbours.begin(), cur_neighbours.end(),
      std::inserter(new_used, new_used.begin()));

    extend(std::move(new_cur_subgraph), std::move(new_candidates), std::move(new_used));
    candidates.erase(cur_str);
    used.emplace(std::move(cur_str));
  }
}

int main(int argc, char* argv[]) {

  str_set candidates;

  std::ifstream file(argv[1]);
  while (file.good()) {
    std::string line;
    std::getline(file, line);
    std::stringstream sstr(line);
    //Thu Dec 11 17:53:01 PST 2008	a@facebook.com	b@facebook.com
    std::string s1, s2;
    sstr >> s1 >> s1 >> s1 >> s1 >> s1 >> s1 >> s1 >> s2;

    auto i1 = strings.emplace(std::move(s1)).first;
    auto i2 = strings.emplace(std::move(s2)).first;

    graph[i1].insert(i2);
    auto& i2n = graph[i2];
    if (i2n.find(i1) != i2n.end()) {
      candidates.insert(i1);
      candidates.insert(i2);
    }
  }

  extend(str_set(), std::move(candidates), str_set());

  std::for_each(result.begin(), result.end(), [](const result_t::value_type& set){
    for (auto i = set.begin(); i != set.end(); ++i) {
      if (i != set.begin()) std::cout << ", ";
      std::cout << *i->i_;
    }
    std::cout << '\n';
  });

  return 0;
}
