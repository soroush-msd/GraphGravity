#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

TEST_CASE("2.6 Comparisons [gdwg.cmp]") {
	SECTION("operator ==") {
		auto g1 = gdwg::graph<std::string, int>{};
		auto g2 = gdwg::graph<std::string, int>{};

		CHECK(g1 == g2);

		g1.insert_node("hello");
		g1.insert_node("how");

		CHECK(g1 != g2);

		g2.insert_node("hello");
		g2.insert_node("how");

		CHECK(g1 == g2);

		g1.insert_edge("hello", "how", 2);
		g2.insert_edge("hello", "how", 2);

		CHECK(g1 == g2);

		g1.insert_edge("hello", "hello", 4333);
		g2.insert_edge("hello", "hello", 1);

		CHECK(g1 != g2);
	}
}

TEST_CASE("2.7 Extractor [gdwg.io]") {
	SECTION("operator <<") {
		using graph = gdwg::graph<int, int>;
		auto const v = std::vector<graph::value_type>{
		   {4, 1, -4},
		   {3, 2, 2},
		   {2, 4, 2},
		   {2, 1, 1},
		   {6, 2, 5},
		   {6, 3, 10},
		   {1, 5, -1},
		   {3, 6, -8},
		   {4, 5, 3},
		   {5, 2, 7},
		};

		auto g = graph{};

		// empty graph
		auto oss1 = std::ostringstream{};
		oss1 << g;
		CHECK(oss1.str() == "");

		for (const auto& [from, to, weight] : v) {
			g.insert_node(from);
			g.insert_node(to);
			g.insert_edge(from, to, weight);
		}

		g.insert_node(64);
		auto out = std::ostringstream{};
		out << g;
		auto const expected_output = std::string_view(R"(1 (
  5 | -1
)
2 (
  1 | 1
  4 | 2
)
3 (
  2 | 2
  6 | -8
)
4 (
  1 | -4
  5 | 3
)
5 (
  2 | 7
)
6 (
  2 | 5
  3 | 10
)
64 (
)
)");
		CHECK(out.str() == expected_output);
	}
}