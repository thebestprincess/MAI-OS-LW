#!/bin/bash

# 1. Normal cases
echo "12 3 4" > test_normal.txt
echo "20 5 2" >> test_normal.txt
echo "100 10 2 5" >> test_normal.txt
echo "8 2" >> test_normal.txt
echo "15 3 5" >> test_normal.txt

# 2. Division by zero
echo "10 2 5" > test_division_by_zero.txt
echo "15 0 3" >> test_division_by_zero.txt
echo "20 4 5" >> test_division_by_zero.txt

# 3. Not enough numbers
echo "100" > test_single_number.txt
echo "50 2" >> test_single_number.txt
echo "25" >> test_single_number.txt
echo "10 5 2" >> test_single_number.txt

# 4. Empty file
touch test_empty.txt

# 5. Negative numbers
echo "-12 3 -4" > test_negative.txt
echo "20 -5 2" >> test_negative.txt
echo "-100 10 -2" >> test_negative.txt

# 6. Large numbers
echo "2147483647 2 3" > test_large_numbers.txt
echo "-2147483648 2 10" >> test_large_numbers.txt
echo "1000000000 1000 10" >> test_large_numbers.txt

# 7. Invalid characters
echo "10 abc 5" > test_invalid_chars.txt
echo "20 3 def" >> test_invalid_chars.txt
echo "ghi 2 3" >> test_invalid_chars.txt
echo "10 2 3junk" >> test_invalid_chars.txt

# 8. Different separators
echo "10    2   3" > test_mixed_spaces.txt
echo -e "20\t5\t2" >> test_mixed_spaces.txt
echo "10 2   3" >> test_mixed_spaces.txt

# 9. Multiline file
echo "10 2" > test_multiple_lines.txt
echo "5 1" >> test_multiple_lines.txt
echo "8 2 4" >> test_multiple_lines.txt
echo "3 1" >> test_multiple_lines.txt
echo "100 10 2 5" >> test_multiple_lines.txt

# 10. Edge cases
echo "0 5 2" > test_edge_cases.txt
echo "10 1" >> test_edge_cases.txt
echo "1 1 1 1 1" >> test_edge_cases.txt
echo "-10 -2 5" >> test_edge_cases.txt

# 11. Test without '\n'
echo "10 2 5" > test_no_newline.txt
echo -n "20 4" >> test_no_newline.txt

# 12. Lines with only spaces
echo "10 2 5" > test_only_spaces.txt
echo "       " >> test_only_spaces.txt
echo "20 4" >> test_only_spaces.txt
echo "    " >> test_only_spaces.txt

echo "Test files have been created!"
