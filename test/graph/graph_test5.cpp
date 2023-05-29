#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <vector>

TEST_CASE("2.8 Iterator [gdwg.iterator]") {
	SECTION("Iterator access and comparison") {
		auto g1 = gdwg::graph<std::string, int>{};

		CHECK(g1.begin() == g1.end());

		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_edge("hello", "how", 1);

		CHECK(g1.begin() != g1.end());

		auto x = g1.begin();

		CHECK((*x).from == "hello");
		CHECK((*x).to == "how");
		CHECK((*x).weight == 1);
	}

	SECTION("Iterator ++ and * traversal") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_node("are");
		g1.insert_edge("hello", "hello", 2);
		g1.insert_edge("hello", "how", 4);
		g1.insert_edge("how", "are", 7);

		auto s = g1.begin();
		CHECK((*s).from == "hello");
		CHECK((*s).to == "hello");
		CHECK((*s).weight == 2);

		++s;
		CHECK((*s).from == "hello");
		CHECK((*s).to == "how");
		CHECK((*s).weight == 4);

		s++;
		CHECK((*s).from == "how");
		CHECK((*s).to == "are");
		CHECK((*s).weight == 7);

		CHECK(++s == g1.end());
	}

	SECTION("Iterator -- and * traversal") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_node("are");
		g1.insert_edge("hello", "hello", 2);
		g1.insert_edge("hello", "how", 4);
		g1.insert_edge("how", "are", 7);

		auto e = g1.end();
		--e;
		CHECK((*e).from == "how");
		CHECK((*e).to == "are");
		CHECK((*e).weight == 7);

		e--;
		CHECK((*e).from == "hello");
		CHECK((*e).to == "how");
		CHECK((*e).weight == 4);

		e--;
		CHECK((*e).from == "hello");
		CHECK((*e).to == "hello");
		CHECK((*e).weight == 2);
	}

	SECTION("erase_edge(i) -> i") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_node("are");
		g1.insert_edge("hello", "hello", 2);
		g1.insert_edge("hello", "how", 4);
		g1.insert_edge("how", "are", 7);

		auto e = g1.erase_edge(g1.begin());
		CHECK(g1.is_connected("hello", "hello") == false);

		CHECK((*e).from == "hello");
		CHECK((*e).to == "how");
		CHECK((*e).weight == 4);

		auto x = g1.erase_edge(e);
		CHECK(g1.is_connected("hello", "how") == false);

		CHECK((*x).from == "how");
		CHECK((*x).to == "are");
		CHECK((*x).weight == 7);

		auto d = g1.erase_edge(x);
		CHECK(g1.is_connected("how", "are") == false);

		CHECK(d == g1.end());
	}

	SECTION("erase_edge(i, i) -> i") {
		// delete [start, end)
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_node("are");
		g1.insert_edge("hello", "hello", 2);
		g1.insert_edge("hello", "how", 4);
		g1.insert_edge("how", "are", 7);

		auto s1 = g1.begin();
		auto d1 = g1.erase_edge(s1, g1.end());
		CHECK(g1.is_connected("hello", "hello") == false);
		CHECK(g1.is_connected("hello", "how") == false);
		CHECK(g1.is_connected("how", "are") == false);
		CHECK(d1 == g1.end());

		// delete [start, end-1)
		auto g2 = gdwg::graph<std::string, int>{};
		g2.insert_node("hello");
		g2.insert_node("how");
		g2.insert_node("are");
		g2.insert_edge("hello", "hello", 2);
		g2.insert_edge("hello", "how", 4);
		g2.insert_edge("how", "are", 7);

		auto s = g2.begin();

		auto e = g2.begin();
		++e;
		++e;

		auto x = g2.erase_edge(s, e);
		CHECK(g1.is_connected("hello", "hello") == false);
		CHECK(g1.is_connected("hello", "how") == false);

		CHECK((*x).from == "how");
		CHECK((*x).to == "are");
		CHECK((*x).weight == 7);
	}

	SECTION("find(src, dst, weight) -> iterator") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_node("are");
		g1.insert_edge("hello", "hello", 2);
		g1.insert_edge("hello", "how", 4);
		g1.insert_edge("how", "are", 7);

		auto x1 = g1.find("hello", "hello", 2);
		CHECK((*x1).from == "hello");
		CHECK((*x1).to == "hello");
		CHECK((*x1).weight == 2);

		auto x2 = g1.find("hello", "how", 4);
		CHECK((*x2).from == "hello");
		CHECK((*x2).to == "how");
		CHECK((*x2).weight == 4);

		auto x3 = g1.find("how", "are", 7);
		CHECK((*x3).from == "how");
		CHECK((*x3).to == "are");
		CHECK((*x3).weight == 7);

		// no such edge
		auto x4 = g1.find("Olympics", "Banana", 23);
		CHECK(x4 == g1.end());
	}
}