touch graph.a
./prog preprocess --nodes tests/1.nodes --edges tests/1.edges --output graph.a
touch out.txt
./prog search --graph graph.a --input tests/1.in --output out.txt --full-output
rm -rf graph.a