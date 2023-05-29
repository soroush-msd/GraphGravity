TEST RATIONALE

graph_test1

for the first test file, I decided to test my graph constructors as they are crucial to work
correctly for the rest of the assignment. I made different sections for different types of
cpnstructors to logically seperate them. I tried to make a combination of empty and populated
graphs to cover edge cases. I tried to use minimal number of dependancy functions to test my
constrcutors. As a result I mostly used empty(), nodes() and == operator to test if the instances
are created correctly. I chose to test with these functions because the implementations for these
functions were quite simple and intuitive + easy to understand the tests.

graph_test2

for this file, I created my graphs initially and then populated them using the modifiers. Then
again to test if they have been populated correctly via modifiers, I checked the objects against
some of the accessors like is_node or is_connected as they had simple implementation and could
quickly test if insert_node and insert_edges are working as they are supposed to. for the
complicated functions like replace_node and merge_replace, I created a seperate graph as the
result of applying those functions to my main grapgh object, and then I used == operator to
compare my graphs. This way I made sure that the graphs are equal and the functions work as
expected.

graph_test3

for testing accessor, I created my graphs, populated them with modifiers and then tested them with
accessors. I created small graphs with easily noticeable nodes and edges, so then I expect the
accessors to return true/false or vector of results as they are expected since it is easy to follow
what nodes and edges are currently in the graph. so for example, I knew is_node should return true
if the node is inserted or is_connected if there is an edge between the two nodes. for connections
and weights, I created a seperate vector and hard coded the expected results and then compared the
returned vector from the functions with the hard coded vector to make sure they are correctly
ordered and also there is no duplicate in the graph.this way I made sure that the functions work as
expected from the spec.

graph_test4

for this file, I tested graph comparison (==) and output stream (<<). for == operator, I first
created empty graphs to make sure empty case is checked. then I inserted nodes to one while keeping
the other empty and then checked them to be unequal. then I inserted equal nodes and edges to both
of them and I check they're equal. then I added two edges with same nodes and different weights and
checked them to be unequal. this way I exhausted all of the edge cases and possible comparisons
of graphs to make sure == operator works ad it is suppopsed to.
for output stream, (<<), I copied from the example given in the spec and comapred the string view
with the output. I also checked that an empty graph returns empty string. so this way I made sure
that my << operator works correctly and outputs in the right format.

graph_test5

for the last file, I tested all iterator_related parts of the assignment, including the erase edge
functions and find.I put those functions inside this file as I thought it'd be easier to comprehend
and debug them using iterators.
for iterator operations, first I tried begin and end methods within an empty graph to make sure
they are equal. I then used reference(*) operator to evaluate the values of the each iterator, and
compared them with their expected value to ensure they're being dereferencing correctly. After that,
I tested ++ and -- operations by creating a graph and going through its edges by ++ or -- the exisiting
iterator.
for erase edges functions, I created and ppopulated some graphs, then I tried to delete one or more
edges between the range of the graph. After that, I evaluated the values of the returned iterator
to make sure the following iterators in range are returned. Then I used is_connected to make sure
the edges specified in the range with the iterators are deleted from the graph so the function works
as from the spec.
for find, I created a graph with some edges, Then I called find with src, dst and weight of the
corresponding edges, after that, I used the returned iterator to evalue the values and made sure
their values are equal as the hardcoded values when inserting new edegs. by doing this I ensured
that find returns the right iterator with the right values.

finally, for the functions throwing exceptions, I hardcoded some random nodes and values that are
not in the graph and then tried to call the function, then I comapred its output with their corresponding
verbatim exception strings to ensure exceptions are thrown as expected.