all:
	mkdir -p build
	g++ -O2 -o build/Day01 Day01/AoC1_part2.cpp
	g++ -O2 -o build/Day02 Day02/AoC2_part2.cpp
	g++ -O2 -o build/Day03 Day03/AoC3_part2.cpp
	g++ -O2 -o build/Day04 Day04/AoC4_part2.cpp
	g++ -O2 -o build/Day05 Day05/AoC5_part2.cpp
	g++ -O2 -o build/Day06 Day06/AoC6_part2.cpp
	g++ -O2 -o build/Day07 Day07/AoC7_part2.cpp
	g++ -O2 -o build/Day08 Day08/AoC8_part2.cpp
	g++ -O2 -o build/Day09 Day09/AoC9_part2.cpp
	g++ -O2 -o build/Day10 Day10/AoC10_part2.cpp
	g++ -O2 -o build/Day11 Day11/AoC11_part2.cpp
	g++ -O2 -o build/Day12 Day12/AoC12_part2.cpp
	g++ -O2 -o build/Day13 Day13/AoC13_part2.cpp
	#g++ -O2 -o build/Day14 Day14/AoC14_part2.cpp
	g++ -O2 -o build/Day15 Day15/AoC15_part2.cpp
	g++ -O2 -o build/Day16 Day16/AoC16_part2.cpp
	g++ -O2 -o build/Day17 Day17/AoC17_part2.cpp
	g++ -O2 -o build/Day18 Day18/AoC18_part2.cpp
	g++ -O2 -o build/Day19 Day19/AoC19_part2.cpp

run:
	./run_all.sh

clean:
	rm -R build