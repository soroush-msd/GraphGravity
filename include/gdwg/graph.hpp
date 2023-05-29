#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <vector>

namespace gdwg {

	template<typename N, typename E>
	class graph {
	public:
		class iterator;

		struct value_type {
			N from;
			N to;
			E weight;
		};

		// Constructors
		graph() noexcept = default;

		graph(std::initializer_list<N> il)
		: graph(il.begin(), il.end()) {}

		template<typename InputIt>
		graph(InputIt first, InputIt last) {
			std::for_each(first, last, [this](N const& value) { insert_node(value); });
		}

		graph(graph&& other) noexcept {
			nodes_ = std::move(other.nodes_);
			edges_ = std::move(other.edges_);
		}

		auto operator=(graph&& other) noexcept -> graph& {
			nodes_ = std::move(other.nodes_);
			edges_ = std::move(other.edges_);
			return *this;
		}

		graph(graph const& other) {
			for (auto node : other.nodes_) {
				insert_node(*node);
			}
			for (const auto& [key, value] : other.edges_) {
				for (const auto& neighbour : value) {
					insert_edge(*key, *neighbour.neighbour_node, neighbour.weight);
				}
			}
		}

		auto operator=(graph const& other) -> graph& {
			if (this == &other) {
				return *this;
			};
			auto tmp = other;
			std::swap(nodes_, tmp.nodes_);
			std::swap(edges_, tmp.edges_);
			return *this;
		}

		~graph() noexcept = default;

		// Your member functions go here

		// Modifiers
		auto insert_node(N const& value) -> bool {
			if (is_node(value) == false) {
				return nodes_.insert(std::make_shared<N>(value)).second;
			}
			return false;
		}

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if ((is_node(src) == false) or (is_node(dst) == false)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
				                         "or dst node does not exist");
			}
			return edges_[*(nodes_.find(src))].insert({*(nodes_.find(dst)), weight}).second;
		}

		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (is_node(old_data) == false) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
			}

			if (is_node(new_data) == true) {
				return false;
			};
			std::map<N, std::map<N, E>> to_be_replaced{};
			for (const auto& [key, val] : edges_) {
				for (const auto& neighbour : val) {
					if (*key == old_data or *neighbour.neighbour_node == old_data) {
						to_be_replaced[*key][*neighbour.neighbour_node] = neighbour.weight;
					}
				}
			}
			insert_node(new_data);
			for (const auto [key, val] : to_be_replaced) {
				for (const auto& neighbour : val) {
					if (key == old_data) {
						if (old_data == neighbour.first) {
							insert_edge(new_data, new_data, neighbour.second);
						}
						else {
							insert_edge(new_data, neighbour.first, neighbour.second);
						}
					}
					else {
						erase_edge(key, neighbour.first, neighbour.second);
						insert_edge(key, new_data, neighbour.second);
					}
				}
			}

			nodes_.erase(*(nodes_.find(old_data)));
			return true;
		}

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if ((is_node(old_data) == false) or (is_node(new_data) == false)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
			}

			std::map<N, std::map<N, E>> to_be_merged{};
			for (const auto& [key, val] : edges_) {
				for (const auto& neighbour : val) {
					if (*key == old_data or *neighbour.neighbour_node == old_data) {
						to_be_merged[*key][*neighbour.neighbour_node] = neighbour.weight;
					}
				}
			}

			for (const auto [key, val] : to_be_merged) {
				for (const auto& neighbour : val) {
					if (key == old_data) {
						insert_edge(new_data, neighbour.first, neighbour.second);
					}
					else {
						erase_edge(key, neighbour.first, neighbour.second);
						insert_edge(key, new_data, neighbour.second);
					}
				}
			}
			erase_node(old_data);
		}

		auto erase_node(N const& value) -> bool {
			if (is_node(value) == false) {
				return false;
			};
			std::map<N, std::map<N, E>> to_be_erased{};
			for (const auto& [key, val] : edges_) {
				for (const auto& neighbour : val) {
					if (*neighbour.neighbour_node == value) {
						to_be_erased[*key][*neighbour.neighbour_node] = neighbour.weight;
					}
				}
			}
			for (const auto [key, val] : to_be_erased) {
				edges_.erase(*(nodes_.find(key)));
			}
			edges_.erase(*(nodes_.find(value)));
			nodes_.erase(*(nodes_.find(value)));
			return true;
		}

		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			if ((is_node(src) == false) or (is_node(dst) == false)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
				                         "they don't exist in the graph");
			}
			return edges_[*(nodes_.find(src))].erase({*(nodes_.find(dst)), weight});
		}

		auto erase_edge(iterator i) -> iterator;

		auto erase_edge(iterator i, iterator s) -> iterator;

		auto clear() noexcept -> void {
			edges_.clear();
			nodes_.clear();
		}

		// Accessors
		[[nodiscard]] auto is_node(N const& value) const noexcept -> bool {
			auto node_exist = nodes_.find(value);
			if (node_exist != nodes_.end()) {
				return true;
			}
			return false;
		}

		[[nodiscard]] auto empty() const noexcept -> bool {
			if (nodes_.size() == 0) {
				return true;
			}
			return false;
		}

		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
			if ((is_node(src) == false) or (is_node(dst) == false)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
				                         "node don't exist in the graph");
			}
			if (edges_.count(src) != 0) {
				auto source = edges_.at(*(nodes_.find(src)));
				for (auto const& destination : source) {
					if (*(destination.neighbour_node) == dst) {
						return true;
					}
				}
			}
			return false;
		}

		[[nodiscard]] auto nodes() const noexcept -> std::vector<N> {
			std::vector<N> nodes_vector{};
			for (auto in_set : nodes_) {
				nodes_vector.push_back(*in_set);
			}
			return nodes_vector;
		}

		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E> {
			if ((is_node(src) == false) or (is_node(dst) == false)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
				                         "don't exist in the graph");
			}
			std::vector<E> weights_vector{};
			if (edges_.count(src) != 0) {
				auto source = edges_.at(*(nodes_.find(src)));
				for (auto const& destination : source) {
					if (*(destination.neighbour_node) == dst) {
						weights_vector.push_back(destination.weight);
					}
				}
			}
			return weights_vector;
		}

		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) const -> iterator {
			for (auto it = begin(); it != end(); ++it) {
				if ((*it).from == src and (*it).to == dst and (*it).weight == weight) {
					return it;
				}
			}
			return end();
		}

		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			if (is_node(src) == false) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't "
				                         "exist in the graph");
			}
			std::vector<N> connections_vector{};
			if (edges_.count(src) != 0) {
				auto source = edges_.at(*(nodes_.find(src)));
				for (auto const& destination : source) {
					connections_vector.push_back(*(destination.neighbour_node));
				}
			}
			return connections_vector;
		}

		// Iterator access
		[[nodiscard]] auto begin() const -> iterator {
			if (edges_.empty() == true) {
				return iterator({}, edges_.end(), edges_.end(), edges_.begin());
			}
			return iterator(edges_.begin()->second.begin(), edges_.begin(), edges_.end(), edges_.begin());
		}

		[[nodiscard]] auto end() const -> iterator {
			return iterator({}, edges_.end(), edges_.end(), edges_.begin());
		}
		[[nodiscard]] auto operator==(graph const& other) const noexcept -> bool {
			if (this == &other) {
				return true;
			};
			if (nodes_.size() != other.nodes_.size() or edges_.size() != other.edges_.size()) {
				return false;
			}
			if (nodes() != other.nodes()) {
				return false;
			};
			if (edges() != other.edges()) {
				return false;
			};
			return true;
		}

		friend auto operator<<(std::ostream& os, graph const& g) noexcept -> std::ostream& {
			if (g.empty() == true) {
				os << "";
				return os;
			}
			for (auto node : g.nodes()) {
				os << node << " "
				   << "("
				   << "\n";
				for (auto neighbour : g.connections(node)) {
					for (auto weight : g.weights(node, neighbour)) {
						os << "  " << neighbour << " | " << weight << "\n";
					}
				}
				os << ")"
				   << "\n";
			}
			return os;
		}

	private:
		// edge structure
		struct neighbour {
			std::shared_ptr<N> neighbour_node;
			E weight;
		};

		// custom comparators
		struct node_comparator {
			using is_transparent = std::true_type;

			auto operator()(std::shared_ptr<N> const& lhs, std::shared_ptr<N> const& rhs) const noexcept
			   -> bool {
				return *lhs < *rhs;
			}

			auto operator()(std::shared_ptr<N> const& lhs, N const& rhs) const noexcept -> bool {
				return *lhs < rhs;
			}

			auto operator()(N const& lhs, std::shared_ptr<N> const& rhs) const noexcept -> bool {
				return lhs < *rhs;
			}
		};

		struct edge_comparator {
			using is_transparent = std::true_type;

			auto operator()(neighbour const& lhs, neighbour const& rhs) const noexcept -> bool {
				if (*(lhs.neighbour_node) != *(rhs.neighbour_node)) {
					return *(lhs.neighbour_node) < *(rhs.neighbour_node);
				}
				return lhs.weight < rhs.weight;
			}
		};

		// data members
		std::set<std::shared_ptr<N>, node_comparator> nodes_;
		std::map<std::shared_ptr<N>, std::set<neighbour, edge_comparator>, node_comparator> edges_;

		[[nodiscard]] auto edges() const noexcept -> std::map<N, std::map<N, E>> {
			std::map<N, std::map<N, E>> edge_map{};

			for (const auto& [key, value] : edges_) {
				for (const auto& neighbour : value) {
					edge_map[*key][*neighbour.neighbour_node] = neighbour.weight;
				}
			}
			return edge_map;
		}
	};

	// Iterator class
	template<typename N, typename E>
	class graph<N, E>::iterator {
		using outer_iterator =
		   typename std::map<std::shared_ptr<N>, std::set<neighbour, edge_comparator>, node_comparator>::const_iterator;
		using inner_iterator = typename std::set<neighbour, edge_comparator>::const_iterator;

	public:
		using value_type = graph<N, E>::value_type;
		using reference = value_type;
		using pointer = void;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		// Iterator constructor
		iterator() = default;

		// Iterator source
		auto operator*() -> reference {
			return reference{*outer_->first, *inner_->neighbour_node, inner_->weight};
		};

		// Iterator traversal
		auto operator++() -> iterator& {
			if (inner_ != outer_->second.end()) {
				++inner_;
			}
			if (inner_ == outer_->second.end()) {
				++outer_;
				if (outer_ != last_) {
					inner_ = outer_->second.begin();
				}
				else {
					inner_ = inner_iterator{};
				}
			}
			return *this;
		}

		auto operator++(int) -> iterator {
			auto temp = *this;
			++*this;
			return temp;
		}

		auto operator--() -> iterator& {
			if (outer_ == last_ or inner_ == outer_->second.begin()) {
				if (outer_ == first_) {
					inner_ = inner_iterator();
					return *this;
				}
				--outer_;
				inner_ = outer_->second.end();
			}
			--inner_;
			return *this;
		}

		auto operator--(int) -> iterator {
			auto temp = *this;
			--*this;
			return temp;
		}

		// Iterator comparison
		auto operator==(iterator const& other) const -> bool = default;

	private:
		inner_iterator inner_{};
		outer_iterator outer_{};
		outer_iterator last_{};
		outer_iterator first_{};

		friend class graph;

		explicit iterator(inner_iterator inner,
		                  outer_iterator outer,
		                  outer_iterator last,
		                  outer_iterator first)
		: inner_{inner}
		, outer_{outer}
		, last_{last}
		, first_{first} {}
	};

	template<typename N, typename E>
	auto graph<N, E>::erase_edge(iterator i) -> iterator {
		if (i == end()) {
			return i;
		}
		auto next = i;
		++next;
		erase_edge((*i).from, (*i).to, (*i).weight);
		if (next == end()) {
			return end();
		}
		return next;
	}

	template<typename N, typename E>
	auto graph<N, E>::erase_edge(iterator i, iterator s) -> iterator {
		if ((i == end()) or (i == s)) {
			return i;
		}
		auto end = s;

		std::map<N, std::map<N, E>> to_be_erased{};
		for (auto x = i; x != s; ++x) {
			to_be_erased[(*x).from][(*x).to] = (*x).weight;
		}

		for (const auto [key, val] : to_be_erased) {
			for (const auto& neighbour : val) {
				erase_edge(key, neighbour.first, neighbour.second);
			}
		}
		return end;
	}

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
