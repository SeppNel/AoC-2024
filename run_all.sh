#!/bin/bash

cd build

# Print table header
printf "%-5s | %-7s\n" "Day" "Time (ms)"
echo "------------------"

total_time_ms=0

for exe in Day*; do
    day=$(basename "$exe")         # Extract the executable name
    cp "../$day/input.txt" input.txt

    # Record start time in nanoseconds
    start_time=$(date +%s%N)

    # Run the executable
    "./$exe" >/dev/null

    # Record end time in nanoseconds
    end_time=$(date +%s%N)

    # Calculate runtime in milliseconds
    runtime_ns=$((end_time - start_time))
    runtime_ms=$(awk -v ns="$runtime_ns" 'BEGIN { printf "%.3f", ns / 1000000 }')

    # Add to total time
    total_time_ms=$(awk -v total="$total_time_ms" -v current="$runtime_ms" 'BEGIN { printf "%.3f", total + current }')

    # Print formatted table row
    printf "%-5s | %-7s\n" "$day" "${runtime_ms} ms"
done

# Print total time
echo "--------------------------"
printf "%-5s | %-7s\n" "Total" "${total_time_ms} ms"