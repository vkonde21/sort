Name: Vaishnavi Konde  
Mis: 111803185  
DSA mini project: Linux Sort command  
This project implements a command line utility which can sort files of large size(even larger than RAM). This is done using the external sorting algorithm.
# Usage
./sort [OPTION]... [FILE]...  
# Options
```
     -b, ignore leading blanks	Ignore leading blanks  
     -n, numersic sort		Compare according to string numeric value  
     -r, reverse		Reverse the result of comparisions  
     -d, dictionary order Arrange in a dictionary order   
     -k, key=KEYDEF		Sort via a key. KEYDEF gives location  
     -m, merge			Merge already sorted files; Do not sort.  
     -u, unique			Remove duplicates from result  
     -i, ignore-nonprint			ignore the nonprintable characters while sorting  
     -h,--help			Print this usage and exit```
 You can also try combination of options eg. ./sort -r -k1 [filename]
