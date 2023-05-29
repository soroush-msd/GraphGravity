#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <vector>

TEST_CASE("2.2 Constructors [gdwg.ctor]") {
	SECTION("graph()") {
		auto g = gdwg::graph<int, std::string>();
		auto empty_nodes = std::vector<int>();

		CHECK(g.empty() == true);
		CHECK(g.nodes() == empty_nodes);
	}

	SECTION("Initializer list Constructor") {
		std::initializer_list<int> il1{3, 5, 7, 9, 10, 45, 3, 9};
		auto g1 = gdwg::graph<int, std::string>(il1);
		auto vector_nodes = std::vector<int>{3, 5, 7, 9, 10, 45};

		CHECK(g1.empty() == false);
		CHECK(g1.nodes() == vector_nodes);

		std::initializer_list<int> il2{};
		auto g = gdwg::graph<int, std::string>(il2);
		CHECK(g.empty() == true);
	}

	SECTION("InputIt Constructor") {
		std::vector<int> v1{43, 34, 123, 3, 11, 4, 5};
		auto g1 = gdwg::graph<int, std::string>(v1.begin(), v1.end());

		auto vector_nodes = std::vector<int>{3, 4, 5, 11, 34, 43, 123};

		CHECK(g1.empty() == false);
		CHECK(g1.nodes() == vector_nodes);

		std::vector<int> v2{};
		auto g2 = gdwg::graph<int, std::string>(v2.begin(), v2.end());
		CHECK(g2.empty() == true);
		CHECK(g2.nodes() == v2);
	}

	SECTION("Move Constructor") {
		std::vector<int> v1{43, 34, 123, 3, 11, 4, 5};
		auto g1 = gdwg::graph<int, std::string>(v1.begin(), v1.end());
		auto g2 = std::move(g1);
		auto vector_nodes = std::vector<int>{3, 4, 5, 11, 34, 43, 123};

		CHECK(g1.empty() == true);
		CHECK(g2.empty() == false);
		CHECK(g2.nodes() == vector_nodes);
	}

	SECTION("Move Assignment") {
		std::vector<int> v1{43, 34, 123, 3, 11, 4, 5};
		auto g1 = gdwg::graph<int, std::string>(v1.begin(), v1.end());
		auto g2 = gdwg::graph<int, std::string>();
		g2 = std::move(g1);
		auto vector_nodes = std::vector<int>{3, 4, 5, 11, 34, 43, 123};

		CHECK(g1.empty() == true);
		CHECK(g2.empty() == false);
		CHECK(g2.nodes() == vector_nodes);
	}

	SECTION("Copy Constructor") {
		std::vector<int> v1{43, 34, 123, 3, 11, 4, 5};
		auto g1 = gdwg::graph<int, std::string>(v1.begin(), v1.end());
		auto g2 = g1;

		CHECK(g1.empty() == false);
		CHECK(g2.empty() == false);
		CHECK(g2.nodes() == g1.nodes());
		CHECK(g2 == g1);
	}

	SECTION("Copy Assignment") {
		std::vector<int> v1{43, 34, 123, 3, 11, 4, 5};
		auto g1 = gdwg::graph<int, std::string>(v1.begin(), v1.end());
		auto g2 = gdwg::graph<int, std::string>();
		g2 = g1;

		CHECK(g1.empty() == false);
		CHECK(g2.empty() == false);
		CHECK(g2.nodes() == g1.nodes());
		CHECK(g2 == g1);
	}
}
