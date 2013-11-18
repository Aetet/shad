#!/bin/bash
./k_statistic_test > test.input
./k_statistic_naive < test.input > naive.output
./floating_k_statisctic < test.input > test.output
diff naive.output test.output
