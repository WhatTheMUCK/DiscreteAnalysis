cd tests
for (( t = 1; t <= 10; t++ ));
do
	rm -rf "$t.nodes" "$t.edges" "$t.in" "$t.out"
	touch "$t.nodes" "$t.edges" "$t.in" "$t.out"
done
touch graph.b 