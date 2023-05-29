#include <iostream>
#include <string>

#include "gdwg/graph.hpp"
#include <initializer_list>

auto main() -> int {
	// gdwg::graph<std::string, int> g;
	//{
	// std::string s1{"Hello"};
	// g.insert_node(s1);
	// g.insert_node("how");
	// std::cout << g.insert_edge("Hello", "how", 5) << "  ewjdbwebjk\n";
	//}
	// auto g = gdwg::graph<std::string, int>{};
	// g.insert_node("Hello");
	// g.insert_edge("Hello", "Hello", 1);
	// g.replace_node("Hello", "Aloe");
	// std::cout << g;
	// Even though s1 has gone out of scope, g has its own
	//  copied resource that it has stored, so the node
	//  will still be in here.
	// std::cout << g.is_node("Hello") << "\n"; // prints 'true';

	auto g = gdwg::graph<std::string, int>{};
	g.insert_node("hello");
	g.insert_node("how");
	g.insert_node("ho");
	// g.insert_node("are");
	// g.insert_node("you?");
	// std::cout << g.is_node("hello") << "\n";
	// std::cout << g.is_node("hell") << "\n";

	// g.insert_node("A");
	// g.insert_node("B");
	// g.insert_node("C");
	// g.insert_node("D");

	// g.insert_edge("B", "A", 3);
	// g.insert_edge("B", "C", 2);
	// g.insert_edge("B", "D", 4);
	// g.insert_edge("B", "B", 1);

	g.insert_edge("hello", "how", 5);
	g.insert_edge("hello", "how", 6);
	g.insert_edge("hello", "how", 7);
	g.insert_edge("how", "ho", 7);
	g.insert_edge("ho", "ho", 8);
	// auto x = g.begin();
	// x++;
	// std::cout << (g.begin() == g.end()) << "\n";
	// std::cout << x.weight << "\n";
	// std::cout << ((*x).from == "hello") << "\n";
	// std::cout << ((*x).weight) << "\n";
	// x++;
	// auto e = g.end();
	// e--;
	// std::cout << ((*e).weight) << "\n";
	// e--;
	// std::cout << ((*e).weight) << "\n";
	// std::cout << (x == g.end()) << "\n";
	// auto f = g.find("hello", "how", 5);
	// std::cout << ((*f).from) << "\n";
	// auto d = g.erase_edge(g.begin());
	// std::cout << ((*d).weight) << "\n";
	// auto d = g.erase_edge(g.begin(), ++g.begin());
	// std::cout << ((*d).weight) << "\n";
	for (auto it = g.begin(); it != g.end(); ++it) {
		// if((*it).to == "hello" and (*it).from == "how" and (*it).weight == 5) {
		std::cout << (*it).to << "\n";
	}
	//}
	for (auto v = g.end(); v-- != g.begin();) {
		std::cout << ((*v).from) << "\n";
	}

	// std::cout << g.erase_edge("hello", "how", 5) << "\n";
	// g.replace_node("hello", "bye");
	// std::cout << g.erase_edge("hello", "how", 5) << "\n";
	// g.erase_node("hello");
	// g.erase_node("how");
	// g.insert_edge("hello", "how", 6);
	// g.insert_node("hell");
	// g.insert_edge("hell", "hello", 6);
	// g.merge_replace_node("B", "A");
	// g.insert_edge("hell", "hello", 5);
	// g.insert_node("hoe");
	// g.erase_node("how");
	// g.insert_node("bye");
	// g.insert_edge("hello", "bye", 2);
	// g.insert_edge("hello", "how", 2);
	// g.insert_edge("hello", "how", 1);
	// std::cout << g.is_connected("hello", "how") << "\n";
	// std::cout << g.is_connected("hello", "bye") << "\n";
	// auto b = gdwg::graph<std::string, int>{};
	// b.insert_node("hello");
	// b.insert_node("how");
	// std::cout << (g == b) << "\n";
	// std::cout << g.erase_node("how") << "\n";
	// std::cout << g << "\n";
	// auto vec = g.weights("hello", "how");
	// for(auto x : vec) {
	// std::cout << x << "\n";
	//}
	// std::cout << vec.size() << "\n";

	// std::cout << g.insert_edge("hello", "how", 5) << "\n";

	// auto d = gdwg::graph<std::string, int>{};
	// d.insert_node("hell");
	// d.insert_node("ho");
	// d.insert_edge("hell", "ho", 5);
	// std::cout << d.insert_edge("hell", "ho", 5) << "\n";
	// d = g;
	// std::cout << d.insert_edge("hello", "how", 5) << "\n";

	// std::cout << d.empty() << "HERE FALSE" << "\n";
	// auto s = gdwg::graph<std::string, int>{};
	// std::cout << s.empty() << "ANd HERE TRUE" << "\n";

	// auto k = gdwg::graph<std::string, int>();

	// std::cout << g.empty() << "\n";
	// g.clear();
	// std::cout << g.empty() << "\n";

	// std::initializer_list<int> il1{3, 5, 7, 9, 10, 45, 3, 9};
	// auto g1 = gdwg::graph<int, std::string>(il1);

	// std::cout << d.insert_edge("hello", "how", 5) << "\n";
	// auto b = g;
	// std::cout << b.is_node("hello") << "\n";
	// std::cout << b.is_node("hell") << "\n";
	// g.insert_edge("hello", "are", 8);
	// g.insert_edge("hello", "are", 2);

	// g.insert_edge("how", "you?", 1);
	// g.insert_edge("how", "hello", 4);

	// g.insert_edge("are", "you?", 3);

	// std::cout << g << "\n";

	// auto g2 = gdwg::graph<std::string, int>(g);

	// std::cout << g2 << "\n";

	// This is a structured binding.
	// https://en.cppreference.com/w/cpp/language/structured_binding
	// It allows you to unpack your tuple.
	// for (auto const& [from, to, weight] : g) {
	// std::cout << from << " -> " << to << " (weight " << weight << ")\n";
	//}
	// std::cout << " HERE WE GO AGAIN " << "\n";
}
