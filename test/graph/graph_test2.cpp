#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <vector>

TEST_CASE("2.3 Modifiers [gdwg.modifiers]") {
	SECTION("Insert Node") {
		auto g = gdwg::graph<std::string, int>{};

		CHECK(g.insert_node("hello") == true);
		CHECK(g.is_node("hello") == true);

		CHECK(g.is_node("how") == false);
		CHECK(g.insert_node("how") == true);
		CHECK(g.insert_node("how") == false);
		CHECK(g.is_node("how") == true);
	}

	SECTION("Insert Edge") {
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("hello");
		g.insert_node("how");

		CHECK(g.insert_edge("hello", "how", 5) == true);
		CHECK(g.insert_edge("hello", "how", 5) == false);
		CHECK(g.insert_edge("hello", "hello", 5) == true);

		CHECK_THROWS_WITH(g.insert_edge("hell", "heaven", 5),
		                  "Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node "
		                  "does not exist");
		CHECK_THROWS_WITH(g.insert_edge("hello", "heaven", 5),
		                  "Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node "
		                  "does not exist");
		CHECK_THROWS_WITH(g.insert_edge("hell", "how", 5),
		                  "Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node "
		                  "does not exist");
	}

	SECTION("Replace Node") {
		// successful replace
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_edge("hello", "how", 1);

		CHECK(g1.replace_node("hello", "bye") == true);
		CHECK(g1.is_connected("bye", "how") == true);

		// reflexive case
		auto g2 = gdwg::graph<std::string, int>{};
		g2.insert_node("hello");
		g2.insert_edge("hello", "hello", 1);

		CHECK(g2.replace_node("hello", "bye") == true);
		CHECK(g2.is_connected("bye", "bye") == true);

		// new node already is a node
		auto g3 = gdwg::graph<std::string, int>{};
		g3.insert_node("hello");
		g3.insert_node("bye");
		g3.insert_edge("hello", "hello", 1);

		CHECK(g3.replace_node("hello", "bye") == false);
		CHECK(g3.is_connected("hello", "hello") == true);

		// only nodes
		auto g4 = gdwg::graph<std::string, int>{};
		g4.insert_node("hello");

		CHECK(g4.replace_node("hello", "bye") == true);

		// old node is not in the graph
		CHECK_THROWS_WITH(g1.replace_node("olympics", "Australia"),
		                  "Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");
	}

	// example from the spec
	SECTION("Merge Replace Node") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("A");
		g1.insert_node("B");
		g1.insert_node("C");
		g1.insert_node("D");

		g1.insert_edge("A", "B", 1);
		g1.insert_edge("A", "C", 2);
		g1.insert_edge("A", "D", 3);

		g1.merge_replace_node("A", "B");

		auto result_graph = gdwg::graph<std::string, int>{};
		result_graph.insert_node("B");
		result_graph.insert_node("C");
		result_graph.insert_node("D");

		result_graph.insert_edge("B", "B", 1);
		result_graph.insert_edge("B", "C", 2);
		result_graph.insert_edge("B", "D", 3);

		CHECK(g1 == result_graph);

		CHECK_THROWS_WITH(g1.merge_replace_node("olympics", "Australia"),
		                  "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if "
		                  "they don't exist in the graph");
		CHECK_THROWS_WITH(g1.merge_replace_node("A", "Australia"),
		                  "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if "
		                  "they don't exist in the graph");
		CHECK_THROWS_WITH(g1.merge_replace_node("olympics", "B"),
		                  "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if "
		                  "they don't exist in the graph");
	}

	// example from the spec
	SECTION("Merge Replace Node with duplicate edges") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("A");
		g1.insert_node("B");
		g1.insert_node("C");
		g1.insert_node("D");

		g1.insert_edge("A", "B", 1);
		g1.insert_edge("A", "C", 2);
		g1.insert_edge("A", "D", 3);
		g1.insert_edge("B", "B", 1);

		g1.merge_replace_node("A", "B");

		auto result_graph = gdwg::graph<std::string, int>{};
		result_graph.insert_node("B");
		result_graph.insert_node("C");
		result_graph.insert_node("D");

		result_graph.insert_edge("B", "B", 1);
		result_graph.insert_edge("B", "C", 2);
		result_graph.insert_edge("B", "D", 3);

		CHECK(g1 == result_graph);
	}

	// from the Diagrammatic paper example in the spec
	SECTION("Merge Replace Node with reflexive cases") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("A");
		g1.insert_node("B");
		g1.insert_node("C");
		g1.insert_node("D");

		g1.insert_edge("A", "B", 3);
		g1.insert_edge("C", "B", 2);
		g1.insert_edge("D", "B", 4);

		g1.merge_replace_node("B", "A");

		auto result_graph = gdwg::graph<std::string, int>{};
		result_graph.insert_node("A");
		result_graph.insert_node("C");
		result_graph.insert_node("D");

		result_graph.insert_edge("A", "A", 3);
		result_graph.insert_edge("C", "A", 2);
		result_graph.insert_edge("D", "A", 4);

		CHECK(g1 == result_graph);
	}

	SECTION("Erase node") {
		// outgoing edge
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_edge("hello", "how", 1);

		CHECK(g1.is_connected("hello", "how") == true);
		CHECK(g1.erase_node("hello") == true);

		CHECK(g1.is_node("hello") == false);
		g1.insert_node("hello");
		CHECK(g1.is_connected("hello", "how") == false);

		// reflexive case
		auto g2 = gdwg::graph<std::string, int>{};
		g2.insert_node("hello");
		g2.insert_edge("hello", "hello", 1);

		CHECK(g2.is_connected("hello", "hello") == true);
		CHECK(g2.erase_node("hello") == true);

		CHECK(g2.is_node("hello") == false);
		g2.insert_node("hello");
		CHECK(g2.is_connected("hello", "hello") == false);

		// incoming edge
		auto g3 = gdwg::graph<std::string, int>{};
		g3.insert_node("hello");
		g3.insert_node("how");
		g3.insert_edge("hello", "how", 1);

		CHECK(g3.is_connected("hello", "how") == true);
		CHECK(g3.erase_node("how") == true);

		CHECK(g3.is_node("how") == false);
		g3.insert_node("how");
		CHECK(g3.is_connected("hello", "how") == false);
	}

	SECTION("Erase edge -> bool") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_edge("hello", "how", 1);

		CHECK(g1.is_connected("hello", "how") == true);
		CHECK(g1.erase_edge("hello", "how", 43) == false);
		CHECK(g1.erase_edge("hello", "how", 1) == true);

		CHECK(g1.is_connected("hello", "how") == false);
		CHECK(g1.erase_edge("hello", "how", 1) == false);

		CHECK_THROWS_WITH(g1.erase_edge("Good", "UNSW", 1),
		                  "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't "
		                  "exist in the graph");
		CHECK_THROWS_WITH(g1.erase_edge("Hello", "UNSW", 1),
		                  "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't "
		                  "exist in the graph");
		CHECK_THROWS_WITH(g1.erase_edge("UNSW", "how", 1),
		                  "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't "
		                  "exist in the graph");
	}

	// iterator functions are in the last cpp test file

	SECTION("Clear") {
		auto g1 = gdwg::graph<std::string, int>{};
		g1.insert_node("hello");
		g1.insert_node("how");
		g1.insert_node("are");
		g1.insert_edge("hello", "how", 1);
		g1.insert_edge("how", "are", 2);

		CHECK(g1.empty() == false);
		g1.clear();
		CHECK(g1.empty() == true);
	}
}