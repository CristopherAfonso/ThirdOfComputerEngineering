# Temperatures

Calculate CPU temperatures statistics over the whole year for each **day**.

This program generates measurements of CPU temperatures randomly for every second of an entire year,
puts them into a vector and calculates the **mean** and **median** of that data.

The program has a menu when it is executed and you can choose whether you want it to run **sequentially**,
in parallel using **producer and consumer threads** (with a shared conditional variable in each thread
and unique locks) or,using **writer and reader threads** (only using an array of 2 atomic variables and
a mutex to control the output of readers, task which is done in parallel with the writing of the next data,
as things were already calculated in sync).
