#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <vector>

TEST_CASE("2.4 Accessors [gdwg.accessors]") {
	SECTION("is_node") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_edge("hello", "how", 1);

		CHECK(g1.is_node("hello") == true);
		CHECK(g1.is_node("how") == true);
		CHECK(g1.is_node("Olympics") == false);
	}

	SECTION("empty()") {
		auto g1 = gdwg::graph<std::string, int>{};

		CHECK(g1.empty() == true);

		g1.insert_node("hello");
		CHECK(g1.empty() == false);
	}

	SECTION("is_connected") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_node("are");
		g1.insert_edge("hello", "how", 1);
		g1.insert_edge("hello", "hello", 1);

		CHECK(g1.is_connected("hello", "how") == true);
		CHECK(g1.is_connected("hello", "are") == false);
		CHECK(g1.is_connected("hello", "hello") == true);

		CHECK_THROWS_WITH(g1.is_connected("hell", "heaven"),
		                  "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't "
		                  "exist in the graph");
		CHECK_THROWS_WITH(g1.is_connected("hello", "heaven"),
		                  "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't "
		                  "exist in the graph");
		CHECK_THROWS_WITH(g1.is_connected("hell", "how"),
		                  "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't "
		                  "exist in the graph");
	}

	SECTION("nodes()") {
		// empty nodes
		auto g1 = gdwg::graph<int, std::string>();
		auto empty_nodes = std::vector<int>();

		CHECK(g1.nodes() == empty_nodes);

		// with nodes
		std::vector<int> v1{43, 34, 123, 3, 11, 4, 5};
		auto g2 = gdwg::graph<int, std::string>(v1.begin(), v1.end());
		auto vector_nodes = std::vector<int>{3, 4, 5, 11, 34, 43, 123};

		CHECK(g2.nodes() == vector_nodes);
	}

	SECTION("weights") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_edge("hello", "how", 43);
		g1.insert_edge("hello", "how", 4);
		g1.insert_edge("hello", "how", 523);
		g1.insert_edge("hello", "how", 23);
		g1.insert_edge("hello", "how", 56);
		std::vector<int> v1{4, 23, 43, 56, 523};

		CHECK(g1.weights("hello", "how") == v1);

		// empty case
		auto g2 = gdwg::graph<std::string, int>{};
		g2.insert_node("hello");
		g2.insert_node("how");
		std::vector<int> v2{};

		CHECK(g2.weights("hello", "how") == v2);

		CHECK_THROWS_WITH(g1.weights("hell", "heaven"),
		                  "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in "
		                  "the graph");
		CHECK_THROWS_WITH(g1.weights("hello", "heaven"),
		                  "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in "
		                  "the graph");
		CHECK_THROWS_WITH(g1.weights("hell", "how"),
		                  "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in "
		                  "the graph");
	}

	// find function is in the last cpp test file

	SECTION("Connections") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_node("are");
		g1.insert_node("you");
		g1.insert_node("today?");

		g1.insert_edge("hello", "how", 4);
		g1.insert_edge("hello", "are", 4);
		g1.insert_edge("hello", "you", 5);
		g1.insert_edge("hello", "today?", 2);

		std::vector<std::string> v1{"are", "how", "today?", "you"};

		CHECK(g1.connections("hello") == v1);

		// empty case
		auto g2 = gdwg::graph<std::string, int>{};
		g2.insert_node("hello");
		std::vector<std::string> v2{};

		CHECK(g2.connections("hello") == v2);

		CHECK_THROWS_WITH(g1.connections("anomaly"),
		                  "Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the "
		                  "graph");
	}
}